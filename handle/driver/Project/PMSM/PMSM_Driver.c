/**********************************************************/
//@Autor:Raw
//@Version:0.0.1
//@End Time:2023/3/21
//@Function:PMSM_Driver.c
/**********************************************************/
#include "PMSM_Driver.h"
#include "tim.h"
#include "gpio.h"
#include "filter.h"
#include "CurveProgramme.h"



sMotorCtrlTypeDef testx = {0};
sMotorCtrlTypeDef PMSM[2] = {0};
APP_PMSM_DRIVER APP_PMSM;


void PMSM_para_init(void)
{
	memset(&PMSM[PMSM_A],0,sizeof(sMotorCtrlTypeDef));
	memset(&PMSM[PMSM_U],0,sizeof(sMotorCtrlTypeDef));
	
	FOC_Config(FOC_1,&PMSM[PMSM_A].foc);
	FilterInit(1000);
	PMSM[PMSM_A].MaxCurrent = IQ_MAX;
	PMSM[PMSM_A].MaxSpeed = Speed_MAX;
	PMSM[PMSM_A].ControlMode = SpeedCtrl_e;//速度，电流环，直接给iq
	PMSM[PMSM_A].MotorState = MotorStop;
	PMSM[PMSM_A].Theta_acc = 0.0008f;
	PMSM[PMSM_A].OpenLoopVoltCmd = 0.12f;
	PMSM[PMSM_A].pairs = 4.0f;
	PMSM[PMSM_A].foc_count = 0;
	PMSM[PMSM_A].S_Add_acc = 3.5f;
	PMSM[PMSM_A].S_low_acc = 0.8f;
	PMSM[PMSM_A].S_Run_acc = 1.5f;
	PMSM[PMSM_A].sec_speed_handle = null_break;
//	PMSM[PMSM_A].MotorCali = MegCaliNone;
	
	FOC_Config(FOC_2,&PMSM[PMSM_U].foc);
	PMSM[PMSM_U].MaxCurrent = IQ_MAX;
	PMSM[PMSM_U].MaxSpeed = Speed_MAX;
	PMSM[PMSM_U].ControlMode = SpeedCtrl_e;//速度，电流环，直接给iq
	PMSM[PMSM_U].MotorState = MotorStop;
	PMSM[PMSM_U].OpenLoopVoltCmd = 0.12f;
	PMSM[PMSM_U].Theta_acc = 0.0008f;
	PMSM[PMSM_U].pairs = 4.0f;
	PMSM[PMSM_U].foc_count = 1;
	PMSM[PMSM_U].S_Add_acc = 3.5f;
	PMSM[PMSM_U].S_low_acc = 0.8f;
	PMSM[PMSM_U].S_Run_acc = 1.5f;
	PMSM[PMSM_U].sec_speed_handle = null_break;
//	PMSM[PMSM_U].MotorCali = MegCaliNone;
	
	pid_init(&PMSM[PMSM_A].angle_pid,10.0f,0.0f,0.0f,PMSM[PMSM_A].MaxSpeed,-PMSM[PMSM_A].MaxSpeed);
//	pid_init(&PMSM[PMSM_A].speed_pid,15.0f,0.15f,0.0f,PMSM[PMSM_A].MaxCurrent,-PMSM[PMSM_A].MaxCurrent);
	pid_init(&PMSM[PMSM_A].speed_pid,0.015f,0.00015f,0.0f,PMSM[PMSM_A].MaxCurrent,-PMSM[PMSM_A].MaxCurrent);
//	pid_init(&PMSM[PMSM_A].speed_pid,0.004f,0.00008f,0.0f,PMSM[PMSM_A].MaxCurrent,-PMSM[PMSM_A].MaxCurrent);
	pid_init(&PMSM[PMSM_A].current_id_pid,0.02f,0.0002f,0.0f,0.6f,-0.6f);
	pid_init(&PMSM[PMSM_A].current_iq_pid,0.02f,0.0002f,0.0f,1.154f,-1.154f);
	
	pid_init(&PMSM[PMSM_U].angle_pid,10.0F,0.0f,0.0f,PMSM[PMSM_U].MaxSpeed,-PMSM[PMSM_U].MaxSpeed);
//	pid_init(&PMSM[PMSM_U].speed_pid,15.0f,0.15f,0.0f,PMSM[PMSM_U].MaxCurrent,-PMSM[PMSM_U].MaxCurrent);
	pid_init(&PMSM[PMSM_U].speed_pid,0.015f,0.00015f,0.0f,PMSM[PMSM_A].MaxCurrent,-PMSM[PMSM_A].MaxCurrent);
	pid_init(&PMSM[PMSM_U].current_id_pid,0.02f,0.0002f,0.0f,0.6f,-0.6f);
	pid_init(&PMSM[PMSM_U].current_iq_pid,0.02f,0.0002f,0.0f,1.154f,-1.154f);
	
	APP_PMSM.axis_x = 127;
	APP_PMSM.axis_y = 127;
	APP_PMSM.axis_y_filter = 127.0f;
	
	curve_config(SCURVE_1);
	curve_config(SCURVE_2);
	
	
	
}


void PMSM_para_clear(PMSM_Num num)
{
//	PMSM[num].AngleSet = 0.0f;
	PMSM[num].SpeedSet = 0.0f;
	PMSM[num].CurrentSet = 0.0f;
	PMSM[num].SpeedNew_100MS = 0;
	PMSM[num].LastSpeedNew = 0;
	PMSM[num].LastSpeedNew_100MS = 0;
	PMSM[num].SpeedNow = 0.0f;
	PMSM[num].SpeedCmd = 0.0f;
}
void pmsm_start(PMSM_Num num)
{
	if(num == PMSM_A)
	{
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);   
		HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1); 
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);   
		HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);    
		HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
//		HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
	}
	else if(num == PMSM_U)
	{
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);   
		HAL_TIMEx_PWMN_Start(&htim8, TIM_CHANNEL_1); 
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);   
		HAL_TIMEx_PWMN_Start(&htim8, TIM_CHANNEL_2);
		HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);    
		HAL_TIMEx_PWMN_Start(&htim8, TIM_CHANNEL_3);
//		HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_4);
	}
}


void pmsm_stop(PMSM_Num num)
{
	if(num == PMSM_A)
	{
		PMSM_A_PWM_CH0(PMSM_A_CAR());
		PMSM_A_PWM_CH1(PMSM_A_CAR());
		PMSM_A_PWM_CH2(PMSM_A_CAR());
	}
	else if(num == PMSM_U)
	{
		PMSM_U_PWM_CH0(PMSM_U_CAR());
		PMSM_U_PWM_CH1(PMSM_U_CAR());
		PMSM_U_PWM_CH2(PMSM_U_CAR());
	}
}


void pmsm_break_io(PMSM_Num num,bool value)
{
	if(!value) 
	{
		
		if(num == PMSM_A)BREAK1_ON();
    else BREAK2_ON();
	} 
	else
	{
		if(num == PMSM_A)BREAK1_OFF();
    else BREAK2_OFF();
	}

}





void PMSM_PWM_RUN(PMSM_Num num,Phase3_value *PWM)
{
	if(num == PMSM_A)
	{
		PMSM_A_PWM_CH0((uint16_t)(PMSM_A_CAR()*PWM->value_a));
		PMSM_A_PWM_CH1((uint16_t)(PMSM_A_CAR()*PWM->value_b));
		PMSM_A_PWM_CH2((uint16_t)(PMSM_A_CAR()*PWM->value_c));
	}
	else if(num == PMSM_U)
	{
		PMSM_U_PWM_CH0((uint16_t)(PMSM_U_CAR()*PWM->value_a));
		PMSM_U_PWM_CH1((uint16_t)(PMSM_U_CAR()*PWM->value_b));
		PMSM_U_PWM_CH2((uint16_t)(PMSM_U_CAR()*PWM->value_c));
	}
}




void PMSM_Start_Filter(PMSM_Num num)
{
	switch(PMSM[num].MotorPreState)
	{
		case MotorStart:
			if(PMSM[num].MotorState != MotorRun&&PMSM[num].first_start == false)
			{
				PMSM[num].MotorState = MotorStart;
				PMSM[num].first_start = true;
				PMSM[num].acc_status = start_acc;
			}
			PMSM[num].first_stop = false;
			break;
		case MotorStop:
			if(PMSM[num].MotorState != MotorWatit&&PMSM[num].first_stop == false)
			{
				PMSM[num].MotorState = MotorStop;
				PMSM[num].first_stop = true;
			}
			PMSM[num].first_start = false;
			break;
		default:
			break;
	}


}






















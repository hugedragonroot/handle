/**********************************************************/
//@Autor:Raw
//@Version:0.0.1
//@End Time:2023/3/21
//@Function:PMSM_Control.c
/**********************************************************/
#include "PMSM_Control.h"
#include "PMSM_Driver.h"
#include "pid.h"
#include "FOC.h"
#include "HALL_Encoder.h"
#include "PMSM_Calib.h"
#include "adc_callback.h"
#include "filter.h"
#include "gpio.h"
#include "CurveProgramme.h"
#define pmsm_current_op	3.3f/(4096.0f*0.002f*14.0f)


#define HZ_200us 4
#define HZ_100us 2




uint16_t RUN_COUNT = 0;
void	Speed_Run_handle(PMSM_Num num)
{

		float new_speeds = 0.0f;
		if(num == PMSM_A) new_speeds =  PMSM[num].SpeedNew;
		if(num == PMSM_U) new_speeds =  -PMSM[num].SpeedNew;
		if(PMSM[num].SpeedSet < new_speeds)
		{
					
			PMSM[num].SpeedSet = PMSM[num].SpeedSet + PMSM[num].S_low_acc ;//正向速度坡度
			if(PMSM[num].SpeedSet > new_speeds)
			{
				PMSM[num].SpeedSet = new_speeds ;
			}
		}
		else if(PMSM[num].SpeedSet > new_speeds)
		{
			PMSM[num].SpeedSet = PMSM[num].SpeedSet - PMSM[num].S_low_acc ;//反向速度坡度
			if(PMSM[num].SpeedSet < new_speeds)
			{
				PMSM[num].SpeedSet = new_speeds ;
			}
		}

	
	
}

void Speed_Wheel_handle(PMSM_Num num)
{
	if(PMSM[num].SpeedSet < PMSM[num].SpeedCmd)
	{
			
		PMSM[num].SpeedSet = PMSM[num].SpeedSet + PMSM[num].S_Run_acc ;//正向速度坡度
		if(PMSM[num].SpeedSet > PMSM[num].SpeedCmd)
		{
			PMSM[num].SpeedSet = PMSM[num].SpeedCmd ;
		}
	}
	else if(PMSM[num].SpeedSet > PMSM[num].SpeedCmd)
	{
		PMSM[num].SpeedSet = PMSM[num].SpeedSet - PMSM[num].S_Run_acc ;//反向速度坡度
		if(PMSM[num].SpeedSet < PMSM[num].SpeedCmd)
		{
			PMSM[num].SpeedSet = PMSM[num].SpeedCmd ;
		}
	}
	
}
void	Speed_Dec_handle(PMSM_Num num)
{
	PMSM[num].S_Add_acc = fabs(sinf(2.0f*fabs(PMSM[num].SpeedSet)/2100.0f*PI_2/4.0f)*PMSM[num].dec_ratio);
	if(fabs(PMSM[num].S_Add_acc)<0.5f)PMSM[num].S_Add_acc = 0.5f;
	
//	if(num == PMSM_U) PMSM[num].SpeedCmd = (float)PMSM[num].SpeedNew*(-1.0f);
//	if(num == PMSM_A) PMSM[num].SpeedCmd = (float)PMSM[num].SpeedNew;
	if(PMSM[num].SpeedSet <  PMSM[num].SpeedCmd )
	{
			
		PMSM[num].SpeedSet = PMSM[num].SpeedSet + PMSM[num].S_Add_acc ;//正向速度坡度
		if(PMSM[num].SpeedSet >  PMSM[num].SpeedCmd )
		{
			PMSM[num].SpeedSet =  PMSM[num].SpeedCmd  ;
		}
	}
	else if(PMSM[num].SpeedSet >  PMSM[num].SpeedCmd )
	{
		PMSM[num].SpeedSet = PMSM[num].SpeedSet - PMSM[num].S_Add_acc ;//反向速度坡度
		if(PMSM[num].SpeedSet <  PMSM[num].SpeedCmd )
		{
			PMSM[num].SpeedSet =  PMSM[num].SpeedCmd  ;
		}
	}
	
}

void	Speed_Start_handle(PMSM_Num num)
{
	if(PMSM[num].S_low_acc<PMSM[num].S_Add_acc)PMSM[num].S_low_acc = PMSM[num].S_low_acc+0.0008f;
	if(PMSM[num].SpeedSet < PMSM[num].SpeedNow)
	{
			
		PMSM[num].SpeedSet = PMSM[num].SpeedSet + PMSM[num].S_low_acc ;//正向速度坡度
		if(PMSM[num].SpeedSet > PMSM[num].SpeedNow)
		{
			PMSM[num].SpeedSet = PMSM[num].SpeedNow ;
		}
	}
	else if(PMSM[num].SpeedSet > PMSM[num].SpeedNow)
	{
		PMSM[num].SpeedSet = PMSM[num].SpeedSet - PMSM[num].S_low_acc ;//反向速度坡度
		if(PMSM[num].SpeedSet < PMSM[num].SpeedNow)
		{
			PMSM[num].SpeedSet = PMSM[num].SpeedNow ;
		}
	}
	
}

float current_datio =  0.00003f;
#define forward_current_max 1.5f
#define forward_current_min 0.1f
void Current_forward_handle(PMSM_Num num)
{
	if(fabs(PMSM[num].SpeedSet)<300.0f&&fabs(PMSM[num].SpeedSet)>10.0f)
	{
		PMSM[num].speed_temp = PMSM[num].SpeedSet - hall_encoder[num].PLL_realspeed;
		PMSM[num].speed_forward = PMSM[num].speed_temp/0.001f*current_datio;
		if(PMSM[num].speed_forward>forward_current_max) PMSM[num].speed_forward = forward_current_max;
		if(PMSM[num].speed_forward<-forward_current_max) PMSM[num].speed_forward = -forward_current_max;
		if(fabs(PMSM[num].speed_forward)<forward_current_min) PMSM[num].speed_forward = 0.0f;
	}
	else
	{
		PMSM[num].speed_forward = 0.0f;
	}
}


void speed_forward_handle(PMSM_Num num)
{
	
	if(fabs(PMSM[num].SpeedSet)<300.0f&&fabs(PMSM[num].SpeedSet)>30.0f)
	{
		PMSM[num].AngleSet = hall_encoder[num].AngleReal +	PMSM[num].SpeedSet/my_rpm*my_unit_angle/5000.0f;
		
		PID_run_FloatspdVolt(&PMSM[num].angle_pid,PMSM[num].AngleSet,hall_encoder[num].AngleReal);
	}
	else
	{
		clear_pid(&PMSM[num].angle_pid);
	}


}

static void foc_speed_ctrl(PMSM_Num num, uint8_t N)
{
	if(++PMSM[num].SpeedCmdCount >= N && N != 0)
	{
//		draw_scurve((SCURVE_NUM)num);
		PMSM[num].SpeedCmdCount = 0;
		running_programme((SCURVE_NUM)num);
	}
	else if(N == 0)
	{
		PMSM[num].SpeedSet = PMSM[num].SpeedNew;
	}
	

}
uint16_t count_test[2] ={0};
float ids = -0.1f;
static void Speed_handle(PMSM_Num num)
{
	PMSM[num].foc.Theta = hall_encoder[num].Theta;
	if(++PMSM[num].speed_pid_count>= 2)
	{
		PMSM[num].speed_pid_count = 0;
//		speed_forward_handle(num);
		PID_run_FloatspdVolt(&PMSM[num].speed_pid,PMSM[num].SpeedSet+PMSM[num].angle_pid.PIDOut,hall_encoder[num].PLL_realspeed);

//		PID_run_FloatspdVolt(&PMSM[num].speed_pid,PMSM[num].SpeedSet+PMSM[num].angle_pid.PIDOut,hall_encoder[num].real_speed);
		PMSM[num].CurrentSet = PMSM[num].speed_pid.PIDOut;
		Current_forward_handle(num);
	}
		PID_run_FloatspdVolt(&PMSM[num].current_id_pid, ids, PMSM[num].foc.Id_Iq.I1);
		
		PMSM[num].foc.Vd_Vq.V1 = PMSM[num].current_id_pid.PIDOut;
			
//		PID_run_FloatspdVolt(&PMSM[num].current_iq_pid, PMSM[num].CurrentSet, PMSM[num].foc.Id_Iq.I2);		
		PID_run_FloatspdVolt(&PMSM[num].current_iq_pid, PMSM[num].CurrentSet+PMSM[num].speed_forward , PMSM[num].foc.Id_Iq.I2);		
		PMSM[num].foc.Vd_Vq.V2 = PMSM[num].current_iq_pid.PIDOut;
}
static void PMSM_Control_Mode_handle(PMSM_Num num)
{
	switch(PMSM[num].ControlMode)
	{
		case SpeedCtrl_e:
			foc_speed_ctrl(num,10);
			Speed_handle(num);
			break;
		case DragMode_e:
			PMSM[num].foc.Theta += PMSM[num].Theta_acc;
			if(PMSM[num].foc.Theta > PI_2)
			{
				PMSM[num].foc.Theta = 0.0f;	
			}
			else if(PMSM[num].foc.Theta < 0)
			{
				PMSM[num].foc.Theta = PI_2 - 0.0f;	
			}
			PMSM[num].foc.Vd_Vq.V1 = 0.0f;
			PMSM[num].foc.Vd_Vq.V2 = PMSM[num].OpenLoopVoltCmd;
			break;
		case VoltCtrl_e:
			PMSM[num].foc.Theta = hall_encoder[num].Theta;
			PMSM[num].foc.Vd_Vq.V1 = 0.0f;
			PMSM[num].foc.Vd_Vq.V2 = PMSM[num].OpenLoopVoltCmd;
			break;
		case TorqueCtrl_e:
			PMSM[num].foc.Theta = hall_encoder[num].Theta;
			PID_run_FloatspdVolt(&PMSM[num].current_id_pid, 0.0f, PMSM[num].foc.Id_Iq.I1);
			PMSM[num].foc.Vd_Vq.V1 = PMSM[num].current_id_pid.PIDOut;
		
		
			PID_run_FloatspdVolt(&PMSM[num].current_iq_pid, PMSM[num].CurrentSet, PMSM[num].foc.Id_Iq.I2);		
			PMSM[num].foc.Vd_Vq.V2 = PMSM[num].current_iq_pid.PIDOut;
			break;
		case AngelCtrl_e:
			
			break;
		case Speed_TorqueCtrl:
					
			break;
		case HallCaliMode_e:
			PMSM[num].foc.Theta = 0.0f;
			PMSM[num].foc.Vd_Vq.V1 = PMSM[num].OpenLoopVoltCmd;
			PMSM[num].foc.Vd_Vq.V2 = 0.0f;
			break;
		default:
			break;
	}
}


uint16_t count_flage = 0;
static void handle_foc(PMSM_Num num)
{
	if(++PMSM[num].foc_count>=2)
	{
		PMSM[num].foc_count = 0;
		if(PMSM[num].MotorCali != MegCaliOK)
		{
			Electric_Angle_Calibration(num);
			Foc_Handle((FOC_num)num);
			PMSM_PWM_RUN(num,&PMSM[num].foc.PWM);
			return;
		}
		Theta_Read(num);
		switch(PMSM[num].MotorState)
		{
			
			case MotorStart:
				PMSM[num].MotorState = MotorPrecharge;
				PMSM[num].Start_time = 0;
				MOS_ENABLE();
				pmsm_start(num);
				break;
			case MotorPrecharge:
				if(++PMSM[num].Start_time>=5)
				{
					pmsm_break_io(num,false);
					MOS_ENABLE();
					PMSM[num].Start_time = 0;
					PMSM[num].MotorState = MotorRun;
				}
				break;
			case MotorStop:
				pmsm_break_io(num,true);
				PMSM_para_clear(num);
				if(++PMSM[num].stop_time>5)
				{
					pmsm_stop(num);
					PMSM[num].stop_time = 0;
					PMSM[num].MotorState = MotorWatit;
//					MOS_DISABLE();
				}
				break;
			case MotorWatit:
				pmsm_break_io(num,true);
//				MOS_DISABLE();
				clear_pid(&PMSM[num].angle_pid);
				clear_pid(&PMSM[num].speed_pid);
				clear_pid(&PMSM[num].current_id_pid);
				clear_pid(&PMSM[num].current_iq_pid);
				PMSM_para_clear(num);
		//			Motor_Para_Clear();
				break;
			case MotorRun:
				PMSM_Control_Mode_handle(num);
				Foc_Handle((FOC_num)num);
				PMSM_PWM_RUN(num,&PMSM[num].foc.PWM);
				break;
			default:
				break;

		}
	}
}
adc_data adc_current0;
static uint16_t ADC0_Offset = 0;
void adc0_callback_handle(void *parm)
{
	memcpy(&adc_current0,parm,sizeof(adc_data));
	
	if(ADC0_Offset < 2000)
	{
		ADC0_Offset++;
		PMSM[PMSM_A].CurrOffset[0] = PMSM[PMSM_A].CurrOffset[0] * 0.9f + (float)adc_current0.Current_A_B_C[0] * 0.1f;
		PMSM[PMSM_A].CurrOffset[1] = PMSM[PMSM_A].CurrOffset[1] * 0.9f + (float)adc_current0.Current_A_B_C[1] * 0.1f;
		PMSM[PMSM_A].CurrOffset[2] = PMSM[PMSM_A].CurrOffset[2] * 0.9f + (float)adc_current0.Current_A_B_C[2] * 0.1f;
	}
	else
	{
			if(PMSM[PMSM_A].first_offer == false)
			{
				PMSM[PMSM_A].first_offer = true;
				if(PMSM[PMSM_A].CurrOffset[0]>2048+100||PMSM[PMSM_A].CurrOffset[0]<2048-100
					||PMSM[PMSM_A].CurrOffset[1]>2048+100||PMSM[PMSM_A].CurrOffset[1]<2048-100
					||PMSM[PMSM_A].CurrOffset[2]>2048+100||PMSM[PMSM_A].CurrOffset[2]<2048-100)
				{
					PMSM[PMSM_A].PMSM_Status.bit.Current_Error = 1;
				}
				else
				{
					PMSM[PMSM_A].PMSM_Status.bit.Current_Error = 0;
				}
			}
			PMSM[PMSM_A].foc.Ia_Ib_Ic.I1 = PMSM[PMSM_A].foc.Ia_Ib_Ic.I1*0.5f + ((float)adc_current0.Current_A_B_C[0] - PMSM[PMSM_A].CurrOffset[0])*pmsm_current_op*0.5f; // ADC * (3.3/4096)/66.5/4/0.001R;//电流钳设置10mv,示波器设置X100倍
			PMSM[PMSM_A].foc.Ia_Ib_Ic.I2 = PMSM[PMSM_A].foc.Ia_Ib_Ic.I2*0.5f + ((float)adc_current0.Current_A_B_C[1] - PMSM[PMSM_A].CurrOffset[1])*pmsm_current_op*0.5f;
			PMSM[PMSM_A].foc.Ia_Ib_Ic.I3 = PMSM[PMSM_A].foc.Ia_Ib_Ic.I3*0.5f + ((float)adc_current0.Current_A_B_C[2] - PMSM[PMSM_A].CurrOffset[2])*pmsm_current_op*0.5f;
			handle_foc(PMSM_A);
		
	}
	
}

adc_data adc_current2;
static uint16_t ADC2_Offset = 0;
void adc2_callback_handle(void *parm)
{
	memcpy(&adc_current2,parm,sizeof(adc_data));
	
	if(ADC2_Offset < 2000)
	{
		ADC2_Offset++;
		PMSM[PMSM_U].CurrOffset[0] = PMSM[PMSM_U].CurrOffset[0] * 0.9f + (float)adc_current2.Current_A_B_C[0] * 0.1f;
		PMSM[PMSM_U].CurrOffset[1] = PMSM[PMSM_U].CurrOffset[1] * 0.9f + (float)adc_current2.Current_A_B_C[1] * 0.1f;
		PMSM[PMSM_U].CurrOffset[2] = PMSM[PMSM_U].CurrOffset[2] * 0.9f + (float)adc_current2.Current_A_B_C[2] * 0.1f;
	}
	else
	{
			if(PMSM[PMSM_U].first_offer == false)
			{
				PMSM[PMSM_U].first_offer = true;
				if(PMSM[PMSM_U].CurrOffset[0]>2048+100||PMSM[PMSM_U].CurrOffset[0]<2048-100
					||PMSM[PMSM_U].CurrOffset[1]>2048+100||PMSM[PMSM_U].CurrOffset[1]<2048-100
					||PMSM[PMSM_U].CurrOffset[2]>2048+100||PMSM[PMSM_U].CurrOffset[2]<2048-100)
				{
					PMSM[PMSM_U].PMSM_Status.bit.Current_Error = 1;
				}
				else
				{
					PMSM[PMSM_U].PMSM_Status.bit.Current_Error = 0;
				}
			}
			PMSM[PMSM_U].foc.Ia_Ib_Ic.I1 = PMSM[PMSM_U].foc.Ia_Ib_Ic.I1*0.5f + ((float)adc_current2.Current_A_B_C[0] - PMSM[PMSM_U].CurrOffset[0])*pmsm_current_op*0.5f; // ADC * (3.3/4096)/66.5/4/0.001R;//电流钳设置10mv,示波器设置X100倍
			PMSM[PMSM_U].foc.Ia_Ib_Ic.I2 = PMSM[PMSM_U].foc.Ia_Ib_Ic.I2*0.5f + ((float)adc_current2.Current_A_B_C[1] - PMSM[PMSM_U].CurrOffset[1])*pmsm_current_op*0.5f;
			PMSM[PMSM_U].foc.Ia_Ib_Ic.I3 = PMSM[PMSM_U].foc.Ia_Ib_Ic.I3*0.5f + ((float)adc_current2.Current_A_B_C[2] - PMSM[PMSM_U].CurrOffset[2])*pmsm_current_op*0.5f;
			handle_foc(PMSM_U);
		
	}
	
}



uint8_t angle_Count = 0;
bool test_falsge = false;
APP_PMSM_DRIVER APP_PMSM_Pre;

uint8_t mag_data_x[Nx]={127,127,127};
uint8_t mag_data_y[Nx]={127,127,127};

uint8_t axis_x_temp = 127; 
uint8_t axis_y_temp = 127; 
void app_angel_handle(void)
{


	if(APP_PMSM.axis_x !=APP_PMSM_Pre .axis_x)
	{
		APP_PMSM.x_new = APP_PMSM.axis_x - 127;
	}
	if(APP_PMSM.axis_y !=APP_PMSM_Pre .axis_y)
	{
		APP_PMSM.y_new = APP_PMSM.axis_y - 127;
	}

	APP_PMSM.SET_COORANGLE_10X =atan2(APP_PMSM.y_new,APP_PMSM.x_new);
	
	APP_PMSM.CoordSqrt = sqrt(APP_PMSM.x_new*APP_PMSM.x_new+APP_PMSM.y_new*APP_PMSM.y_new);
	
	if(APP_PMSM.CoordSqrt  > 80.0f)APP_PMSM.CoordSqrt  = 80.0f;
	if(APP_PMSM.CoordSqrt  < 0.01f)APP_PMSM.CoordSqrt  = 0.0f;
	
	
	APP_PMSM.CoordSqrt =APP_PMSM.CoordSqrt/100.0f;
	
	if(	APP_PMSM.SET_COORANGLE_10X < 0.0f)
		APP_PMSM.SET_COORANGLE_10X = APP_PMSM.SET_COORANGLE_10X+ PI_2;
	
	APP_PMSM_Pre = APP_PMSM;
}

uint16_t PMSMCurrentFaultNumber[2]={0};		
void PMSM_Status_Loop(void)
{
	if(PMSM[PMSM_A].MotorState == MotorRun )
	{
		if(fabs(PMSM[PMSM_A].foc.Id_Iq.I2)>PMSM[PMSM_A].Current_over)
		{
			PMSMCurrentFaultNumber[PMSM_A]++;
			if(PMSMCurrentFaultNumber[PMSM_A]>20)
			{
				PMSMCurrentFaultNumber[PMSM_A] = 0;
				PMSM[PMSM_A].PMSM_Status.bit.OverCur =1;
			}
		}
		else
		{
			PMSMCurrentFaultNumber[PMSM_A] = 0;
		}
	}
	
	if(PMSM[PMSM_U].MotorState == MotorRun )
	{
		if(fabs(PMSM[PMSM_U].foc.Id_Iq.I2)>PMSM[PMSM_U].Current_over)
		{
			PMSMCurrentFaultNumber[PMSM_U]++;
			if(PMSMCurrentFaultNumber[PMSM_U]>20)
			{
				PMSMCurrentFaultNumber[PMSM_U] = 0;
				PMSM[PMSM_U].PMSM_Status.bit.OverCur =1;
			}
		}
		else
		{
			PMSMCurrentFaultNumber[PMSM_U] = 0;
		}
	}
	
	if(PMSM[PMSM_A].PMSM_Status.bit.hall_calib_error ==1||PMSM[PMSM_A].PMSM_Status.bit.hall_Error == 1
		||PMSM[PMSM_A].PMSM_Status.bit.OverTemp == 1||PMSM[PMSM_A].PMSM_Status.bit.OverCur == 1)
	{
		APP_PMSM.APP_ERROR.bit.PMSM_A_ERROR = 1;
	}
	else
	{
		APP_PMSM.APP_ERROR.bit.PMSM_A_ERROR = 0;
	}
	
	if(PMSM[PMSM_U].PMSM_Status.bit.hall_calib_error ==1||PMSM[PMSM_U].PMSM_Status.bit.hall_Error == 1
		||PMSM[PMSM_U].PMSM_Status.bit.OverTemp == 1||PMSM[PMSM_U].PMSM_Status.bit.OverCur == 1)
	{
		APP_PMSM.APP_ERROR.bit.PMSM_U_ERROR = 1;
	}
	else
	{
		APP_PMSM.APP_ERROR.bit.PMSM_U_ERROR = 0;
	}
	
	if(PMSM[PMSM_A].PMSM_Status.bit.Bandbrake == 1||PMSM[PMSM_U].PMSM_Status.bit.Bandbrake == 1)
	{
		APP_PMSM.APP_ERROR.bit.BREAK_ERROR =1;
	}
	else
	{
		APP_PMSM.APP_ERROR.bit.BREAK_ERROR =0;
	}
	
}





uint16_t count_stop = 0;
void PMSM_APP_LOOP(void)
{
	if(PMSM[PMSM_A].PMSM_Status.bit.hall_calib_error!=1&&PMSM[PMSM_A].PMSM_Status.bit.hall_Error!=1&&PMSM[PMSM_A].PMSM_Status.bit.Current_Error!=1
		&&PMSM[PMSM_U].PMSM_Status.bit.hall_calib_error!=1&&PMSM[PMSM_U].PMSM_Status.bit.hall_Error!=1&&PMSM[PMSM_U].PMSM_Status.bit.Current_Error!=1)
	{
		app_angel_handle();
		
		if(APP_PMSM.CoordSqrt>0.0F)
		{
			if(PMSM[PMSM_A].MotorState != MotorRun&&PMSM[PMSM_A].first_start == false
				&&PMSM[PMSM_U].MotorState != MotorRun&&PMSM[PMSM_U].first_start == false)
			{
				PMSM[PMSM_A].MotorState = MotorStart;
				PMSM[PMSM_A].first_start = true;
				PMSM[PMSM_U].MotorState = MotorStart;
				PMSM[PMSM_U].first_start = true;
			}
			PMSM[PMSM_U].first_stop = false;
			PMSM[PMSM_A].first_stop = false;

			count_stop = 0;
		}
		
		else if(PMSM[PMSM_A].SpeedSet == 0.0f&&PMSM[PMSM_U].SpeedSet == 0.0f)
		{
			if(++count_stop>100*10)
			{
				
				if(PMSM[PMSM_A].MotorState != MotorStop&&PMSM[PMSM_A].first_stop == false
					&&PMSM[PMSM_U].MotorState != MotorStop&&PMSM[PMSM_U].first_stop == false)
				{
					PMSM[PMSM_A].S_low_acc = 0.8f;
					PMSM[PMSM_A].S_Add_acc = 3.0f;
	
					PMSM[PMSM_A].MotorState = MotorStop;
					PMSM[PMSM_A].first_stop = true;
					PMSM[PMSM_U].MotorState = MotorStop;
					PMSM[PMSM_U].S_low_acc = 0.8f;
					PMSM[PMSM_U].S_Add_acc = 3.0f;
					PMSM[PMSM_U].first_stop = true;
				}
				PMSM[PMSM_A].first_start = false;
				PMSM[PMSM_U].first_start = false;
			}
		}
		else
		{
			count_stop = 0;
		}
		
	}
}

uint16_t test_count  = 0;
uint8_t flage_test = 0;
void PMSM_TEST_LOOP(void)
{
	if(flage_test == 0)
	{
		if(++test_count>=30)//3s启动
		{
			test_count = 0;
			flage_test = 1;//下一个状态
			PMSM[PMSM_A].SpeedNew = 1500.0f;
			PMSM[PMSM_U].SpeedNew = 1500.0f;
			return;
		}
	}
	if(flage_test == 1)
	{
		if(++test_count>=10*60*3)//运行3分钟
		{
			test_count = 0;
			flage_test = 2;	//下一个状态		
			PMSM[PMSM_A].SpeedNew = -1500.0f;
			PMSM[PMSM_U].SpeedNew = -1500.0f;
			return;
		}
	}
	if(flage_test == 2)
	{
		if(++test_count>=10*60*3)//运行3分钟
		{
			test_count = 0;
			flage_test = 3;	//下一个状态		
			PMSM[PMSM_A].SpeedNew = 1500.0f;
			PMSM[PMSM_U].SpeedNew = 1500.0f;
			return;
		}
	}
	if(flage_test == 3)
	{
		test_count = 0;
		flage_test = 1;	//下一个状态		
		return;
	}
}




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

#define kk 0.5714285f
#define bb 128.57142857f
#define acc_k 0.00085f
#define acc_b	212.5f
uint16_t acc_count[2] ={0};


#define threshold_speed 100.0f
#define bx  -1.485f														//380ms
#define k  0.143564356f
float con_dtheta[2][310] ={0.0f};
void speed_temp_handle(PMSM_Num num)
{
	float count_temp_turn = 0.0f;
	PMSM[num].SpeedTemp 	= PMSM[num].SpeedNew_100MS - PMSM[num].SpeedSet;
	if(PMSM[num].SpeedTemp > PMSM[num].SpeedTemp/5*APP_PMSM.Gear)
	{
		PMSM[num].SpeedTemp = PMSM[num].SpeedTemp/5*APP_PMSM.Gear;
	}
	else if(PMSM[num].SpeedTemp < PMSM[num].SpeedTemp/5*APP_PMSM.Gear)
	{
		PMSM[num].SpeedTemp =  PMSM[num].SpeedTemp/5*APP_PMSM.Gear;
	}
}
void Normal_distribution(PMSM_Num num)
{
	int i =0;
	if(PMSM[num].SpeedNew_100MS != PMSM[num].LastSpeedNew_100MS)
	{
		PMSM[num].LastSpeedNew_100MS = PMSM[num].SpeedNew_100MS;
		speed_temp_handle(num);
		if(fabs((float)PMSM[num].SpeedTemp)>=threshold_speed/5*APP_PMSM.Gear)
		{
			PMSM[num].count_buffers=(uint16_t)(k*fabs((float)PMSM[num].SpeedTemp)+bx +0.5f);
		}
		else
		{
			PMSM[num].count_buffers = (uint16_t)bx;
		}
		PMSM[num].count_put = 0;
		PMSM[num].mean_value = PMSM[num].count_buffers/2.0f;
		PMSM[num].variance = PMSM[num].mean_value/3.0f;
		for(i = 0;i<=PMSM[num].count_buffers;i++)
		{
			con_dtheta[num][i] = (1.0f/(pow((2.0f*PI),0.5f)*PMSM[num].variance)) * exp(-(pow(((float)(i+1)-PMSM[num].mean_value),2))/(2*pow(PMSM[num].variance,2)) );
			con_dtheta[num][i] = con_dtheta[num][i]*PMSM[num].SpeedTemp/0.9973f;
		}
	}
}

float put_s_value(PMSM_Num num)
{
		if(PMSM[num].count_put<PMSM[num].count_buffers)
		{	
			PMSM[num].SpeedSet = PMSM[num].SpeedSet + con_dtheta[num][PMSM[num].count_put] ;//正向速度坡度
			PMSM[num].count_put ++;	
		}
		else if(PMSM[num].count_put == PMSM[num].count_buffers&&PMSM[num].count_put != 0)
		{
			PMSM[num].SpeedSet = PMSM[num].SpeedNew_100MS ;
		}
}
void Speed_Status_Handle(void)
{
	if(((fabs(PMSM[PMSM_A].SpeedSet-PMSM[PMSM_A].SpeedNew)<15.0f&&fabs(PMSM[PMSM_U].SpeedSet-PMSM[PMSM_U].SpeedNew*(-1))<15.0f)
			||(PMSM[PMSM_A].S_low_acc >= PMSM[PMSM_A].S_Add_acc&&PMSM[PMSM_U].S_low_acc >= PMSM[PMSM_U].S_Add_acc))
			&&PMSM[PMSM_A].acc_status == start_acc&&PMSM[PMSM_U].acc_status == start_acc
			&&(fabs((float)PMSM[PMSM_A].SpeedNew)>=450.0f&&fabs((float)PMSM[PMSM_U].SpeedNew)>=450.0f)
			)
	{
		PMSM[PMSM_A].acc_status = run_acc;
		PMSM[PMSM_U].acc_status = run_acc;
	}
	if((fabs((float)PMSM[PMSM_A].SpeedNew)==0.0f&&fabs((float)PMSM[PMSM_U].SpeedNew)==0.0f)
	&&((PMSM[PMSM_A].acc_status == run_acc&&PMSM[PMSM_U].acc_status == run_acc)||(PMSM[PMSM_A].acc_status == start_acc&&PMSM[PMSM_U].acc_status == start_acc)))
	{
		PMSM[PMSM_A].acc_status = dec_acc;
		PMSM[PMSM_U].acc_status = dec_acc;
	}
}

uint16_t RUN_COUNT = 0;
void	Speed_Run_handle(PMSM_Num num)
{
	#if 0
	if(fabs(PMSM[PMSM_A].SpeedSet - PMSM[PMSM_A].SpeedNew)<15.0f&&PMSM[PMSM_A].acc_status == start_acc
		&&fabs((float)PMSM[PMSM_A].SpeedNew_100MS)>50.0f&&fabs(PMSM[PMSM_U].SpeedSet - PMSM[PMSM_U].SpeedNew*(-1))<15.0f
		&&PMSM[PMSM_U].acc_status == start_acc&&fabs((float)PMSM[PMSM_U].SpeedNew_100MS)>50.0f)
		{
			if(++RUN_COUNT>=5)
			{
				RUN_COUNT = 0;
				PMSM[PMSM_A].acc_status = run_acc;
				PMSM[PMSM_U].acc_status = run_acc;
			}
		}
		else
		{
			RUN_COUNT = 0;
		}

	if(PMSM[num].acc_status == start_acc)
	{
//		if(PMSM[num].S_low_acc<3.0f)PMSM[num].S_low_acc = PMSM[num].S_low_acc+0.0006f;
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
	else if(PMSM[num].acc_status == run_acc)
	{
		PMSM[num].SpeedSet = PMSM[num].SpeedNow;
	}
	#endif
	#if 0
//	if(fabs(fabs(PMSM[num].SpeedNow)-fabs(PMSM[num].SpeedSet))>PMSM[num].S_Run_acc)
//	{
//		if(PMSM[num].SpeedSet < PMSM[num].SpeedNow)
//		{
//				
//			PMSM[num].SpeedSet = PMSM[num].SpeedSet + PMSM[num].S_Run_acc ;//正向速度坡度
//			if(PMSM[num].SpeedSet > PMSM[num].SpeedNow)
//			{
//				PMSM[num].SpeedSet = PMSM[num].SpeedNow ;
//			}
//		}
//		else if(PMSM[num].SpeedSet > PMSM[num].SpeedNow)
//		{
//			PMSM[num].SpeedSet = PMSM[num].SpeedSet - PMSM[num].S_Run_acc ;//反向速度坡度
//			if(PMSM[num].SpeedSet < PMSM[num].SpeedNow)
//			{
//				PMSM[num].SpeedSet = PMSM[num].SpeedNow ;
//			}
//		}
//	}
//	else
//	{
//		PMSM[num].SpeedSet = PMSM[num].SpeedNow;
//	}
	#endif
	#if 1

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
	#endif
	#if 0
	if(++PMSM[num].count_10ms>=10)
	{
		PMSM[num].count_10ms = 0;
		put_s_value(num);
	}
	
	#endif
	
	
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

float current_datio =  0.00005f;
#define forward_current_max 2.0f
#define forward_current_min 0.1f
void Current_forward_handle(PMSM_Num num)
{
	if(fabs(PMSM[num].SpeedSet)<150.0f&&fabs(PMSM[num].SpeedSet)>10.0f)
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
	
	if(fabs(PMSM[num].SpeedSet)<150.0f&&fabs(PMSM[num].SpeedSet)>10.0f)
	{
		PMSM[num].AngleSet = hall_encoder[num].AngleReal +	PMSM[num].SpeedSet/my_rpm*my_unit_angle/5000.0f;
		
		PID_run_FloatspdVolt(&PMSM[num].angle_pid,PMSM[num].AngleSet,hall_encoder[num].AngleReal);
	}
	else
	{
		clear_pid(&PMSM[num].angle_pid);
	}


}
uint16_t count_test[2] ={0};
float ids = -0.0f;
static void Speed_handle(PMSM_Num num)
{
	PMSM[num].foc.Theta = hall_encoder[num].Theta;
	if(++PMSM[num].speed_pid_count>= 2)
	{
		PMSM[num].speed_pid_count = 0;
		speed_forward_handle(num);
//		PID_run_FloatspdVolt(&PMSM[num].speed_pid,PMSM[num].SpeedSet+PMSM[num].angle_pid.PIDOut,hall_encoder[num].PLL_realspeed);
		PID_run_FloatspdVolt(&PMSM[num].speed_pid,PMSM[num].SpeedSet+PMSM[num].angle_pid.PIDOut,hall_encoder[num].real_speed);
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



int16_t walking_speed[5] = {
	500,1300,2000,2800,3500
};
float M_RAT = 0.37f;

#define cos_theta 0.39714789f
#define sin_theta 0.9177546256839f
#define temp_b_ratio	0.16f
#define k_4 (2.0f*cos_theta*0.65f-2.0f*temp_b_ratio)

void angle_inverse_kinematics(void)
{
	
	if(PMSM[PMSM_A].SpeedSet < 0 && -PMSM[PMSM_U].SpeedSet < 0){
		APP_PMSM.a_temp_new = 0.5f;
//		subSpeed = 
//		PMSM[PMSM_A].SpeedSet > -PMSM[PMSM_U].SpeedSet ? 
//		PMSM[PMSM_A].SpeedSet : -PMSM[PMSM_U].SpeedSet ;

		PMSM[PMSM_A].appspeed_l = PMSM[PMSM_A].SpeedSet/(1.0f+APP_PMSM.a_temp_new);
		PMSM[PMSM_U].appspeed_r = -PMSM[PMSM_U].SpeedSet/(1.0f+APP_PMSM.a_temp_new);
	// 增大前进
	}else if(PMSM[PMSM_A].SpeedSet > 0 && -PMSM[PMSM_U].SpeedSet > 0){
		APP_PMSM.a_temp_new = 0.1f;
//		subSpeed = 
//		PMSM[PMSM_A].SpeedSet < -PMSM[PMSM_U].SpeedSet ? 
//		PMSM[PMSM_A].SpeedSet : -PMSM[PMSM_U].SpeedSet ;

		PMSM[PMSM_A].appspeed_l = PMSM[PMSM_A].SpeedSet/(1.0f+APP_PMSM.a_temp_new);
		PMSM[PMSM_U].appspeed_r = -PMSM[PMSM_U].SpeedSet/(1.0f+APP_PMSM.a_temp_new);

	}
	else
	{
		PMSM[PMSM_A].appspeed_l = PMSM[PMSM_A].SpeedSet;
		PMSM[PMSM_U].appspeed_r = -PMSM[PMSM_U].SpeedSet;
	}
	
	APP_PMSM.c_acc_d = (PMSM[PMSM_A].appspeed_l + PMSM[PMSM_U].appspeed_r)/2.0f/0.65f/sin_theta;
	APP_PMSM.c_dec_2b = (PMSM[PMSM_A].appspeed_l - PMSM[PMSM_U].appspeed_r)/k_4;
	APP_PMSM.temp_tanf = atan2f(APP_PMSM.c_acc_d,APP_PMSM.c_dec_2b);
	APP_PMSM.vector = PMSM[PMSM_A].appspeed_l/(sin_theta*0.65f*sinf(APP_PMSM.temp_tanf)+cos_theta*0.65f*cosf(APP_PMSM.temp_tanf) - cosf(APP_PMSM.temp_tanf)*temp_b_ratio)*100.0f/walking_speed[APP_PMSM.Gear-1] ;
	
	if(APP_PMSM.vector == 0.0f) APP_PMSM.vector=5.0f;
	if(APP_PMSM.vector  > 100.0f)APP_PMSM.vector  = 100.0f;
	
}
void angele_fitting(void)
{
	APP_PMSM.a_temp = 0.65f;
	APP_PMSM.a = APP_PMSM.Gear*1.75f;
	if(APP_PMSM.last_angle != APP_PMSM.angle&&fabs((float)APP_PMSM.angle - APP_PMSM.last_angle)>3.0f&&fabs((float)APP_PMSM.angle - APP_PMSM.last_angle)<357.0f)
	{
		APP_PMSM.CoordSpeed = walking_speed[APP_PMSM.Gear-1]*APP_PMSM.vector/100.0f;
//		if(APP_PMSM.angle>=0&&APP_PMSM.angle<=90) APP_PMSM.CoordSpeed = APP_PMSM.SET_COORANGLE;
//		APP_PMSM.CoordSpeed = APP_PMSM.Gear*50.0f+50.0f;
		APP_PMSM.a_temp_new = 0.65f;
		APP_PMSM.b_temp_new = cosf(APP_PMSM.temp_tanf)*APP_PMSM.CoordSpeed/(APP_PMSM.Gear/((APP_PMSM.Gear-1)/5.0f));
		APP_PMSM.c_new = APP_PMSM.CoordSpeed*cosf((APP_PMSM.temp_tanf - M_RAT*PI))*APP_PMSM.a_temp_new;
		APP_PMSM.d_new = APP_PMSM.CoordSpeed*sinf((APP_PMSM.temp_tanf - 1.0f/2.0f*PI + M_RAT*PI))*APP_PMSM.a_temp_new;
		APP_PMSM.l_speed =  (APP_PMSM.c_new - APP_PMSM.b_temp_new);
		APP_PMSM.r_speed = (APP_PMSM.d_new + APP_PMSM.b_temp_new);
		
		int16_t subSpeedX;
		//减小后退
		if(APP_PMSM.l_speed < 0 && APP_PMSM.r_speed< 0){
			APP_PMSM.a_temp_new = 0.5f;
			subSpeedX = 
			APP_PMSM.l_speed > APP_PMSM.r_speed ? 
			APP_PMSM.l_speed : APP_PMSM.r_speed ;

			APP_PMSM.l_speed -= subSpeedX*APP_PMSM.a_temp_new;
			APP_PMSM.r_speed -= subSpeedX*APP_PMSM.a_temp_new;
		// 增大前进
		}else if(APP_PMSM.l_speed > 0 && APP_PMSM.r_speed > 0){
			APP_PMSM.a_temp_new = 0.1f;
			subSpeedX = 
			APP_PMSM.l_speed < APP_PMSM.r_speed ? 
			APP_PMSM.l_speed : APP_PMSM.r_speed ;

			APP_PMSM.l_speed += subSpeedX*APP_PMSM.a_temp_new;
			APP_PMSM.r_speed += subSpeedX*APP_PMSM.a_temp_new;

		}
		APP_PMSM.start_first = 1;
		PMSM[PMSM_A].SpeedCmd = APP_PMSM.r_speed;
		PMSM[PMSM_U].SpeedCmd = -APP_PMSM.l_speed;
		if(PMSM[PMSM_A].SpeedNew < PMSM[PMSM_A].SpeedSet)
		{
			PMSM[PMSM_A].S_Run_acc = 1.5f;
		}
		else
		{
			PMSM[PMSM_A].S_Run_acc = 0.8f;
		}
		if(PMSM[PMSM_U].SpeedNew < -PMSM[PMSM_U].SpeedSet)
		{
			PMSM[PMSM_U].S_Run_acc = 1.5f;
		}
		else
		{
			PMSM[PMSM_U].S_Run_acc = 0.8f;
		}
		
	}
	
	
	
	
}

void app_angel_handle(void)
{
	APP_PMSM.x_new = APP_PMSM.axis_x - 127;
	APP_PMSM.y_new = APP_PMSM.axis_y - 127;
	APP_PMSM.SET_COORANGLE_10X =atan2(APP_PMSM.y_new,APP_PMSM.x_new);
	
	APP_PMSM.CoordSqrt = sqrt(APP_PMSM.x_new*APP_PMSM.x_new+APP_PMSM.y_new*APP_PMSM.y_new);
	
	if(APP_PMSM.CoordSqrt  > 70.0f)APP_PMSM.CoordSqrt  = 70.0f;
	
	APP_PMSM.CoordSqrt =APP_PMSM.CoordSqrt/100.0f;
	
	APP_PMSM.SET_COORANGLE_10X =APP_PMSM.SET_COORANGLE_10X ;
	if(	APP_PMSM.SET_COORANGLE_10X < 0.0f)
		APP_PMSM.SET_COORANGLE_10X = APP_PMSM.SET_COORANGLE_10X+ PI_2;
	
	running_programme(SCURVE_1);
	running_programme(SCURVE_2);
	
#if 0
	APP_PMSM.CoordSpeed_new = walking_speed[APP_PMSM.Gear - 1]*APP_PMSM.CoordSqrt/100.0f;
	
	if(APP_PMSM.axis_x == 127&&APP_PMSM.axis_y == 127)
	{
		PMSM[PMSM_A].SpeedNew = 0;
		PMSM[PMSM_U].SpeedNew = 0;
		APP_PMSM.axis_y = 127;
		APP_PMSM.axis_y_filter = 127.0f;
		APP_PMSM.axis_x = 127;
	}
	else
	{
		if(APP_PMSM.Gear>1)
		{
			APP_PMSM.a_temp = 0.65f;
			APP_PMSM.a = APP_PMSM.Gear*1.75f;
			APP_PMSM.b_temp = cosf(APP_PMSM.SET_COORANGLE_10X)*APP_PMSM.CoordSpeed_new/(APP_PMSM.Gear/((APP_PMSM.Gear-1)/5.0f));
			APP_PMSM.c = APP_PMSM.CoordSpeed_new*cosf((APP_PMSM.SET_COORANGLE_10X - M_RAT*PI))*APP_PMSM.a_temp;
			APP_PMSM.d = APP_PMSM.CoordSpeed_new*sinf((APP_PMSM.SET_COORANGLE_10X - 1.0f/2.0f*PI + M_RAT*PI))*APP_PMSM.a_temp;
		}
		else
		{
			APP_PMSM.b_temp = cosf(APP_PMSM.SET_COORANGLE_10X)*APP_PMSM.CoordSpeed_new*0.01f;
			APP_PMSM.c = APP_PMSM.CoordSpeed_new*cosf((APP_PMSM.SET_COORANGLE_10X - M_RAT*PI))*APP_PMSM.a_temp;
			APP_PMSM.d = APP_PMSM.CoordSpeed_new*sinf((APP_PMSM.SET_COORANGLE_10X - 1.0f/2.0f*PI + M_RAT*PI))*APP_PMSM.a_temp;
		}
		
		PMSM[PMSM_A].SpeedNew =  APP_PMSM.c - APP_PMSM.b_temp;
		PMSM[PMSM_U].SpeedNew =  APP_PMSM.d + APP_PMSM.b_temp;
		
		int16_t subSpeed;
		//减小后退
		if(PMSM[PMSM_A].SpeedNew < 0 && PMSM[PMSM_U].SpeedNew < 0){
			APP_PMSM.a_temp = 0.5f;
			subSpeed = 
			PMSM[PMSM_A].SpeedNew > PMSM[PMSM_U].SpeedNew ? 
			PMSM[PMSM_A].SpeedNew : PMSM[PMSM_U].SpeedNew ;

			PMSM[PMSM_A].SpeedNew -= subSpeed*APP_PMSM.a_temp;
			PMSM[PMSM_U].SpeedNew -= subSpeed*APP_PMSM.a_temp;
		// 增大前进
		}else if(PMSM[PMSM_A].SpeedNew > 0 && PMSM[PMSM_U].SpeedNew > 0){
			APP_PMSM.a_temp = 0.1f;
			subSpeed = 
			PMSM[PMSM_A].SpeedNew < PMSM[PMSM_U].SpeedNew ? 
			PMSM[PMSM_A].SpeedNew : PMSM[PMSM_U].SpeedNew ;

			PMSM[PMSM_A].SpeedNew += subSpeed*APP_PMSM.a_temp;
			PMSM[PMSM_U].SpeedNew += subSpeed*APP_PMSM.a_temp;

		}
	}
	#endif
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
}




void PMSM_Mode_Loop(PMSM_Num num)
{
	if(PMSM[num].PMSM_Status.bit.hall_calib_error!=1
		&&PMSM[num].PMSM_Status.bit.hall_Error!=1
		&&PMSM[num].PMSM_Status.bit.Current_Error!=1)
	{
		if(num == PMSM_U) PMSM[num].SpeedNow = (float)PMSM[num].SpeedNew*(-1.0f);
		else	PMSM[num].SpeedNow = (float)PMSM[num].SpeedNew;
#if 0
		switch(PMSM[num].MotorPreState)
		{
			case MotorStart:
				if(PMSM[num].MotorState != MotorRun&&PMSM[num].first_start == false)
				{
					PMSM[num].MotorState = MotorStart;
					PMSM[num].first_start = true;
				}
				PMSM[num].first_stop = false;
				break;
			case MotorStop:
				if(PMSM[num].MotorState != MotorStop&&PMSM[num].first_stop == false)
				{
					PMSM[num].MotorState = MotorStop;
					PMSM[num].first_stop = true;
				}
				PMSM[num].first_start = false;
				break;
			default:
				break;		
			
		}
		
		if(PMSM[num].first_start == true&&PMSM[num].MotorState == MotorRun)
		{
			Speed_Curve_handle(num);
		}
#endif
	}
}
int16_t mag_data_a[Nx]={0};
int16_t mag_data_u[Nx]={0};

int16_t mag_data_ay[Nx]={0};


void SPEED_STATUS_HANDLE_A(void)
{
	if(++PMSM[PMSM_A].count_100ms == 100)
	{
		PMSM[PMSM_A].count_100ms = 0;
		PMSM[PMSM_A].SpeedNew_100MS = PMSM[PMSM_A].SpeedNew;
	}
#if 1
		if(PMSM[PMSM_A].SpeedNew_100MS>0&&PMSM[PMSM_A].SpeedSet>0.0f)
		{
			PMSM[PMSM_A].speed_status = hight_hight;
		}
		else if(PMSM[PMSM_A].SpeedNew_100MS>0&&PMSM[PMSM_A].SpeedSet<0.0f)
		{
			PMSM[PMSM_A].speed_status = low_hight;
		}
		else if(PMSM[PMSM_A].SpeedNew_100MS<0&&PMSM[PMSM_A].SpeedSet<0)
		{
			PMSM[PMSM_A].speed_status = low_low;
		}
		else if(PMSM[PMSM_A].SpeedNew_100MS<0&&PMSM[PMSM_A].SpeedSet>0)
		{
			PMSM[PMSM_A].speed_status = hight_low;
		}
#endif
}

void SPEED_STATUS_HANDLE_U(void)
{
//	if(PMSM[PMSM_U].SpeedNew != PMSM[PMSM_U].LastSpeedNew)
//	{
//		PMSM[PMSM_U].LastSpeedNew = PMSM[PMSM_U].SpeedNew ;
//		PMSM[PMSM_U].count_100ms ++;
//	}
	if(++PMSM[PMSM_U].count_100ms == 100)
	{
		PMSM[PMSM_U].count_100ms = 0;
		PMSM[PMSM_U].SpeedNew_100MS = PMSM[PMSM_U].SpeedNew*(-1);
	}
//	if(PMSM[PMSM_U].LastSpeedNew_100MS!=PMSM[PMSM_U].SpeedNew_100MS)
//	{
	#if 1
		if(PMSM[PMSM_U].SpeedNew_100MS<0&&PMSM[PMSM_U].SpeedSet<0.0f)
		{
			PMSM[PMSM_U].speed_status = hight_hight;
		}
		else if(PMSM[PMSM_U].SpeedNew_100MS<0&&PMSM[PMSM_U].SpeedSet>0.0f)
		{
			PMSM[PMSM_U].speed_status = low_hight;
		}
		else if(PMSM[PMSM_U].SpeedNew_100MS>0&&PMSM[PMSM_U].SpeedSet>0.0f)
		{
			PMSM[PMSM_U].speed_status = low_low;
		}
		else if(PMSM[PMSM_U].SpeedNew_100MS>0&&PMSM[PMSM_U].SpeedSet<0.0f)
		{
			PMSM[PMSM_U].speed_status = hight_low;
		}
	#endif
//		else if(PMSM[PMSM_U].SpeedNew_100MS == 0&&PMSM[PMSM_U].LastSpeedNew_100MS == 0)
//		{
//			PMSM[PMSM_U].speed_status = zero_zero;
//		}
}

void Wheel_Speed_Handle(void)
{
	if(PMSM[PMSM_A].SpeedNew_100MS > 0&&PMSM[PMSM_U].SpeedNew_100MS<0 )
	{
		if(PMSM[PMSM_A].SpeedNew_100MS < PMSM[PMSM_A].SpeedNew)
		{
			if(fabs((float)PMSM[PMSM_A].SpeedNew - PMSM[PMSM_A].SpeedNew_100MS)>100)
			{
				PMSM[PMSM_A].quadrant = firt_q;
				if(PMSM[PMSM_A].SpeedNew_100MS>PMSM[PMSM_U].SpeedNew_100MS*(-1))
				{
					PMSM[PMSM_U].quadrant = firt_q;
				}
				else
				{
					PMSM[PMSM_U].quadrant = sec_q;
				}
			}
		}
		else
		{
			PMSM[PMSM_A].quadrant = sec_q;
		}
	}
	else if(PMSM[PMSM_A].SpeedNew_100MS < 0&&PMSM[PMSM_U].SpeedNew_100MS>0)
	{
		if(PMSM[PMSM_A].SpeedNew_100MS > PMSM[PMSM_A].SpeedNew&&fabs((float)PMSM[PMSM_A].SpeedNew - PMSM[PMSM_A].SpeedNew_100MS)>100)
		{
			if(fabs((float)PMSM[PMSM_A].SpeedNew - PMSM[PMSM_A].SpeedNew_100MS)>100)
			{
				PMSM[PMSM_A].quadrant = firt_q;
				if(PMSM[PMSM_A].SpeedNew_100MS>PMSM[PMSM_U].SpeedNew_100MS*(-1))
				{
					PMSM[PMSM_U].quadrant = third_q;
				}
				else
				{
					PMSM[PMSM_U].quadrant = four_q;
				}
			}
		}
		else
		{
			PMSM[PMSM_A].quadrant = sec_q;
		}
	}
}
float angle_speed = 0.0f;
void SPEED_LOOP(void)
{
//	Wheel_Speed_Handle();
//	switch(PMSM[PMSM_A].quadrant)
//	{
//		case firt_q:
//			if((PMSM[PMSM_A].speed_status<<4|PMSM[PMSM_U].speed_status)!=0x40&&(PMSM[PMSM_A].speed_status<<4|PMSM[PMSM_U].speed_status)!=0x04)
//			{
//				PMSM[PMSM_A].sec_speed_handle = rise_first;
//				PMSM[PMSM_U].sec_speed_handle = rise_first;
//				PMSM[PMSM_A].S_low_acc = 1.0f;
//				PMSM[PMSM_U].S_low_acc = 1.0f;
////				PMSM[PMSM_A].SpeedCmd = PMSM[PMSM_A].SpeedSet;
////				PMSM[PMSM_U].SpeedCmd =  -PMSM[PMSM_A].SpeedSet;
//				PMSM[PMSM_A].SpeedCmd = (PMSM[PMSM_A].SpeedSet - PMSM[PMSM_U].SpeedSet)/2.0f;
//				PMSM[PMSM_U].SpeedCmd = (PMSM[PMSM_A].SpeedSet - PMSM[PMSM_U].SpeedSet)/2.0f;
//			}
//			break;
//		default:
//			break;

//	}
//	angle_speed = atan2(PMSM[PMSM_A].SpeedNew,PMSM[PMSM_U].SpeedNew);
	switch(PMSM[PMSM_A].speed_status<<4|PMSM[PMSM_U].speed_status)
	{
		case 0x11:
				PMSM[PMSM_A].sec_speed_handle = dir_first;
				PMSM[PMSM_U].sec_speed_handle = dir_first;
				PMSM[PMSM_A].SpeedCmd = 0.0f;
				PMSM[PMSM_U].SpeedCmd = 0.0f;
				PMSM[PMSM_A].dec_ratio = (float)APP_PMSM.Gear + 3.0f;
				PMSM[PMSM_U].dec_ratio = (float)APP_PMSM.Gear + 3.0f;
			break;
		case 0x44:
				PMSM[PMSM_A].sec_speed_handle = dir_first;
				PMSM[PMSM_U].sec_speed_handle = dir_first;
				PMSM[PMSM_A].SpeedCmd = 0.0f;
				PMSM[PMSM_U].SpeedCmd = 0.0f;
				PMSM[PMSM_A].dec_ratio = (float)APP_PMSM.Gear*0.25f+1.75f;
				PMSM[PMSM_U].dec_ratio = (float)APP_PMSM.Gear*0.25f+1.75f;
			break;
		default:
//			PMSM[PMSM_A].S_low_acc = 0.5f;
//			PMSM[PMSM_U].S_low_acc = 0.5f;
			break;
	}
	switch(PMSM[PMSM_A].sec_speed_handle<<4|PMSM[PMSM_U].sec_speed_handle)
	{
		case 0x22:
			if(fabs(hall_encoder[PMSM_A].PLL_realspeed)<20.0f
				&&fabs(hall_encoder[PMSM_U].PLL_realspeed)<20.0f)
			{
				PMSM[PMSM_A].sec_speed_handle = dir_sec;
				PMSM[PMSM_U].sec_speed_handle = dir_sec;
				PMSM[PMSM_A].S_low_acc = 0.8f;
				PMSM[PMSM_U].S_low_acc = 0.8f;
				PMSM[PMSM_A].acc_status = start_acc;
				PMSM[PMSM_U].acc_status = start_acc;
			}
			break;
//		case 0x55:
//			if((fabs(PMSM[PMSM_A].SpeedCmd - PMSM[PMSM_A].SpeedSet)<15.0f)&
//				(fabs(PMSM[PMSM_U].SpeedCmd - PMSM[PMSM_U].SpeedSet)<15.0f))
//			{
//				PMSM[PMSM_A].sec_speed_handle = wheel_sec;
//				PMSM[PMSM_U].sec_speed_handle = wheel_sec;
//				PMSM[PMSM_A].S_low_acc = 1.0f;
//				PMSM[PMSM_U].S_low_acc = 1.0f;
//				PMSM[PMSM_A].acc_status = start_acc;
//				PMSM[PMSM_U].acc_status = start_acc;
//			}
//			break;
	}

	switch(APP_PMSM.start_first)
	{
		case 1:
//			if(PMSM[PMSM_A].SpeedCmd == PMSM[PMSM_A].SpeedSet&&
//			PMSM[PMSM_U].SpeedCmd == PMSM[PMSM_U].SpeedSet)
			if(fabs(PMSM[PMSM_A].SpeedSet+PMSM[PMSM_U].SpeedSet)<30)
			{
				APP_PMSM.start_first = 2;
			}
			break;
		case 2:
			if(PMSM[PMSM_A].SpeedNew < PMSM[PMSM_A].SpeedSet)
			{
				PMSM[PMSM_A].S_low_acc = 2.5f;
			}
			else
			{
				PMSM[PMSM_A].S_low_acc = 0.8f;
			}
			if(PMSM[PMSM_U].SpeedNew < -PMSM[PMSM_U].SpeedSet)
			{
				PMSM[PMSM_U].S_low_acc = 2.5f;
			}
			else
			{
				PMSM[PMSM_U].S_low_acc = 0.8f;
			}
			break;
	}
	
//	if(PMSM[PMSM_A].SpeedNew > PMSM[PMSM_A].SpeedSet&&PMSM[PMSM_U].SpeedNew > -PMSM[PMSM_U].SpeedSet)
//	{
//		PMSM[PMSM_A].S_low_acc = 0.8f;
//		PMSM[PMSM_U].S_low_acc = 0.8f;
//	}
		
}

void ANGEL_APP_HANDLE(void)
{
	
	SPEED_STATUS_HANDLE_A();
	SPEED_STATUS_HANDLE_U();
	if(++APP_PMSM.count_angle == 100)
	{
		APP_PMSM.count_angle = 0;	
#if 0		
		if(APP_PMSM.angle>=0&&APP_PMSM.angle<90)
		{
			if((APP_PMSM.angle - APP_PMSM.last_angle)>3)
			{
				APP_PMSM.rise_low = rise_speed;
			}
			else if((APP_PMSM.angle - APP_PMSM.last_angle)<=3&&(APP_PMSM.angle - APP_PMSM.last_angle)>=0)
			{
				APP_PMSM.rise_low = rise_mid_speed;
			}
			else if((APP_PMSM.angle - APP_PMSM.last_angle)>-3)
			{
				APP_PMSM.rise_low = low_speed;
			}
		}
		else if(APP_PMSM.angle>=90&&APP_PMSM.angle<180)
		{
			if(APP_PMSM.last_angle>=90&&APP_PMSM.last_angle<180)
			{
				APP_PMSM.quadrant_t = sec_q;
			}
			if((APP_PMSM.last_angle - APP_PMSM.angle)>10)
			{
				APP_PMSM.rise_low = rise_speed;
			}
			else if((APP_PMSM.last_angle - APP_PMSM.angle)<10&&(APP_PMSM.last_angle - APP_PMSM.angle)>=0)
			{
				APP_PMSM.rise_low = mid_speed;
			}
			else if((APP_PMSM.last_angle - APP_PMSM.angle)<0)
			{
				APP_PMSM.rise_low = low_speed;
			}
		}
		else if(APP_PMSM.angle<0&&APP_PMSM.angle>-990)
		{
			if(APP_PMSM.last_angle<0&&APP_PMSM.last_angle>-90)
			{
				APP_PMSM.quadrant_t = four_q;
			}
			if((APP_PMSM.last_angle - APP_PMSM.angle)>10)
			{
				APP_PMSM.rise_low = rise_speed;
			}
			else if((APP_PMSM.last_angle - APP_PMSM.angle)<10&&(APP_PMSM.last_angle - APP_PMSM.angle)>=0)
			{
				APP_PMSM.rise_low = mid_speed;
			}
			else if((APP_PMSM.last_angle - APP_PMSM.angle)<0)
			{
				APP_PMSM.rise_low = low_speed;
			}
		}
		else if(APP_PMSM.angle<-90&&APP_PMSM.angle>=-180)
		{
			if(APP_PMSM.last_angle<-90&&APP_PMSM.last_angle>-180)
			{
				APP_PMSM.quadrant_t = third_q;
			}
			if((APP_PMSM.angle - APP_PMSM.last_angle)>10)
			{
				APP_PMSM.rise_low = rise_speed;
			}
			else if((APP_PMSM.last_angle - APP_PMSM.angle)<10&&(APP_PMSM.last_angle - APP_PMSM.angle)>=0)
			{
				APP_PMSM.rise_low = mid_speed;
			}
			else if((APP_PMSM.last_angle - APP_PMSM.angle)<0)
			{
				APP_PMSM.rise_low = low_speed;
			}
		}
#endif
#if 0
		if(APP_PMSM.angle!=APP_PMSM.last_angle)
		{
			if((APP_PMSM.angle - APP_PMSM.last_angle)>=3&&(APP_PMSM.angle - APP_PMSM.last_angle)<=357)
			{
				APP_PMSM.rise_low = rise_speed;
			}
			else if((APP_PMSM.angle - APP_PMSM.last_angle)<3&&(APP_PMSM.angle - APP_PMSM.last_angle)>=0)
			{
				APP_PMSM.rise_low = rise_mid_speed;
			}
			else if((APP_PMSM.angle - APP_PMSM.last_angle)<=-3&&(APP_PMSM.angle - APP_PMSM.last_angle)>=-357)
			{
				APP_PMSM.rise_low = low_speed;
			}
			else if((APP_PMSM.angle - APP_PMSM.last_angle)>-3&&(APP_PMSM.angle - APP_PMSM.last_angle)<0)
			{
				APP_PMSM.rise_low = low_mid_speed;
			}
				
			if(APP_PMSM.rise_low == rise_speed)
			{
					PMSM[PMSM_A].S_low_acc = 0.8f;
					PMSM[PMSM_U].S_low_acc = 0.8f;
			}
			else if(APP_PMSM.rise_low == low_speed)
			{
				PMSM[PMSM_A].S_low_acc = 2.0f;
				PMSM[PMSM_U].S_low_acc = 2.0f;
			}
		}
		#endif
//		if(APP_PMSM.rise_low == rise_speed)
//		{
//				PMSM[PMSM_A].S_low_acc = 0.8f;
//				PMSM[PMSM_U].S_low_acc = 0.8f;
//		}
//		else if(APP_PMSM.rise_low == low_speed)
//		{
//			PMSM[PMSM_A].S_low_acc = 2.0f;
//			PMSM[PMSM_U].S_low_acc = 2.0f;
//		}
		angele_fitting();
		
//		Normal_distribution(PMSM_A);
//		Normal_distribution(PMSM_U);
		APP_PMSM.last_angle = APP_PMSM.angle;
		
	}
	SPEED_LOOP();
	angle_inverse_kinematics();
	if(PMSM[PMSM_A].SpeedNew == 0&&PMSM[PMSM_U].SpeedNew == 0)
	{
		if(PMSM[PMSM_A].SpeedSet>0.0f) PMSM[PMSM_A].dec_ratio = (float)APP_PMSM.Gear + 3.0f;
		if(PMSM[PMSM_A].SpeedSet<0.0f) PMSM[PMSM_A].dec_ratio = (float)APP_PMSM.Gear*0.25f+1.75f;
		if(PMSM[PMSM_U].SpeedSet>0.0f) PMSM[PMSM_U].dec_ratio = (float)APP_PMSM.Gear*0.25f+1.75f;
		if(PMSM[PMSM_U].SpeedSet<0.0f) PMSM[PMSM_U].dec_ratio = (float)APP_PMSM.Gear+ 3.0f;
		PMSM[PMSM_A].SpeedCmd = 0.0f;
		PMSM[PMSM_U].SpeedCmd = 0.0f;
		Speed_Dec_handle(PMSM_A);
		Speed_Dec_handle(PMSM_U);
		PMSM[PMSM_A].S_low_acc = 0.8f;
		PMSM[PMSM_U].S_low_acc = 0.8f;
		PMSM[PMSM_A].acc_status = start_acc;
		PMSM[PMSM_U].acc_status = start_acc;
		return;
	}
	if(PMSM[PMSM_A].sec_speed_handle == dir_first&&PMSM[PMSM_U].sec_speed_handle == dir_first)
	{
		Speed_Dec_handle(PMSM_A);
		Speed_Dec_handle(PMSM_U);
	}
	else if(APP_PMSM.start_first == 1)
	{
		Speed_Wheel_handle(PMSM_A);
		Speed_Wheel_handle(PMSM_U);
	}
	else
	{
		Speed_Run_handle(PMSM_A);
		Speed_Run_handle(PMSM_U);
	}
}
uint16_t count_stop = 0;
void PMSM_APP_LOOP(void)
{
	if(PMSM[PMSM_A].PMSM_Status.bit.hall_calib_error!=1&&PMSM[PMSM_A].PMSM_Status.bit.hall_Error!=1&&PMSM[PMSM_A].PMSM_Status.bit.Current_Error!=1
		&&PMSM[PMSM_U].PMSM_Status.bit.hall_calib_error!=1&&PMSM[PMSM_U].PMSM_Status.bit.hall_Error!=1&&PMSM[PMSM_U].PMSM_Status.bit.Current_Error!=1)
	{
		app_angel_handle();
		
		
		#if 1
		if(APP_PMSM.CoordSqrt>0.0F)
		{
			if(PMSM[PMSM_A].MotorState != MotorRun&&PMSM[PMSM_A].first_start == false
				&&PMSM[PMSM_U].MotorState != MotorRun&&PMSM[PMSM_U].first_start == false)
			{
				PMSM[PMSM_A].MotorState = MotorStart;
				PMSM[PMSM_A].acc_status =start_acc;
				PMSM[PMSM_A].first_start = true;
				PMSM[PMSM_U].MotorState = MotorStart;
				PMSM[PMSM_U].acc_status =start_acc;
				PMSM[PMSM_U].first_start = true;
			}
			if(PMSM[PMSM_A].acc_status == dec_acc&&PMSM[PMSM_U].acc_status == dec_acc)
			{
				PMSM[PMSM_A].acc_status =start_acc;
				PMSM[PMSM_U].acc_status =start_acc;
			}
			PMSM[PMSM_A].first_stop = false;
			PMSM[PMSM_U].first_stop = false;
			count_stop = 0;
		}
		
		else if(PMSM[PMSM_A].SpeedSet == 0.0f&&PMSM[PMSM_U].SpeedSet == 0.0f)
		{
			if(++count_stop>200)
			{
				count_stop = 0;
				if(PMSM[PMSM_A].MotorState != MotorStop&&PMSM[PMSM_A].first_stop == false
					&&PMSM[PMSM_U].MotorState != MotorStop&&PMSM[PMSM_U].first_stop == false)
				{
					PMSM[PMSM_A].S_low_acc = 0.8f;
					PMSM[PMSM_A].S_Add_acc = 3.0f;
					PMSM[PMSM_A].acc_status =start_acc;
					PMSM[PMSM_A].MotorState = MotorStop;
					PMSM[PMSM_A].first_stop = true;
					PMSM[PMSM_U].MotorState = MotorStop;
					PMSM[PMSM_U].acc_status =start_acc;
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
		
//		Speed_Status_Handle();
#if 0
		if(PMSM[PMSM_A].first_start == true&&PMSM[PMSM_A].MotorState == MotorRun
			&&PMSM[PMSM_U].first_start == true&&PMSM[PMSM_U].MotorState == MotorRun)
		{
			if(PMSM[PMSM_A].acc_status == start_acc&&PMSM[PMSM_U].acc_status == start_acc)
			{
				Speed_Start_handle(PMSM_A);
				Speed_Start_handle(PMSM_U);
			}
//			else
//			{
//				Speed_Run_handle(PMSM_A);
//				Speed_Run_handle(PMSM_U);
//			}
			else if(PMSM[PMSM_A].acc_status == run_acc&&PMSM[PMSM_U].acc_status == run_acc)
			{
				Speed_Run_handle(PMSM_A);
				Speed_Run_handle(PMSM_U);
			}
			else if(PMSM[PMSM_A].acc_status == dec_acc&&PMSM[PMSM_U].acc_status == dec_acc)
			{
				Speed_Dec_handle(PMSM_A);
				Speed_Dec_handle(PMSM_U);
			}
		}
#endif
#if 0
		if(PMSM[PMSM_A].first_start == true&&PMSM[PMSM_A].MotorState == MotorRun
			&&PMSM[PMSM_U].first_start == true&&PMSM[PMSM_U].MotorState == MotorRun)
		{
//			SPEED_LOOP();
//			ANGEL_APP_HANDLE();
//			app_angel_handle();
			
		}
#endif
#if 0
		if(PMSM[PMSM_A].MotorState == MotorRun
			&&PMSM[PMSM_U].MotorState == MotorRun)
		{
			if(PMSM[PMSM_A].acc_status == start_acc&&PMSM[PMSM_U].acc_status == start_acc)
			{
				Speed_Start_handle(PMSM_A);
				Speed_Start_handle(PMSM_U);
			}
//			else
//			{
//				Speed_Run_handle(PMSM_A);
//				Speed_Run_handle(PMSM_U);
//			}
			else if(PMSM[PMSM_A].acc_status == run_acc&&PMSM[PMSM_U].acc_status == run_acc)
			{
				Speed_Run_handle(PMSM_A);
				Speed_Run_handle(PMSM_U);
			}
			else if(PMSM[PMSM_A].acc_status == dec_acc&&PMSM[PMSM_U].acc_status == dec_acc)
			{
				Speed_Dec_handle(PMSM_A);
				Speed_Dec_handle(PMSM_U);
			}
		}
#endif

#endif
		
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




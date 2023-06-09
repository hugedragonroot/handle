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
#define pmsm_current_op	3.3f/(4096.0f*0.002f*14.0f)


#define HZ_200us 4
#define HZ_100us 2

#define kk 0.5714285f
#define bb 128.57142857f
#define acc_k 0.00085f
#define acc_b	212.5f
uint16_t acc_count[2] ={0};
void Speed_Status_Handle(void)
{
	if(((fabs(PMSM[PMSM_A].SpeedSet-PMSM[PMSM_A].SpeedNew)<15.0f&&fabs(PMSM[PMSM_U].SpeedSet-PMSM[PMSM_U].SpeedNew*(-1))<15.0f)
			||(PMSM[PMSM_A].S_low_acc >= PMSM[PMSM_A].S_Add_acc&&PMSM[PMSM_U].S_low_acc >= PMSM[PMSM_U].S_Add_acc))
			&&PMSM[PMSM_A].acc_status == start_acc&&PMSM[PMSM_U].acc_status == start_acc
			&&(fabs((float)PMSM[PMSM_A].SpeedNew)>=450.0f&&fabs((float)PMSM[PMSM_U].SpeedNew)>=450.0f)
			)
//	if((PMSM[PMSM_A].S_low_acc >= PMSM[PMSM_A].S_Add_acc&&PMSM[PMSM_U].S_low_acc >= PMSM[PMSM_U].S_Add_acc)
//		/*&&fabs((float)PMSM[PMSM_A].SpeedNew)>=10.0f&&fabs((float)PMSM[PMSM_U].SpeedNew)>=10.0f*/
//		&&PMSM[PMSM_A].acc_status != run_acc&&PMSM[PMSM_U].acc_status != run_acc)
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

void	Speed_Run_handle(PMSM_Num num)
{
	#if 1
	if(fabs(PMSM[PMSM_A].SpeedSet - PMSM[PMSM_A].SpeedNew_100MS)<5.0f&&PMSM[PMSM_A].acc_status == start_acc
		&&fabs((float)PMSM[PMSM_A].SpeedNew)>50.0f&&fabs(PMSM[PMSM_U].SpeedSet - PMSM[PMSM_U].SpeedNew_100MS*(-1.0f))<5.0f
		&&PMSM[PMSM_U].acc_status == start_acc&&fabs((float)PMSM[PMSM_U].SpeedNew)>50.0f)
		{
			PMSM[PMSM_A].acc_status = run_acc;
			PMSM[PMSM_U].acc_status = run_acc;
		}

	if(PMSM[num].acc_status == start_acc)
	{
		if(PMSM[num].S_low_acc<3.0f)PMSM[num].S_low_acc = PMSM[num].S_low_acc+0.0008f;
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
	
}

void Speed_Wheel_handle(PMSM_Num num)
{
	if(PMSM[num].SpeedSet < PMSM[num].SpeedCmd)
	{
			
		PMSM[num].SpeedSet = PMSM[num].SpeedSet + PMSM[num].S_low_acc ;//正向速度坡度
		if(PMSM[num].SpeedSet > PMSM[num].SpeedCmd)
		{
			PMSM[num].SpeedSet = PMSM[num].SpeedCmd ;
		}
	}
	else if(PMSM[num].SpeedSet > PMSM[num].SpeedCmd)
	{
		PMSM[num].SpeedSet = PMSM[num].SpeedSet - PMSM[num].S_low_acc ;//反向速度坡度
		if(PMSM[num].SpeedSet < PMSM[num].SpeedCmd)
		{
			PMSM[num].SpeedSet = PMSM[num].SpeedCmd ;
		}
	}
	
}
void	Speed_Dec_handle(PMSM_Num num)
{
	PMSM[num].S_Add_acc = fabs(sinf(2.0f*fabs(PMSM[num].SpeedSet)/2100.0f*PI_2/4.0f)*PMSM[num].dec_ratio);
	if(fabs(PMSM[num].S_Add_acc)<0.2f)PMSM[num].S_Add_acc = 0.2f;
	
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
uint16_t count_test[2] ={0};
float ids = -0.1f;
static void Speed_handle(PMSM_Num num)
{
	PMSM[num].foc.Theta = hall_encoder[num].Theta;
	if(++PMSM[num].speed_pid_count>= 2)
	{
		PMSM[num].speed_pid_count = 0;
		PID_run_FloatspdVolt(&PMSM[num].speed_pid,PMSM[num].SpeedSet,hall_encoder[num].PLL_realspeed);
		PMSM[num].CurrentSet = PMSM[num].speed_pid.PIDOut;
	}
		PID_run_FloatspdVolt(&PMSM[num].current_id_pid, ids, PMSM[num].foc.Id_Iq.I1);
		
		PMSM[num].foc.Vd_Vq.V1 = PMSM[num].current_id_pid.PIDOut;
			
		PID_run_FloatspdVolt(&PMSM[num].current_iq_pid, PMSM[num].CurrentSet, PMSM[num].foc.Id_Iq.I2);		
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
				pmsm_break_io(num,false);
				PMSM[num].MotorState = MotorPrecharge;
				PMSM[num].Start_time = 0;
				break;
			case MotorPrecharge:
				if(++PMSM[num].Start_time>=50)
				{
					pmsm_start(num);
					PMSM[num].Start_time = 0;
					PMSM[num].MotorState = MotorRun;
				}
				break;
			case MotorStop:
				pmsm_stop(num);
				if(++PMSM[num].stop_time>20)
				{
					pmsm_break_io(num,true);
					PMSM[num].stop_time = 0;
					PMSM[num].MotorState = MotorWatit;
				}
				break;
			case MotorWatit:
				pmsm_break_io(num,true);
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

void SPEED_STATUS_HANDLE_A(void)
{
	if(++PMSM[PMSM_A].count_100ms == 100)
	{
		PMSM[PMSM_A].count_100ms = 0;
		PMSM[PMSM_A].SpeedNew_100MS = PMSM[PMSM_A].SpeedNew;
	}

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

	if(PMSM[PMSM_A].LastSpeedNew_100MS!=PMSM[PMSM_A].SpeedNew_100MS)
	{
		PMSM[PMSM_A].LastSpeedNew_100MS = PMSM[PMSM_A].SpeedNew_100MS;
	}
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
//		else if(PMSM[PMSM_U].SpeedNew_100MS == 0&&PMSM[PMSM_U].LastSpeedNew_100MS == 0)
//		{
//			PMSM[PMSM_U].speed_status = zero_zero;
//		}
	if(PMSM[PMSM_U].LastSpeedNew_100MS!=PMSM[PMSM_U].SpeedNew_100MS)
	{
		PMSM[PMSM_U].LastSpeedNew_100MS = PMSM[PMSM_U].SpeedNew_100MS;
	}
}

void SPEED_LOOP(void)
{
	if(PMSM[PMSM_A].SpeedNew_100MS == 0&&PMSM[PMSM_U].SpeedNew_100MS == 0)
	{
		if(PMSM[PMSM_A].SpeedSet>0.0f) PMSM[PMSM_A].dec_ratio = 8.0f;
		if(PMSM[PMSM_A].SpeedSet<0.0f) PMSM[PMSM_A].dec_ratio = 3.0f;
		if(PMSM[PMSM_U].SpeedSet>0.0f) PMSM[PMSM_U].dec_ratio = 3.0f;
		if(PMSM[PMSM_U].SpeedSet<0.0f) PMSM[PMSM_U].dec_ratio = 8.0f;
		Speed_Dec_handle(PMSM_A);
		Speed_Dec_handle(PMSM_U);
		PMSM[PMSM_A].S_low_acc = 0.5f;
		PMSM[PMSM_U].S_low_acc = 0.5f;
		PMSM[PMSM_A].acc_status = start_acc;
		PMSM[PMSM_U].acc_status = start_acc;
	}
	switch(PMSM[PMSM_A].speed_status<<4|PMSM[PMSM_U].speed_status)
	{
		case 0x40:
			if(fabs((float)(PMSM[PMSM_A].SpeedNew - (float)PMSM[PMSM_U].SpeedNew))<500.0f)
			{
				PMSM[PMSM_A].sec_speed_handle = wheel_first;
				PMSM[PMSM_U].sec_speed_handle = wheel_first;
				PMSM[PMSM_A].SpeedCmd = (PMSM[PMSM_A].SpeedSet - PMSM[PMSM_U].SpeedSet)/2.0f;
				PMSM[PMSM_U].SpeedCmd = (PMSM[PMSM_A].SpeedSet - PMSM[PMSM_U].SpeedSet)/2.0f;
			}
			break;
		case 0x04:
			if(fabs((float)(PMSM[PMSM_A].SpeedNew -(float)PMSM[PMSM_U].SpeedNew))<500.0f)
			{
				PMSM[PMSM_A].sec_speed_handle = wheel_first;
				PMSM[PMSM_U].sec_speed_handle = wheel_first;
				PMSM[PMSM_A].SpeedCmd = (PMSM[PMSM_A].SpeedSet - PMSM[PMSM_U].SpeedSet)/2.0f;
				PMSM[PMSM_U].SpeedCmd = (PMSM[PMSM_A].SpeedSet - PMSM[PMSM_U].SpeedSet)/2.0f;
			}
			break;
		case 0x11:
				PMSM[PMSM_A].sec_speed_handle = dir_first;
				PMSM[PMSM_U].sec_speed_handle = dir_first;
				PMSM[PMSM_A].SpeedCmd = 0.0f;
				PMSM[PMSM_U].SpeedCmd = 0.0f;
				PMSM[PMSM_A].dec_ratio = 8.0f;
				PMSM[PMSM_U].dec_ratio = 8.0f;
			break;
		case 0x44:
				PMSM[PMSM_A].sec_speed_handle = dir_first;
				PMSM[PMSM_U].sec_speed_handle = dir_first;
				PMSM[PMSM_A].SpeedCmd = 0.0f;
				PMSM[PMSM_U].SpeedCmd = 0.0f;
				PMSM[PMSM_A].dec_ratio = 3.0f;
				PMSM[PMSM_U].dec_ratio = 3.0f;
			break;
		default:
//			PMSM[PMSM_A].S_low_acc = 0.5f;
//			PMSM[PMSM_U].S_low_acc = 0.5f;
			break;
	}
	switch(PMSM[PMSM_A].sec_speed_handle<<4|PMSM[PMSM_U].sec_speed_handle)
	{
		case 0x00:
			if((fabs(PMSM[PMSM_A].SpeedCmd - hall_encoder[PMSM_A].PLL_realspeed)<10.0f)&
				(fabs(PMSM[PMSM_U].SpeedCmd - hall_encoder[PMSM_U].PLL_realspeed)<10.0f))
			{
				PMSM[PMSM_A].sec_speed_handle = wheel_sec;
				PMSM[PMSM_U].sec_speed_handle = wheel_sec;
				PMSM[PMSM_A].S_low_acc = 0.5f;
				PMSM[PMSM_U].S_low_acc = 0.5f;
				PMSM[PMSM_A].acc_status = start_acc;
				PMSM[PMSM_U].acc_status = start_acc;
			}
			break;
		case 0x22:
			if(fabs(hall_encoder[PMSM_A].PLL_realspeed)<2.0f
				&&fabs(hall_encoder[PMSM_U].PLL_realspeed)<2.0f)
			{
				PMSM[PMSM_A].sec_speed_handle = dir_sec;
				PMSM[PMSM_U].sec_speed_handle = dir_sec;
				PMSM[PMSM_A].S_low_acc = 0.5f;
				PMSM[PMSM_U].S_low_acc = 0.5f;
				PMSM[PMSM_A].acc_status = start_acc;
				PMSM[PMSM_U].acc_status = start_acc;
			}
			break;
	}
	if(PMSM[PMSM_A].sec_speed_handle == dir_first&&PMSM[PMSM_U].sec_speed_handle == dir_first)
	{
		Speed_Dec_handle(PMSM_A);
		Speed_Dec_handle(PMSM_U);
	}
	else if(PMSM[PMSM_A].sec_speed_handle == wheel_first&&PMSM[PMSM_U].sec_speed_handle == wheel_first)
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
void PMSM_APP_LOOP(void)
{
	if(PMSM[PMSM_A].PMSM_Status.bit.hall_calib_error!=1&&PMSM[PMSM_A].PMSM_Status.bit.hall_Error!=1&&PMSM[PMSM_A].PMSM_Status.bit.Current_Error!=1
		&&PMSM[PMSM_U].PMSM_Status.bit.hall_calib_error!=1&&PMSM[PMSM_U].PMSM_Status.bit.hall_Error!=1&&PMSM[PMSM_U].PMSM_Status.bit.Current_Error!=1)
	{
		if(PMSM[PMSM_U].SpeedNew>PMSM[PMSM_U].MaxSpeed) PMSM[PMSM_U].SpeedNew = PMSM[PMSM_U].MaxSpeed;
		if(PMSM[PMSM_U].SpeedNew<-PMSM[PMSM_U].MaxSpeed) PMSM[PMSM_U].SpeedNew = -PMSM[PMSM_U].MaxSpeed;
		if(PMSM[PMSM_A].SpeedNew>PMSM[PMSM_A].MaxSpeed) PMSM[PMSM_A].SpeedNew = PMSM[PMSM_A].MaxSpeed;
		if(PMSM[PMSM_A].SpeedNew<-PMSM[PMSM_A].MaxSpeed) PMSM[PMSM_A].SpeedNew = -PMSM[PMSM_A].MaxSpeed;
		
		
		SPEED_STATUS_HANDLE_A();
		SPEED_STATUS_HANDLE_U();
		PMSM[PMSM_A].SpeedFIL = GildeAverageValueFilter_MAG(PMSM[PMSM_A].SpeedNew,mag_data_a);
		PMSM[PMSM_U].SpeedFIL = GildeAverageValueFilter_MAG(PMSM[PMSM_U].SpeedNew*(-1),mag_data_u);
		
		
			PMSM[PMSM_U].SpeedNow = (float)PMSM[PMSM_U].SpeedFIL*0.01f+PMSM[PMSM_U].SpeedNow*0.99f;
			PMSM[PMSM_A].SpeedNow = (float)PMSM[PMSM_A].SpeedFIL*0.01f+PMSM[PMSM_A].SpeedNow*0.99f;
		
		if(fabs((float)PMSM[PMSM_A].SpeedNew)>10.0f&&fabs((float)PMSM[PMSM_U].SpeedNew)>10.0f)
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
		}
		
		else if(fabs((float)PMSM[PMSM_A].SpeedNew)==0.0f&&fabs((float)PMSM[PMSM_U].SpeedNew)==0.0f
			&&fabs(hall_encoder[PMSM_A].PLL_realspeed)<2.0f&&fabs(hall_encoder[PMSM_U].PLL_realspeed)<2.0f)
		{
			if(PMSM[PMSM_A].MotorState != MotorStop&&PMSM[PMSM_A].first_stop == false
				&&PMSM[PMSM_U].MotorState != MotorStop&&PMSM[PMSM_U].first_stop == false)
			{
				PMSM[PMSM_A].S_low_acc = 0.3f;
				PMSM[PMSM_A].S_Add_acc = 3.5f;
				PMSM[PMSM_A].acc_status =start_acc;
				PMSM[PMSM_A].MotorState = MotorStop;
				PMSM[PMSM_A].first_stop = true;
				PMSM[PMSM_U].MotorState = MotorStop;
				PMSM[PMSM_U].acc_status =start_acc;
				PMSM[PMSM_U].S_low_acc = 0.3f;
				PMSM[PMSM_U].S_Add_acc = 3.5f;
				PMSM[PMSM_U].first_stop = true;
//				clear_pid(&PMSM[PMSM_U].angle_pid);
//				clear_pid(&PMSM[PMSM_U].speed_pid);
//				clear_pid(&PMSM[PMSM_U].current_id_pid);
//				clear_pid(&PMSM[PMSM_U].current_iq_pid);
//				PMSM_para_clear(PMSM_U);
//				clear_pid(&PMSM[PMSM_A].angle_pid);
//				clear_pid(&PMSM[PMSM_A].speed_pid);
//				clear_pid(&PMSM[PMSM_A].current_id_pid);
//				clear_pid(&PMSM[PMSM_A].current_iq_pid);
//				PMSM_para_clear(PMSM_A);
			}
			PMSM[PMSM_A].first_start = false;
			PMSM[PMSM_U].first_start = false;
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
#if 1
		if(PMSM[PMSM_A].first_start == true&&PMSM[PMSM_A].MotorState == MotorRun
			&&PMSM[PMSM_U].first_start == true&&PMSM[PMSM_U].MotorState == MotorRun)
		{
			SPEED_LOOP();
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




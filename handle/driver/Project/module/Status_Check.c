/**********************************************************/
//@Autor:Raw
//@Version:0.0.1
//@End Time:2023/4/21
//@Function:Status_Check.c
/**********************************************************/
#include "Status_Check.h"
#include "adc_callback.h"
#include "gpio.h"
#include "Brush_Driver.h"
#include "Brush_Control.h"
#include "PMSM_Driver.h"
#include "key_filter.h"
#include "xsto_api_ii.h"
#include "weak_handle.h"
#include "can.h"
extern API_Config_t  API_Config;
uint32_t BZ_MS = 0;


IMU_T imu_mpu;
void Status_Check_Loop(void)
{
	static uint8_t vd_count = 0;
	BZ_MS++;
	if(++vd_count>=5)
	{
		vd_count = 0;
		GetVoltage_MOSTEMP();//获取母线电压
		if(ADC_Data.Bus_Volt>=30)//过压30v
		{
			Brush[Brush_A].brush_state.bit.OverVoltag = 1;
			Brush[Brush_U].brush_state.bit.OverVoltag = 1;
			PMSM[PMSM_A].PMSM_Status.bit.OverVoltag = 1;
			PMSM[PMSM_U].PMSM_Status.bit.OverVoltag = 1;
		}
		else if(ADC_Data.Bus_Volt<16)//欠压16v
		{
			Brush[Brush_A].brush_state.bit.Undervoltag = 1;
			Brush[Brush_U].brush_state.bit.Undervoltag = 1;
			PMSM[PMSM_A].PMSM_Status.bit.Undervoltag = 1;
			PMSM[PMSM_U].PMSM_Status.bit.Undervoltag = 1;
		}
		else if(ADC_Data.Bus_Volt>16.0f&&ADC_Data.Bus_Volt<30.0f)//正常电压范围内
		{
				
			Brush[Brush_A].brush_state.bit.Undervoltag = 0;
			Brush[Brush_U].brush_state.bit.Undervoltag = 0;
			PMSM[PMSM_A].PMSM_Status.bit.Undervoltag = 0;
			PMSM[PMSM_U].PMSM_Status.bit.Undervoltag = 0;
			
			Brush[Brush_A].brush_state.bit.OverVoltag = 0;
			Brush[Brush_U].brush_state.bit.OverVoltag = 0;
			PMSM[PMSM_A].PMSM_Status.bit.OverVoltag = 0;
			PMSM[PMSM_U].PMSM_Status.bit.OverVoltag = 0;
		}
		if(ADC_Data.mos_temp > 80.0f)//过温
		{
			Brush[Brush_A].brush_state.bit.OverTemp = 1;
			Brush[Brush_U].brush_state.bit.OverTemp = 1;
			
			PMSM[PMSM_A].PMSM_Status.bit.OverTemp = 1;
			PMSM[PMSM_A].PMSM_Status.bit.OverTemp = 1;
		}
		else
		{
			Brush[Brush_A].brush_state.bit.OverTemp = 0;
			Brush[Brush_U].brush_state.bit.OverTemp = 0;
			
			PMSM[PMSM_A].PMSM_Status.bit.OverTemp = 0;
			PMSM[PMSM_A].PMSM_Status.bit.OverTemp = 0;
		}
//		if(PMSM[PMSM_A].MotorPreState == StopMotor_e)
		PMSM[PMSM_A].PMSM_Status.bit.Bandbrake = hold_filter(&Spacing[key_bz1],BREAK1_READ_INPUT(),BZ_MS);//抱闸状态
		PMSM[PMSM_U].PMSM_Status.bit.Bandbrake = hold_filter(&Spacing[key_bz2],BREAK2_READ_INPUT(),BZ_MS);//抱闸状态
	}
}



uint32_t ZERO_MS = 0;
uint8_t top_value = 0;
uint8_t bottom_value =0;
uint8_t origin_value = 0;
void KEY_ZERO_CHECK_LOOP(void)
{
	ZERO_MS++;
	top_value = hold_filter(&Spacing[key_top],KEY_TOP_INPUT(),ZERO_MS);
	
	bottom_value = hold_filter(&Spacing[key_bottom],KEY_BOTTOM_INPUT(),ZERO_MS);
	
	
	if(Brush[Brush_A].brush_state.bit.FINISH_calibration_One == 0&&ZERO_MS>=50)
	{
		switch(top_value<<4|bottom_value)
		{
			case 0x00:
				if(Brush[Brush_U].brush_state.bit.FINISH_calibration_One == 1)
				{
					Brush[Brush_A].brush_state.bit.FINISH_calibration_One = 1;
					Brush[Brush_A].Control_mode = mpu_mode;
					Brush[Brush_U].brush_Cmd_Pre = Brush_Cmd_Stop;
					Brush[Brush_U].Control_mode = speed_mode;
					Brush[Brush_A].brush_Cmd_Pre = Brush_Cmd_Star;
					Brush_Start_Filter(Brush_A);
					Brush_Start_Filter(Brush_U);
				}
//				Brush[Brush_A].brush_Cmd_Pre = Brush_Cmd_Stop;
				break;
			case 0x01:
				Brush[Brush_A].Control_mode = speed_mode;
				Brush[Brush_A].SpeedlNew = -300;
				Brush[Brush_A].brush_Cmd_Pre = Brush_Cmd_Star;
				Brush_Start_Filter(Brush_A);
				break;
			case 0x10:
				Brush[Brush_A].Control_mode = speed_mode;
				Brush[Brush_A].SpeedlNew = 300;
				Brush[Brush_A].brush_Cmd_Pre = Brush_Cmd_Star;
				Brush_Start_Filter(Brush_A);
				break;
			default:
				
			break;
		
		}
	}
	if(Brush[Brush_U].brush_state.bit.FINISH_calibration_One == 0&&ZERO_MS>=50)
	{
		Brush[Brush_U].Control_mode = mpu_mode;
		Brush[Brush_U].brush_Cmd_Pre = Brush_Cmd_Star;
		Brush_Start_Filter(Brush_U);
	}
	
}

bool flage_firast = false;
void ZERO_HANDLE_CHECK_LOOP(void)
{
	
	#if 0
	ZERO_MS++;
	top_value = hold_filter(&Spacing[key_top],KEY_TOP_INPUT(),ZERO_MS);
	
	bottom_value = hold_filter(&Spacing[key_bottom],KEY_BOTTOM_INPUT(),ZERO_MS);
	
	
	if(Brush[Brush_A].Control_mode == zero_mode&&Brush[Brush_U].Control_mode == zero_mode)
	{
		switch(top_value<<4|bottom_value)
		{
			case 0x00:
					Brush[Brush_A].brush_state.bit.FINISH_calibration_One = 1;
					Brush[Brush_A].Control_mode = mpu_mode;
					Brush[Brush_U].brush_Cmd_Pre = Brush_Cmd_Stop;
					Brush[Brush_U].Control_mode = speed_mode;
					Brush[Brush_A].brush_Cmd_Pre = Brush_Cmd_Star;
					RESET_BRUSH_PARA(Brush_A);
					RESET_BRUSH_PARA(Brush_U);
//					clear_pid(&Brush[Brush_A].ANGLE_PID);//清除应用pid参数
					clear_pid(&Brush[Brush_A].SPEED_PID);
//
			clear_pid(&Brush[Brush_A].CURRENT_PID);
					clear_pid(&Brush[Brush_A].MPU_PID);
				break;
			case 0x01:
				Brush[Brush_A].SpeedlNew = -300;
				Brush[Brush_A].brush_Cmd_Pre = Brush_Cmd_Star;
//				Brush_Start_Filter(Brush_A);
				break;
			case 0x10:
				Brush[Brush_A].SpeedlNew = 300;
				Brush[Brush_A].brush_Cmd_Pre = Brush_Cmd_Star;
//				Brush_Start_Filter(Brush_A);
				break;
			default:
				
			break;
		
		}
//		if(flage_firast == false)
//			
//		{
//					flage_firast = true;
//					Brush[Brush_A].brush_Cmd_Pre = Brush_Cmd_Star;
//					Brush[Brush_A].Control_mode = mpu_mode;
//				}
	}
	if(Brush[Brush_U].Control_mode == zero_mode)
	{
		Brush[Brush_U].brush_Cmd_Pre = Brush_Cmd_Star;
//		Brush_Start_Filter(Brush_U);
	}
//	
//	if(flage_firast == false&&ZERO_MS>=500)	
//	{
//		flage_firast = true;
//		Brush[Brush_A].brush_Cmd_Pre = Brush_Cmd_Star;
//		Brush[Brush_A].Control_mode = mpu_mode;
//		Brush[Brush_A].first_start = false;
////		Brush_Start_Filter(Brush_A);
//	}
	#endif
	#if 1
	if(Brush[Brush_U].Control_mode == zero_mode)
	{
		Brush[Brush_U].brush_Cmd_Pre = Brush_Cmd_Star;
		Brush[Brush_U].SpeedlNew = -150;
//		Brush_Start_Filter(Brush_U);
	}
	
	if(Brush[Brush_A].Control_mode == zero_mode)
	{
		Brush[Brush_A].brush_Cmd_Pre = Brush_Cmd_Star;
		Brush[Brush_A].SpeedlNew = 300;
//		Brush_Start_Filter(Brush_U);
	}
	#endif
}
void Brush_Fold_Check(Brush_Num num)
{
	
	if(Brush[num].Control_mode == fold_mode&&Brush[num].brush_Cmd == Brush_Cmd_Run)
	{
		if(fabs(Brush[num].real_speed)<10.0f)
		{
			if(++Brush[num].fold_speed_count>=10)
			{
				Brush[num].fold_speed_count = 0;
				Brush[num].brush_Cmd_Pre = Brush_Cmd_Stop;
			}
		}
		else
		{
			Brush[num].fold_speed_count = 0;
		}
	}
}
uint16_t online_count = 0;
void online_loop(void)
{
	#if 0
	if(API_Config.Status == MODULE_STATUS_OFFLINE)
	{
			Brush[Brush_U].brush_Cmd_Pre = Brush_Cmd_Stop;
			Brush[Brush_A].brush_Cmd_Pre = Brush_Cmd_Stop;
		
			Brush_Start_Filter(Brush_A);
			Brush_Start_Filter(Brush_U);
		
			PMSM[PMSM_A].SpeedNew = 0;
			PMSM[PMSM_U].SpeedNew = 0;
//			PMSM[PMSM_A].MotorPreState = MotorStop;
//			PMSM[PMSM_U].MotorPreState = MotorStop;
//		
//			PMSM_Start_Filter(PMSM_A);
//			PMSM_Start_Filter(PMSM_U);
	}
	#endif
	#if 1
	if(message_online_flage == true)
	{
		message_online_flage = false;
		online_count = 0;
	}
	else
	{
		if(++online_count>=20)
		{
			Brush[Brush_U].brush_Cmd_Pre = Brush_Cmd_Stop;
			Brush[Brush_A].brush_Cmd_Pre = Brush_Cmd_Stop;
		
//			Brush_Start_Filter(Brush_A);
//			Brush_Start_Filter(Brush_U);
		
			PMSM[PMSM_A].SpeedNew = 0;
			PMSM[PMSM_U].SpeedNew = 0;
		}
	}
	
	#endif


	float pitch_temp = fabs(mpu_filter.ptich - Brush[Brush_A].MPU_Basic);
	imu_mpu.pitch_return = (int16_t)(pitch_temp*100.0f);
	imu_mpu.roll_return =  (int16_t)(mpu_filter.roll*100.0f);
}



void MPU_PROTECT_CHECK(void)
{
	
	if(fabs(mpu_filter.ptich - Brush[Brush_A].MPU_Basic)>=2.0f
		||fabs(mpu_filter.ptich - Brush[Brush_U].MPU_Basic)>=2.0f)
	{
		Brush[Brush_U].brush_state.bit.pitch_error = 1;
		Brush[Brush_A].brush_state.bit.pitch_error = 1;
	}
	else
	{
		Brush[Brush_A].brush_state.bit.pitch_error = 0;
		Brush[Brush_U].brush_state.bit.pitch_error = 0;
	}
	
	if(fabs(mpu_filter.roll)>=10.0f
		||fabs(mpu_filter.roll)>=10.0f)
	{
		Brush[Brush_U].brush_state.bit.rol_error = 1;
		Brush[Brush_A].brush_state.bit.rol_error = 1;
	}
	else
	{
		Brush[Brush_A].brush_state.bit.rol_error = 0;
		Brush[Brush_U].brush_state.bit.rol_error = 0;
	}
}












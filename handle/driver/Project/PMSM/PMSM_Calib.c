/**********************************************************/
//@Autor:Raw
//@Version:0.0.1
//@End Time:2023/3/23
//@Function:PMSM_Calib.c
/**********************************************************/
#include "PMSM_Calib.h"
#include "PMSM_Driver.h"
#include "HALL_Encoder.h"
#include "adc_callback.h"
#include "bsp_flash.h"
#include "gpio.h"
typedef struct
{
	three_hall_t Amplitude;
	three_hall_t Median;
	int32_t	Calib_Bias;
	uint16_t OVER_Current;
}__attribute__((packed)) PMSM_data_t;
PMSM_data_t PMSM_data[2];
PMSM_Calib_t PMSM_Calib[2]={0};
bool PMSM_Write_FlAGE[2] = {false};

#define PMSM_Data_Flash_A	0x08020000  //240k
#define PMSM_Data_Flash_U	0x08020800  //242k

void Calibration_SaveCalibrationElectricalData(PMSM_Num num)
{
		if(hall_encoder[num].RANGE_Amplitude.HALL_U_VALUE != PMSM_data[num].Amplitude.HALL_U_VALUE
			||hall_encoder[num].RANGE_Amplitude.HALL_V_VALUE != PMSM_data[num].Amplitude.HALL_V_VALUE
			||hall_encoder[num].RANGE_Amplitude.HALL_W_VALUE != PMSM_data[num].Amplitude.HALL_W_VALUE
			||hall_encoder[num].Median.HALL_U_VALUE != PMSM_data[num].Median.HALL_U_VALUE
			||hall_encoder[num].Median.HALL_V_VALUE != PMSM_data[num].Median.HALL_V_VALUE
			||hall_encoder[num].Median.HALL_W_VALUE != PMSM_data[num].Median.HALL_W_VALUE
			||PMSM_data[num].Calib_Bias != (int32_t)(hall_encoder[num].Theta_Calib_Bias*10000.0f)
			||PMSM_data[num].OVER_Current != (uint16_t)(PMSM[num].Current_over*1000.0f)
		)
	{
		PMSM_data[num].Amplitude.HALL_U_VALUE = hall_encoder[num].RANGE_Amplitude.HALL_U_VALUE;
		PMSM_data[num].Amplitude.HALL_V_VALUE = hall_encoder[num].RANGE_Amplitude.HALL_V_VALUE;
		PMSM_data[num].Amplitude.HALL_W_VALUE = hall_encoder[num].RANGE_Amplitude.HALL_W_VALUE;
		PMSM_data[num].Median.HALL_U_VALUE = hall_encoder[num].Median.HALL_U_VALUE;
		PMSM_data[num].Median.HALL_V_VALUE = hall_encoder[num].Median.HALL_V_VALUE;
		PMSM_data[num].Median.HALL_W_VALUE = hall_encoder[num].Median.HALL_W_VALUE;
		PMSM_data[num].Calib_Bias = (int32_t)(hall_encoder[num].Theta_Calib_Bias*10000.0f);
		PMSM_data[num].OVER_Current = (uint16_t)(PMSM[num].Current_over*1000.0f);
		PMSM_Write_FlAGE[num] = true;
	}
	else 
	{
		PMSM_Write_FlAGE[num] = false;
		return;
	}
	
//	if(num == PMSM_A)
//	{
//		
//		fmc_erase_pages(CTL_SECTOR_NUMBER_5,1);
//		fmc_program(PMSM_Data_Flash_A,(uint8_t *)&PMSM_data[num],sizeof(PMSM_data_t));
//	}
//	else if(num == PMSM_U)
//	{
////		fmc_erase_pages(PMSM_Data_Flash_U,sizeof(PMSM_data_t));
//		fmc_program(PMSM_Data_Flash_U,(uint8_t *)&PMSM_data[num],sizeof(PMSM_data_t));
//	}

}

void Calibration_Save_Write(void)
{
	if(PMSM_Write_FlAGE[PMSM_A] == true || PMSM_Write_FlAGE[PMSM_U] == true)
	{
		PMSM_Write_FlAGE[PMSM_A] = false ;
		PMSM_Write_FlAGE[PMSM_U] = false ;
		fmc_erase_pages(5,1);
		fmc_program(PMSM_Data_Flash_A,(uint8_t *)&PMSM_data[PMSM_A],sizeof(PMSM_data_t));
		fmc_program(PMSM_Data_Flash_U,(uint8_t *)&PMSM_data[PMSM_U],sizeof(PMSM_data_t));
	}
}
#define first_Current_lim 25.0f
void Calibration_ReadCalibrationElectricalData(PMSM_Num num)
{
	if(num == PMSM_A)
	{
		fmc_read_data(PMSM_Data_Flash_A,(uint8_t *)&PMSM_data[num],sizeof(PMSM_data_t));
	}
	else if(num == PMSM_U)
	{
		fmc_read_data(PMSM_Data_Flash_U,(uint8_t *)&PMSM_data[num],sizeof(PMSM_data_t));
	}
	
	if(((PMSM_data[num].Amplitude.HALL_U_VALUE&0xffff) == 0xffff||PMSM_data[num].Amplitude.HALL_U_VALUE == 0x0000)
		||((PMSM_data[num].Amplitude.HALL_V_VALUE&0xffff) == 0xffff||PMSM_data[num].Amplitude.HALL_V_VALUE == 0x0000)
		||((PMSM_data[num].Amplitude.HALL_W_VALUE&0xffff) == 0xffff||PMSM_data[num].Amplitude.HALL_W_VALUE == 0x0000))
	{
		PMSM[num].MotorCali = MegCaliNone;
	}
	else
	{
		memcpy((void *)&hall_encoder[num].RANGE_Amplitude.HALL_U_VALUE,(void *)&PMSM_data[num].Amplitude.HALL_U_VALUE,6);
//		hall_encoder[num].RANGE_Amplitude.HALL_U_VALUE = PMSM_data[num].Amplitude.HALL_U_VALUE;
//		hall_encoder[num].RANGE_Amplitude.HALL_V_VALUE = PMSM_data[num].Amplitude.HALL_V_VALUE;
//		hall_encoder[num].RANGE_Amplitude.HALL_W_VALUE = PMSM_data[num].Amplitude.HALL_W_VALUE;
	}
	
	if(((PMSM_data[num].Median.HALL_U_VALUE&0xffff) == 0xffff||PMSM_data[num].Median.HALL_U_VALUE == 0x0000)
		||((PMSM_data[num].Median.HALL_V_VALUE&0xffff) == 0xffff||PMSM_data[num].Median.HALL_V_VALUE == 0x0000)
		||((PMSM_data[num].Median.HALL_W_VALUE&0xffff) == 0xffff||PMSM_data[num].Median.HALL_W_VALUE == 0x0000))
	{
		PMSM[num].MotorCali = MegCaliNone;
	}
	else
	{
		memcpy((void *)&hall_encoder[num].Median.HALL_U_VALUE,(void *)&PMSM_data[num].Median.HALL_U_VALUE,6);
//		hall_encoder[num].Median.HALL_U_VALUE = PMSM_data[num].Median.HALL_U_VALUE;
//		hall_encoder[num].Median.HALL_V_VALUE = PMSM_data[num].Median.HALL_V_VALUE;
//		hall_encoder[num].Median.HALL_W_VALUE = PMSM_data[num].Median.HALL_W_VALUE;
	}
	if(PMSM_data[num].Calib_Bias == 0xffffffff || PMSM_data[num].Calib_Bias == 0x00000000)
	{
		PMSM[num].MotorCali = MegCaliNone;
	}
	else
	{
		hall_encoder[num].Theta_Calib_Bias = (float)PMSM_data[num].Calib_Bias/10000.0f;
	}
	if(PMSM_data[num].OVER_Current == 0xffff || PMSM_data[num].OVER_Current == 0x0000)
	{
		PMSM[num].Current_over = first_Current_lim;
	}
	else
	{
		PMSM[num].Current_over = (float)PMSM_data[num].OVER_Current/1000.0f; 
	}
	Electric_Angle_Calibration_Check(num);
	Calibration_SaveCalibrationElectricalData(num);
}
void Electric_Angle_Calibration_Check(PMSM_Num num)
{
	if(hall_encoder[num].RANGE_Amplitude.HALL_U_VALUE>=Hall_adc_lim&&hall_encoder[num].RANGE_Amplitude.HALL_V_VALUE>=Hall_adc_lim&&hall_encoder[num].RANGE_Amplitude.HALL_W_VALUE>=Hall_adc_lim)
	{
		if(hall_encoder[num].Median.HALL_U_VALUE>Hall_adc_mid&&hall_encoder[num].Median.HALL_V_VALUE>Hall_adc_mid&&hall_encoder[num].Median.HALL_W_VALUE>Hall_adc_mid)
		{
			if(fabs(hall_encoder[num].Theta_Calib_Bias)<PI_2/2.0f)
			{
				PMSM[num].MotorCali = MegCaliOK;
				PMSM[num].PMSM_Status.bit.hall_calib_error = 0;
				PMSM[num].PMSM_Status.bit.hall_Error = 0;
			}
		}
	}
	else
	{
		PMSM[num].MotorCali = MegCaliNone;
		PMSM[num].PMSM_Status.bit.hall_calib_error = 1;
		PMSM[num].PMSM_Status.bit.hall_Error = 1;
	}
}
void Electric_Angle_Calibration(PMSM_Num num)
{
	#define T 100     
	#define DRAG_TIME   100    
	#define ZERO_CNT    1000
	
	#define DRAG_VOL    0.12f   

	uint16_t HALL_U[2] = {0};
	uint16_t HALL_V[2] = {0};
	uint16_t HALL_W[2] = {0};
	HALL_U[num] = adc_ReadAvgRaw(HALL_U_ADC,num);
	HALL_V[num] = adc_ReadAvgRaw(HALL_V_ADC,num);
	HALL_W[num] = adc_ReadAvgRaw(HALL_W_ADC,num);
	if (PMSM_Calib[num].firstFlag == 0)
	{
		PMSM_Calib[num].firstFlag = 1;
		PMSM_Calib[num].DIR_cnt = 0;
		PMSM_Calib[num].HallMin_A = 2048;
		PMSM_Calib[num].HallMin_B = 2048;
		PMSM_Calib[num].HallMin_C = 2048;
		PMSM[num].Theta_acc = 0.0008f;
		pmsm_break_io(num,false);
		MOS_ENABLE();
		pmsm_start(num);
		PMSM[num].foc.Theta	= PMSM[num].Theta_acc;
		PMSM[num].OpenLoopVoltCmd = (float)DRAG_VOL;		
		PMSM[num].foc.Vd_Vq.V1 = PMSM[num].OpenLoopVoltCmd;
		PMSM[num].foc.Vd_Vq.V2 = 0.0f;	
		PMSM[num].MotorCali = MegCaliPositive;
	}
	
	if(++PMSM_Calib[num].timeCnt >= (DRAG_TIME/T))
	{
		PMSM_Calib[num].timeCnt = 0;
		PMSM[num].OpenLoopVoltCmd = (float)DRAG_VOL;
		PMSM[num].foc.Vd_Vq.V1 = PMSM[num].OpenLoopVoltCmd; 
		PMSM[num].foc.Vd_Vq.V2 = 0.0f;		
//		
		if(fabs(PMSM[num].foc.Theta) >= PI_2)
		{
			PMSM[num].foc.Theta = 0.0f;				
			PMSM_Calib[num].AngleCnt++; 
		}
		if ((PMSM_Calib[num].AngleCnt>2) && (PMSM[num].foc.Theta == 0.0f) && (PMSM_Calib[num].ZeroAngleCnt <= ZERO_CNT))//电机转动一圈（7对级，7个电角度）后记录电角度偏置数据
		{
			PMSM_Calib[num].ZeroAngleCnt++;
			if (PMSM_Calib[num].ZeroAngleCnt == ZERO_CNT)		
			{
				PMSM_Calib[num].HallMedian_A = (PMSM_Calib[num].HallMax_A + PMSM_Calib[num].HallMin_A) >> 1;
				PMSM_Calib[num].HallMedian_B = (PMSM_Calib[num].HallMax_B + PMSM_Calib[num].HallMin_B) >> 1;
				PMSM_Calib[num].HallMedian_C = (PMSM_Calib[num].HallMax_C + PMSM_Calib[num].HallMin_C) >> 1;

				PMSM_Calib[num].Amplitude_A = (PMSM_Calib[num].HallMax_A - PMSM_Calib[num].HallMin_A) >> 1;
				PMSM_Calib[num].Amplitude_B = (PMSM_Calib[num].HallMax_B - PMSM_Calib[num].HallMin_B) >> 1;
				PMSM_Calib[num].Amplitude_C = (PMSM_Calib[num].HallMax_C - PMSM_Calib[num].HallMin_C) >> 1;

				
				if(PMSM_Calib[num].Amplitude_A>=Hall_adc_lim&&PMSM_Calib[num].Amplitude_B>=Hall_adc_lim&&PMSM_Calib[num].Amplitude_C>=Hall_adc_lim)
				{
					PMSM_Calib[num].f_HallNormalized_A = (float)(HALL_U[num]  - (int16_t)PMSM_Calib[num].HallMedian_A) \
																	/ (float)PMSM_Calib[num].Amplitude_A;						
					PMSM_Calib[num].f_HallNormalized_B = (float)(HALL_V[num] - (int16_t)PMSM_Calib[num].HallMedian_B) \
																	/ (float)PMSM_Calib[num].Amplitude_B;		
					PMSM_Calib[num].f_HallNormalized_C = (float)(HALL_W[num] - (int16_t)PMSM_Calib[num].HallMedian_C) \
																	/ (float)PMSM_Calib[num].Amplitude_C;		

					PMSM_Calib[num].f_HallNormalized_Alpha = PMSM_Calib[num].f_HallNormalized_A - PMSM_Calib[num].f_HallNormalized_B*my_cosf_1_3 - PMSM_Calib[num].f_HallNormalized_C*my_cosf_1_3;
					PMSM_Calib[num].f_HallNormalized_Beta = -PMSM_Calib[num].f_HallNormalized_B*my_sinf_1_3 + PMSM_Calib[num].f_HallNormalized_C*my_sinf_1_3;

					PMSM_Calib[num].f_Hall[PMSM_Calib[num].i] = atan2f(PMSM_Calib[num].f_HallNormalized_Beta,PMSM_Calib[num].f_HallNormalized_Alpha);	
//					PMSM_Calib[num].f_Hall[PMSM_Calib[num].i] = atan2f(PMSM_Calib[num].f_HallNormalized_Beta,PMSM_Calib[num].f_HallNormalized_Alpha);						
					PMSM_Calib[num].i++;	
				}				
				else			
				{
					PMSM_Calib[num].AngleCnt --;
				}
			}
		}
		else
		{
			if (HALL_U[num] > PMSM_Calib[num].HallMax_A)
				PMSM_Calib[num].HallMax_A = HALL_U[num]; 					
			if (HALL_U[num] < PMSM_Calib[num].HallMin_A)
				PMSM_Calib[num].HallMin_A = HALL_U[num];

			if (HALL_V[num] > PMSM_Calib[num].HallMax_B)
				PMSM_Calib[num].HallMax_B = HALL_V[num]; 					
			if (HALL_V[num] < PMSM_Calib[num].HallMin_B)
				PMSM_Calib[num].HallMin_B = HALL_V[num]; 

			if (HALL_W[num] > PMSM_Calib[num].HallMax_C)
				PMSM_Calib[num].HallMax_C = HALL_W[num]; 					
			if (HALL_W[num] < PMSM_Calib[num].HallMin_C)
				PMSM_Calib[num].HallMin_C = HALL_W[num]; 			
			

			
			PMSM[num].foc.Theta = PMSM[num].foc.Theta + PMSM[num].Theta_acc;  
			PMSM_Calib[num].ZeroAngleCnt = 0;  
		}
		if((PMSM_Calib[num].AngleCnt > POLE_PAIRS*3) &&(PMSM[num].MotorCali == MegCaliPositive)&&(++PMSM_Calib[num].DIR_cnt>=ZERO_CNT))	
		{
			PMSM_Calib[num].DIR_cnt = 0;
			hall_encoder[num].Theta_Calib_Bias = 0;
			PMSM_Calib[num].AngleCnt = 0;
			PMSM[num].Theta_acc = -0.0008f;
			PMSM[num].foc.Theta = 0.0f;
			PMSM_Calib[num].timeCnt = 0;
			PMSM[num].MotorCali = MegCaliReverse;	
			PMSM_Calib[num].AngleCnt = 0 ;
			PMSM_Calib[num].ZeroAngleCnt = 0;
		}
		else if((PMSM_Calib[num].AngleCnt > POLE_PAIRS*3) &&(PMSM[num].MotorCali == MegCaliReverse)&&(++PMSM_Calib[num].DIR_cnt>=ZERO_CNT))
		{
			pmsm_stop(num);
			pmsm_break_io(num,true);
			for(PMSM_Calib[num].j=0; PMSM_Calib[num].j<PMSM_Calib[num].i; PMSM_Calib[num].j++)
			{
				PMSM_Calib[num].f_HallAngleCalibration += PMSM_Calib[num].f_Hall[PMSM_Calib[num].j];
			}
			
			PMSM_Calib[num].f_HallAngleCalibration = PMSM_Calib[num].f_HallAngleCalibration / (float)PMSM_Calib[num].i;
			

			hall_encoder[num].Median.HALL_U_VALUE = PMSM_Calib[num].HallMedian_A;
			hall_encoder[num].Median.HALL_V_VALUE = PMSM_Calib[num].HallMedian_B;
			hall_encoder[num].Median.HALL_W_VALUE = PMSM_Calib[num].HallMedian_C;
			
			hall_encoder[num].RANGE_Amplitude.HALL_U_VALUE = PMSM_Calib[num].Amplitude_A;
			hall_encoder[num].RANGE_Amplitude.HALL_V_VALUE = PMSM_Calib[num].Amplitude_B;
			hall_encoder[num].RANGE_Amplitude.HALL_W_VALUE = PMSM_Calib[num].Amplitude_C;
			hall_encoder[num].Theta_Calib_Bias = PMSM_Calib[num].f_HallAngleCalibration;
				
			Electric_Angle_Calibration_Check(num);
			if(PMSM[num].MotorCali == MegCaliOK)
			{
				PMSM[num].Theta_acc = 0.0008f;
				
				PMSM_Calib[num].DIR_cnt = 0;
				PMSM_Calib[num].timeCnt = 0;
				PMSM_Calib[num].AngleCnt = 0 ;
				PMSM_Calib[num].firstFlag = 0;
				PMSM_Calib[num].ZeroAngleCnt = 0;
				
				Calibration_SaveCalibrationElectricalData(num);
				Calibration_Save_Write();
			}
			if(PMSM[PMSM_A].MotorCali == MegCaliOK&&PMSM[PMSM_U].MotorCali == MegCaliOK)
			{
				MOS_DISABLE();
			}

			//Calibration_SaveCalibrationElectricalData();
			//GEAR_DATA_WRITE();
		}
	}
}


















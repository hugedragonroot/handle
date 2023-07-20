/**********************************************************/
//@Autor:Raw
//@Version:0.0.1
//@End Time:2023/3/21
//@Function:HALL_Encoder.c
/**********************************************************/

#include "HALL_Encoder.h"
#include "adc_callback.h"
#include "PLL.h"
#include "PMSM_Calib.h"
#include "filter.h"

hall_encoder_t hall_test ={0};
hall_encoder_t hall_encoder[2] ={0};



void HALL_Encoder_init(void)
{
	
//	hall_encoder[PMSM_A].RANGE_Amplitude.HALL_U_VALUE = 1256;
//	hall_encoder[PMSM_A].RANGE_Amplitude.HALL_V_VALUE = 1056;
//	hall_encoder[PMSM_A].RANGE_Amplitude.HALL_W_VALUE = 1043;
//	
//	hall_encoder[PMSM_A].Median.HALL_U_VALUE = 2207;
//	hall_encoder[PMSM_A].Median.HALL_V_VALUE= 2221;
//	hall_encoder[PMSM_A].Median.HALL_W_VALUE = 2213;
//	hall_encoder[PMSM_A].Theta_Calib_Bias = 1.01045001f;
//	
//	hall_encoder[PMSM_U].RANGE_Amplitude.HALL_U_VALUE = 918;
//	hall_encoder[PMSM_U].RANGE_Amplitude.HALL_V_VALUE = 970;
//	hall_encoder[PMSM_U].RANGE_Amplitude.HALL_W_VALUE = 1161;
//	
//	hall_encoder[PMSM_U].Median.HALL_U_VALUE = 2228;
//	hall_encoder[PMSM_U].Median.HALL_V_VALUE = 2231;
//	hall_encoder[PMSM_U].Median.HALL_W_VALUE = 2219;
//	hall_encoder[PMSM_U].Theta_Calib_Bias = 1.0726018f;
//	
//	Electric_Angle_Calibration_Check(PMSM_A);
//	Electric_Angle_Calibration_Check(PMSM_U);
//	
//	Calibration_SaveCalibrationElectricalData(PMSM_A);
//	Calibration_SaveCalibrationElectricalData(PMSM_U);
	
	Calibration_ReadCalibrationElectricalData(PMSM_A);
	Calibration_ReadCalibrationElectricalData(PMSM_U);
	Calibration_Save_Write();
	
	PLL_init((PLL_Num)PMSM_A,190.0f,4.0f,0.0001f);
	PLL_init((PLL_Num)PMSM_U,190.0f,4.0f,0.0001f);
	
}
void Theta_Read(PMSM_Num num)
{
	
	float TEMP_PMSM_HALL_U = 0.0f;	
	float TEMP_PMSM_HALL_V = 0.0f;
	float TEMP_PMSM_HALL_W = 0.0f;
	
	uint16_t HALL_U = adc_ReadAvgRaw(HALL_A_ADC,num);
	uint16_t HALL_V = adc_ReadAvgRaw(HALL_B_ADC,num);
	uint16_t HALL_W = adc_ReadAvgRaw(HALL_C_ADC,num);

	TEMP_PMSM_HALL_U = (float)(HALL_U - hall_encoder[num].Median.HALL_U_VALUE)/(float)(hall_encoder[num].RANGE_Amplitude.HALL_U_VALUE);
	TEMP_PMSM_HALL_V = (float)(HALL_V - hall_encoder[num].Median.HALL_V_VALUE)/(float)(hall_encoder[num].RANGE_Amplitude.HALL_V_VALUE);
	TEMP_PMSM_HALL_W = (float)(HALL_W - hall_encoder[num].Median.HALL_W_VALUE)/(float)(hall_encoder[num].RANGE_Amplitude.HALL_W_VALUE);
	
	hall_encoder[num].Theta_Beta = TEMP_PMSM_HALL_U - TEMP_PMSM_HALL_V*my_cosf_1_3 - TEMP_PMSM_HALL_W*my_cosf_1_3;
	hall_encoder[num].Theta_Alpha = -TEMP_PMSM_HALL_V*my_sinf_1_3 + TEMP_PMSM_HALL_W*my_sinf_1_3;
	
	hall_encoder[num].Theta_Compose = atan2f(hall_encoder[num].Theta_Alpha,hall_encoder[num].Theta_Beta);
//	hall_encoder[num].Theta_Compose = atan2f(hall_encoder[num].Theta_Beta,hall_encoder[num].Theta_Alpha);
	hall_encoder[num].Theta = hall_encoder[num].Theta_Compose - hall_encoder[num].Theta_Calib_Bias;
//	hall_encoder[num].Theta = hall_encoder[num].Theta_Compose + basic_temp;
	hall_encoder[num].ElecAngle = hall_encoder[num].Theta_Compose*my_rad+my_half_angle;
	
	
	
	if(hall_encoder[num].first_speed_flage == false)
	{
		hall_encoder[num].first_speed_flage = true;
		hall_encoder[num].last_Theta = hall_encoder[num].Theta;
		hall_encoder[num].turns = 0;
		
	}
	
	hall_encoder[num].PLL_realspeed = PllGetSpeed((PLL_Num)num,hall_encoder[num].Theta)*my_rpm*my_pairs;
//	hall_encoder[num].whill_pll = hall_encoder[num].whill_pll*0.99f + hall_encoder[num].PLL_realspeed*0.01f;
	if(num == PMSM_A) hall_encoder[PMSM_A].return_speed = (int16_t)hall_encoder[PMSM_A].PLL_realspeed ;
	if(num == PMSM_U) hall_encoder[PMSM_U].return_speed = (int16_t)(hall_encoder[PMSM_U].PLL_realspeed *-1.0f);
	hall_encoder[num].Theta_Interval = hall_encoder[num].Theta - hall_encoder[num].last_Theta;
	if(fabs(hall_encoder[num].Theta_Interval) > 3) 
	{
		if(hall_encoder[num].Theta > hall_encoder[num].last_Theta)
		{
			hall_encoder[num].Theta_Interval = hall_encoder[num].Theta - (PI_2 + hall_encoder[num].last_Theta);
			hall_encoder[num].turns --;
		}
		else
		{
			hall_encoder[num].Theta_Interval = (hall_encoder[num].Theta + PI_2) - hall_encoder[num].last_Theta;
			hall_encoder[num].turns ++;
		}
	}
	hall_encoder[num].last_Theta = hall_encoder[num].Theta;
	hall_encoder[num].AngleReal = ((float)hall_encoder[num].turns * my_unit_angle + hall_encoder[num].ElecAngle)*my_pairs; 
	
	if(++hall_encoder[num].speed_count>=2)
	{
//		hall_encoder[num].PLL_realspeed = PllGetSpeed((PLL_Num)num,hall_encoder[num].Theta)*my_rpm*my_pairs;
		hall_encoder[num].speed_count = 0;
		hall_encoder[num].real_speed = (hall_encoder[num].AngleReal - hall_encoder[num].lastAngle)*5000.0f*my_para_speed;
		hall_encoder[num].filter_speed = hall_encoder[num].filter_speed*0.9f+hall_encoder[num].real_speed*0.1f;
		if(num == PMSM_A)
		hall_encoder[num].whill_pll=biquadFilterApply(&accFilterLPF[0],hall_encoder[PMSM_A].PLL_realspeed);
		
		if(num == PMSM_U)
		hall_encoder[num].whill_pll=biquadFilterApply(&accFilterLPF[1],hall_encoder[PMSM_U].PLL_realspeed);
		hall_encoder[num].lastAngle = hall_encoder[num].AngleReal;
	}
//	hall_encoder[num].ElecAngle = hall_encoder[num].Theta_Compose*my_rad+180.0f;
	
}











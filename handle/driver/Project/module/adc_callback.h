/**********************************************************/
//@Autor:Raw
//@Version:0.0.1
//@End Time:2023/3/20
/**********************************************************/
#ifndef __ADC_CALLBACK_H__
#define __ADC_CALLBACK_H__
#include "adc.h"
#include "common.h"

#define Nx 3
typedef struct
{
	uint16_t adc_offer;
	float adc_current;
}Brush_adc_t;

typedef struct
{
	uint16_t HALL_A_offer;
	uint16_t HALL_B_offer;
	uint16_t HALL_C_offer;
	uint16_t HALL_A_RAW;
	uint16_t HALL_B_RAW;
	uint16_t HALL_C_RAW;
}PMSM_adc_t;

typedef struct
{
	
	float Bus_Volt;
	float mos_temp;
	Brush_adc_t Brush_adc[2];
	PMSM_adc_t 	PMSM_adc[2];
}ADC_Data_t;
typedef enum
{
	HALL_U_ADC = 0,
	HALL_V_ADC = 1,
	HALL_W_ADC = 2,
	Brush_adc_A = 3,
	Brush_adc_U = 4,
	MOS_TEMP = 5,
	MOTOR_TEMP1 = 6,
	MOTOR_TEMP2 = 7,
}adc2_num;

typedef enum
{
	HALL_A_ADC = 0,
	HALL_B_ADC = 1,
	HALL_C_ADC = 2,
	V_BUS = 3,
}adc0_num;

extern ADC_Data_t ADC_Data;
void GetVoltage_MOSTEMP(void);
void Brush_ADC_OffsetSample(void);
void Brush_GetCurrent(void);
uint16_t adc_ReadAvgRaw(uint8_t channel,PMSM_Num num);
uint8_t GildeAverageValueFilter_MAG(uint8_t NewValue,uint8_t *Data);
float Get_MOTOR_MOSTEMP(PMSM_Num num);
#endif

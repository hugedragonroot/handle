/**********************************************************/
//@Autor:Raw
//@Version:0.0.1
//@End Time:
/**********************************************************/
#ifndef __PMSM_CALIB_H__
#define __PMSM_CALIB_H__
#include "common.h"
#define POLE_PAIRS 4
#define Hall_adc_lim	800
#define Hall_adc_mid 2000
typedef struct
{
	uint32_t HallMax_A ;   //记录一个电周期线性霍尔A轴最大值
	uint32_t HallMin_A ;   //记录一个电周期线性霍尔A最小值

	uint32_t HallMax_B ;    //记录一个电周期线性霍尔B轴最大值
	uint32_t HallMin_B ;    //记录一个电周期线性霍尔B轴最小值

	uint32_t HallMax_C ;   //记录一个电周期线性霍尔C轴最大值
	uint32_t HallMin_C ;   //记录一个电周期线性霍尔C轴最小值

	uint32_t HallMedian_A ;   	//保存A轴线性霍尔中值
	uint32_t HallMedian_B ;    //保存B轴线性霍尔中值
	uint32_t HallMedian_C ;    //保存C轴线性霍尔中值

	uint16_t Amplitude_A;       	//记录A轴线性霍尔幅值 (最大值减去最小值除以2)
	uint16_t Amplitude_B;       	//记录B轴线性霍尔幅值  (最大值减去最小值除以2)
	uint16_t Amplitude_C;       	//记录C轴线性霍尔幅值  (最大值减去最小值除以2)
	
	float  f_HallNormalized_A ;          //A轴线性霍尔归一化值
	float  f_HallNormalized_B ;          //B轴线性霍尔归一化值
	float  f_HallNormalized_C ;          //C轴线性霍尔归一化值
	
	float f_HallNormalized_Beta;
	float f_HallNormalized_Alpha;
	float f_HallAngleCalibration;
	float f_Hall[POLE_PAIRS*6] ;
	uint16_t timeCnt ;	//进入当前任务的次数
	uint8_t firstFlag;   //首次进入函数标志  此标志首次进入时为0，此时赋予电角度初值

	uint8_t AngleCnt;	//角度计数，用于计算极对数的

	uint16_t ZeroAngleCnt;   //0角度计数
	uint16_t DIR_cnt;
	uint16_t Reverse_cnt;
	bool turn_flage;
	uint8_t i;
	uint8_t j;
}PMSM_Calib_t;
extern PMSM_Calib_t PMSM_Calib[2];
void Electric_Angle_Calibration(PMSM_Num num);
void Calibration_ReadCalibrationElectricalData(PMSM_Num num);
void Electric_Angle_Calibration_Check(PMSM_Num num);
void Calibration_SaveCalibrationElectricalData(PMSM_Num num);
void Calibration_Save_Write(void);

#endif



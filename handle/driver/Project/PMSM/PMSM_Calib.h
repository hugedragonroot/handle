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
	uint32_t HallMax_A ;   //��¼һ�����������Ի���A�����ֵ
	uint32_t HallMin_A ;   //��¼һ�����������Ի���A��Сֵ

	uint32_t HallMax_B ;    //��¼һ�����������Ի���B�����ֵ
	uint32_t HallMin_B ;    //��¼һ�����������Ի���B����Сֵ

	uint32_t HallMax_C ;   //��¼һ�����������Ի���C�����ֵ
	uint32_t HallMin_C ;   //��¼һ�����������Ի���C����Сֵ

	uint32_t HallMedian_A ;   	//����A�����Ի�����ֵ
	uint32_t HallMedian_B ;    //����B�����Ի�����ֵ
	uint32_t HallMedian_C ;    //����C�����Ի�����ֵ

	uint16_t Amplitude_A;       	//��¼A�����Ի�����ֵ (���ֵ��ȥ��Сֵ����2)
	uint16_t Amplitude_B;       	//��¼B�����Ի�����ֵ  (���ֵ��ȥ��Сֵ����2)
	uint16_t Amplitude_C;       	//��¼C�����Ի�����ֵ  (���ֵ��ȥ��Сֵ����2)
	
	float  f_HallNormalized_A ;          //A�����Ի�����һ��ֵ
	float  f_HallNormalized_B ;          //B�����Ի�����һ��ֵ
	float  f_HallNormalized_C ;          //C�����Ի�����һ��ֵ
	
	float f_HallNormalized_Beta;
	float f_HallNormalized_Alpha;
	float f_HallAngleCalibration;
	float f_Hall[POLE_PAIRS*6] ;
	uint16_t timeCnt ;	//���뵱ǰ����Ĵ���
	uint8_t firstFlag;   //�״ν��뺯����־  �˱�־�״ν���ʱΪ0����ʱ�����Ƕȳ�ֵ

	uint8_t AngleCnt;	//�Ƕȼ��������ڼ��㼫������

	uint16_t ZeroAngleCnt;   //0�Ƕȼ���
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



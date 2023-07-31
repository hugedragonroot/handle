#ifndef __CURVEPROGRAMME_H__
#define __CURVEPROGRAMME_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "Scurve.h"


/* ���Ͷ��� ------------------------------------------------------------------*/
typedef struct{
	float *speed_set;
	float *speed_now;
	float *speed_unit;
	float speed_unit_pre;
	SCURVE_STATE *state;
}CURVE_PARA_STR;


typedef enum{
	CtrlCurveWait,
	CtrlCurveStart,
	CtrlCurveQuickAdj,
	CtrlCurveNormalAdj,
	CtrlCurvelow,
	CtrlCurveEnd,
}Running_Curve_State;
typedef struct{
	CURVE_PARA_STR curve[SCURVE_ALL_NUM];
	float *VecAngle;
	float *VecValue;
	float VecAnglePre;
	float VecValuePre;
	uint8_t *Rank;
	uint8_t	RankPre;
	SYNC_STATE SyncFlag;
	float SpeedSetLeft;
	float SpeedSetRight;
	uint8_t region;           //ʸ����ǰ����λ��
	uint8_t region_last;
	uint8_t region_last_pre;
	uint8_t confine;          //�������ٶȴ�С�綨
	uint16_t SyncCount;
	bool start_flage;
	bool satrt_flage_last;
}RUNNING_CURVE_STR;


/* �궨�� --------------------------------------------------------------------*/


/* ��չ���� ------------------------------------------------------------------*/


/* �������� ------------------------------------------------------------------*/
void curve_config(SCURVE_NUM num);
void running_programme(SCURVE_NUM num);
void test_runningCurve(void);

#endif  // __SCURVE_H__

/******************* (C) COPYRIGHT 2015-2020  *****END OF FILE****/

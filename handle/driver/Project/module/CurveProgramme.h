#ifndef __CURVEPROGRAMME_H__
#define __CURVEPROGRAMME_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "Scurve.h"


/* 类型定义 ------------------------------------------------------------------*/
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
	uint8_t region;           //矢量当前所处位置
	uint8_t region_last;
	uint8_t region_last_pre;
	uint8_t confine;          //左右轮速度大小界定
	uint16_t SyncCount;
	bool start_flage;
	bool satrt_flage_last;
}RUNNING_CURVE_STR;


/* 宏定义 --------------------------------------------------------------------*/


/* 扩展变量 ------------------------------------------------------------------*/


/* 函数声明 ------------------------------------------------------------------*/
void curve_config(SCURVE_NUM num);
void running_programme(SCURVE_NUM num);
void test_runningCurve(void);

#endif  // __SCURVE_H__

/******************* (C) COPYRIGHT 2015-2020  *****END OF FILE****/

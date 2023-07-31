#ifndef __SCURVE_H__
#define __SCURVE_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "PMSM_Driver.h"
#include "common.h"


/* 类型定义 ------------------------------------------------------------------*/


typedef enum{
	SCURVE_1,
	SCURVE_2,
	SCURVE_ALL_NUM,
}SCURVE_NUM;

typedef enum{
	Sync,
	OutSync,
}SYNC_STATE;

typedef enum{
	ScurveWait,
	ScurveRunning,
	ScurveEnd,
	ScurveRunningRetry,
}SCURVE_STATE;

typedef struct{
	float T_per;
	float T_now;
	bool flag_running;
	float *speed_now;
	float *speed_target;
	float speed_buff;
	float speed_target_buff;
	float speed_target_pre;
	float speed_range;
	float acc;
	float acc_retry;
	int8_t polar;
	uint8_t zero_flag:1;
	bool sub_falge;
	float time;
	SCURVE_STATE ScurveState;
	bool overzero_dec_flag;
	float overzero_speed_range;
}SCURVE_STR;



/* 宏定义 --------------------------------------------------------------------*/
//#define SCURVE_MODUL_USED
#define LCURVE_MODUL_USED


/* 扩展变量 ------------------------------------------------------------------*/
extern SCURVE_STR scurve[SCURVE_ALL_NUM];
extern SYNC_STATE sync_flag;
extern uint8_t RetryDraw;
extern SYNC_STATE fast_flag;

/* 函数声明 ------------------------------------------------------------------*/
void scurve_config(SCURVE_NUM num);
void draw_scurve(SCURVE_NUM num);

#endif  // __SCURVE_H__

/******************* (C) COPYRIGHT 2015-2020  *****END OF FILE****/

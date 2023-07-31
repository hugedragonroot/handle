/**
  ******************************************************************************
  * 文件名程: CurveProgramme.c 
  * 作    者: 
  * 版    本: V0.1
  * 编写日期: 2023-06-14
  * 功    能: 规划运动曲线（双轮）
  ******************************************************************************
  * 说明：
  * 
  * 
  * 
  ******************************************************************************
  */
/* 包含头文件 ----------------------------------------------------------------*/
#include "CurveProgramme.h"
#include "common.h"
#include "arm_math.h"




/* 私有类型定义 --------------------------------------------------------------*/
typedef enum{
	speed_push_I = 500,
	speed_push_II = 400,
	speed_push_III = 900,
	speed_run_I = 1600,
	speed_run_II = 2000,
	speed_run_III = 2500,
}SpeedRank;//速度挡位设置



/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
RUNNING_CURVE_STR RunningCurve;
Running_Curve_State runstate = CtrlCurveWait;
SpeedRank SpeedMax[6] = {speed_push_I, speed_push_II, speed_push_III, speed_run_I, speed_run_II, speed_run_III};
//float wheel_gear[6] = {0.5f,0.8f,0.6f,0.5f,0.3f,0.26f};
//float return_gear[6] = {0.5f,0.5f,0.4f,0.3f,0.3f,0.3f};
//float quicky_gear[6] = {0.1f,0.1f,0.2f,0.5f,0.5f,0.5f};

float wheel_gear[6] = {0.4f,0.5f,0.35f,0.3f,0.25f,0.25f};
float return_gear[6] = {0.5f,0.5f,0.4f,0.3f,0.3f,0.3f};
float quicky_gear[6] = {0.1f,0.1f,0.2f,0.5f,0.5f,0.5f};

/* 扩展变量 ------------------------------------------------------------------*/




/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/

void curve_config(SCURVE_NUM num)
{
	scurve_config(num);
	RunningCurve.curve[num].speed_set = scurve[num].speed_target;
	RunningCurve.curve[num].speed_now = scurve[num].speed_now;
	RunningCurve.curve[num].state = &scurve[num].ScurveState;
	RunningCurve.VecAngle = &APP_PMSM.SET_COORANGLE_10X;
	RunningCurve.VecValue = &APP_PMSM.CoordSqrt;
	RunningCurve.Rank = &APP_PMSM.Gear;
	
//	RunningCurve.VecAngle = &angle_sss;
//	RunningCurve.VecValue = &sqrt_sss;
//	RunningCurve.Rank = &gear_sss;
}

static void LRspeed_delimit(void);
float test_key = 0;
void test_runningCurve(void)
{
	/* 模拟摇杆，取值：0 - 360 */
	#if 1
		*RunningCurve.VecValue = 1;
		*RunningCurve.VecAngle = test_key / 360 * PI_2;
		running_programme(SCURVE_1);
		running_programme(SCURVE_2);
	#endif
	
	/* 速度曲线规划 */
	#if 0
		*RunningCurve.VecValue = 1;
		*RunningCurve.VecAngle += 0.005;
		if(*RunningCurve.VecAngle > PI_2)
			*RunningCurve.VecAngle = 0;
		LRspeed_delimit();
		*RunningCurve.curve[SCURVE_1].speed_set = RunningCurve.SpeedSetLeft;
		*RunningCurve.curve[SCURVE_2].speed_set = RunningCurve.SpeedSetRight;
	#endif
}

/**
	* 函数功能: 划分左右轮速度.
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：估算左右轮平稳运行时的速度大小
  */
#define UNITMAXBACK     0.3f  //后退时速度相对于总速度的最大比例
#define UNITMINRUNNING  0.2f  //最低速度比例
#define PI_2l (PI/2.0f)
float test_speed;
float bxx;
float kxx;
static void LRspeed_delimit(void)
{
	float speedbuff;
	float speedbuff_temp;
	if(*RunningCurve.VecAngle >=0.0f&&*RunningCurve.VecAngle <= PI)
	{
		speedbuff = *RunningCurve.VecValue * SpeedMax[*RunningCurve.Rank] * ((1.0f - wheel_gear[*RunningCurve.Rank]) * arm_sin_f32(*RunningCurve.VecAngle) + wheel_gear[*RunningCurve.Rank]);
//		speedbuff_temp = *RunningCurve.VecValue * SpeedMax[*RunningCurve.Rank] * ((1.0f + wheel_gear[*RunningCurve.Rank]) * arm_sin_f32(*RunningCurve.VecAngle) - wheel_gear[*RunningCurve.Rank]);
		bxx = (*RunningCurve.VecValue) * (SpeedMax[*RunningCurve.Rank]) * (wheel_gear[*RunningCurve.Rank]);
		kxx = ((*RunningCurve.VecValue) * (SpeedMax[*RunningCurve.Rank]) +bxx)/ PI_2l;
		
//		bxx = (*RunningCurve.VecValue) * (SpeedMax[*RunningCurve.Rank]) * (wheel_gear[*RunningCurve.Rank]);
//		kxx = ((1.0f+wheel_gear[*RunningCurve.Rank])/1.0f)/ PI_2l;
		
		if(*RunningCurve.VecAngle >=0.0f&&*RunningCurve.VecAngle <= PI)
		{
			if(*RunningCurve.VecAngle > PI_2l)
			{
//				
				RunningCurve.SpeedSetLeft = kxx*(PI - *RunningCurve.VecAngle) - bxx;
				RunningCurve.SpeedSetRight = speedbuff;
				
//				RunningCurve.SpeedSetLeft = speedbuff_temp;
//				RunningCurve.SpeedSetRight = speedbuff;
				RunningCurve.region = 0x00;
				if(*RunningCurve.VecAngle <= PI_2l*95.0f/90.0f)
				{
					RunningCurve.region = 0x01;
				}
			}
			else if(*RunningCurve.VecAngle == PI_2l)
			{

				RunningCurve.region = 0x01;
				
				RunningCurve.SpeedSetLeft = kxx*(PI - *RunningCurve.VecAngle) - bxx;
				RunningCurve.SpeedSetRight = speedbuff;
			}
			else
			{
//				
				RunningCurve.SpeedSetLeft =speedbuff;
				RunningCurve.SpeedSetRight = kxx*(*RunningCurve.VecAngle) - bxx;
				
//				RunningCurve.SpeedSetLeft = speedbuff;
//				RunningCurve.SpeedSetRight = speedbuff_temp;
				RunningCurve.region = 0x00;
				if(*RunningCurve.VecAngle >= PI_2l*85.0f/90.0f)
				{
					RunningCurve.region = 0x01;
				}
			}
		}
	}
	else if(*RunningCurve.VecAngle <= PI_2&&*RunningCurve.VecAngle >= PI)
	{
		speedbuff = -*RunningCurve.VecValue * SpeedMax[*RunningCurve.Rank] * ((return_gear[*RunningCurve.Rank] - wheel_gear[*RunningCurve.Rank]) * arm_sin_f32(*RunningCurve.VecAngle - PI) + wheel_gear[*RunningCurve.Rank]);
//		speedbuff_temp = -*RunningCurve.VecValue * SpeedMax[*RunningCurve.Rank] * ((return_gear[*RunningCurve.Rank] + wheel_gear[*RunningCurve.Rank]) * arm_sin_f32(*RunningCurve.VecAngle - PI) - wheel_gear[*RunningCurve.Rank]);
		if(*RunningCurve.VecAngle < PI_2l * 3.0f)
		{
			bxx = -*RunningCurve.VecValue * SpeedMax[*RunningCurve.Rank]* wheel_gear[*RunningCurve.Rank];
			kxx = (-*RunningCurve.VecValue* SpeedMax[*RunningCurve.Rank]*return_gear[*RunningCurve.Rank] + bxx)/PI_2l;
			RunningCurve.SpeedSetLeft = speedbuff;
			RunningCurve.SpeedSetRight = kxx*(*RunningCurve.VecAngle - PI) - bxx;
			
//			RunningCurve.SpeedSetLeft = speedbuff;
//			RunningCurve.SpeedSetRight = speedbuff_temp;
			
			RunningCurve.region = 0x00;
		}
		else
		{
			kxx = *RunningCurve.VecValue* SpeedMax[*RunningCurve.Rank]*(return_gear[*RunningCurve.Rank] + wheel_gear[*RunningCurve.Rank])/PI_2l;
			bxx = -*RunningCurve.VecValue * SpeedMax[*RunningCurve.Rank]* (2.0f*return_gear[*RunningCurve.Rank] + wheel_gear[*RunningCurve.Rank]);
			RunningCurve.SpeedSetLeft = kxx*(*RunningCurve.VecAngle - PI) + bxx;
			RunningCurve.SpeedSetRight = speedbuff;
			
//			RunningCurve.SpeedSetLeft = speedbuff;
//			RunningCurve.SpeedSetRight = speedbuff_temp;
			RunningCurve.region = 0x00;
		}
	}
	if(*RunningCurve.VecValue == 0.0f)
	{
		RunningCurve.region_last_pre =	RunningCurve.region_last = RunningCurve.region;
	}

	RunningCurve.SpeedSetLeft = RunningCurve.SpeedSetLeft;
	RunningCurve.SpeedSetRight = -RunningCurve.SpeedSetRight;
	

}
/**
	* 函数功能: 运动曲线规划.
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
//#define abs(a)         (a < 0 ? -a : a)
//#define max(a, b)      (a > b ? a : b)
//#define abs_sub(a, b)  (abs(abs(a) - abs(b)))
uint16_t temp_region  = 0;
void running_programme(SCURVE_NUM num)
{
	float speedbuff = 0.0f;
//	float LRUnitMax = 0.0f;
//	float UnitFB = 0.0f;
	
	
	switch(runstate)
	{
		case CtrlCurveWait:
			if(RunningCurve.VecValuePre == 0.0f&&*RunningCurve.VecValue !=0.0f)
			{
				RunningCurve.start_flage = true;
			}
			else if(*RunningCurve.curve[SCURVE_1].speed_now!=0.0f&&*RunningCurve.curve[SCURVE_2].speed_now!=0.0f&&RunningCurve.VecValuePre >=0.3f)
			{
				if((*RunningCurve.curve[SCURVE_1].speed_now>=*RunningCurve.curve[SCURVE_1].speed_set*0.5f) && (*RunningCurve.curve[SCURVE_2].speed_now>=*RunningCurve.curve[SCURVE_2].speed_set*0.5f))
				RunningCurve.start_flage = false;
			}
			if((*RunningCurve.VecAngle != RunningCurve.VecAnglePre) || (*RunningCurve.VecValue != RunningCurve.VecValuePre)||*RunningCurve.Rank != RunningCurve.RankPre)
			{
				RunningCurve.VecAnglePre = *RunningCurve.VecAngle;
				RunningCurve.VecValuePre = *RunningCurve.VecValue;
				RunningCurve.RankPre = *RunningCurve.Rank;
				runstate = CtrlCurveStart;
			}
		
//			if((*RunningCurve.curve[SCURVE_1].speed_unit != RunningCurve.curve[SCURVE_1].speed_unit_pre) || (*RunningCurve.curve[SCURVE_2].speed_unit != RunningCurve.curve[SCURVE_2].speed_unit_pre))
//			{
//				RunningCurve.curve[SCURVE_1].speed_unit_pre = *RunningCurve.curve[SCURVE_1].speed_unit;
//				RunningCurve.curve[SCURVE_2].speed_unit_pre = *RunningCurve.curve[SCURVE_2].speed_unit;
//				runstate = CtrlCurveStart;
//			}
		break;
		case CtrlCurveStart:
			/* 计算当前坐标下左右轮的目标速度 */
			/*正方形控制*/
			/*
			if((*RunningCurve.curve[SCURVE_1].speed_unit > 0) && (*RunningCurve.curve[SCURVE_2].speed_unit > 0))
				UnitFB = 1.0f;
			else
				UnitFB = UNITMAXBACK;
			LRUnitMax = max(abs(*RunningCurve.curve[SCURVE_1].speed_unit), abs(*RunningCurve.curve[SCURVE_2].speed_unit));
			speedbuff = SpeedMax[*RunningCurve.Rank] * LRUnitMax * ((UnitFB - wheel_gear[*RunningCurve.Rank]) \
									* (1 - abs_sub(*RunningCurve.curve[SCURVE_1].speed_unit, *RunningCurve.curve[SCURVE_2].speed_unit) / LRUnitMax) * wheel_gear[*RunningCurve.Rank]);
			RunningCurve.SpeedSetLeft = speedbuff * *RunningCurve.curve[SCURVE_1].speed_unit;
			RunningCurve.SpeedSetRight = speedbuff * *RunningCurve.curve[SCURVE_2].speed_unit;
			*/
		
			/* 圆形控制，左右轮速度大小分配 */
			LRspeed_delimit();
		
			/* 确定运动曲线的调整方式 */
			runstate = CtrlCurveNormalAdj;
			sync_flag = Sync;
			fast_flag = Sync;
			/*//快速调整位置
			if(*RunningCurve.curve[SCURVE_1].speed_now > *RunningCurve.curve[SCURVE_2].speed_now)
			{
				if((RunningCurve.SpeedSetLeft < *RunningCurve.curve[SCURVE_2].speed_now) && (RunningCurve.SpeedSetRight > *RunningCurve.curve[SCURVE_1].speed_now))
				{
					*RunningCurve.curve[SCURVE_1].speed_set = *RunningCurve.curve[SCURVE_2].speed_now;//快速调整速度
					*RunningCurve.curve[SCURVE_2].speed_set = *RunningCurve.curve[SCURVE_1].speed_now;
					runstate = CtrlCurveQuickAdj;
					sync_flag = OutSync;
				}
				RunningCurve.confine = 0x10;
			}
			else if(*RunningCurve.curve[SCURVE_1].speed_now < *RunningCurve.curve[SCURVE_2].speed_now)
			{
				if((RunningCurve.SpeedSetLeft > *RunningCurve.curve[SCURVE_2].speed_now) && (RunningCurve.SpeedSetRight < *RunningCurve.curve[SCURVE_1].speed_now))
				{
					*RunningCurve.curve[SCURVE_1].speed_set = *RunningCurve.curve[SCURVE_2].speed_now;//快速调整速度
					*RunningCurve.curve[SCURVE_2].speed_set = *RunningCurve.curve[SCURVE_1].speed_now;
					runstate = CtrlCurveQuickAdj;
					sync_flag = OutSync;
				}
				RunningCurve.confine = 0x01;
			}*/
			
//			if(RunningCurve.start_flage  == true)
//			{
//				runstate = CtrlCurvelow;
//				sync_flag = OutSync;
//				*RunningCurve.curve[SCURVE_1].speed_set = RunningCurve.SpeedSetLeft;
//				*RunningCurve.curve[SCURVE_2].speed_set = RunningCurve.SpeedSetRight;
////				fast_flag = OutSync;
//			}
			/*快速修正角度，速度的正负与电机的固有正方向有关*/

				if(*RunningCurve.Rank >=3)
				{
						if(RunningCurve.region != RunningCurve.region_last)
						{
							temp_region = RunningCurve.region<<4|RunningCurve.region_last<<0;
							if(temp_region == 0x10)
							{
								if(-*RunningCurve.curve[SCURVE_1].speed_now > *RunningCurve.curve[SCURVE_2].speed_now)
								{
									*RunningCurve.curve[SCURVE_1].speed_set = -*RunningCurve.curve[SCURVE_2].speed_now;//快速调整速度
									*RunningCurve.curve[SCURVE_2].speed_set = -*RunningCurve.curve[SCURVE_1].speed_now;
									runstate = CtrlCurveQuickAdj;
									sync_flag = OutSync;
									fast_flag = OutSync;
								
									RunningCurve.confine = 0x10;
								}
								else if(-*RunningCurve.curve[SCURVE_1].speed_now < *RunningCurve.curve[SCURVE_2].speed_now)
								{
									*RunningCurve.curve[SCURVE_1].speed_set = -*RunningCurve.curve[SCURVE_2].speed_now;//快速调整速度
									*RunningCurve.curve[SCURVE_2].speed_set = -*RunningCurve.curve[SCURVE_1].speed_now;
									runstate = CtrlCurveQuickAdj;
									sync_flag = OutSync;
									fast_flag = OutSync;
									RunningCurve.confine = 0x01;
								}
							}
							RunningCurve.region_last_pre = RunningCurve.region_last;
							RunningCurve.region_last = RunningCurve.region;
//						}
						}						
				}
			
			
				if(sync_flag == Sync)
				{
					*RunningCurve.curve[SCURVE_1].speed_set = RunningCurve.SpeedSetLeft;
					*RunningCurve.curve[SCURVE_2].speed_set = RunningCurve.SpeedSetRight;
				}
			
		break;
		case CtrlCurveQuickAdj:
//			if((*RunningCurve.VecAngle != RunningCurve.VecAnglePre) || (*RunningCurve.VecValue != RunningCurve.VecValuePre)||(*RunningCurve.Rank != RunningCurve.RankPre))
//			{
//				runstate = CtrlCurveWait;
//				break;
//			}
			/* 检测是否快速调整到目标位置，速度的正负与电机的固有正方向有关 */
			if(((RunningCurve.confine == 0x10) && (-*RunningCurve.curve[SCURVE_1].speed_now <= *RunningCurve.curve[SCURVE_2].speed_now))\
				 || ((RunningCurve.confine == 0x01) && (-*RunningCurve.curve[SCURVE_1].speed_now >= *RunningCurve.curve[SCURVE_2].speed_now))
			||((RunningCurve.confine == 0x10) && (fabs(-*RunningCurve.curve[SCURVE_1].speed_now - *RunningCurve.curve[SCURVE_2].speed_now))<(160.0f*quicky_gear[*RunningCurve.Rank]))
			||((RunningCurve.confine == 0x01) && (fabs(-*RunningCurve.curve[SCURVE_2].speed_now - *RunningCurve.curve[SCURVE_1].speed_now)<(160.0f*quicky_gear[*RunningCurve.Rank]))))
			{
					*RunningCurve.curve[SCURVE_1].speed_set = RunningCurve.SpeedSetLeft;
					*RunningCurve.curve[SCURVE_2].speed_set = RunningCurve.SpeedSetRight;
					runstate = CtrlCurveNormalAdj;
					sync_flag = Sync;
					fast_flag = Sync;
					RetryDraw = 1;

			}
			draw_scurve(num);
		break;
		case CtrlCurveNormalAdj:
			if((*RunningCurve.VecAngle != RunningCurve.VecAnglePre) || (*RunningCurve.VecValue != RunningCurve.VecValuePre)||(*RunningCurve.Rank != RunningCurve.RankPre))
			{
				runstate = CtrlCurveWait;
				break;
			}
			*RunningCurve.curve[SCURVE_1].speed_set = RunningCurve.SpeedSetLeft;
			*RunningCurve.curve[SCURVE_2].speed_set = RunningCurve.SpeedSetRight;
			draw_scurve(num);
//			if((*RunningCurve.curve[SCURVE_1].state == ScurveWait) && (*RunningCurve.curve[SCURVE_2].state == ScurveWait))
			if((*RunningCurve.curve[SCURVE_1].speed_set == *RunningCurve.curve[SCURVE_1].speed_now) && (*RunningCurve.curve[SCURVE_2].speed_set == *RunningCurve.curve[SCURVE_2].speed_now))
			{
				RunningCurve.start_flage = false;
				runstate = CtrlCurveEnd;
			}
		break;
		case CtrlCurveEnd:
			runstate = CtrlCurveWait;
		break;
		default:
			
		break;
	}
}




/******************* (C) COPYRIGHT 2015-2020  *****END OF FILE****/

/**
  ******************************************************************************
  * 文件名程: Scurve.c 
  * 作    者: 
  * 版    本: V0.1
  * 编写日期: 2023-4-12
  * 功    能: S型曲线
  ******************************************************************************
  * 说明：
  * 
  * 
  * 
  ******************************************************************************
  */
/* 包含头文件 ----------------------------------------------------------------*/
#include "Scurve.h"
#include "CurveProgramme.h"
#include "common.h"
//#include "my_system.h"



/* 私有类型定义 --------------------------------------------------------------*/
typedef enum{
	UnoverZero,
	OverZero,
}ZEROSTATE_E;

/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/


/* 扩展变量 ------------------------------------------------------------------*/
SCURVE_STR scurve[SCURVE_ALL_NUM] = {{.ScurveState = ScurveWait},{.ScurveState = ScurveWait}};
uint8_t RetryDraw = 0;//重新绘制速度曲线
extern RUNNING_CURVE_STR RunningCurve;

/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/

void scurve_config(SCURVE_NUM num)
{
//	scurve[num].speed_now = &PMSM[(PMSM_Num)num].SpeedCmd;
//	scurve[num].speed_target = &PMSM[(PMSM_Num)num].SpeedSet;
	
	scurve[num].speed_now = &PMSM[(PMSM_Num)num].SpeedSet;
	scurve[num].speed_target = &PMSM[(PMSM_Num)num].SpeedNew;
}

#define SPEEDTOTAL     2100.0f
#define TIME_ACC       4.0f   //单位：s
#define TIME_SUB       0.9f
#define TIME_STOP_MIN  0.75f
#define BUMPER_VALUE 	30.0f
#define TIME_GEAR			 3.2f
#define BACK_ACC			 3.2F
//float acc_gear[6]={4.0f,4.0f,3.0f,2.4f,1.8f,1.4f};
//float acc_normal[6] ={3.5f,3.5f,3.2f,3.0f,3.0f,2.5f};
//float sub_normal[6] ={1.6f,1.6f,1.4f,1.1f,0.9f,0.6f};
//float acc_gear[6]={4.5f,4.5f,3.8f,3.0f,2.5f,2.0f};
//float acc_normal[6] ={4.0f,4.0f,4.0f,3.5f,3.0f,4.2f};
//float sub_normal[6] ={2.0f,2.0f,2.0f,1.2f,1.2f,1.4f};
//float dec_accgear[6] = {5.5f,5.5f,5.0f,4.5f,4.2f,3.5f};
//float dec_subgear[6] = {6.0f,6.0f,5.5f,5.0f,4.5f,3.5f};




float TIMER_ACC_T = 0.0f;
float TIMER_SUB_T = 0.0f;
bool flage_back = false;
static void get_time_for_scurve(SCURVE_NUM num)
{
//	
		if(fast_flag == OutSync)
		{
//			TIMER_ACC_T = acc_gear[*RunningCurve.Rank]+fabs(arm_sin_f32(*RunningCurve.VecAngle))*(*RunningCurve.VecValue)*(*RunningCurve.Rank/3.5f);
//			TIMER_SUB_T = acc_gear[*RunningCurve.Rank]+fabs(arm_sin_f32(*RunningCurve.VecAngle))*(*RunningCurve.VecValue)*(*RunningCurve.Rank/3.5f);
			TIMER_ACC_T = TIME_GEAR;
			TIMER_SUB_T = TIME_GEAR;
		}
		else if((*scurve[SCURVE_1].speed_now <0&&*scurve[SCURVE_2].speed_now>0)||((*scurve[SCURVE_1].speed_now)*(*scurve[SCURVE_2].speed_now))>0.0f)
		{
			TIMER_ACC_T = TIME_GEAR;
			TIMER_SUB_T = TIME_GEAR;
		}
		else
		{
			TIMER_ACC_T = TIME_ACC;
			TIMER_SUB_T = TIME_SUB;
		}
		
//		if(RunningCurve.start_flage == true)
//		{
//			TIMER_ACC_T = TIME_ACC*2.0f;
//		}
//		else
//		{
//			TIMER_ACC_T = TIME_ACC;
//		}
//		
//		TIMER_ACC_T = acc_normal[*RunningCurve.Rank];
//		TIMER_SUB_T = sub_normal[*RunningCurve.Rank];
		
	if(*scurve[num].speed_now < 0)
	{
		if(*scurve[num].speed_target <= *scurve[num].speed_now)
		{
			scurve[num].time = TIMER_ACC_T;
			scurve[num].sub_falge = false;
			scurve[num].zero_flag = UnoverZero;
		}
		else if(*scurve[num].speed_target > 0)
		{
			scurve[num].time = TIMER_SUB_T;
			scurve[num].zero_flag = OverZero;
			scurve[num].sub_falge = false;
		}
		else
		{
			scurve[num].sub_falge = true;
			scurve[num].time = TIMER_SUB_T;
			scurve[num].zero_flag = UnoverZero;
		}
	}
	else if(*scurve[num].speed_now > 0)
	{
		if(*scurve[num].speed_target >= *scurve[num].speed_now)
		{
			scurve[num].sub_falge = false;
			scurve[num].time = TIMER_ACC_T;
			scurve[num].zero_flag = UnoverZero;
		}
		else if(*scurve[num].speed_target < 0)
		{

			scurve[num].time = TIMER_SUB_T;
			scurve[num].zero_flag = OverZero;
			scurve[num].sub_falge = false;
		}
		else
		{
			scurve[num].time = TIMER_SUB_T;
			scurve[num].zero_flag = UnoverZero;
			scurve[num].sub_falge = true;
		}
	}
	else if(*scurve[num].speed_now == 0)
	{
		scurve[num].sub_falge = true;
		scurve[num].time = TIMER_ACC_T;
		scurve[num].zero_flag = UnoverZero;
	}
}

/**
	* 函数功能: 同步函数.
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：曲线同步处理，若改变曲线数量，此次需要修改
  */
#define min(a, b)      (a < b ? a : b)
uint8_t synccount = 0;
SYNC_STATE sync_flag = OutSync;
SYNC_STATE fast_flag = OutSync;
float  T_MIN = 	0.0025f;
static float update_time_per = 0.001f;
void sync_curve_handle(SCURVE_NUM num)
{
	float T;
	
	if(sync_flag == Sync)
	{
		if(scurve[num].zero_flag == UnoverZero)
		{
			if(++synccount == SCURVE_ALL_NUM)
			{
				if(num != SCURVE_1)
				{
					if(scurve[SCURVE_1].T_now != 0)
					{
						scurve[SCURVE_1].ScurveState = ScurveWait;
						synccount--;
						return;
					}
				}
				if(num != SCURVE_2)
				{
					if(scurve[SCURVE_2].T_now != 0)
					{
						scurve[SCURVE_2].ScurveState = ScurveWait;
						synccount--;
						return;
					}
				}
				
				T = min(scurve[SCURVE_1].T_per, scurve[SCURVE_2].T_per);
				
				if((*scurve[SCURVE_1].speed_now > 0 && *scurve[SCURVE_2].speed_now < 0) && (scurve[SCURVE_1].sub_falge != scurve[SCURVE_2].sub_falge))
				{
					if(T>update_time_per / TIME_STOP_MIN) T = update_time_per / TIME_STOP_MIN;
				}
				if(T)
				{
					scurve[SCURVE_1].T_per = T;
					scurve[SCURVE_2].T_per = T;
				}
				sync_flag = OutSync;
				synccount = 0;
			}
			
			scurve[num].overzero_dec_flag = false;
		}
//		else if(*scurve[SCURVE_1].speed_target * *scurve[SCURVE_1].speed_now <= 0 && *scurve[SCURVE_2].speed_target * *scurve[SCURVE_2].speed_now <= 0)
//		else if(*scurve[num].speed_target * *scurve[num].speed_now <= 0)
//		{
//			if(!scurve[num].overzero_dec_flag)
//				scurve[num].overzero_speed_range = scurve[num].speed_range;
//			else
//			{
//				scurve[num].speed_range = scurve[num].overzero_speed_range;
//			}
//			
//				if(scurve[num].T_per > update_time_per / TIME_STOP_MIN)
//					scurve[num].T_per = update_time_per / TIME_STOP_MIN;
//			
//			scurve[num].overzero_dec_flag = true;
//		}
	}
}

/**
	* 函数功能: S型曲线.
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
float test_scurve = 0;
float ratio_Acc = 1.0f;

void draw_scurve(SCURVE_NUM num)
{
	switch(scurve[num].ScurveState)
	{
		case ScurveWait:
			if(*scurve[num].speed_now != *scurve[num].speed_target)
			{
				get_time_for_scurve(num);
				
				scurve[num].speed_buff = *scurve[num].speed_now;
				scurve[num].speed_target_pre = *scurve[num].speed_target;
				if(scurve[num].zero_flag == UnoverZero)
					scurve[num].speed_target_buff = *scurve[num].speed_target;
				else if(scurve[num].zero_flag == OverZero)//曲线越过零点时速度先归零，再反向加速
					scurve[num].speed_target_buff = 0;
				
				if(scurve[num].speed_target_buff < scurve[num].speed_buff)
				{
					scurve[num].speed_range = scurve[num].speed_buff - scurve[num].speed_target_buff;
					scurve[num].polar = -1;
				}
				else
				{
					scurve[num].speed_range = scurve[num].speed_target_buff - scurve[num].speed_buff;
					scurve[num].polar = 1;
				}
				scurve[num].T_per = update_time_per * SPEEDTOTAL / scurve[num].speed_range / scurve[num].time;
//				if((scurve[num].T_per > update_time_per / TIME_STOP_MIN) && *scurve[num].speed_target == 0)
//				if((scurve[num].T_per > update_time_per / TIME_STOP_MIN)&&((fabsf(*scurve[num].speed_target) <= BUMPER_VALUE)))
					if((scurve[num].T_per > update_time_per / TIME_STOP_MIN)&&((scurve[SCURVE_1].sub_falge == true&&scurve[SCURVE_2].sub_falge == true)||(fabsf(*scurve[num].speed_target) <= BUMPER_VALUE)))
					scurve[num].T_per = update_time_per / TIME_STOP_MIN;
					
				/* 曲线同步处理函数 */
				sync_curve_handle(num);
				
				scurve[num].ScurveState = ScurveRunning;
				scurve[num].T_now = 0;
				
//				scurve[num].speed_target_pre = *scurve[num].speed_target;
				
				test_scurve = system_times;//测试
			}
		break;
		case ScurveRunning:
//			if(RunningCurve.start_flage !=RunningCurve.satrt_flage_last )
//			{
//				scurve[num].ScurveState = ScurveWait;
//				RunningCurve.satrt_flage_last = RunningCurve.start_flage ;
//				break;
//			}
//			get_time_for_scurve(num);
			#ifdef SCURVE_MODUL_USED
		 	  scurve[num].acc = 6.0f * scurve[num].T_now * (1.0f - scurve[num].T_now) * scurve[num].T_per * scurve[num].polar * scurve[num].speed_range;
			#endif
			#ifdef LCURVE_MODUL_USED
		
//			if(RunningCurve.start_flage == true)
//			{
//				ratio_Acc = fabs(*scurve[num].speed_now)/fabs(*scurve[num].speed_target)*20.0f;
//			}
//			else
//			{
//				ratio_Acc = ratio_Acc+0.00001f;
//			}
//			if(ratio_Acc>1.0f) ratio_Acc = 1.0f;
//			if(ratio_Acc<0.5f) ratio_Acc = 0.5f;
//				if(fabs(*scurve[num].speed_now)<=300.0f) ratio_Acc = fabs(*scurve[num].speed_now)/300.0f;
//				else 
//				{
//					ratio_Acc = ratio_Acc+0.0001f;
//				}
//					
//				if(ratio_Acc>1.0f) ratio_Acc = 1.0f;
//				if(ratio_Acc<0.1f) ratio_Acc = 0.1f;
//				scurve[num].T_per *=ratio_Acc;
//		
				scurve[num].acc = scurve[num].T_per * scurve[num].polar*scurve[num].speed_range*ratio_Acc;//SPEEDTOTAL * update_time_per / scurve[num].time;
			#endif
			scurve[num].speed_buff += scurve[num].acc;
			if(((scurve[num].polar == 1) && (scurve[num].speed_buff > scurve[num].speed_target_buff)) || ((scurve[num].polar == -1) && (scurve[num].speed_buff < scurve[num].speed_target_buff)))
				scurve[num].speed_buff = scurve[num].speed_target_buff;
//				scurve[num].ScurveState = ScurveEnd;
			*scurve[num].speed_now = scurve[num].speed_buff;
			scurve[num].T_now += scurve[num].T_per*ratio_Acc;
			if(scurve[num].T_now > 1)
				scurve[num].ScurveState = ScurveEnd;
			
			#ifdef SCURVE_MODUL_USED
			if((*scurve[num].speed_target != scurve[num].speed_target_buff) && (*scurve[num].speed_target * *scurve[num].speed_now >= 0))
			{
				scurve[num].acc_retry = scurve[num].acc;
				scurve[num].ScurveState = ScurveRunningRetry;
			}
			#endif
			#ifdef LCURVE_MODUL_USED
//			if(((*scurve[num].speed_target != scurve[num].speed_target_buff) && (*scurve[num].speed_target * *scurve[num].speed_now >= 0)) || RetryDraw == 1)
			if((*scurve[num].speed_target != scurve[num].speed_target_pre) )
			{
				scurve[num].ScurveState = ScurveWait;
//				if(RetryDraw)
//					RetryDraw = 0;
			}
			#endif
		break;
		case ScurveEnd:
			*scurve[num].speed_now = scurve[num].speed_target_buff;
			scurve[num].zero_flag = UnoverZero;
			scurve[num].ScurveState = ScurveWait;
			if(*scurve[num].speed_now == *scurve[num].speed_target)//到达目标速度后清除同步标志
			{
				sync_flag = OutSync;
				synccount = 0;
			}
			
			
			test_scurve = system_times - test_scurve;//测试
		break;
		case ScurveRunningRetry:
			scurve[num].acc_retry -= 0.1f * scurve[num].acc;
			if(scurve[num].acc_retry <= 0)
			{
//				scurve[num].zero_flag = UnoverZero;
				scurve[num].ScurveState = ScurveWait;
				break;
			}
			scurve[num].speed_buff += scurve[num].acc_retry;
			if(((scurve[num].polar == 1) && (scurve[num].speed_buff > scurve[num].speed_target_buff)) || ((scurve[num].polar == -1) && (scurve[num].speed_buff < scurve[num].speed_target_buff)))
				scurve[num].speed_buff = scurve[num].speed_target_buff;
			*scurve[num].speed_now = scurve[num].speed_buff;
			
		break;
		default:
		break;
	}
	
}




/******************* (C) COPYRIGHT 2015-2020  *****END OF FILE****/

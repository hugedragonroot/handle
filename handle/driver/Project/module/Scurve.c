/**
  ******************************************************************************
  * �ļ�����: Scurve.c 
  * ��    ��: 
  * ��    ��: V0.1
  * ��д����: 2023-4-12
  * ��    ��: S������
  ******************************************************************************
  * ˵����
  * 
  * 
  * 
  ******************************************************************************
  */
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "Scurve.h"
#include "CurveProgramme.h"
#include "common.h"
//#include "my_system.h"



/* ˽�����Ͷ��� --------------------------------------------------------------*/
typedef enum{
	UnoverZero,
	OverZero,
}ZEROSTATE_E;

/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/


/* ��չ���� ------------------------------------------------------------------*/
SCURVE_STR scurve[SCURVE_ALL_NUM] = {{.ScurveState = ScurveWait},{.ScurveState = ScurveWait}};
uint8_t RetryDraw = 0;//���»����ٶ�����
extern RUNNING_CURVE_STR RunningCurve;

/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/

void scurve_config(SCURVE_NUM num)
{
//	scurve[num].speed_now = &PMSM[(PMSM_Num)num].SpeedCmd;
//	scurve[num].speed_target = &PMSM[(PMSM_Num)num].SpeedSet;
	
	scurve[num].speed_now = &PMSM[(PMSM_Num)num].SpeedSet;
	scurve[num].speed_target = &PMSM[(PMSM_Num)num].SpeedNew;
}

#define SPEEDTOTAL     2100.0f
#define TIME_ACC       2.5f   //��λ��s
#define TIME_SUB       0.6f
#define TIME_STOP_MIN  0.3f
//float acc_gear[6]={4.0f,4.0f,3.0f,2.4f,1.8f,1.4f};
//float acc_normal[6] ={3.5f,3.5f,3.2f,3.0f,3.0f,2.5f};
//float sub_normal[6] ={1.6f,1.6f,1.4f,1.1f,0.9f,0.6f};
float acc_gear[6]={4.0f,4.0f,3.0f,2.4f,1.8f,1.4f};
float acc_normal[6] ={3.5f,3.5f,3.2f,3.0f,3.0f,2.5f};
float sub_normal[6] ={1.6f,1.6f,1.4f,1.1f,0.9f,0.7f};

static void get_time_for_scurve(SCURVE_NUM num)
{
	float TIMER_ACC_T = 0.0f;
	float TIMER_SUB_T = 0.0f;
	if(fast_flag == OutSync)
	{
		TIMER_ACC_T = acc_gear[*RunningCurve.Rank];
		TIMER_SUB_T = acc_gear[*RunningCurve.Rank];
	}
	else
	{
		TIMER_ACC_T = acc_normal[*RunningCurve.Rank];
		TIMER_SUB_T = sub_normal[*RunningCurve.Rank];
	}
//	if(fast_flag == OutSync)
//	{
//		TIMER_ACC_T = TIME_ACC;
//		TIMER_SUB_T = TIME_SUB;
//	}
//	else
//	{
//		TIMER_ACC_T = TIME_ACC;
//		TIMER_SUB_T = TIME_SUB;
//	}
	if(*scurve[num].speed_now < 0)
	{
		if(*scurve[num].speed_target < *scurve[num].speed_now)
		{
			scurve[num].time = TIMER_ACC_T;
			scurve[num].zero_flag = UnoverZero;
		}
		else if(*scurve[num].speed_target > 0)
		{
			scurve[num].time = TIMER_SUB_T;
			scurve[num].zero_flag = OverZero;
		}
		else
		{
			scurve[num].time = TIMER_SUB_T;
			scurve[num].zero_flag = UnoverZero;
		}
	}
	else if(*scurve[num].speed_now > 0)
	{
		if(*scurve[num].speed_target > *scurve[num].speed_now)
		{
			scurve[num].time = TIMER_ACC_T;
			scurve[num].zero_flag = UnoverZero;
		}
		else if(*scurve[num].speed_target < 0)
		{
			scurve[num].time = TIMER_SUB_T;
			scurve[num].zero_flag = OverZero;
		}
		else
		{
			scurve[num].time = TIMER_SUB_T;
			scurve[num].zero_flag = UnoverZero;
		}
	}
	else if(*scurve[num].speed_now == 0)
	{
		scurve[num].time = TIMER_ACC_T;
		scurve[num].zero_flag = UnoverZero;
	}
}

/**
	* ��������: ͬ������.
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��������ͬ���������ı������������˴���Ҫ�޸�
  */
#define min(a, b)      (a < b ? a : b)
uint8_t synccount = 0;
SYNC_STATE sync_flag = OutSync;
SYNC_STATE fast_flag = Sync;
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
				if(T)
				{
					scurve[SCURVE_1].T_per = T;
					scurve[SCURVE_2].T_per = T;
				}
				sync_flag = OutSync;
				synccount = 0;
			}
		}
	}
}

/**
	* ��������: S������.
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
float test_scurve = 0;

static float update_time_per = 0.001f;
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
				else if(scurve[num].zero_flag == OverZero)//����Խ�����ʱ�ٶ��ȹ��㣬�ٷ������
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
				if((scurve[num].T_per > update_time_per / TIME_STOP_MIN) && *scurve[num].speed_target == 0)
					scurve[num].T_per = update_time_per / TIME_STOP_MIN;
					
				/* ����ͬ�������� */
				sync_curve_handle(num);
				
				scurve[num].ScurveState = ScurveRunning;
				scurve[num].T_now = 0;
				
				test_scurve = system_times;//����
			}
		break;
		case ScurveRunning:
			
			
			#ifdef SCURVE_MODUL_USED
		 	  scurve[num].acc = 6.0f * scurve[num].T_now * (1.0f - scurve[num].T_now) * scurve[num].T_per * scurve[num].polar * scurve[num].speed_range;
			#endif
			#ifdef LCURVE_MODUL_USED
				scurve[num].acc = scurve[num].T_per * scurve[num].polar * scurve[num].speed_range;//SPEEDTOTAL * update_time_per / scurve[num].time;
			#endif
			scurve[num].speed_buff += scurve[num].acc;
			if(((scurve[num].polar == 1) && (scurve[num].speed_buff > scurve[num].speed_target_buff)) || ((scurve[num].polar == -1) && (scurve[num].speed_buff < scurve[num].speed_target_buff)))
				scurve[num].speed_buff = scurve[num].speed_target_buff;
			*scurve[num].speed_now = scurve[num].speed_buff;
			scurve[num].T_now += scurve[num].T_per;
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
			if((*scurve[num].speed_target != scurve[num].speed_target_pre) || RetryDraw == 1)
			{
				scurve[num].ScurveState = ScurveWait;
				if(RetryDraw)
					RetryDraw = 0;
			}
			#endif
		break;
		case ScurveEnd:
			*scurve[num].speed_now = scurve[num].speed_target_buff;
			scurve[num].zero_flag = UnoverZero;
			scurve[num].ScurveState = ScurveWait;
			if(*scurve[num].speed_now == *scurve[num].speed_target)//����Ŀ���ٶȺ����ͬ����־
			{
				sync_flag = OutSync;
				synccount = 0;
			}
			
			
			test_scurve = system_times - test_scurve;//����
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

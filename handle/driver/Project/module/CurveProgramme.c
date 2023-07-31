/**
  ******************************************************************************
  * �ļ�����: CurveProgramme.c 
  * ��    ��: 
  * ��    ��: V0.1
  * ��д����: 2023-06-14
  * ��    ��: �滮�˶����ߣ�˫�֣�
  ******************************************************************************
  * ˵����
  * 
  * 
  * 
  ******************************************************************************
  */
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "CurveProgramme.h"
#include "common.h"
#include "arm_math.h"




/* ˽�����Ͷ��� --------------------------------------------------------------*/
typedef enum{
	speed_push_I = 500,
	speed_push_II = 400,
	speed_push_III = 900,
	speed_run_I = 1600,
	speed_run_II = 2000,
	speed_run_III = 2500,
}SpeedRank;//�ٶȵ�λ����



/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
RUNNING_CURVE_STR RunningCurve;
Running_Curve_State runstate = CtrlCurveWait;
SpeedRank SpeedMax[6] = {speed_push_I, speed_push_II, speed_push_III, speed_run_I, speed_run_II, speed_run_III};
//float wheel_gear[6] = {0.5f,0.8f,0.6f,0.5f,0.3f,0.26f};
//float return_gear[6] = {0.5f,0.5f,0.4f,0.3f,0.3f,0.3f};
//float quicky_gear[6] = {0.1f,0.1f,0.2f,0.5f,0.5f,0.5f};

float wheel_gear[6] = {0.4f,0.5f,0.35f,0.3f,0.25f,0.25f};
float return_gear[6] = {0.5f,0.5f,0.4f,0.3f,0.3f,0.3f};
float quicky_gear[6] = {0.1f,0.1f,0.2f,0.5f,0.5f,0.5f};

/* ��չ���� ------------------------------------------------------------------*/




/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/

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
	/* ģ��ҡ�ˣ�ȡֵ��0 - 360 */
	#if 1
		*RunningCurve.VecValue = 1;
		*RunningCurve.VecAngle = test_key / 360 * PI_2;
		running_programme(SCURVE_1);
		running_programme(SCURVE_2);
	#endif
	
	/* �ٶ����߹滮 */
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
	* ��������: �����������ٶ�.
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��������������ƽ������ʱ���ٶȴ�С
  */
#define UNITMAXBACK     0.3f  //����ʱ�ٶ���������ٶȵ�������
#define UNITMINRUNNING  0.2f  //����ٶȱ���
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
	* ��������: �˶����߹滮.
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
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
			/* ���㵱ǰ�����������ֵ�Ŀ���ٶ� */
			/*�����ο���*/
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
		
			/* Բ�ο��ƣ��������ٶȴ�С���� */
			LRspeed_delimit();
		
			/* ȷ���˶����ߵĵ�����ʽ */
			runstate = CtrlCurveNormalAdj;
			sync_flag = Sync;
			fast_flag = Sync;
			/*//���ٵ���λ��
			if(*RunningCurve.curve[SCURVE_1].speed_now > *RunningCurve.curve[SCURVE_2].speed_now)
			{
				if((RunningCurve.SpeedSetLeft < *RunningCurve.curve[SCURVE_2].speed_now) && (RunningCurve.SpeedSetRight > *RunningCurve.curve[SCURVE_1].speed_now))
				{
					*RunningCurve.curve[SCURVE_1].speed_set = *RunningCurve.curve[SCURVE_2].speed_now;//���ٵ����ٶ�
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
					*RunningCurve.curve[SCURVE_1].speed_set = *RunningCurve.curve[SCURVE_2].speed_now;//���ٵ����ٶ�
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
			/*���������Ƕȣ��ٶȵ����������Ĺ����������й�*/

				if(*RunningCurve.Rank >=3)
				{
						if(RunningCurve.region != RunningCurve.region_last)
						{
							temp_region = RunningCurve.region<<4|RunningCurve.region_last<<0;
							if(temp_region == 0x10)
							{
								if(-*RunningCurve.curve[SCURVE_1].speed_now > *RunningCurve.curve[SCURVE_2].speed_now)
								{
									*RunningCurve.curve[SCURVE_1].speed_set = -*RunningCurve.curve[SCURVE_2].speed_now;//���ٵ����ٶ�
									*RunningCurve.curve[SCURVE_2].speed_set = -*RunningCurve.curve[SCURVE_1].speed_now;
									runstate = CtrlCurveQuickAdj;
									sync_flag = OutSync;
									fast_flag = OutSync;
								
									RunningCurve.confine = 0x10;
								}
								else if(-*RunningCurve.curve[SCURVE_1].speed_now < *RunningCurve.curve[SCURVE_2].speed_now)
								{
									*RunningCurve.curve[SCURVE_1].speed_set = -*RunningCurve.curve[SCURVE_2].speed_now;//���ٵ����ٶ�
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
			/* ����Ƿ���ٵ�����Ŀ��λ�ã��ٶȵ����������Ĺ����������й� */
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

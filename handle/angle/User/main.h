#ifndef __MAIN_H
#define	__MAIN_H


/*---------------------------  ����ͷ�ļ���Ӷ�  -----------------------------*/
#include "math.h"
#include ".\rtc\bsp_rtc.h"
#include ".\i2c\bsp_i2c.h"
#include ".\adc\bsp_adc.h"
#include ".\can\bsp_can.h"
#include ".\gpio\bsp_gpio.h"
#include ".\wwdg\bsp_wwdg.h"
#include ".\Sysclk\bsp_Sysclk.h"
#include ".\systick\bsp_SysTick.h"
#include ".\TiMbase\bsp_TiMbase.h"
#include ".\ATusart\bsp_ATusart.h"
#include ".\internal_flash\bsp_internal_flash.h"
#include ".\QMI8658C\qmi8658c.h"
#include "app_loopqueue.h"
#include "app_protocol.h"
#include "app_angle.h"
#include "app_led.h"
#include "app_kalman.h"

/*----------------------------------- END ------------------------------------*/


/*-----------------------------  ��ز�������  -------------------------------*/
#define PI    3.1415926
#define TEMP_Limit  600     /* �����¶ȱ��� */
#define IgnoreTime  600     /* ֹͣ��Ӧʱ��(s) */

/*---------------------------  ��������������  -----------------------------*/
/* ���ϱ�־λ */
typedef enum
{
	WDGR  = (1<<0),           /* ���Ź���λ */
	ACTX  = (1<<1),           /* �ⲿ�����ź� */
	MPU6  = (1<<2),           /* ������ʧ�� */
	Pitch = (1<<3),           /* �������쳣 */
	ROLL  = (1<<4),           /* ����ǹ��� */
	TEMP  = (1<<5),           /* �¶ȹ��� */
} ERR_flag;

/* ������״̬���� */
typedef struct
{
	uint8_t   MCU_State;      /* MCU����״ָ̬ʾ   1 1 1 1    1 1 1 1
                                                 0x02    MCU������    0x20
                                    MCU׼������  0x01    MCU������    0x10 */
	uint16_t  LowPWR_Delay;   /* �͹�����ʱ */
	uint16_t  WWDG_num;       /* ���Ź����� */
} Drive_CtrlDef;

///////////////////////////////////////////////////////////////////////////////////////////////
/* ���״̬���� */
typedef struct
{
	uint8_t   Function;       /* ������ܱ�־ */
	uint8_t   Update;         /* ���״̬���� */
	uint8_t   Error;          /* �澯״̬ */
	int16_t   Pitch;          /* ������ */
	int16_t   Roll;           /* ��ת�� */
	int16_t   Yaw;            /* ƫ���� */
	int16_t   pTemp;          /* �����¶� */
	int16_t   Vbus;           /* ��ص�ѹֵ */
	uint8_t   Ignore;         /* ����״̬ */
} Moto_CtrlDef;

/* ʱ������־ */
typedef struct
{
	uint8_t   CNT;            /* ��ʱ��־ */
	uint8_t   ms10;           /* 10ms ��־ */
	uint8_t   ms20;           /* 20ms ��־ */
	uint8_t   ms50;           /* 50ms ��־ */
	uint8_t   ms100;          /* 100ms ��־ */
	uint8_t   ms1000;         /* 1000ms ��־ */
	uint8_t   Err_MPU;        /* Err_MPU ���� */
	uint8_t   Err_ADCV;       /* Err_ADCV ���� */
	uint16_t  Sleep;          /* ˯�ߵȴ���ʱ */
} Time_CtrlDef;


extern Drive_CtrlDef      Drive;
extern Moto_CtrlDef       MotoA;
extern Moto_CtrlDef       MotoU;
extern Time_CtrlDef       Time;
extern uint8_t Access[20];
extern int16_t Angle_ref,Angle_last;




/*----------------------------------- END ------------------------------------*/

#endif /* __MAIN_H */

#ifndef __MAIN_H
#define	__MAIN_H


/*---------------------------  外设头文件添加段  -----------------------------*/
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


/*-----------------------------  相关参数定义  -------------------------------*/
#define PI    3.1415926
#define TEMP_Limit  600     /* 环境温度报警 */
#define IgnoreTime  600     /* 停止响应时间(s) */

/*---------------------------  驱动器动作配置  -----------------------------*/
/* 故障标志位 */
typedef enum
{
	WDGR  = (1<<0),           /* 看门狗复位 */
	ACTX  = (1<<1),           /* 外部控制信号 */
	MPU6  = (1<<2),           /* 传感器失联 */
	Pitch = (1<<3),           /* 俯仰角异常 */
	ROLL  = (1<<4),           /* 侧倾角过大 */
	TEMP  = (1<<5),           /* 温度过高 */
} ERR_flag;

/* 驱动器状态控制 */
typedef struct
{
	uint8_t   MCU_State;      /* MCU运行状态指示   1 1 1 1    1 1 1 1
                                                 0x02    MCU工作中    0x20
                                    MCU准备休眠  0x01    MCU已休眠    0x10 */
	uint16_t  LowPWR_Delay;   /* 低功耗延时 */
	uint16_t  WWDG_num;       /* 看门狗计数 */
} Drive_CtrlDef;

///////////////////////////////////////////////////////////////////////////////////////////////
/* 电机状态控制 */
typedef struct
{
	uint8_t   Function;       /* 电机功能标志 */
	uint8_t   Update;         /* 电机状态更新 */
	uint8_t   Error;          /* 告警状态 */
	int16_t   Pitch;          /* 俯仰角 */
	int16_t   Roll;           /* 滚转角 */
	int16_t   Yaw;            /* 偏航角 */
	int16_t   pTemp;          /* 环境温度 */
	int16_t   Vbus;           /* 电池电压值 */
	uint8_t   Ignore;         /* 无视状态 */
} Moto_CtrlDef;

/* 时间量标志 */
typedef struct
{
	uint8_t   CNT;            /* 计时标志 */
	uint8_t   ms10;           /* 10ms 标志 */
	uint8_t   ms20;           /* 20ms 标志 */
	uint8_t   ms50;           /* 50ms 标志 */
	uint8_t   ms100;          /* 100ms 标志 */
	uint8_t   ms1000;         /* 1000ms 标志 */
	uint8_t   Err_MPU;        /* Err_MPU 计数 */
	uint8_t   Err_ADCV;       /* Err_ADCV 计数 */
	uint16_t  Sleep;          /* 睡眠等待计时 */
} Time_CtrlDef;


extern Drive_CtrlDef      Drive;
extern Moto_CtrlDef       MotoA;
extern Moto_CtrlDef       MotoU;
extern Time_CtrlDef       Time;
extern uint8_t Access[20];
extern int16_t Angle_ref,Angle_last;




/*----------------------------------- END ------------------------------------*/

#endif /* __MAIN_H */

#ifndef __RTC_H
#define	__RTC_H

#include "stm32f10x.h"


//#define	RTC_Alarm    /* 是否启用RTC闹钟 */

/* 使用LSE外部时钟 或 LSI内部时钟 */
//#define RTC_CLOCK_SOURCE_LSE      
#define RTC_CLOCK_SOURCE_LSI

/* RCT初始时间值 */
#define Initial_Time         1000

/* RCT闹钟定时值 */
#define Alarm_Time           (Initial_Time + 86400)/* 24h */
//#define Alarm_Time           (Initial_Time + 60)/* 60s */
#define RTC_BKP_DRX          BKP_DR1

/* 写入到备份寄存器的数据宏定义 */
#define RTC_BKP_DATA         0xA5A5

/* 如果定义了下面这个宏的话,PC13就会输出频率为RTC Clock/64的时钟 */   
//#define RTCClockOutput_Enable  /* RTC Clock/64 is output on tamper pin(PC.13) */


void RTC_CheckAndConfig(uint32_t RTCtime);/* 检查并配置RTC */

#endif /* __XXX_H */

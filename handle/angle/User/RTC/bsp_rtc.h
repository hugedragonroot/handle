#ifndef __RTC_H
#define	__RTC_H

#include "stm32f10x.h"


//#define	RTC_Alarm    /* �Ƿ�����RTC���� */

/* ʹ��LSE�ⲿʱ�� �� LSI�ڲ�ʱ�� */
//#define RTC_CLOCK_SOURCE_LSE      
#define RTC_CLOCK_SOURCE_LSI

/* RCT��ʼʱ��ֵ */
#define Initial_Time         1000

/* RCT���Ӷ�ʱֵ */
#define Alarm_Time           (Initial_Time + 86400)/* 24h */
//#define Alarm_Time           (Initial_Time + 60)/* 60s */
#define RTC_BKP_DRX          BKP_DR1

/* д�뵽���ݼĴ��������ݺ궨�� */
#define RTC_BKP_DATA         0xA5A5

/* ������������������Ļ�,PC13�ͻ����Ƶ��ΪRTC Clock/64��ʱ�� */   
//#define RTCClockOutput_Enable  /* RTC Clock/64 is output on tamper pin(PC.13) */


void RTC_CheckAndConfig(uint32_t RTCtime);/* ��鲢����RTC */

#endif /* __XXX_H */

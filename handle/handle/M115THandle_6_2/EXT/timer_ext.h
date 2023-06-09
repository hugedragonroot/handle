#ifndef __TIMER_EXT_H
#define __TIMER_EXT_H
#include "stdint.h"

typedef struct  
{
	uint16_t cnt;
	uint8_t time_1ms;
	uint8_t time_10ms;
	uint8_t time_100ms;
	uint8_t time_1000ms;
} Time_Flag_STR;

extern Time_Flag_STR time_flag;


void MX_TIM2_SCAN_Init(void);
void timer_myinit(void);


#endif

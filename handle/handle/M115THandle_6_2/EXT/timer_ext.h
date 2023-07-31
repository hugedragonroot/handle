#ifndef __TIMER_EXT_H
#define __TIMER_EXT_H
#include "stdint.h"

typedef struct  
{
	uint32_t cnt;
	// uint16_t cnt;
	uint8_t time_1ms:1;
	uint8_t time_2ms:1;
	uint8_t time_5ms:1;
	uint8_t time_10ms:1;
	uint8_t time_20ms:1;
	uint8_t time_100ms:1;
	uint8_t time_200ms:1;
	uint8_t time_1000ms:1;
} Time_Flag_STR;

extern Time_Flag_STR time_flag;


void MX_TIM2_SCAN_Init(void);
void timer_myinit(void);


#endif

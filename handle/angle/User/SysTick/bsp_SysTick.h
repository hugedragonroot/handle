#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"

//#define SYSFrequency       SystemCoreClock
#define SYSFrequency       72000000
#define Delay_ms(x) Delay_10us(100*x)	 //��λms

void SysTick_Init(void);
void Delay_10us(__IO u32 nTime);
void SysTick_Delay_10Us( __IO uint32_t us);
void SysTick_Delay_Ms( __IO uint32_t ms);


#endif /* __SYSTICK_H */

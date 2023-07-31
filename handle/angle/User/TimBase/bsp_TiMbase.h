#ifndef __BSP_TIMEBASE_H
#define __BSP_TIMEBASE_H


#include "stm32f10x.h"



/*********************  基本定时器TIM参数定义,只限TIM6,7  *********************/
//#define BASIC_TIM6 /* 如果使用TIM7,注释掉这个宏即可 */

#ifdef  BASIC_TIM6 /* 使用基本定时器TIM6 */
#define            BASIC_TIM                   TIM6
#define            BASIC_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            BASIC_TIM_CLK               RCC_APB1Periph_TIM6
#define            BASIC_TIM_Period            1000-1                 // 计数周期 1000
#define            BASIC_TIM_Prescaler         719                   // 72M 720分频
#define            BASIC_TIM_IRQ               TIM6_IRQn
#define            BASIC_TIM_IRQHandler        TIM6_IRQHandler

#else              /* 使用通用定时器TIM4 */
#define            BASIC_TIM                   TIM4
#define            BASIC_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            BASIC_TIM_CLK               RCC_APB1Periph_TIM4
#define            BASIC_TIM_Period            (1000-1)
#define            BASIC_TIM_Prescaler         (720-1)       /* 72M */
#define            BASIC_TIM_IRQ               TIM4_IRQn
#define            BASIC_TIM_IRQHandler        TIM4_IRQHandler

#endif

/*********************************  函数声明  *********************************/
void BASIC_TIM_Init(void);          /* 基本定时器初始化 */
void TIMdelay_Decrement(void);      /* 获取TIM中断节拍 */
void TIMdelay_10Ms(uint16_t ms);    /* TIM定时器延时 */

#endif	/* __BSP_TIMEBASE_H */

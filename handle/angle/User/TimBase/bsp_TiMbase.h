#ifndef __BSP_TIMEBASE_H
#define __BSP_TIMEBASE_H


#include "stm32f10x.h"



/*********************  ������ʱ��TIM��������,ֻ��TIM6,7  *********************/
//#define BASIC_TIM6 /* ���ʹ��TIM7,ע�͵�����꼴�� */

#ifdef  BASIC_TIM6 /* ʹ�û�����ʱ��TIM6 */
#define            BASIC_TIM                   TIM6
#define            BASIC_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            BASIC_TIM_CLK               RCC_APB1Periph_TIM6
#define            BASIC_TIM_Period            1000-1                 // �������� 1000
#define            BASIC_TIM_Prescaler         719                   // 72M 720��Ƶ
#define            BASIC_TIM_IRQ               TIM6_IRQn
#define            BASIC_TIM_IRQHandler        TIM6_IRQHandler

#else              /* ʹ��ͨ�ö�ʱ��TIM4 */
#define            BASIC_TIM                   TIM4
#define            BASIC_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            BASIC_TIM_CLK               RCC_APB1Periph_TIM4
#define            BASIC_TIM_Period            (1000-1)
#define            BASIC_TIM_Prescaler         (720-1)       /* 72M */
#define            BASIC_TIM_IRQ               TIM4_IRQn
#define            BASIC_TIM_IRQHandler        TIM4_IRQHandler

#endif

/*********************************  ��������  *********************************/
void BASIC_TIM_Init(void);          /* ������ʱ����ʼ�� */
void TIMdelay_Decrement(void);      /* ��ȡTIM�жϽ��� */
void TIMdelay_10Ms(uint16_t ms);    /* TIM��ʱ����ʱ */

#endif	/* __BSP_TIMEBASE_H */

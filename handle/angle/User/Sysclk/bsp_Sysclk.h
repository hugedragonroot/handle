#ifndef __SYSCLK_H
#define	__SYSCLK_H


#include "stm32f10x.h"

void HSI_SetSysClock(uint32_t pllmul);    /* ʹ��HSI��ʱ��Դ */
void SYSCLKConfig_STOPMode(void);             /* ͣ�����Ѻ�����ϵͳʱ�� */

#endif /* __SYSCLK_H */

#ifndef __SYSCLK_H
#define	__SYSCLK_H


#include "stm32f10x.h"

void HSI_SetSysClock(uint32_t pllmul);    /* 使用HSI做时钟源 */
void SYSCLKConfig_STOPMode(void);             /* 停机唤醒后配置系统时钟 */

#endif /* __SYSCLK_H */

#ifndef _DELAY_H
#define _DELAY_H

#include "main.h"
//0,��֧��os
//1,֧��os
#define SYSTEM_SUPPORT_OS		1		//����ϵͳ�ļ����Ƿ�֧��OS
																	    
	 
void delay_init(void);
void delay_ms(u32 nms);
void delay_us(u32 nus);
void delay_xms(u32 nms);
#endif


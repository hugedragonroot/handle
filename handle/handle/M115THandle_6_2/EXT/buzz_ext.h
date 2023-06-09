#ifndef __BUZZ_EXT_H
#define __BUZZ_EXT_H
#include "main.h"

typedef struct 
{
	uint8_t mode;
	uint8_t per;
	uint8_t cnt;
}beep_str;

void buzz_init(void);
#endif

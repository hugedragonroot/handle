#ifndef __POWER_CONTROL
#define __POWER_CONTROL
#include "main.h"

#define POWER_CONTROL_PORT GPIOB 
#define POWER_CONTROL_PIN GPIO_PIN_3

void powerContorIOInit(void);
void powerON(void);
void powerOFF(void);

#endif

#ifndef __BUZZ_DRV_H
#define __BUZZ_DRV_H

#include "gd32f30x.h"

#define BUZZ_GPIO_PORT 	GPIOC 
#define BUZZ_PIN			GPIO_PIN_0	


#define BUZZ_OFF()  		gpio_bit_write(BUZZ_GPIO_PORT,BUZZ_PIN,RESET)
#define BUZZ_ON()  			gpio_bit_write(BUZZ_GPIO_PORT,BUZZ_PIN,SET)

void buzz_initIO(void);

#endif


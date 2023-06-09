#ifndef __JOYSTICK_DRV_H
#define __JOYSTICK_DRV_H

#include "gd32f30x.h"

#define JOY_X_GPIO_PORT 	GPIOA 
#define JOY_X_PIN			GPIO_PIN_0

#define JOY_Y_GPIO_PORT 	GPIOA 
#define JOY_Y_PIN			GPIO_PIN_1

#define JOY_PWR_GPIO_PORT 	GPIOA 
#define JOY_PWR_PIN			GPIO_PIN_4

#define JOY_PWR_OFF() 		gpio_bit_write(JOY_PWR_GPIO_PORT,JOY_PWR_PIN,SET)
#define JOY_PWR_ON() 			gpio_bit_write(JOY_PWR_GPIO_PORT,JOY_PWR_PIN,RESET)

void joystick_initIO(void);

#endif


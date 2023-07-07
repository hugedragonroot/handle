#ifndef __EV1527_DRV_H
#define __EV1527_DRV_H

#include "main.h"

#if USING_LED_POINT_DISPLAY && USING_6KEY
// #if 1

#define EV1527_GPIO_PORT   GPIOB
#define EV1527_GPIO_PIN    GPIO_PIN_7
#define EV1527()		   gpio_input_bit_get(EV1527_GPIO_PORT, EV1527_GPIO_PIN)

#define MODE_GPIO_PORT		GPIOB
#define MODE_GPIO_PIN		GPIO_PIN_6
#define MODE()				gpio_input_bit_get(MODE_GPIO_PORT, MODE_GPIO_PIN)
																						
#define LED_GPIO_PORT		GPIOB
#define LED_GPIO_PIN		GPIO_PIN_5

#define WARMING_GPIO_PORT	GPIOB
#define WARMING_GPIO_PIN	GPIO_PIN_4
#else

#define EV1527_GPIO_PORT	GPIOB
#define EV1527_GPIO_PIN 	GPIO_PIN_10
#define EV1527()			gpio_input_bit_get(EV1527_GPIO_PORT, EV1527_GPIO_PIN)

#define MODE_GPIO_PORT		GPIOC
#define MODE_GPIO_PIN		GPIO_PIN_14
#define MODE()				gpio_input_bit_get(MODE_GPIO_PORT, MODE_GPIO_PIN)
																						
#define LED_GPIO_PORT		GPIOB
#define LED_GPIO_PIN		GPIO_PIN_9

#define WARMING_GPIO_PORT	GPIOC
#define WARMING_GPIO_PIN	GPIO_PIN_13

#endif
#define LED(x)	do{x?\
				gpio_bit_set(LED_GPIO_PORT,LED_GPIO_PIN):\
				gpio_bit_reset(LED_GPIO_PORT,LED_GPIO_PIN);\
				}while(0)	

#define WARMING(x)	do{x?\
				gpio_bit_set(WARMING_GPIO_PORT,WARMING_GPIO_PIN):\
				gpio_bit_reset(WARMING_GPIO_PORT,WARMING_GPIO_PIN);\
				}while(0)


void ev1527_initIO(void);
#endif

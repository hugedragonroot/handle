#ifndef __MUSIC_DRV_H
#define __MUSIC_DRV_H

#define MUSIC_BUSY_GPIO_PORT 					GPIOC 
#define MUSIC_BUSY_PIN								GPIO_PIN_15

#define READ_MUSIC_BUSY()				gpio_input_bit_get(MUSIC_BUSY_GPIO_PORT, MUSIC_BUSY_PIN)


void music_initIO(void);
#endif


#ifndef __GPS_DRV_H
#define __GPS_DRV_H
#include "main.h"


#if !HARD_TYPE_NEW

#define GPS_PWR_GPIO_PORT		GPIOC 
#define GPS_PWR_PIN					GPIO_PIN_9
#define GPS_RST_GPIO_PORT		GPIOC 
#define GPS_RST_PIN					GPIO_PIN_7


#define GPS_PWR_ON() 		gpio_bit_write(GPS_PWR_GPIO_PORT,GPS_PWR_PIN,SET)
#define GPS_PWR_OFF() 		gpio_bit_write(GPS_PWR_GPIO_PORT,GPS_PWR_PIN,RESET)

#define GPS_RST_ON() 		gpio_bit_write(GPS_RST_GPIO_PORT,GPS_RST_PIN,SET)
#define GPS_RST_OFF() 		gpio_bit_write(GPS_RST_GPIO_PORT,GPS_RST_PIN,RESET)
#endif
void gps_initIO(void);
#endif

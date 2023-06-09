#ifndef __DRV_H
#define __DRV_H

#include "gd32f30x.h"
#include "key_drv.h"
#include "buzz_drv.h"
#include "joystick_drv.h"
#include "lcd_drv.h"
#include "uart_drv.h"
#include "bt_drv.h"
#include "gps_drv.h"
#include "rf_315m_drv.h"
#include "music_drv.h"
#include "led_drv.h"
#include "power_control.h"


void GPIO_InitIO(uint32_t  gpio_periph,uint32_t GPIO_Pin,uint32_t GPIO_Mode,uint32_t speed);
void UART_Init(rcu_periph_enum periph,uint32_t UARTx,uint32_t baud);

void drv_init(void);
#endif

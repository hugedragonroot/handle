#ifndef __LED_EXT_H
#define __LED_EXT_H
#include "led_drv.h"

#define LED_SCK_1() gpio_bit_set(LED_SCK_GPIO_PORT,LED_SCK_GPIO_PIN)
#define LED_SCK_0() gpio_bit_reset(LED_SCK_GPIO_PORT,LED_SCK_GPIO_PIN)

#define LED_RCK_1() gpio_bit_set(LED_RCK_GPIO_PORT,LED_RCK_GPIO_PIN)
#define LED_RCK_0() gpio_bit_reset(LED_RCK_GPIO_PORT,LED_RCK_GPIO_PIN)

#define LED_DATA_1() gpio_bit_set(LED_DATA_GPIO_PORT,LED_DATA_GPIO_PIN)
#define LED_DATA_0() gpio_bit_reset(LED_DATA_GPIO_PORT,LED_DATA_GPIO_PIN)

#define LED_POWER_1() gpio_bit_set(LED_POWER_GPIO_PORT,LED_POWER_GPIO_PIN)
#define LED_POWER_0() gpio_bit_reset(LED_POWER_GPIO_PORT,LED_POWER_GPIO_PIN)

// #define LED1_1()   gpio_bit_set(LED1_GPIO_PORT,LED1_GPIO_PIN);

#define LED1_BLINK()\
if(gpio_output_bit_get(LED1_GPIO_PORT,LED1_GPIO_PIN)){\
    gpio_bit_reset(LED1_GPIO_PORT,LED1_GPIO_PIN);\
}else{\
    gpio_bit_set(LED1_GPIO_PORT,LED1_GPIO_PIN);\
}\


void led_ext_init(void);

void ledShow(void);

void ledSpiDmaRefreshRow(void);
// void ledSpiDmaRefreshRow(uint8_t h);

// void led8x8Test(void);

#endif



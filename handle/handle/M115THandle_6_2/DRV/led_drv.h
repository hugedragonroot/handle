#ifndef __LED_DRV_H
#define __LED_DRV_H
#include "main.h"


#define USING_LED_DMA_SEND 1

#if USING_LED_DMA_SEND

#define LED_DATA_GPIO_PORT      GPIOB
#define LED_DATA_GPIO_PIN       GPIO_PIN_15//spi_mosi

#define LED_RCK_GPIO_PORT       GPIOB
#define LED_RCK_GPIO_PIN        GPIO_PIN_12//spi_nss

#define LED_SCK_GPIO_PORT       GPIOB
#define LED_SCK_GPIO_PIN        GPIO_PIN_13//spi_sck

#else

#define LED_DATA_GPIO_PORT      GPIOB
// #define LED_DATA_GPIO_PIN       GPIO_PIN_12
#define LED_DATA_GPIO_PIN       GPIO_PIN_15

#define LED_RCK_GPIO_PORT       GPIOB
// #define LED_RCK_GPIO_PIN        GPIO_PIN_13
#define LED_RCK_GPIO_PIN        GPIO_PIN_12

#define LED_SCK_GPIO_PORT       GPIOB
// #define LED_SCK_GPIO_PIN        GPIO_PIN_14
#define LED_SCK_GPIO_PIN        GPIO_PIN_13

//8x8
// #define LED_DATA_GPIO_PORT      GPIOA
// #define LED_DATA_GPIO_PIN       GPIO_PIN_9

// #define LED_RCK_GPIO_PORT       GPIOB
// #define LED_RCK_GPIO_PIN        GPIO_PIN_3

// #define LED_SCK_GPIO_PORT       GPIOA
// #define LED_SCK_GPIO_PIN        GPIO_PIN_10
#endif

void led_initIO(void);
#endif


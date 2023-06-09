#include "led_driver.h"
#include <stdint.h>

#include "main.h"
#define is_led(v)       (v & led_all)
#define LED_ALL_PORT    (__HAL_RCC_GPIOD_CLK_ENABLE()|__HAL_RCC_GPIOG_CLK_ENABLE())

typedef struct
{
    GPIO_TypeDef * port; //port address
    uint16_t  pin;
} led_hw;

led_hw * led_get_hw(led_hw_t led)
{
    static led_hw ret;
    switch(led)
    {
        case led_pow_red:
            ret.port = GPIOG;
            ret.pin = GPIO_PIN_10;//C5
            break;
        case led_pow_green:
            ret.port = GPIOG;
            ret.pin = GPIO_PIN_11;
            break;
//        case led_gps_red:
//            ret.port = GPIOA;
//            ret.pin = GPIO_PIN_0;
//            break;
//        case led_gps_green:
//            ret.port = GPIOC;
//            ret.pin = GPIO_PIN_3;//C3
//            break;
        case led_rf_red:
            ret.port = GPIOD;
            ret.pin = GPIO_PIN_5;
            break;
//        case led_rf_green:
//            ret.port = GPIOC;
//            ret.pin = GPIO_PIN_1;
//            break;
        case led_net_red:
            ret.port = GPIOD;
            ret.pin = GPIO_PIN_3;
            break;
        case led_net_green:
            ret.port = GPIOD;
            ret.pin = GPIO_PIN_4;
            break;
        default:
            break;
    }
    return &ret;
}

void led_hw_init(void)
{
    led_hw * led;
    uint16_t i;
	  GPIO_InitTypeDef GPIO_InitStruct = {0};
    /* LED GPIO Periph clock enable */
    //RCC_AHBPeriphClockCmd(LED_ALL_PORT, ENABLE);
		__HAL_RCC_GPIOD_CLK_ENABLE();
		__HAL_RCC_GPIOG_CLK_ENABLE();
	
    for(i = 0; i < 16; i++)
    {
        if(!is_led((1 << i))) continue;
        led = led_get_hw((led_hw_t)(1 << i));
			  GPIO_InitStruct.Pin = led->pin;
				GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
				GPIO_InitStruct.Pull = GPIO_NOPULL;
				GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
				HAL_GPIO_Init(led->port, &GPIO_InitStruct);
    }
    led_off(led_all);
}

void led_on(led_hw_t leds)
{
    led_hw * led;
    uint16_t i;
    for(i = 0; i < 16; i++)
    {
        if(!is_led(leds & (1 << i))) continue;
        led = led_get_hw((led_hw_t)(1 << i));
        HAL_GPIO_WritePin(led->port, led->pin,0);
    }
}

void led_off(led_hw_t leds)
{
    led_hw * led;
    uint16_t  i;
    for(i = 0; i < 16; i ++)
    {
        if(!is_led(leds & (1 << i))) continue;
        led = led_get_hw((led_hw_t)(1 << i));
        HAL_GPIO_WritePin(led->port, led->pin,1);
    }
}

void led_toggle(led_hw_t leds)
{
    led_hw * led;
    uint16_t  i;
		uint8_t led_statue = 0; 
    for(i = 0; i < 16; i ++)
    {
        if(!is_led(leds & (1 << i))) continue;
        led = led_get_hw((led_hw_t)(1 << i));
				led_statue = HAL_GPIO_ReadPin(led->port,led->pin);
				if(led_statue == 0) HAL_GPIO_WritePin(led->port, led->pin,1);
        if(led_statue == 1) HAL_GPIO_WritePin(led->port, led->pin,0);
    }
}

#ifndef __RF_315M_DRV_H
#define __RF_315M_DRV_H

#define RF_D0_GPIO_PORT 					GPIOB 
#define RF_D0_PIN								GPIO_PIN_4

#define RF_D1_GPIO_PORT 					GPIOB 
#define RF_D1_PIN								GPIO_PIN_5

#define RF_D2_GPIO_PORT 					GPIOB 
#define RF_D2_PIN								GPIO_PIN_6

#define RF_D3_GPIO_PORT 					GPIOB 
#define RF_D3_PIN								GPIO_PIN_7

#define READ_RF_D0()				gpio_input_bit_get(RF_D0_GPIO_PORT, RF_D0_PIN)
#define READ_RF_D1()				gpio_input_bit_get(RF_D1_GPIO_PORT, RF_D1_PIN)
#define READ_RF_D2()				gpio_input_bit_get(RF_D2_GPIO_PORT, RF_D2_PIN)
#define READ_RF_D3()				gpio_input_bit_get(RF_D3_GPIO_PORT, RF_D3_PIN)

void rf_315m_initIO(void);
#endif


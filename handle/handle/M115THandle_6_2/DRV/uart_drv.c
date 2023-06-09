#include "uart_drv.h"
#include "drv.h"


void uart_initIO(void)
{	
	// gpio_pin_remap_config(GPIO_USART0_REMAP,ENABLE);
	GPIO_InitIO(GPIOA,GPIO_PIN_9,GPIO_MODE_AF_PP,GPIO_OSPEED_10MHZ);				//WIRE
	GPIO_InitIO(GPIOA,GPIO_PIN_10,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_10MHZ);	//WIRE		
	// GPIO_InitIO(GPIOB,GPIO_PIN_6,GPIO_MODE_AF_PP,GPIO_OSPEED_10MHZ);				//WIRE
	// GPIO_InitIO(GPIOB,GPIO_PIN_7,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_10MHZ);	//WIRE		

	GPIO_InitIO(GPIOB,GPIO_PIN_10,GPIO_MODE_AF_PP,GPIO_OSPEED_10MHZ);				//DEBUG
	GPIO_InitIO(GPIOB,GPIO_PIN_11,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_10MHZ);	//DEBUG

}
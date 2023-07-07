#include "power_control.h"
#include "ev1527_drv.h"
//控制器开关初始化
void powerContorIOInit(void){

	GPIO_InitIO(POWER_CONTROL_PORT,POWER_CONTROL_PIN,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ);

	gpio_bit_reset(POWER_CONTROL_PORT,POWER_CONTROL_PIN);
}

//控制器开
void powerON(void){
	#if 0
	do{
		GPIO_InitIO(POWER_CONTROL_PORT,POWER_CONTROL_PIN,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ);
		delay_ms(1);
		gpio_bit_set(POWER_CONTROL_PORT,POWER_CONTROL_PIN);
		delay_ms(1);
		GPIO_InitIO(POWER_CONTROL_PORT,POWER_CONTROL_PIN,GPIO_MODE_IPU,GPIO_OSPEED_50MHZ);
		delay_ms(1);
		WARMING(1);
	} while (RESET == gpio_input_bit_get(POWER_CONTROL_PORT,POWER_CONTROL_PIN));
	#else
		gpio_bit_set(POWER_CONTROL_PORT,POWER_CONTROL_PIN);
		// WARMING(1);
	#endif
	
}

//控制器关
void powerOFF(void){
	#if 0
	do{
		GPIO_InitIO(POWER_CONTROL_PORT,POWER_CONTROL_PIN,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ);
		delay_ms(1);
		gpio_bit_reset(POWER_CONTROL_PORT,POWER_CONTROL_PIN);
		delay_ms(1);
		GPIO_InitIO(POWER_CONTROL_PORT,POWER_CONTROL_PIN,GPIO_MODE_IPU,GPIO_OSPEED_50MHZ);
		delay_ms(1);
		WARMING(0);
	} while (SET == gpio_input_bit_get(POWER_CONTROL_PORT,POWER_CONTROL_PIN));
	#else
		gpio_bit_reset(POWER_CONTROL_PORT,POWER_CONTROL_PIN);
		// WARMING(0);
	#endif
}

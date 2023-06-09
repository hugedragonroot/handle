#include "power_control.h"
//控制器开关初始化
void powerContorIOInit(void){

	GPIO_InitIO(POWER_CONTROL_PORT,POWER_CONTROL_PIN,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ);

	gpio_bit_reset(POWER_CONTROL_PORT,POWER_CONTROL_PIN);
}

//控制器开
void powerON(void){
	gpio_bit_set(POWER_CONTROL_PORT,POWER_CONTROL_PIN);
}

//控制器关
void powerOFF(void){
	gpio_bit_reset(POWER_CONTROL_PORT,POWER_CONTROL_PIN);
}

#include "key_drv.h"
#include "drv.h"
void key_initIO(void)
{
	
#if USING_TPAD


    rcu_periph_clock_enable(RCU_AF);
	gpio_pin_remap_config(GPIO_TIMER2_FULL_REMAP,ENABLE);

	GPIO_InitIO(KEY_SPEED_ADD_PORT,KEY_SPEED_ADD_PIN,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ);
	GPIO_InitIO(KEY_SPEED_SUB_PORT,KEY_SPEED_SUB_PIN,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ);

	GPIO_InitIO(KEY_POWER_PORT,KEY_POWER_PIN,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ);

	GPIO_InitIO(KEY_PUSH_ROB_UP_PORT,KEY_PUSH_ROB_UP_PIN,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ);
	GPIO_InitIO(KEY_PUSH_ROB_DOWN_PORT,KEY_PUSH_ROB_DOWN_PIN,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ);

	GPIO_InitIO(KEY_HORN_PORT,KEY_HORN_PIN,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ);

#else
	#if USING_6KEY

	GPIO_InitIO(KEY_SPEED_ADD_PORT,KEY_SPEED_ADD_PIN,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ);
	GPIO_InitIO(KEY_SPEED_SUB_PORT,KEY_SPEED_SUB_PIN,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ);
	GPIO_InitIO(KEY_POWER_PORT,KEY_POWER_PIN,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ);
	GPIO_InitIO(KEY_PUSH_ROB_ADD_PORT,KEY_PUSH_ROB_ADD_PIN,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ);
	GPIO_InitIO(KEY_PUSH_ROB_SUB_PORT,KEY_PUSH_ROB_SUB_PIN,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ);
	GPIO_InitIO(KEY_HORN_PORT,KEY_HORN_PIN,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ);

	#else
	GPIO_InitIO(KEY_COL1_GPIO_PORT,KEY_COL1_PIN,GPIO_MODE_IPU,GPIO_OSPEED_50MHZ);
	GPIO_InitIO(KEY_COL2_GPIO_PORT,KEY_COL2_PIN,GPIO_MODE_IPU,GPIO_OSPEED_50MHZ);
	GPIO_InitIO(KEY_COL3_GPIO_PORT,KEY_COL3_PIN,GPIO_MODE_IPU,GPIO_OSPEED_50MHZ);
	
	GPIO_InitIO(KEY_ROW1_GPIO_PORT,KEY_ROW1_PIN,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ);	
	GPIO_InitIO(KEY_ROW2_GPIO_PORT,KEY_ROW2_PIN,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ);	
	GPIO_InitIO(KEY_ROW3_GPIO_PORT,KEY_ROW3_PIN,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ);

	gpio_bit_set(KEY_ROW1_GPIO_PORT,KEY_ROW1_PIN);
	gpio_bit_set(KEY_ROW2_GPIO_PORT,KEY_ROW2_PIN);
	gpio_bit_set(KEY_ROW3_GPIO_PORT,KEY_ROW3_PIN);
	#endif
#endif


		
			
}


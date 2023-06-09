#include "ext.h"

// API_Config_t  API_Config;
 void ext_init(void)
{	
	mem_init();	

	// bt_init();
	// wire_Init();
	CAN_Init();
	// BTlink_Init();
	// buzz_init();
	joystick_init();
	#if USING_TPAD
	TPAD_Init();
	#else
	key_init();
	#endif

	uart_init();
	// gps_init();
	
	//注册，心跳变量初始化
	// API_Config.Type = NODE_SELF;
	// api_init(&API_Config);

	can_myinit();

	rf_315m_init();
	// music_init();

#if USING_LED_POINT_DISPLAY
	#if USING_LED_DMA_SEND
		led_ext_init();
	#endif
#else
	lcd_init();
#endif

	fwdgt_init();

	timer_myinit();
	
}

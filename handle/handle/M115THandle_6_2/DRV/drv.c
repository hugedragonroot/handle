#include "drv.h"


void GPIO_InitIO(uint32_t  gpio_periph,uint32_t GPIO_Pin,uint32_t GPIO_Mode,uint32_t speed)
{
	gpio_init(gpio_periph,  GPIO_Mode,  speed,  GPIO_Pin);	
		
}


void UART_Init(rcu_periph_enum periph,uint32_t UARTx,uint32_t baud)
{
	rcu_periph_clock_enable(periph);
	
		/* USART configure */
	usart_deinit(UARTx);
	
	usart_baudrate_set(UARTx, baud);
	usart_parity_config(UARTx,USART_PM_NONE);
	usart_word_length_set(UARTx,USART_WL_8BIT);
	usart_stop_bit_set(UARTx,USART_STB_1BIT);
	usart_receive_config(UARTx, USART_RECEIVE_ENABLE);
	usart_transmit_config(UARTx, USART_TRANSMIT_ENABLE);
	

}

void drv_init(void)
{
  rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_GPIOB);
	rcu_periph_clock_enable(RCU_GPIOC);
  rcu_periph_clock_enable(RCU_GPIOD);
	rcu_periph_clock_enable(RCU_AF);	
	rcu_periph_clock_enable(RCU_DMA0);	
	rcu_periph_clock_enable(RCU_DMA1);	

	delay_init();
	nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0);
  gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP,ENABLE);

  dbg_trace_pin_disable();
	
	powerContorIOInit();
	key_initIO();
	buzz_initIO();
	joystick_initIO();
	uart_initIO();	

#if USING_LED_POINT_DISPLAY
	led_initIO();
#else
 	lcd_initIO_PwrOff();	
#endif

	bt_initIO();
	gps_initIO();
	//rf_315m_initIO();
	music_initIO();
	ev1527_initIO();
}


#include "task.h"
#include "led.h"
#include "usart.h"
#include "led.h"
#include "serial_debug.h"
#include "adc.h"
#include "tim.h"
#include "adc_callback.h"
#include "gpio.h"
#include "can.h"
//#include "bsp_iic.h"
#include "PMSM_Driver.h"
#include "PMSM_Control.h"
#include "Brush_Driver.h"
#include "Brush_Control.h"
#include "HALL_Encoder.h"
#include "Status_Check.h"
#include "weak_handle.h"
#include "protocol.h"
#include "xsto_api_ii.h"
//#include "QMI8658C.h"
#include "icm42688.h"
#include "dma.h"
#include "key_filter.h"
#include "ultrasonic.h"

//#include "qmi8658c.h"
void main_config(void)
{
	
	//led
	led_init();
	led_set(led_sys, led_mode_init);	
	led_set(led_rf, led_mode_off);
	led_set(led_net, led_mode_off);
	
	MX_DMA_Init();
	MX_GPIO_Init();
	Key_Array_Init();
//	
	

	protocol_init();
	MX_CAN1_Init();
	
	Icm42688Init();
//	QMI8658_init();
//	
	DypRd_Init();
	MX_USART1_UART_Init();
	MX_USART3_UART_Init();
	MX_USART3_START();
//	
	
	PMSM_para_init();
	HALL_Encoder_init();


  MX_ADC1_Init(adc0_callback_handle);
	MX_TIM1_Init();
	MX_ADC3_Init(adc2_callback_handle);
	MX_TIM8_Init();
	
	
	Brush_para_init();
	MX_TIM3_Init();
	MX_TIM4_Init();
	MX_TIM6_Init();
  MX_TIM7_Init();
	MX_TIM2_Init();
	MX_TIM9_Init();
//	

}








void task_1ms(void)
{
	if(task_time.timer_1ms >= 1)
	{
		task_time.timer_1ms = 0;
		//loop
//
		can_receive();
		MPU_ZERO_CHECK_HANDLE();
		Brush_Mode_Loop(Brush_A);
		Brush_Mode_Loop(Brush_U);	
	}
}



uint8_t buffer_test[8] = {0x00,0x05,0x00,0x01,0x00,0x00,0x04,0x00};
void task_2ms(void)
{
	if(task_time.timer_2ms >= 2)
	{
		task_time.timer_2ms = 0;
		//loop
		serial_loop();
	}
}

void task_5ms(void)
{
	if(task_time.timer_5ms >= 3)
	{
		task_time.timer_5ms = 0;
		//loop
		can_transmit();
	}
}



void task_10ms(void)
{
	if(task_time.timer_10ms >= 10)
	{
		task_time.timer_10ms = 0;
		//loop
//		Read_Accele();
		PMSM_APP_LOOP();
		IcmGetRawData(&stAccData,&stGyroData);
//		QMI_ReadData();
		ADC_CURRENT_HANDLE();
		BRUSH_SPEED_ANGLE_A();
		BRUSH_SPEED_ANGLE_U();
		brush_Cmd_handle(Brush_A);
		brush_Cmd_handle(Brush_U);
		protocol_subscribe_loop();
	}
}


static uint8_t count_300ms= 0;
void task_100ms(void)
{
	if(task_time.timer_100ms >= 100)
	{
		task_time.timer_100ms = 0;
		//loop
		led_loop();
		test_BRUSH();
		online_loop();
		Status_Check_Loop();
		PMSM_Status_Loop();
		Brush_STATUS_LOOP();
		modbus_recve();
		if(++count_300ms>=3)
		{
			count_300ms = 0;
//			api_loop(system_times);
			serial3_send();
			
		}
	}
}


//ÈÎÎñº¯Êı//

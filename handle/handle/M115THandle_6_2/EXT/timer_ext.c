#include "timer_ext.h"
#include "main.h"

/*FreeRtos includes*/
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

//static TimerHandle_t scantimer;
Time_Flag_STR time_flag;

//static void ScanTimerCallback(xTimerHandle xTimer)
//{
//	time_flag.cnt++;
//	if((time_flag.cnt%10)==0)
//	{		
//		scan_joyxy();
//		rf_315m_scan();		
//		time_flag.time_10ms=1;
//	}		
//	if((time_flag.cnt%20)==0)
//	{
//		bsp_KeyScan10ms();
//	}
//	if((time_flag.cnt%100)==0)
//	{
//		time_flag.time_100ms=1;
//	}
//	if((time_flag.cnt%1000)==0)
//	{
//		time_flag.time_1000ms=1;
//		time_flag.cnt=0;
//	}	
//}
void timer_myinit(void)
{
	
//	scantimer = xTimerCreate("scantimer", 10, pdTRUE,
//						0, ScanTimerCallback);
//	xTimerStart(scantimer,0);
	MX_TIM2_SCAN_Init();

	
	time_flag.time_1ms=0;
	time_flag.time_10ms=0;
	time_flag.time_100ms=0;
	time_flag.time_1000ms=0;	

}

/* TIM2 init function */
void MX_TIM2_SCAN_Init(void)
{
		timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(RCU_TIMER0);
    timer_deinit(TIMER0);

    /* TIMER0 configuration */
    timer_initpara.prescaler         = 119;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    // timer_initpara.period            = 999;//1ms进一次中断
    timer_initpara.period            = 199;//200us进一次中断
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER0,&timer_initpara);	
		
	nvic_irq_enable(TIMER0_UP_IRQn,2,0);
	timer_interrupt_enable(TIMER0,TIMER_INT_UP);
    timer_auto_reload_shadow_disable(TIMER0);
    timer_enable(TIMER0);	

}



/* USER CODE BEGIN 1 */
//回调函数，定时器中断服务函数调用
void TIMER0_UP_IRQHandler(void)
{
	static u8 h = 0;

  if(RESET != timer_interrupt_flag_get(TIMER0,TIMER_INT_FLAG_UP)){	

		time_flag.cnt++;
		// if((time_flag.cnt%5)==0)
		// if((time_flag.cnt%2)==0)
		// {
		scan_joyxy();
		#if USING_LED_POINT_DISPLAY
		if((time_flag.cnt % 5) == 0){ // 5* 200 us == 1ms
			ledSpiDmaRefreshRow(h);
			if(++h > 6) h = 0;
		}
		#endif



		// }
		// if((time_flag.cnt%100)==0)
		// {			
		// 	rf_315m_scan();				
		// 	time_flag.time_10ms=1;
		// }	
		// if((time_flag.cnt%100)==0)
		// {
		// 	time_flag.time_100ms=1;
		// }
		if((time_flag.cnt%1000)==0)
		{
		// 	time_flag.time_1000ms=1;
		// 	//gpio_bit_write(LED1_GPIO_PORT,LED1_PIN,gpio_output_bit_get(LED1_GPIO_PORT,LED1_PIN)^1);
		// 	//gpio_bit_write(LED2_GPIO_PORT,LED2_PIN,gpio_output_bit_get(LED2_GPIO_PORT,LED2_PIN)^1);
			time_flag.cnt=0;
		}		
		
		timer_interrupt_flag_clear(TIMER0,TIMER_INT_FLAG_UP);	
		
	}
			
}

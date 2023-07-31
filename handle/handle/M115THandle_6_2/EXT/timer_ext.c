#include "timer_ext.h"
#include "main.h"

/*FreeRtos includes*/
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

//static TimerHandle_t scantimer;
Time_Flag_STR time_flag;

void timer_myinit(void)
{
	
//	scantimer = xTimerCreate("scantimer", 10, pdTRUE,
//						0, ScanTimerCallback);
//	xTimerStart(scantimer,0);
	MX_TIM2_SCAN_Init();

	
	// time_flag.time_1ms=0;
	// time_flag.time_10ms=0;
	// time_flag.time_100ms=0;
	// time_flag.time_1000ms=0;	
	memset(&time_flag,0x00,sizeof(time_flag));

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
    // timer_initpara.period            = 199;//200us进一次中断
    // timer_initpara.period            = 49;//50us进一次中断
    timer_initpara.period            = 9;//10us进一次中断
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
  if(RESET != timer_interrupt_flag_get(TIMER0,TIMER_INT_FLAG_UP)){	
	#if 1
		time_flag.cnt++;
		if((time_flag.cnt%20)== 0){

			// if(Remote_setting_para.RemoteBTConnect != eBlubtooth){
			// 	scan_joyxy();
			// }
		}
		
		if((time_flag.cnt%9) == 0){
				ev1527_scan();
				// time_flag.time_90us = 1;
		}
		
		if((time_flag.cnt%100) == 0){ // 1ms
			#if USING_LED_POINT_DISPLAY
				ledSpiDmaRefreshRow();
			#endif
			scan_joyxy();

			time_flag.time_1ms=1;
		}
		if((time_flag.cnt%200) == 0){ //200*10us == 2ms
			time_flag.time_2ms=1;
		}

		if((time_flag.cnt%1000) == 0){ // 1000* 10us == 10ms
			time_flag.time_10ms=1;
		}
		if((time_flag.cnt%2000) == 0){ // 2000* 10 us == 20ms
			time_flag.time_20ms=1;
		}

		if((time_flag.cnt%10000) == 0){ // 10000* 10 us == 100ms
			time_flag.time_100ms=1;
		}

		if((time_flag.cnt%20000) == 0){ // 20000* 10 us == 200ms
			time_flag.time_200ms=1;
		}

		if((time_flag.cnt%100000) == 0) //100000 * 10 us == 1s
		{
			time_flag.time_1000ms=1;
			time_flag.cnt=0;
		}		
	#elif 1
		time_flag.cnt++;
		if(Remote_setting_para.RemoteBTConnect != eBlubtooth){
			scan_joyxy();
		}

		if((time_flag.cnt % 20) == 0){ // 20* 50 us == 1ms
			#if USING_LED_POINT_DISPLAY
				ledSpiDmaRefreshRow();
			#endif
			time_flag.time_1ms=1;
		}

		if((time_flag.cnt % 200) == 0){ // 200* 50 us == 10ms
			time_flag.time_10ms=1;
		}
		if((time_flag.cnt % 400) == 0){ // 400* 50 us == 20ms
			time_flag.time_20ms=1;
		}

		if((time_flag.cnt % 2000) == 0){ // 2000* 50 us == 100ms
			time_flag.time_100ms=1;
		}

		if((time_flag.cnt % 4000) == 0){ // 4000* 50 us == 200ms
			time_flag.time_200ms=1;
		}

		if((time_flag.cnt%20000) == 0) //20000 * 50 us == 1s
		{
			time_flag.time_1000ms=1;
			time_flag.cnt=0;
		}
	#else
		time_flag.cnt++;
		if(Remote_setting_para.RemoteBTConnect != eBlubtooth){
			scan_joyxy();
		}

		if((time_flag.cnt % 5) == 0){ // 5* 200 us == 1ms
			#if USING_LED_POINT_DISPLAY
				ledSpiDmaRefreshRow();
			#endif
			time_flag.time_1ms=1;
		}

		if((time_flag.cnt % 50) == 0){ // 50* 200 us == 10ms
			time_flag.time_10ms=1;
		}
		if((time_flag.cnt % 100) == 0){ // 100* 200 us == 20ms
			time_flag.time_20ms=1;
		}

		if((time_flag.cnt % 500) == 0){ // 500* 200 us == 100ms
			time_flag.time_100ms=1;
		}

		if((time_flag.cnt % 1000) == 0){ // 1000* 200 us == 200ms
			time_flag.time_200ms=1;
		}

		if((time_flag.cnt%5000) == 0) //5000 * 200 us == 1s
		{
			time_flag.time_1000ms=1;
			time_flag.cnt=0;
		}		
		#endif
		timer_interrupt_flag_clear(TIMER0,TIMER_INT_FLAG_UP);	
	}
			
}

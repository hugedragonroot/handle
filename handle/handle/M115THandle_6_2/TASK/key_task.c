#include <stdbool.h>
#include "key_task.h"
/*FreeRtos includes*/
#include "FreeRTOS.h"
#include "task.h"
#include "key_ext.h"
#include "uart_ext.h"
#include <stdio.h>
void keyTask(void* param)
{
	TickType_t PreviousWakeTime = xTaskGetTickCount();
	while(1)
	{
		// vTaskDelay(20);
		#if USING_TPAD 
		TPAD_SCAN();
		#else
		bsp_KeyScan10ms();
		#endif
		vTaskDelayUntil(&PreviousWakeTime,pdMS_TO_TICKS(10));
	}
}


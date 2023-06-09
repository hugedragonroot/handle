#include "main.h"
/*FreeRtos includes*/
#include "FreeRTOS.h"
#include "task.h"

/*œ‘ æ»ŒŒÒ*/

void displayTask(void* param)
{

	#if USING_LED_POINT_DISPLAY
	TickType_t PreviousWakeTime = xTaskGetTickCount();
	#else
	vTaskDelay(5);
	#endif
	while(1)
	{
		#if USING_LED_POINT_DISPLAY
		ledShow();
		vTaskDelayUntil(&PreviousWakeTime,pdMS_TO_TICKS(10));
		#else
		app_display();
		#endif
	}
}


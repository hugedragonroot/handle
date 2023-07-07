#include "main.h"
/*FreeRtos includes*/
#include "FreeRTOS.h"
#include "task.h"

/*��ʾ����*/

void displayTask(void* param)
{

	while(1)
	{
		vTaskDelay(10);
		#if USING_LED_POINT_DISPLAY
		ledShow();
		#else
		app_display();
		#endif
	}
}


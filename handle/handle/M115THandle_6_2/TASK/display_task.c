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
		app_display();
	}
}


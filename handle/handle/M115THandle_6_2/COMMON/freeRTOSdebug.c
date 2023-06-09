#include "main.h"
#include "FreeRTOSConfig.h"

/*FreeRTOS���ͷ�ļ�*/
#include "FreeRTOS.h"
#include "task.h"


u32 traceTickCount;

void vApplicationMallocFailedHook( void )
{
	portDISABLE_INTERRUPTS();
	//printf("\nMalloc failed!\n");
	while(1);
}

#if (configCHECK_FOR_STACK_OVERFLOW == 1)
void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed portCHAR *pcTaskName)
{
	portDISABLE_INTERRUPTS();
	//printf("\nStack overflow!\n");
	while(1);
}
#endif

void vApplicationIdleHook( void )
{
    __WFI();	/*����͹���ģʽ*/
}


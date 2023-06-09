#include "main.h"


static TaskHandle_t startTaskHandle;
static void startTask(void *param);


int main(void)
{	
	//uint8_t keyS;
	
	drv_init();	
	ext_init();

	Remote_buff_init();	
	Remote_para_init();

#if !USING_LED_POINT_DISPLAY
	display_init();
#endif
	mem_read();

	//protocol_init();
  

	xTaskCreate(startTask, "START_TASK", 128, NULL, 2, &startTaskHandle);/*������ʼ����*/
	vTaskStartScheduler();/*�����������*/

	while(1){	};/* ������Ⱥ󲻻�ִ�е��� */
	
}

/*��������*/
void startTask(void *param)
{
	taskENTER_CRITICAL();	/*�����ٽ���*/
	
	xTaskCreate(keyTask, "KEY_SCAN", 128, NULL, 5, NULL);
	#if USING_LED_POINT_DISPLAY
	xTaskCreate(displayTask, "LCD_DISPLAY", 256, NULL, 6, NULL);
	#else
	xTaskCreate(displayTask, "LCD_DISPLAY", 256, NULL, 1, NULL);
	#endif
	#if USING_CAN
	xTaskCreate(CANRxTask, "CAN_RX", 128, NULL, 3, NULL);
	xTaskCreate(CANTxTask, "CAN_TX", 256, NULL, 3, NULL);
	#else
	xTaskCreate(wireRxTask, "WIRE_RX", 128, NULL, 3, NULL);
	xTaskCreate(wireTxTask, "WIRE_TX", 128, NULL, 3, NULL);
	xTaskCreate(BTRxTask, "BT_RX", 128, NULL, 6, NULL);
	xTaskCreate(BTTxTask, "BT_TX", 128, NULL, 6, NULL);		
	#endif

	#if USING_CAN
	xTaskCreate(CANProcessTask, "CAN_DATA", 256, NULL, 4, NULL);
	#else
	xTaskCreate(wireProcessTask, "WIRELINK_DATA", 128, NULL, 4, NULL);
	xTaskCreate(BTProcessTask, "BTLINK_DATA", 128, NULL, 4, NULL);
	#endif

	// xTaskCreate(gpsTask, "GPS_DATA", 128, NULL, 6, NULL);	

	xTaskCreate(commandTxTask, "COMMAND_TX", 256, NULL, 2, NULL);	

	// xTaskCreate(debugTask, "debug", 128, NULL, 1, NULL);	


	vTaskDelete(startTaskHandle);									/*ɾ����ʼ����*/
	
	taskEXIT_CRITICAL();	/*�˳��ٽ���*/
}



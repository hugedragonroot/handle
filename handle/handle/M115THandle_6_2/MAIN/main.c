#include "main.h"

#if USING_RTOS
static TaskHandle_t startTaskHandle;
static void startTask(void *param);
#endif


int main(void)
{	
	//uint8_t keyS;
	
	
    drv_init();	
	ext_init();

	Remote_buff_init();	
	Remote_para_init();

	mem_init();
  
#if USING_RTOS
	xTaskCreate(startTask, "START_TASK", 128, NULL, 2, &startTaskHandle);/*������ʼ����*/
	vTaskStartScheduler();/*�����������*/
#else

	while(1){

		if(time_flag.time_10ms == 1){

			if(Remote_setting_para.PowerStatus == ePowerOn){
				BTNoneOSTask();
				app_cltprocess();
				CANNoneOsTxTask();
			}
			bsp_KeyScan10ms();

			time_flag.time_10ms = 0;
		}

		if(time_flag.time_20ms == 1){

			if(Remote_setting_para.PowerStatus == ePowerOn){
				can_receive();
			}
			time_flag.time_20ms = 0;
		}

		if(time_flag.time_100ms == 1){
			#if USING_LED_POINT_DISPLAY
			ledShow();
			#else
			app_display();
			#endif
			music_play_task();
			ev1527_task();
			time_flag.time_100ms = 0;
		}

		if(time_flag.time_200ms == 1){
			LED1_BLINK();
			time_flag.time_200ms = 0;
		}

		if(time_flag.time_1000ms == 1){

			#if USING_DEBUG
			debugUartPrint();
			#endif
			fwdgt_feed();

			if(Remote_setting_para.PowerStatus == ePowerOff){
				BTdisconnect();
				powerOFF();
			}else{
				powerON();
				mem_save();
			}
			if(Remote_setting_para.HandleLock == 1){
				Remote_setting_para.CoordX = Coord_Base;
				Remote_setting_para.CoordY = Coord_Base;
				Remote_trans_para.push_rod_speed = 0;
				BTdisconnect();
			}

			// static uint8_t array[] = "AT\r\n";
			// uart_bt_send(array,sizeof(array));
			// uart_gps_send(array, sizeof(array));
	// static uint8_t array1[] = "AT+NAMEXSTOM115000115\r\n";
	// uart_bt_send(array1, sizeof(array1));

			time_flag.time_1000ms = 0;
		}
			
		

	}/* ������Ⱥ󲻻�ִ�е��� */
#endif
	
}

#if USING_RTOS
/*��������*/
void startTask(void *param)
{
	taskENTER_CRITICAL();	/*�����ٽ���*/
	
	xTaskCreate(keyTask, "KEY_SCAN", 128, NULL, 5, NULL);

	xTaskCreate(displayTask, "LCD_DISPLAY", 256, NULL, 1, NULL);
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

	xTaskCreate(gpsTask, "GPS_DATA", 128, NULL, 6, NULL);	

	xTaskCreate(commandTxTask, "COMMAND_TX", 256, NULL, 2, NULL);	

#if USING_TASK_DEBUG
	xTaskCreate(debugTask, "debug", 128, NULL, 1, NULL);	
#endif


	vTaskDelete(startTaskHandle);									/*ɾ����ʼ����*/
	
	taskEXIT_CRITICAL();	/*�˳��ٽ���*/
}
#endif


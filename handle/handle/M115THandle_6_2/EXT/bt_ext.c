#include "bt_ext.h"
#include "main.h"
/*FreeRtos includes*/
#if USING_RTOS
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "string.h"
#endif

static uint8_t Bt_recv_buff[64];//接收缓存
static uint8_t Bt_data_buff[128];//队列处理缓存

TLoopBuf loop_bt_uart;
// TCOMMUN BtPacks[8];

#if USING_RTOS
static xQueueHandle BTDataDelivery;		/*串口数据接收队列*/
#endif

void BTdisconnect(void){
#if 1

	static u8 btDisconnectFlag = 0;

	if((SET == gpio_input_bit_get(BT_LINK_STATE_GPIO_PORT,BT_LINK_STATE_PIN))
	&&(joyetick_adc.valX != Coord_Base 
	|| joyetick_adc.valY != Coord_Base 
	|| Remote_setting_para.HandleLock == eLock
	#if ERROR_CONTROL_FLAG
	|| Remote_trans_para.errorFlag.all[0] != 0 
	#endif
	|| Remote_setting_para.PowerStatus == ePowerOff
	||(Remote_setting_para.RemoteBTConnect == eBlubtooth && 
		++Remote_setting_para.RemoteBTCount == 10))){

		Remote_setting_para.RemoteBTConnect = eOffline;
		Remote_setting_para.CoordX = Coord_Base;
		Remote_setting_para.CoordY = Coord_Base;
		Remote_trans_para.push_rod_speed = PUSH_ORD_SPEED_ZERO;
		Remote_setting_para.RemoteBTCount = 0;
		LoopQueue_DeInit(&loop_bt_uart);

		switch (btDisconnectFlag)
		{
		case 0:
		case 1:
			BT_KEY_ON();
			++btDisconnectFlag;
			break;
		case 2:
		case 3:
		case 4:
			BT_KEY_OFF();
			++btDisconnectFlag;
			break;
		case 5:
			BT_KEY_ON();
			btDisconnectFlag = 0;

			break;
		
		default:
			break;
		}
	}else{
		btDisconnectFlag = 0;
	}
#else

	if(gpio_input_bit_get(BT_LINK_STATE_GPIO_PORT,BT_LINK_STATE_PIN) == SET){
		BT_KEY_ON();
		delay_ms(1);
		BT_KEY_OFF();
		delay_ms(250);
		BT_KEY_ON();
		LoopQueue_DeInit(&loop_bt_uart);
	}

#endif
}

#if 0
void BTEnterStandby(void){
	BT_KEY_ON();
	delay_ms(1);
	BT_KEY_OFF();
	delay_ms(250);
	BT_KEY_ON();
}
#endif



void bt_init(void)
{	
	#if USING_RTOS
		BTDataDelivery = xQueueCreate(256, sizeof(uint8_t));	
	#endif


	LoopQueue_Init(&loop_bt_uart,Bt_data_buff,sizeof(Bt_data_buff));
	// LoopQueue_Init(&loop_bt_uart,(uint8_t*)BtPacks,sizeof(BtPacks));
	//Rx_DMA
	dma_channel_disable(DMA0,DMA_CH5);
	dma_transfer_number_config(DMA0,DMA_CH5,sizeof(Bt_recv_buff));
	dma_memory_address_config(DMA0,DMA_CH5,(uint32_t)Bt_recv_buff);
	dma_channel_enable(DMA0,DMA_CH5);

	const u8 test[] = "AT\r\n";
	//Tx_DMA
	dma_channel_disable(DMA0,DMA_CH6);
	dma_transfer_number_config(DMA0,DMA_CH6,sizeof(test));
	dma_memory_address_config(DMA0,DMA_CH6,(uint32_t)test);
	dma_channel_enable(DMA0,DMA_CH6);

	// BTdisconnect();

#if BT_CHANGE_NAME_FLAG
	static uint8_t array2[] = "AT+DEFAULT\r\n";
	uart_bt_send(array2, sizeof(array2));

	delay_ms(200);
	static uint8_t array1[] = "AT+NAMEXSTOM115temp000115\r\n";
	uart_bt_send(array1, sizeof(array1));
	delay_ms(200);
	static uint8_t array[] = "AT+BAUD7\r\n";
	uart_bt_send(array, sizeof(array));

#endif

}
						   

// const char head[] ={FrameHeadUpH,FrameHeadUpH}; 
void USART1_IRQHandler(void)
{
	#if USING_RTOS
	uint8_t data;
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	#endif
	
    if(RESET != usart_interrupt_flag_get(USART1, USART_INT_FLAG_IDLE)){
		uint8_t bt_recv_len = sizeof(Bt_recv_buff) - dma_transfer_number_get(DMA0,DMA_CH5);

		dma_channel_disable(DMA0,DMA_CH5);

		// if(strncmp((const char *)Bt_recv_buff,head,sizeof(2)) == 0){
		// 	LoopQueue_Write(&loop_bt_uart,&Bt_recv_buff[2],sizeof(TCOMMUN));
		// }
		// uart_wire_send((uint8_t *)Bt_recv_buff,bt_recv_len);

		LoopQueue_Write(&loop_bt_uart,Bt_recv_buff,bt_recv_len);

		dma_transfer_number_config(DMA0,DMA_CH5,sizeof(Bt_recv_buff));

		dma_channel_enable(DMA0,DMA_CH5);

		usart_data_receive(USART1);
		usart_interrupt_flag_clear(USART1,USART_INT_FLAG_IDLE);

	#if USING_RTOS
       xQueueSendFromISR(BTDataDelivery, &data, &xHigherPriorityTaskWoken);
	#endif
    }       

}

uint8_t BTGetDataWithTimout(uint8_t *c)
{
	#if USING_RTOS
	if (xQueueReceive(BTDataDelivery, c, 1000) == pdTRUE)
	{
		return 1;
	}
	*c = 0;
	#else

	return LoopQueue_ReadRelease(&loop_bt_uart,c,1);

	#endif

}

uint8_t BTGetPack(TCOMMUN *pack)
{
	return LoopQueue_ReadRelease(&loop_bt_uart,(uint8_t*)pack,sizeof(TCOMMUN));
}


			   
/*****************  发送字符串 **********************/
void BLT_Usart_SendString(uint8_t *str)
{
	unsigned int k=0;
  do 
  {
		uart_bt_sendbyte(str[k]);
     k++;
  } while(*(str + k)!='\0');

}	



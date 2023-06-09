#include "bt_ext.h"
#include "main.h"
/*FreeRtos includes*/
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "string.h"

static xQueueHandle BTDataDelivery;		/*串口数据接收队列*/
uint8_t Bt_data_buff[256],buf_len=0;
void bt_init(void)
{	
	BTDataDelivery = xQueueCreate(256, sizeof(uint8_t));	

}
						   


void USART1_IRQHandler(void)
{
	uint8_t data;
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	
    if(RESET != usart_interrupt_flag_get(USART1, USART_INT_FLAG_RBNE)){
        /* read one byte from the receive data register */
        data = (uint8_t)usart_data_receive(USART1);
//				Bt_data_buff[buf_len++]=data;
       xQueueSendFromISR(BTDataDelivery, &data, &xHigherPriorityTaskWoken);
    }       

}


int BTGetDataWithTimout(uint8_t *c)
{

	if (xQueueReceive(BTDataDelivery, c, 1000) == pdTRUE)
	{
		return 1;
	}
	*c = 0;
	return 0;
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



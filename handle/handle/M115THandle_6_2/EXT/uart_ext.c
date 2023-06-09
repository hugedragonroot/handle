#include "uart_ext.h"
#include "main.h"
#include "string.h"
/*FreeRtos includes*/
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
/* USER CODE END 0 */


static xQueueHandle uartData;		/*串口数据接收队列*/
void WIRE_Init( uint32_t baudval);
void BT_Init( uint32_t baudval);
void DEBUG_Init( uint32_t baudval);
void MUSIC_Init( uint32_t baudval);
void Gps_Init( uint32_t baudval);
void uart_init(void)
{
	// WIRE_Init(9600);
	WIRE_Init(115200);
	BT_Init(9600);
	DEBUG_Init(115200);	
	MUSIC_Init(9600);	
	Gps_Init(115200);		

	uartData = xQueueCreate(256, sizeof(uint8_t));
}


void WIRE_Init( uint32_t baudval)
{
	UART_Init(RCU_USART0,USART0,baudval);
	
	nvic_irq_enable(USART0_IRQn, 6, 2);
	/* enable USART0 receive interrupt */
	usart_interrupt_enable(USART0, USART_INT_RBNE);
	
	usart_enable(USART0);
}


void BT_Init( uint32_t baudval)
{
	UART_Init(RCU_USART1,USART1,baudval);
	
	nvic_irq_enable(USART1_IRQn, 6, 2);
	/* enable USART0 receive interrupt */
	usart_interrupt_enable(USART1, USART_INT_RBNE);
	
	usart_enable(USART1);

}


void DEBUG_Init( uint32_t baudval)
{
	UART_Init(RCU_USART2,USART2,baudval);
	usart_enable(USART2);	

}

void MUSIC_Init( uint32_t baudval)
{
	UART_Init(RCU_UART4,UART4,baudval);
	usart_enable(UART4);	
}

void Gps_Init( uint32_t baudval)
{
	UART_Init(RCU_UART3,UART3,baudval);
	nvic_irq_enable(UART3_IRQn, 6, 2);
	/* enable UART3 receive interrupt */
	usart_interrupt_enable(UART3, USART_INT_RBNE);
	usart_enable(UART3);	
}



void uart_wire_sendbyte(uint8_t ch)
{
#if !APP_BT_TEST	
		while(usart_flag_get(USART0,USART_FLAG_TBE)==RESET){}
		usart_data_transmit(USART0,ch);
#endif
#if !DEBUG_CMDOUT				
		while(usart_flag_get(USART2,USART_FLAG_TBE)==RESET){}
		usart_data_transmit(USART2,ch);		
#endif
}

void uart_wire_send(uint8_t *array, uint16_t len)
{
  uint8_t i;

  for(i=0; i<len; i++)
  {
		uart_wire_sendbyte(array[i]);
  }
}



void USART0_IRQHandler(void)
{
	uint8_t data;
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	
    if(RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE)){
        /* read one byte from the receive data register */
		data = 	(uint8_t)usart_data_receive(USART0);
		xQueueSendFromISR(uartData, &data, &xHigherPriorityTaskWoken);
    }       

}
int WireGetDataWithTimout(uint8_t *c)
{
	if (xQueueReceive(uartData, c, 1000) == pdTRUE)
	{
		return 1;
	}
	*c = 0;
	return 0;
}



void uart_bt_sendbyte(uint8_t ch)
{
		while(usart_flag_get(USART1,USART_FLAG_TBE)==RESET){}
		usart_data_transmit(USART1,ch);
	
}

void uart_bt_send(uint8_t *array, uint16_t len)
{
  uint8_t i;

  for(i=0; i<len; i++)
  {
		uart_bt_sendbyte(array[i]);
  }
}



void uart_music_sendbyte(uint8_t ch)
{
	while(usart_flag_get(UART4,USART_FLAG_TBE)==RESET){}
	usart_data_transmit(UART4,ch);
		
}

void uart_music_send(uint8_t *array, uint16_t len)
{
  uint8_t i;

  for(i=0; i<len; i++)
  {
		uart_music_sendbyte(array[i]);
  }
	
}

void uart_gps_sendbyte(uint8_t ch)
{
	while(usart_flag_get(UART3,USART_FLAG_TBE)==RESET){}
	usart_data_transmit(UART3,ch);
		
}

void uart_gps_send(uint8_t *array, uint16_t len)
{
  uint8_t i;

  for(i=0; i<len; i++)
  {
		uart_gps_sendbyte(array[i]);
  }
	
}



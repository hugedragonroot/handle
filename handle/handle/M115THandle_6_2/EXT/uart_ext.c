#include "uart_ext.h"
#include "main.h"
#include "string.h"

#if USING_RTOS
/*FreeRtos includes*/
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
/* USER CODE END 0 */


static xQueueHandle uartData;		/*串口数据接收队列*/
#endif

void WIRE_Init( uint32_t baudval);
void BT_Init( uint32_t baudval);
void DEBUG_Init( uint32_t baudval);
void MUSIC_Init( uint32_t baudval);
void Gps_Init( uint32_t baudval);
void uart_init(void)
{
	#if USING_DEBUG
	WIRE_Init(115200);
	#endif

	#if BT_CHANGE_NAME_FLAG
	BT_Init(9600);
	#else
	BT_Init(115200);

	#endif

	// DEBUG_Init(115200);	
	MUSIC_Init(9600);	
	Gps_Init(115200);		

	#if USING_RTOS
	uartData = xQueueCreate(256, sizeof(uint8_t));
	#endif
}


void WIRE_Init( uint32_t baudval)
{
	u8 test  = 0x30;
	dma_parameter_struct dma_init_struct;
	dma_deinit(DMA0,DMA_CH3);
	dma_init_struct.periph_addr = (uint32_t) &USART_DATA(USART0);
	dma_init_struct.memory_addr = (uint32_t) &test;
	dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
	dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
	dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
	dma_init_struct.priority = DMA_PRIORITY_LOW;
	dma_init_struct.number = 1;
	dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
	dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
	dma_init(DMA0,DMA_CH3,&dma_init_struct);
	dma_circulation_disable(DMA0,DMA_CH3);
	dma_memory_to_memory_disable(DMA0,DMA_CH3);


	UART_Init(RCU_USART0,USART0,baudval);
	// nvic_irq_enable(USART0_IRQn, 6, 2);
	/* enable USART0 receive interrupt */
	// usart_interrupt_enable(USART0, USART_INT_RBNE);
	usart_enable(USART0);

	dma_channel_enable(DMA0,DMA_CH3);
	usart_dma_transmit_config(USART0,USART_DENT_ENABLE);

}


void BT_Init( uint32_t baudval)
{
	//Rx_DMA
	dma_parameter_struct dma_init_struct;
	dma_deinit(DMA0,DMA_CH5);
	dma_init_struct.periph_addr = (uint32_t) &USART_DATA(USART1);
	dma_init_struct.memory_addr = (uint32_t) NULL;
	dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
	dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
	dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
	dma_init_struct.priority = DMA_PRIORITY_MEDIUM;
	dma_init_struct.number = 0;
	dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
	dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
	dma_init(DMA0,DMA_CH5,&dma_init_struct);
	dma_circulation_disable(DMA0,DMA_CH5);
	dma_memory_to_memory_disable(DMA0,DMA_CH5);

	//Tx_DMA
	#if 1
	dma_deinit(DMA0,DMA_CH6);
	dma_init_struct.periph_addr = (uint32_t) &USART_DATA(USART1);
	dma_init_struct.memory_addr = (uint32_t) NULL;
	dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
	dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
	dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
	dma_init_struct.priority = DMA_PRIORITY_MEDIUM;
	dma_init_struct.number = 0;
	dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
	dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
	dma_init(DMA0,DMA_CH6,&dma_init_struct);
	dma_circulation_disable(DMA0,DMA_CH6);
	dma_memory_to_memory_disable(DMA0,DMA_CH6);
	#endif


	UART_Init(RCU_USART1,USART1,baudval);
	
	nvic_irq_enable(USART1_IRQn, 4, 0);
	/* enable USART0 receive interrupt */
	usart_interrupt_enable(USART1, USART_INT_IDLE);
	
	usart_enable(USART1);
	// dma_channel_enable(DMA0,DMA_CH5);
	dma_channel_disable(DMA0,DMA_CH5);
	dma_channel_disable(DMA0,DMA_CH6);

	usart_dma_receive_config(USART1,USART_DENR_ENABLE);
	usart_dma_transmit_config(USART1,USART_DENT_ENABLE);

	//
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
	#if 1
	//Tx DMA
	dma_parameter_struct dma_init_struct;
	dma_deinit(DMA1,DMA_CH4);
	dma_init_struct.periph_addr = (uint32_t) &USART_DATA(UART3);
	dma_init_struct.memory_addr = (uint32_t) NULL;
	dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
	dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
	dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
	dma_init_struct.priority = DMA_PRIORITY_HIGH;
	dma_init_struct.number = 0;
	dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
	dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
	dma_init(DMA1,DMA_CH4,&dma_init_struct);
	dma_circulation_disable(DMA1,DMA_CH4);
	dma_memory_to_memory_disable(DMA1,DMA_CH4);
	#endif

	#if 1
	//Rx DMA
	dma_deinit(DMA1,DMA_CH2);
	dma_init_struct.periph_addr = (uint32_t) &USART_DATA(UART3);
	dma_init_struct.memory_addr = (uint32_t) NULL;
	dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
	dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
	dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
	dma_init_struct.priority = DMA_PRIORITY_HIGH;
	dma_init_struct.number = 0;
	dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
	dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
	dma_init(DMA1,DMA_CH2,&dma_init_struct);
	dma_circulation_disable(DMA1,DMA_CH2);
	dma_memory_to_memory_disable(DMA1,DMA_CH2);
	#endif



	UART_Init(RCU_UART3,UART3,baudval);
	nvic_irq_enable(UART3_IRQn, 5, 0);
	/* enable UART3 receive interrupt */
	// usart_interrupt_enable(UART3, USART_INT_RBNE);
	usart_interrupt_enable(UART3, USART_INT_IDLE);
	usart_enable(UART3);	


	dma_channel_disable(DMA1,DMA_CH2);
	usart_dma_transmit_config(UART3,USART_DENT_ENABLE);

	dma_channel_disable(DMA1,DMA_CH2);
	usart_dma_receive_config(UART3,USART_DENR_ENABLE);

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
  #if 0
  uint8_t i;

  for(i=0; i<len; i++)
  {
		uart_wire_sendbyte(array[i]);
  }
  #else

if(dma_flag_get(DMA0,DMA_CH3,DMA_FLAG_FTF) == RESET) return; 

  dma_channel_disable(DMA0,DMA_CH3);
  dma_memory_address_config(DMA0,DMA_CH3,(uint32_t)array);
  dma_transfer_number_config(DMA0,DMA_CH3,len);
  dma_channel_enable(DMA0,DMA_CH3);

  #endif
}



void USART0_IRQHandler(void)
{
	#if USING_RTOS
	uint8_t data;
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
    if(RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE)){
        /* read one byte from the receive data register */
		data = 	(uint8_t)usart_data_receive(USART0);

		xQueueSendFromISR(uartData, &data, &xHigherPriorityTaskWoken);
    }       
	#endif

}

#if USING_RTOS
int WireGetDataWithTimout(uint8_t *c)
{
	if (xQueueReceive(uartData, c, 1000) == pdTRUE)
	{
		return 1;
	}
	*c = 0;
	return 0;
}
#endif



void uart_bt_sendbyte(uint8_t ch)
{
		while(usart_flag_get(USART1,USART_FLAG_TBE)==RESET){}
		usart_data_transmit(USART1,ch);
	
}

bool uart_bt_send(uint8_t *array, uint16_t len)
{
	#if 0
  uint8_t i;

  for(i=0; i<len; i++)
  {
		uart_bt_sendbyte(array[i]);
  }
  #else

	// while (dma_flag_get(DMA0,DMA_CH6,DMA_FLAG_FTF) == RESET){}
	if(dma_flag_get(DMA0,DMA_CH6,DMA_FLAG_FTF) != RESET){
		dma_channel_disable(DMA0,DMA_CH6);
		dma_transfer_number_config(DMA0,DMA_CH6,len);
		dma_memory_address_config(DMA0,DMA_CH6,(uint32_t)array);
		dma_channel_enable(DMA0,DMA_CH6);
		return true;
	}
	return false;

  #endif
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
	#if 0
  uint8_t i;

  for(i=0; i<len; i++)
  {
		uart_gps_sendbyte(array[i]);
  }
  #else
if(dma_flag_get(DMA1,DMA_CH4,DMA_FLAG_FTF) == RESET) return; 

  dma_channel_disable(DMA1,DMA_CH4);
  dma_memory_address_config(DMA1,DMA_CH4,(uint32_t)array);
  dma_transfer_number_config(DMA1,DMA_CH4,len);
  dma_channel_enable(DMA1,DMA_CH4);

  #endif
	
}



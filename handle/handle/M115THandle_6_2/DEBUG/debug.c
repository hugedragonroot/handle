#include "debug.h"
#include "math.h"
#if 1
void debug_test(void)
{
	uint8_t buf[2];
	uint8_t key;
	float CoordAngle;
	
	while(1)
	{
//		if(LoopQueue_DataLen(&loop_debug_uart))
//		{
//			LoopQueue_ReadRelease(&loop_debug_uart,buf,1);
//			printf("%c",buf[0]);
//		}
//		if(loop_debug_uart.nReadIndex!=loop_debug_uart.nWriteIndex)
//		{
//			
//			
//			printf("%c",loop_debug_uart.nData[loop_debug_uart.nReadIndex++]);
//			if(loop_debug_uart.nReadIndex>=100)loop_debug_uart.nReadIndex=0;
//		}
//		printf("123");		
	}
}

void assertFail(char *exp, char *file, int line)
{
	//printf("Assert failed %s:%d\n", file, line);
	while (1);
}


/* USER CODE BEGIN 1 */
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 

/**
  * @brief �ض���c�⺯��printf��USARTx
  * @retval None
  */
int fputc(int ch, FILE *f)
{
		while(usart_flag_get(USART2,USART_FLAG_TBE)==RESET){}
		usart_data_transmit(USART2,ch);
    return ch;
}

#define USING_TASK_DEBUG 0
#if USING_TASK_DEBUG
u8 debugflag = 1;
char sendBuffer[512];
#endif

void debugTask(void* param){


	while (1) {
	vTaskDelay(pdMS_TO_TICKS(5000));

	#if USING_TASK_DEBUG
	if(debugflag){
		memset(sendBuffer,0,sizeof(sendBuffer));
		sendBuffer[0] = '\n';
		vTaskList((char *)&sendBuffer[1]);
		uart_wire_send((uint8_t *)sendBuffer,strlen((char *)sendBuffer));
	}
	#endif

	}
	
}


#endif
#endif


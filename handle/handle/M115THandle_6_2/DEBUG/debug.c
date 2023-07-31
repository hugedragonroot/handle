#include "debug.h"
#include "math.h"
#if 0
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
#endif

void assertFail(char *exp, char *file, int line)
{
	//printf("Assert failed %s:%d\n", file, line);
	while (1);
}


/* USER CODE BEGIN 1 */
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 

/**
  * @brief 重定向c库函数printf到USARTx
  * @retval None
  */
int fputc(int ch, FILE *f)
{
		while(usart_flag_get(USART2,USART_FLAG_TBE)==RESET){}
		usart_data_transmit(USART2,ch);
    return ch;
}

#if USING_TASK_DEBUG
u8 debugflag = 1;
char sendBuffer[512];

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
#elif 1

#define VALNAME_AND_VAL(x) (#x),(x)

static char debugTxBuffer[512] = "test 123";

void debugUartPrint(void){

	#if USING_DEBUG
	// u8 powerControlFlag = gpio_input_bit_get(POWER_CONTROL_PORT,POWER_CONTROL_PIN);

	sprintf((char *)debugTxBuffer,
	"\r\n\
	%s = %d\r\n\
	%s = %d\r\n\
	%s = %d\r\n\
	%s = %d\r\n\
	%s = %d\r\n\
	%s = %d\r\n\
	%s = %d\r\n\
	%s = %d\r\n\
	",
	VALNAME_AND_VAL(Remote_receive_para.walking_speed_L),
	VALNAME_AND_VAL(Remote_receive_para.walking_speed_R),
	VALNAME_AND_VAL(Remote_setting_para.CoordX),
	VALNAME_AND_VAL(Remote_setting_para.CoordY),
	VALNAME_AND_VAL(Phone_receive_para.CoordX),
	VALNAME_AND_VAL(Phone_receive_para.CoordY),
	VALNAME_AND_VAL(Remote_setting_para.CANQueueLenMax),
	VALNAME_AND_VAL(Remote_setting_para.CANBufferQueueLenMax)
	);

	uart_wire_send((uint8_t *)debugTxBuffer,strlen(debugTxBuffer));
	#endif
}

#endif

#endif


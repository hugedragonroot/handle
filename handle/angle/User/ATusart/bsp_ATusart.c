/**
  ******************************************************************************
  * @file    bsp_AT_usart.c
  * @author  Rhui
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   ATָ���շ�Usart�˿�
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

#include ".\ATusart\bsp_ATusart.h"
#include ".\TiMbase\bsp_TiMbase.h"
//#include ".\systick\bsp_SysTick.h"
#include "app_loopqueue.h"
#include <stdarg.h>


UsartStreamDef  UsartStr1;
UsartStreamDef  UsartStr2;
UsartStreamDef  UsartStr3;

static uint8_t loopBufUsart1[StreamLen];
static uint8_t loopBufUsart2[StreamLen];
static uint8_t loopBufUsart3[StreamLen];


/**
  * @brief  USART_COM1��ʼ��
  * @param  ��
  * @retval ��
  */
static void USART1_Config(uint32_t bound)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef  NVIC_InitStructure;
  DMA_InitTypeDef   DMA_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
//GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);/* �����ض������Ҫʹ�� */
	/* USART1_Tx */
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  /* USART1_Rx */
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* USART3��ʼ������ */
	USART_InitStructure.USART_BaudRate            = bound;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No ;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1, &USART_InitStructure);
  
  DMA_InitStruct.DMA_BufferSize         = StreamLen;
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStruct.DMA_MemoryDataSize     = DMA_PeripheralDataSize_Byte;
  DMA_InitStruct.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
  DMA_InitStruct.DMA_MemoryInc          = DMA_MemoryInc_Enable;
  DMA_InitStruct.DMA_Mode               = DMA_Mode_Normal;
  DMA_InitStruct.DMA_M2M                = DMA_M2M_Disable;
  /* DMA1 Channel4 USART1_TX */
  DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR);
  DMA_InitStruct.DMA_MemoryBaseAddr     = (uint32_t)UsartStr1.TxBuf;
  DMA_InitStruct.DMA_Priority           = DMA_Priority_Low;
  DMA_InitStruct.DMA_DIR                = DMA_DIR_PeripheralDST;
  DMA_Init(DMA1_Channel4, &DMA_InitStruct);
  /* DMA1 Channel5 USART1_RX */
  DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR);
  DMA_InitStruct.DMA_MemoryBaseAddr     = (uint32_t)UsartStr1.RxBuf;
  DMA_InitStruct.DMA_Priority           = DMA_Priority_Medium;
  DMA_InitStruct.DMA_DIR                = DMA_DIR_PeripheralSRC;
  DMA_Init(DMA1_Channel5, &DMA_InitStruct);
  
  NVIC_InitStructure.NVIC_IRQChannel                    = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 4;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority         = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel                    = DMA1_Channel4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 4;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority         = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
  DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
  USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
  USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
  DMA_Cmd(DMA1_Channel4, ENABLE);
  DMA_Cmd(DMA1_Channel5, ENABLE);
  USART_Cmd(USART1, ENABLE);
}

/**
  * @brief  USART1_DMA����8λ����
  * @param  ��
  * @retval ��
  */
void USART1_DMA_Send(uint8_t *array, uint16_t num)
{
  static uint16_t i;
  for(i=0; i<num; i++) UsartStr1.TxBuf[i] = array[i];
  if(UsartStr1.TxEN) {
    UsartStr1.TxEN = false;
    DMA_SetCurrDataCounter(DMA1_Channel4, num);
    DMA_Cmd(DMA1_Channel4, ENABLE);
  }
}

/**
  * @brief  USART_COM2��ʼ��
  * @param  ��
  * @retval ��
  */
static void USART2_Config(uint32_t bound)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef  NVIC_InitStructure;
  DMA_InitTypeDef   DMA_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	/* USART2_Tx */
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  /* USART2_Rx */
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* USART3��ʼ������ */
	USART_InitStructure.USART_BaudRate            = bound;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No ;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART2, &USART_InitStructure);
  
  DMA_InitStruct.DMA_BufferSize         = StreamLen;
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStruct.DMA_MemoryDataSize     = DMA_PeripheralDataSize_Byte;
  DMA_InitStruct.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
  DMA_InitStruct.DMA_MemoryInc          = DMA_MemoryInc_Enable;
  DMA_InitStruct.DMA_Mode               = DMA_Mode_Normal;
  DMA_InitStruct.DMA_M2M                = DMA_M2M_Disable;
  /* DMA1 Channel6 USART2_RX */
  DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);
  DMA_InitStruct.DMA_MemoryBaseAddr     = (uint32_t)UsartStr2.RxBuf;
  DMA_InitStruct.DMA_Priority           = DMA_Priority_Medium;
  DMA_InitStruct.DMA_DIR                = DMA_DIR_PeripheralSRC;
  DMA_Init(DMA1_Channel6, &DMA_InitStruct);
  /* DMA1 Channel7 USART2_TX */
  DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);
  DMA_InitStruct.DMA_MemoryBaseAddr     = (uint32_t)UsartStr2.TxBuf;
  DMA_InitStruct.DMA_Priority           = DMA_Priority_Low;
  DMA_InitStruct.DMA_DIR                = DMA_DIR_PeripheralDST;
  DMA_Init(DMA1_Channel7, &DMA_InitStruct);
  
  NVIC_InitStructure.NVIC_IRQChannel                    = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 4;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority         = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel                    = DMA1_Channel7_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 4;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority         = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
  DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);
  USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
  USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
  DMA_Cmd(DMA1_Channel6, ENABLE);
  DMA_Cmd(DMA1_Channel7, ENABLE);
	USART_Cmd(USART2, ENABLE);
}

/**
  * @brief  USART2_DMA����8λ����
  * @param  ��
  * @retval ��
  */
void USART2_DMA_Send(uint8_t *array, uint16_t num)
{
  static uint16_t i;
  for(i=0; i<num; i++) UsartStr2.TxBuf[i] = array[i];
  if(UsartStr2.TxEN) {
    UsartStr2.TxEN = false;
    DMA_SetCurrDataCounter(DMA1_Channel7, num);
    DMA_Cmd(DMA1_Channel7, ENABLE);
  }
}

/**
  * @brief  USART_COM3��ʼ��
  * @param  ��
  * @retval ��
  */
static void USART3_Config(uint32_t bound)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef  NVIC_InitStructure;
  DMA_InitTypeDef   DMA_InitStruct;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  /* USART3_DR */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;    
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_ResetBits(GPIOB, GPIO_Pin_2);  /* USART3_DR */
  /* USART3_TX */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;    
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  /* USART3_RX */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  /* USART3��ʼ������ */
  USART_InitStructure.USART_BaudRate            = bound;
  USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits            = USART_StopBits_1;
  USART_InitStructure.USART_Parity              = USART_Parity_No;
  USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_Init(USART3, &USART_InitStructure);
  
  DMA_InitStruct.DMA_BufferSize         = StreamLen;
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStruct.DMA_MemoryDataSize     = DMA_PeripheralDataSize_Byte;
  DMA_InitStruct.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
  DMA_InitStruct.DMA_MemoryInc          = DMA_MemoryInc_Enable;
  DMA_InitStruct.DMA_Mode               = DMA_Mode_Normal;
  DMA_InitStruct.DMA_M2M                = DMA_M2M_Disable;
  /* DMA1 Channel2 USART3_TX */
  DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&(USART3->DR);
  DMA_InitStruct.DMA_MemoryBaseAddr     = (uint32_t)UsartStr3.TxBuf;
  DMA_InitStruct.DMA_Priority           = DMA_Priority_Low;
  DMA_InitStruct.DMA_DIR                = DMA_DIR_PeripheralDST;
  DMA_Init(DMA1_Channel2, &DMA_InitStruct);
  /* DMA1 Channel3 USART3_RX */
  DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&(USART3->DR);
  DMA_InitStruct.DMA_MemoryBaseAddr     = (uint32_t)UsartStr3.RxBuf;
  DMA_InitStruct.DMA_Priority           = DMA_Priority_Medium;
  DMA_InitStruct.DMA_DIR                = DMA_DIR_PeripheralSRC;
  DMA_Init(DMA1_Channel3, &DMA_InitStruct);
  
  NVIC_InitStructure.NVIC_IRQChannel                    = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 4;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority         = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel                    = DMA1_Channel2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 4;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority         = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
  DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);
  USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);
  USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);
  DMA_Cmd(DMA1_Channel2, ENABLE);
  DMA_Cmd(DMA1_Channel3, ENABLE);
  USART_Cmd(USART3, ENABLE);
}

/**
  * @brief  USART3_DMA����8λ����
  * @param  ��
  * @retval ��
  */
void USART3_DMA_Send(uint8_t *array, uint16_t num)
{
  static uint16_t i;
  GPIO_SetBits(GPIOB, GPIO_Pin_2);
  for(i=0; i<num; i++) UsartStr3.TxBuf[i] = array[i];
  if(UsartStr3.TxEN) {
    UsartStr3.TxEN = false;
    DMA_SetCurrDataCounter(DMA1_Channel2, num);
    DMA_Cmd(DMA1_Channel2, ENABLE);
  }
}

/* �ض���c�⺯��printf�����ڣ��ض�����ʹ��printf���� */
int fputc(int ch, FILE *f)
{
	/* ����һ���ֽ����ݵ����� */
	USART_SendData(USART2, (uint8_t) ch);
	/* �ȴ�������� */
	while( USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET );
	return (ch);
}

/* �ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ��� */
int fgetc(FILE *f)
{
	/* �ȴ������������� */
	while( USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET );
	return (int)USART_ReceiveData(USART2);
}

/**
  * @brief  AT�豸��ʼ������
  * @param  ��
  * @retval ��
  */
void AT_Equipment_Init(void)
{
  LoopQueue_Init(&loopPointUsart1, loopBufUsart1, sizeof(loopBufUsart1));
  LoopQueue_Init(&loopPointUsart2, loopBufUsart2, sizeof(loopBufUsart2));
  LoopQueue_Init(&loopPointUsart3, loopBufUsart3, sizeof(loopBufUsart3));
  USART1_Config(256000);
	USART2_Config(256000);
	USART3_Config(9600);
}

/*****************  ����һ���ֽ� **********************/
void USARTx_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	USART_SendData(pUSARTx,ch);                                       /* ����һ���ֽ����ݵ�USART */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);    /* �ȴ��������ݼĴ���Ϊ�� */
}
/****************** ����8λ������ ************************/
void USARTx_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num)
{
  static uint8_t i;
	for(i=0; i<num; i++)
  {
	  USARTx_SendByte(pUSARTx,array[i]);                              /* ����һ���ֽ����ݵ�USART */
  }
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);         /* �ȴ�������� */
}
/*****************  �����ַ��� **********************/
void USARTx_SendString( USART_TypeDef * pUSARTx, char *str)
{
	static uint16_t k=0;
  do 
  {
    USARTx_SendByte( pUSARTx, *(str + k) );
    k++;
  } while(*(str + k)!='\0');
	k = 0;
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);         /* �ȴ�������� */
}
/*****************  ����һ��16λ�� **********************/
void USARTx_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	static uint8_t temp_h, temp_l;
	
	temp_h = (ch&0XFF00)>>8;                                          /* ȡ���߰�λ */
	temp_l = ch&0XFF;                                                 /* ȡ���Ͱ�λ */
	USART_SendData(pUSARTx,temp_h);                                   /* ���͸߰�λ */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);
	USART_SendData(pUSARTx,temp_l);                                   /* ���͵Ͱ�λ */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);	
}
/*****************  ����һ��32λ�� **********************/
void USARTx_SendWord( USART_TypeDef * pUSARTx, uint32_t ch)
{
	static uint8_t temp[4];
	
	temp[0] = ch >> 24;
	temp[1] = ch >> 16;
	temp[2] = ch >> 8;
	temp[3] = ch & 0XFF;
	USARTx_SendArray(pUSARTx, temp, 4);
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);
}

/**
  * @brief  ��ȡ�ض��ַ���
  * @param  pOutput: ���buf
  * @param  pInput: ����buf
  * @param  reply: ʶ������
  * @param  skew: ƫ��λ
  * @param  len: ��ȡ����
  * @retval ��
  */
void Usart_CutStr( char *pOutput, char *pInput, char *reply, char skew, char len )
{
	static char Cut_buf [24];
	static char *eCut_buf = Cut_buf;
	
	eCut_buf = strstr( pInput, reply );
	strncpy(pOutput, eCut_buf+skew, len);  
//	#ifdef DEBUG_USART
//	ForPC_Usart( "%s\r\n" ,pOutput );
//	#endif
}

/**
  * @brief  ��ʽ�����,������C���е�printf,������û���õ�C��
  * @param  USARTx: ����ͨ��,����ֻ�õ��˴���1,��USART1
  * @param  Data:   Ҫ���͵����ڵ����ݵ�ָ��
  * @param  ... :   ��������
  * @retval ��
  *                 USART_printf( USART1, "\r\n this is a demo \r\n" );
  *            		  USART_printf( USART1, "\r\n %d \r\n", i );
  *            		  USART_printf( USART1, "\r\n %s \r\n", j );
  */
void USARTx_printf ( USART_TypeDef * USARTx, char * Data, ... )
{
	const char *s;
	int d;
	char buf[16];

	va_list ap;
	va_start(ap, Data);

	while ( * Data != 0 )     // �ж��Ƿ񵽴��ַ���������
	{
		if ( * Data == 0x5c )  //'\'
		{
			switch ( *++Data )
			{
				case 'r':							          //�س���
				USART_SendData(USARTx, 0x0d);
				Data ++;
				break;

				case 'n':							          //���з�
				USART_SendData(USARTx, 0x0a);
				Data ++;
				break;

				default:
				Data ++;
				break;
			}
		}
		else if ( * Data == '%')
		{									  //
			switch ( *++Data )
			{
				case 's':										  //�ַ���
				s = va_arg(ap, const char *);
				for ( ; *s; s++)
				{
					USART_SendData(USARTx,*s);
					while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
				}
				Data++;
				break;

				case 'd':
					//ʮ����
				d = va_arg(ap, int);
				itoa(d, buf, 10);
				for (s = buf; *s; s++)
				{
					USART_SendData(USARTx,*s);
					while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
				}
				Data++;
				break;
				
				default:
				Data++;
				break;
			}
		}
		else USART_SendData(USARTx, *Data++);
		while ( USART_GetFlagStatus ( USARTx, USART_FLAG_TXE ) == RESET );
	}
}

/**
  * @brief  ������ת��Ϊ�ַ���
  * @param  value:  ��ת����������
  * @param  string: ת������ַ�������
  * @param  radix: ת������
  *         radix =10 ��ʾ10����,�������Ϊ0
  * @retval ��
  */
char * itoa( int value, char *string, char radix )
{
	int     i, d;
	int     flag = 0;
	char    *ptr = string;

	/* This implementation only works for decimal numbers. */
	if (radix != 10)
	{
		*ptr = 0;
		return string;
	}
	if (!value)
	{
		*ptr++ = 0x30;
		*ptr = 0;
		return string;
	}
	/* if this is a negative value insert the minus sign. */
	if (value < 0)
	{
		*ptr++ = '-';
		/* Make the value positive. */
		value *= -1;
	}
	for (i = 10000; i > 0; i /= 10)
	{
		d = value / i;
		if (d || flag)
		{
			*ptr++ = (char)(d + 0x30);
			value -= (d * i);
			flag = 1;
		}
	}
	/* Null terminate the string. */
	*ptr = 0;
	return string;
} /* NCL_Itoa */




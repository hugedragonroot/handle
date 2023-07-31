#ifndef __AT_USART_H
#define	__AT_USART_H


#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>  
#include <stdbool.h>


#define DEBUG_USART   /* 是否输出调试信息 */
#define ForPC_Usart( fmt, ... )     printf( fmt, ##__VA_ARGS__ )
#define DEBUG_USARTx      USART2

#define StreamLen         128
#define StreamBaudRate    256000

/* 传输端口 */
typedef enum {
  UCOM1 = 0x01,
  UCOM2 = 0x02,
  UCOM3 = 0x03,
  UCOM4 = 0x04,
  UCOM5 = 0x05,
} UART_COM;
/* 串口收发缓存结构体 */
typedef struct
{
  bool    TxEN;
  bool    RxEN;
  uint8_t TxCNT;
  uint8_t RxCNT;
  uint8_t TxBuf[StreamLen];
  uint8_t RxBuf[StreamLen];
} UsartStreamDef;

extern UsartStreamDef  UsartStr1;
extern UsartStreamDef  UsartStr2;
extern UsartStreamDef  UsartStr3;

void USART1_DMA_Send(uint8_t *array, uint16_t num);
void USART2_DMA_Send(uint8_t *array, uint16_t num);
void USART3_DMA_Send(uint8_t *array, uint16_t num);
void AT_Equipment_Init(void);
char * itoa( int value, char *string, char radix );
void USARTx_printf( USART_TypeDef * USARTx, char * Data, ... );
void USARTx_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);/* 发送一个字节 */
void USARTx_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num);/* 发送8位的数组 */
void USARTx_SendString( USART_TypeDef * pUSARTx, char *str);/* 发送字符串 */
void USARTx_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);/* 发送一个16位数 */
void USARTx_SendWord( USART_TypeDef * pUSARTx, uint32_t ch);

#endif /* __AT_USART_H */

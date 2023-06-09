#ifndef __UART_EXT_H
#define __UART_EXT_H

#include "stdint.h"
#include "app_loopqueue.h"

#define USART_RECV_LEN		300
#define USART_SEND_LEN		50

extern uint8_t WIRE_RX_BUF[USART_RECV_LEN];  	//有线-接收缓冲
extern uint8_t BT_RX_BUF[USART_RECV_LEN];  	//无线-接收缓冲

extern TLoopBuf loop_debug_uart;
extern TLoopBuf loop_bt_uart;
extern TLoopBuf loop_wire_uart;

void uart_init(void);
void UART_Receive_IT(void);;
void uart_bt_sendbyte(uint8_t ch);
void uart_wire_send(uint8_t *array, uint16_t len);
void uart_bt_send(uint8_t *array, uint16_t len);
void uart_music_send(uint8_t *array, uint16_t len);
void uart_gps_send(uint8_t *array, uint16_t len);
int WireGetDataWithTimout(uint8_t *c);
#endif



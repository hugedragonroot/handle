#ifndef __CAN_EXT_H__
#define __CAN_EXT_H__
#include "gd32f30x.h"
#include "gd32f30x_can.h"
// #include "common.h"
#if 0
//手柄作为从机
#define CAN_SEND_ID 0X500
#define CAN_RECEIVE_ID 0X580
#else
//手柄作为主机
#define CAN_SEND_ID 0X580
#define CAN_RECEIVE_ID 0X500
#endif

#define CAN_BUFFER_SIZE   (256)

typedef struct
{
    uint8_t *data;
    uint16_t read_index, write_index;
    uint16_t size;
} Can_fifo_buffer_t;

void can_myinit(void);
uint8_t CanSend(uint32_t id, uint8_t *data, uint8_t size);
//void bsp_can_send(u32 id,uint8_t* buffer, uint16_t length);

uint16_t can_serial_available(void);
uint8_t can_serial_read_char(void);
uint16_t can_serial_write(uint8_t *buffer, uint16_t length); //·¢ËÍ
uint16_t can_serial_read(uint8_t *buffer, uint16_t length);  //½ÓÊÕ
void can_transmit(void);
void can_receive(void);

int CANGetDataWithTimeOut(void);


#endif



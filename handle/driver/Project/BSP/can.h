/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    can.h
  * @brief   This file contains all the function prototypes for
  *          the can.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CAN_H__
#define __CAN_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "common.h"
#define CAN_SEND_ID 0X500
#define CAN_RECEIVE_ID 0X580

//#define CAN_SEND_ID 0X580
//#define CAN_RECEIVE_ID 0X500

#define CAN_BUFFER_SIZE   (128)
typedef struct
{
    uint8_t *data;
    uint16_t read_index, write_index;
    uint16_t size;
} Can_fifo_buffer_t;
extern rece_angle mpu_filter;
/* USER CODE END Includes */

extern CAN_HandleTypeDef hcan1;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_CAN1_Init(void);

/* USER CODE BEGIN Prototypes */
uint8_t CanSend(uint32_t id, uint8_t *data, uint8_t size);
//void bsp_can_send(u32 id,uint8_t* buffer, uint16_t length);

uint16_t can_serial_available(void);
uint8_t can_serial_read_char(void);
uint16_t can_serial_write(uint8_t *buffer, uint16_t length); //∑¢ÀÕ
uint16_t can_serial_read(uint8_t *buffer, uint16_t length);  //Ω” ’
void can_transmit(void);
extern uint8_t Car_buff[CAN_BUFFER_SIZE];
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __CAN_H__ */


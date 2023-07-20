/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
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
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "common.h"

typedef struct
{
	uint16_t length_first;
	uint16_t length_second;
	uint16_t length_third;
	uint16_t length_four;
}ultrasonic_t;

extern ultrasonic_t app_ultrasonic;
#define BUFFER_MAX 128
#define SERIAL_STATUS_IDLE  0x00
#define SERIAL_STATUS_IT    0x01
#define SERIAL_STATUS_DMA   0x02

#define RS485_EN_ON()			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET)
#define RS485_EN_OFF()	 	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET)
#define	RS485_EN_READ()		HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)
/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;

extern UART_HandleTypeDef huart3;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);
void MX_USART3_UART_Init(void);
void MX_USART3_START(void);
/* USER CODE BEGIN Prototypes */
void serial0_send(uint8_t *data,uint16_t len);
void serial3_send(void);
void serial3_update(serialx_t port);
uint8_t serial_rx_queue_empty(serialx_t port);
uint8_t serial_receive(serialx_t port);



void DypRd_Init(void);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */


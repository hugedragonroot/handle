/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   This file contains all the function prototypes for
  *          the gpio.c file
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
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "common.h"
/* USER CODE BEGIN Includes */

#define BEEP_ON()		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15,GPIO_PIN_SET)
#define BEEP_OFF()	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15,GPIO_PIN_RESET)

#define BREAK1_ON()	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8,GPIO_PIN_SET)
#define BREAK2_ON()	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15,GPIO_PIN_SET)
#define BREAK1_WRITE_READ() HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8)
#define BREAK2_WRITE_READ() HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15)
#define BREAK1_OFF()	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8,GPIO_PIN_RESET)
#define BREAK2_OFF()	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15,GPIO_PIN_RESET)
//#define BREAK1_READ_INPUT()	gpio_input_bit_get(GPIOA, GPIO_PIN_8)
//#define BREAK2_READ_INPUT()	gpio_input_bit_get(GPIOA, GPIO_PIN_15)
#define BREAK1_READ_INPUT()	HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9)
#define BREAK2_READ_INPUT()	HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_0)


#define KEY_BOTTOM_INPUT() 	 HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5)
#define KEY_TOP_INPUT()			 HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_3)
#define KEY_ORIGIN_INPUT()	 HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4)

//#define STOP_ON()	gpio_bit_set(GPIOD, GPIO_PIN_10)
//#define STOP_OFF()	gpio_bit_reset(GPIOD, GPIO_PIN_10)

#define Brush1_A_ON()	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3,GPIO_PIN_SET)
#define Brush1_B_ON()	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4,GPIO_PIN_SET)
#define Brush2_A_ON()	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_9,GPIO_PIN_SET)
#define Brush2_B_ON()	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7,GPIO_PIN_SET)
#define Brush1_A_OFF()	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3,GPIO_PIN_RESET)
#define Brush1_B_OFF()	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4,GPIO_PIN_RESET)
#define Brush2_A_OFF()	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_9,GPIO_PIN_RESET)
#define Brush2_B_OFF()	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7,GPIO_PIN_RESET)

#define MOS_ENABLE()		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1,GPIO_PIN_SET)
#define MOS_DISABLE()		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1,GPIO_PIN_RESET)
/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */


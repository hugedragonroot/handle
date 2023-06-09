/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.h
  * @brief   This file contains all the function prototypes for
  *          the adc.c file
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
#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "common.h"
/* USER CODE BEGIN Includes */
#define ADC0_SAMPLING_NUMBER  	5 		//单通道采样点数
#define ADC0_CHANNEL_NUMBER  	4 		//采样通道数

#define ADC2_SAMPLING_NUMBER  	5 		//单通道采样点数
#define ADC2_CHANNEL_NUMBER  	6 		//采样通道数


extern uint16_t adcDMAvalue_adc0[ADC0_SAMPLING_NUMBER][ADC0_CHANNEL_NUMBER];	
extern uint16_t adcDMAvalue_adc2[ADC2_SAMPLING_NUMBER][ADC2_CHANNEL_NUMBER];	
extern uint16_t ADC2_DMA[ADC2_SAMPLING_NUMBER][ADC2_CHANNEL_NUMBER];
/* USER CODE END Includes */

extern ADC_HandleTypeDef hadc1;

extern ADC_HandleTypeDef hadc3;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_ADC1_Init(void (*handler)(void *pram));
void MX_ADC3_Init(void (*handler)(void *pram));

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */


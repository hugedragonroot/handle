/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.h
  * @brief   This file contains all the function prototypes for
  *          the tim.c file
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
#ifndef __TIM_H__
#define __TIM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "common.h"
/* USER CODE BEGIN Includes */
#define CKTIM	            168000000                     //系统时钟频率,72MHz
#define PWM_PRSC          0                          //分频
#define PWM_FREQ          40000                         //单位: kHz  20

//PWM周期
#define PWM_PERIOD_U          ((uint16_t)(CKTIM / PWM_FREQ / (PWM_PRSC + 1) - 1))   
#define DCMOTOR_DUTY_FULL_U 	PWM_PERIOD_U    //跟定时器的PWM发波有关


#define PWM_FRQ		20000

#define PWM_PERIOD_A          ((uint16_t)(CKTIM / PWM_FRQ / (PWM_PRSC + 1) - 1))   
#define DCMOTOR_DUTY_FULL_A 	PWM_PERIOD_A    //跟定时器的PWM发波有关
#define FullPeriodCnt (uint16_t)(CKTIM/2/PWM_FRQ)
#define DEADTIME_NS 2300
#define DEADTIME (uint16_t)(168/2*DEADTIME_NS/1000)
#define DEADTIME_NS_BRUSH 600
#define DEADTIME_BRUSH (uint16_t)(168/2*DEADTIME_NS_BRUSH/1000)

/* USER CODE END Includes */
extern volatile TimerInputCaptureSpeed_t BrushSpeed[2];

extern TIM_HandleTypeDef htim1;

extern TIM_HandleTypeDef htim2;

extern TIM_HandleTypeDef htim3;

extern TIM_HandleTypeDef htim4;

extern TIM_HandleTypeDef htim6;

extern TIM_HandleTypeDef htim7;

extern TIM_HandleTypeDef htim8;

extern TIM_HandleTypeDef htim9;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_TIM1_Init(void);
void MX_TIM2_Init(void);
void MX_TIM3_Init(void);
void MX_TIM4_Init(void);
void MX_TIM6_Init(void);
void MX_TIM7_Init(void);
void MX_TIM8_Init(void);
void MX_TIM9_Init(void);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* USER CODE BEGIN Prototypes */
#define BrushA_PWM_A(value) __HAL_TIM_SET_COMPARE(&htim9, TIM_CHANNEL_1, value)
#define BrushA_PWM_B(value) __HAL_TIM_SET_COMPARE(&htim9, TIM_CHANNEL_2, value)

#define BrushU_PWM_A(value) __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, value)
#define BrushU_PWM_B(value) __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, value)

#define PMSM_A_START() __HAL_TIM_ENABLE(&htim1)
#define PMSM_U_START() __HAL_TIM_ENABLE(&htim8)

#define PMSM_A_STOP() __HAL_TIM_DISABLE(&htim1)
#define PMSM_U_STOP() __HAL_TIM_DISABLE(&htim8)

#define PMSM_A_CAR()	__HAL_TIM_GET_AUTORELOAD(&htim1) 
#define PMSM_U_CAR()	__HAL_TIM_GET_AUTORELOAD(&htim8) 

#define PMSM_A_PWM_CH0(value) __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,value)
#define PMSM_A_PWM_CH1(value) __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,value)
#define PMSM_A_PWM_CH2(value) __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,value)

#define PMSM_U_PWM_CH0(value) __HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_1,value)
#define PMSM_U_PWM_CH1(value) __HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_2,value)
#define PMSM_U_PWM_CH2(value) __HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_3,value)

#define Brush_A_CNT() __HAL_TIM_GET_COUNTER(&htim3)
#define Brush_U_CNT() __HAL_TIM_GET_COUNTER(&htim4)

#define Brush_A_DIR ((htim3.Instance->CR1)&0x10)
#define Brush_U_DIR ((htim4.Instance->CR1)&0x10)

#define Brush1_A_PWM_READ() __HAL_TIM_GET_COMPARE(&htim9,TIM_CHANNEL_1)
#define Brush1_B_PWM_READ() __HAL_TIM_GET_COMPARE(&htim9,TIM_CHANNEL_2)

#define Brush2_A_PWM_READ() __HAL_TIM_GET_COMPARE(&htim2,TIM_CHANNEL_3)
#define Brush2_B_PWM_READ() __HAL_TIM_GET_COMPARE(&htim2,TIM_CHANNEL_4)
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */


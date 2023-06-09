/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define M3_EN2_Pin GPIO_PIN_3
#define M3_EN2_GPIO_Port GPIOE
#define M3_EN1_Pin GPIO_PIN_4
#define M3_EN1_GPIO_Port GPIOE
#define ORIGIN_Pin GPIO_PIN_4
#define ORIGIN_GPIO_Port GPIOC
#define TOP_Pin GPIO_PIN_5
#define TOP_GPIO_Port GPIOC
#define BEEP_Pin GPIO_PIN_15
#define BEEP_GPIO_Port GPIOE
#define EN485_Pin GPIO_PIN_12
#define EN485_GPIO_Port GPIOB
#define BZ1IN_Pin GPIO_PIN_9
#define BZ1IN_GPIO_Port GPIOC
#define BZ1_Pin GPIO_PIN_8
#define BZ1_GPIO_Port GPIOA
#define BZ2_Pin GPIO_PIN_15
#define BZ2_GPIO_Port GPIOA
#define BZ2IN_Pin GPIO_PIN_0
#define BZ2IN_GPIO_Port GPIOD
#define FLED1_Pin GPIO_PIN_1
#define FLED1_GPIO_Port GPIOD
#define FLED2_Pin GPIO_PIN_2
#define FLED2_GPIO_Port GPIOD
#define FLED3_Pin GPIO_PIN_3
#define FLED3_GPIO_Port GPIOD
#define M4_EN2_Pin GPIO_PIN_7
#define M4_EN2_GPIO_Port GPIOD
#define M4_EN1_Pin GPIO_PIN_9
#define M4_EN1_GPIO_Port GPIOG
#define LED1_Pin GPIO_PIN_10
#define LED1_GPIO_Port GPIOG
#define LED2_Pin GPIO_PIN_11
#define LED2_GPIO_Port GPIOG
#define SCL_Pin GPIO_PIN_6
#define SCL_GPIO_Port GPIOB
#define SDA_Pin GPIO_PIN_7
#define SDA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
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
/***********�����ͺ�**************/
#define  M130T			0 
#define  M115				1


#define USING_FLASH 1
#define USING_XY_TO_SPEED 0
#define USING_6KEY 1
#define USING_LED_POINT_DISPLAY 1
#define USING_DEBUG 1
#define USING_TASK_DEBUG 0
#define USING_RTOS 0
#define FWDGT_OPEN 0
#define BT_CHANGE_NAME_FLAG 0

#define EV1527_USER_NUM 1


#define	DRIVER_MODEL	M130T


/***********����汾��**************/
#define SOFTWARE_VER0 			0 
#define SOFTWARE_VER1 			0 
#define SOFTWARE_VER2 			3 



/***********����**************/
#define DEBUG_WAVEOUT			0 	//��ӡ�����������
#define DEBUG_MODE 				0		//�ֶ�����¥
#define DEBUG_OUT 				0		//��ӡ������Ϣ���
#define DEBUG_CMDOUT 			1 	//��ӡ�������
#define DEBUG_MAX3000			0 	//�������+-3000
#define DEBUG_BACK_DOWN		1 	//���Ժ�֧������¥
#define MEM_SAVE_EN 			0		//���籣��
#define APP_BT_TEST		 		0		//app��������
#define DRIVE_TYPE_NEW		1 	//�¾ɻ���
#define HARD_TYPE_NEW			1 	//�¾��ֱ�
#define DEBUG_MODECHANGE	0 	//����һ���л�ģʽ
/* Includes ------------------------------------------------------------------*/
#include "gd32f30x.h"
#include "ext.h"
#include "drv.h"
#include "app_protocol.h"
#include "app_display.h"
#include "app_bmp.h"
#include "app_remote_para.h"
#include "app_mem.h"
/* task*/
#include "key_task.h"
#include "display_task.h"
#include "wirelink_task.h"
#include "data_pack_task.h"
#include "BTlink_task.h"
#include "commandTx_task.h"
#include "gps_task.h"
#include "CAN_task.h"
/* FreeRtos includes */

#if USING_RTOS
#include "FreeRTOS.h"
#include "task.h"
#endif

#include "debug.h"
/*protocol*/
#include "para.h"


/* USER CODE END Private defines */
void Error_Handler(void);
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    can.c
  * @brief   This file provides code for the configuration
  *          of the CAN instances.
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
/* Includes ------------------------------------------------------------------*/
#include "can.h"

/* USER CODE BEGIN 0 */
uint8_t Car_buff[CAN_BUFFER_SIZE] = {0};
Can_fifo_buffer_t CANRx_Buffer;
uint8_t CAN_Rx_buffer[CAN_BUFFER_SIZE];

Can_fifo_buffer_t CANTx_Buffer;
uint8_t CAN_Tx_buffer[CAN_BUFFER_SIZE];
/* USER CODE END 0 */

CAN_HandleTypeDef hcan1;
void CAN_Para_Init(void)
{
	
	CANTx_Buffer.data = CAN_Tx_buffer;
	CANTx_Buffer.size = CAN_BUFFER_SIZE;
	CANTx_Buffer.read_index = 0;
	CANTx_Buffer.write_index = 0;
	
	CANRx_Buffer.data = CAN_Rx_buffer;
	CANRx_Buffer.size = CAN_BUFFER_SIZE;
	CANRx_Buffer.read_index = 0;
	CANRx_Buffer.write_index = 0;
}

static void StdCanSetID(uint8_t filterNumber, uint32_t id1,uint32_t id2)//标准帧普通过滤函数filterNumber：通道   一个通道最多支持2个id号，一共13个通道，最多只支持过滤26个id号
{

	CAN_FilterTypeDef sFilterConfig;
	
	sFilterConfig.FilterBank = 0;						//筛选器编号, CAN1是0-13, CAN2是14-27
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;	//采用掩码模式
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;	//设置筛选器的尺度, 采用32位
	id1 %= (1 << 11);
	id2 %= (1 << 11);
	sFilterConfig.FilterIdHigh = id1<<5;				//过滤器ID高16位,即CAN_FxR1寄存器的高16位
	sFilterConfig.FilterIdLow = CAN_ID_STD|CAN_RTR_DATA;					//过滤器ID低16位,即CAN_FxR1寄存器的低16位
	sFilterConfig.FilterMaskIdHigh = id2<<5;			//过滤器掩码高16位,即CAN_FxR2寄存器的高16位
	sFilterConfig.FilterMaskIdLow =  CAN_ID_STD|CAN_RTR_DATA;				//过滤器掩码低16位,即CAN_FxR2寄存器的低16位
	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO1;	//设置经过筛选后数据存储到哪个接收FIFO
	sFilterConfig.FilterActivation = ENABLE;			//是否使能本筛选器
	sFilterConfig.SlaveStartFilterBank = filterNumber+14;			//指定为CAN1分配多少个滤波器组
	
	if(HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK)
	{
		Error_Handler();
	}
}
/* CAN1 init function */
void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */
	CAN_Para_Init();
  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */
	HAL_CAN_DeInit(&hcan1);
  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 6;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_12TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_1TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = ENABLE;
  hcan1.Init.AutoWakeUp = ENABLE;
  hcan1.Init.AutoRetransmission = ENABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = ENABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */
//	StdCanSetID(1,0x590,0);
	StdCanSetID(0,CAN_RECEIVE_ID,0);


	
	
	HAL_CAN_Start(&hcan1);
  /* USER CODE END CAN1_Init 2 */
	
		if(HAL_CAN_ActivateNotification(&hcan1,CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK)//开启接受邮箱0挂起中断
	{
		Error_Handler();
	}

}

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* CAN1 clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**CAN1 GPIO Configuration
    PA11     ------> CAN1_RX
    PA12     ------> CAN1_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* CAN1 interrupt Init */
    HAL_NVIC_SetPriority(CAN1_RX1_IRQn, 0, 4);
    HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);
//		CLEAR_BIT(canHandle->Instance->MCR, CAN_MCR_SLEEP);
  /* USER CODE BEGIN CAN1_MspInit 1 */

  /* USER CODE END CAN1_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();

    /**CAN1 GPIO Configuration
    PA11     ------> CAN1_RX
    PA12     ------> CAN1_TX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11|GPIO_PIN_12);

    /* CAN1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(CAN1_RX1_IRQn);
  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

//*****************************************************************************
//原  型：uint8_t CanSend(u32 id, u8 *data, u8 size)
//功  能:  
//输  入：
//输  出：
//***************************************************************************** 
uint8_t CanSend(uint32_t id, uint8_t *data, uint8_t size)
{
	CAN_TxHeaderTypeDef TxHeader;
	uint32_t pTxMailbox = 1;
//	uint8_t  Mailbox = CAN_NOMAILBOX;
	if(id <= 0x7FF)
	{
		TxHeader.StdId = id;
		TxHeader.RTR = CAN_ID_STD;
	}
	else
	{
		TxHeader.ExtId = id;
		TxHeader.RTR = CAN_ID_EXT;
	}
	if(size > 8)
		size = 8;
//	g_sCAN_Handler[dwDevice].Init.NART  = DISABLE;//开启自动重发
	TxHeader.IDE = CAN_RTR_DATA;
	TxHeader.DLC =size;
	TxHeader.TransmitGlobalTime = DISABLE;
//	while(HAL_CAN_GetTxMailboxesFreeLevel(&hcan1)<1)
	while( HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0 );
//	HAL_CAN_Transmit();
	HAL_CAN_AddTxMessage(&hcan1,&TxHeader,data,&pTxMailbox);
	return 1;
}


//void gd32_can_error(void)
//{
//	if(	can_flag_get(CAN0, CAN_FLAG_MTE2) != RESET ||
//		can_flag_get(CAN0, CAN_FLAG_MTE0) != RESET ||
//		can_flag_get(CAN0, CAN_FLAG_MTE1) != RESET || 
//		can_flag_get(CAN0, CAN_FLAG_PERR) != RESET ||
//		can_flag_get(CAN0, CAN_FLAG_WERR) != RESET)
//	{
//		can_flag_clear(CAN0, CAN_FLAG_MTE0);
//		can_flag_clear(CAN0, CAN_FLAG_MTE1);
//		can_flag_clear(CAN0, CAN_FLAG_MTE2);
//		can_flag_clear(CAN0, CAN_FLAG_PERR);
//		can_flag_clear(CAN0, CAN_FLAG_WERR);
//		can_wakeup(CAN0);
//		bsp_can_init();
//	}
//}


/*****************************************************************************
*原  型：uint16_t can_serial_available(void)
*功  能: 从缓存区读取数据
*输  入：
*输  出：
******************************************************************************/
uint16_t can_serial_available(void)
{
    uint16_t len = 0;
	  uint16_t CANRx_Buffer_read_index = CANRx_Buffer.read_index;
	  uint16_t CANRx_Buffer_write_index = CANRx_Buffer.write_index;
	
    if (CANRx_Buffer_read_index > CANRx_Buffer_write_index)
    {
        len = CANRx_Buffer.size + CANRx_Buffer_write_index - CANRx_Buffer_read_index;
    }
    else if (CANRx_Buffer_read_index  < CANRx_Buffer_write_index)
    {
        len = CANRx_Buffer_write_index - CANRx_Buffer_read_index;
    }
    return len;
}
uint8_t can_serial_read_char(void)
{
    uint8_t ch = 0;
    ch = CANRx_Buffer.data[CANRx_Buffer.read_index];
    CANRx_Buffer.read_index = (CANRx_Buffer.read_index + 1) % CANRx_Buffer.size;
    return ch;
}
/*****************************************************************************
*原  型：uint16_t can_serial_read(uint8_t *buffer, uint16_t length)
*功  能: 从缓存区读取数据
*输  入：
*输  出：
******************************************************************************/
uint16_t can_serial_read(uint8_t *buffer, uint16_t length)
{
    uint16_t i = 0;
	
    for (i = 0; i < length; i++)
    {
        buffer[i] = CANRx_Buffer.data[CANRx_Buffer.read_index];
        CANRx_Buffer.read_index = (CANRx_Buffer.read_index + 1) % CANRx_Buffer.size;
    }
    return i;
}
/*****************************************************************************
*原  型：uint16_t can_tx_buf(uint8_t *buffer, uint16_t length, Can_fifo_buffer_t* Tx_Buffer)
*功  能: 把待发送的数据写入缓冲区
*输  入：
*输  出：
******************************************************************************/
uint16_t can_tx_buf(uint8_t *buffer, uint16_t length, Can_fifo_buffer_t* Tx_Buffer)
{
    uint16_t i = 0;
	
    for (i = 0; i < length; i++)
    {
        Tx_Buffer->data[Tx_Buffer->write_index] = buffer[i];
        Tx_Buffer->write_index = (Tx_Buffer->write_index + 1) % Tx_Buffer->size;
    }
    return i;
}
/*****************************************************************************
*原  型：uint16_t can_serial_write(uint8_t *buffer, uint16_t length)
*功  能: 把待发送的数据写入缓冲区
*输  入：
*输  出：
******************************************************************************/
uint16_t can_serial_write(uint8_t *buffer, uint16_t length)
{
    uint16_t i = 0;
	  can_tx_buf(buffer, length, &CANTx_Buffer);
    return i;
}

/*****************************************************************************
*原  型：uint16_t can_tx_available(Can_fifo_buffer_t * Tx_Buffer)
*功  能: 检测待发送数据的长度
*输  入：
*输  出：
******************************************************************************/
uint16_t can_tx_available(Can_fifo_buffer_t * Tx_Buffer)
{
    uint16_t len = 0;
    if (Tx_Buffer->read_index > Tx_Buffer->write_index)
    {
        len = Tx_Buffer->size + Tx_Buffer->write_index - Tx_Buffer->read_index;
    }
    else if (Tx_Buffer->read_index  < Tx_Buffer->write_index)
    {
        len = Tx_Buffer->write_index - Tx_Buffer->read_index;
    }
    return len;
}


/*****************************************************************************
*原  型：void can_transmit(void)
*功  能: can总线数据发出
*输  入：
*输  出：
******************************************************************************/
uint16_t tx_len1 = 0;
uint8_t TxMsg[8];
void can_transmit(void)
{  
	tx_len1 =  can_tx_available(&CANTx_Buffer);
	if(tx_len1 > 0)
	{
		
		uint16_t temp;
		if(CAN_SEND_ID != 0)
		{
			if(tx_len1 > 8)
			{
				temp = CANTx_Buffer.size - CANTx_Buffer.read_index;
				if(temp < 8)
				{
					memcpy(TxMsg,&(CANTx_Buffer.data[CANTx_Buffer.read_index]), temp);
					memcpy(&TxMsg[temp],&(CANTx_Buffer.data[0]) ,8-temp);
				}
				else
				{
					memcpy(TxMsg, &(CANTx_Buffer.data[CANTx_Buffer.read_index]), 8);
				}
				CanSend(CAN_SEND_ID, TxMsg, 8);
				CANTx_Buffer.read_index = (CANTx_Buffer.read_index + 8)%CANTx_Buffer.size;
			}
			else
			{
				temp = CANTx_Buffer.size - CANTx_Buffer.read_index;
				if(temp < tx_len1)
				{
					memcpy(TxMsg,&(CANTx_Buffer.data[CANTx_Buffer.read_index]), temp);
					memcpy(&TxMsg[temp],&(CANTx_Buffer.data[0]) ,tx_len1-temp);
				}
				else
				{
					memcpy(TxMsg,&(CANTx_Buffer.data[CANTx_Buffer.read_index]), tx_len1);
				}
				CanSend(CAN_SEND_ID, TxMsg, tx_len1);
				CANTx_Buffer.read_index = (CANTx_Buffer.read_index + tx_len1)%CANTx_Buffer.size;
			}
		}
	}
	
}

rece_angle mpu_angle;
rece_angle mpu_filter;
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	uint8_t i = 0;

	uint8_t buf[8]={0};
	if(hcan == &hcan1)
	{
		CAN_RxHeaderTypeDef RXHeader;
		HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO1, &RXHeader, buf);
		if(RXHeader.RTR == CAN_ID_STD)
		{
			if(RXHeader.StdId == CAN_RECEIVE_ID)
			{
				 for(i=0;i<RXHeader.DLC;i++)
				{
					CANRx_Buffer.data[CANRx_Buffer.write_index] = buf[i];	
					CANRx_Buffer.write_index = (CANRx_Buffer.write_index + 1) % CANRx_Buffer.size;
				}	
			}
			else if(RXHeader.StdId == 0x590)
			{
				memcpy((uint8_t *)&mpu_angle,&buf[0],8);
				mpu_filter.ptich = mpu_filter.ptich*0.9f + -mpu_angle.ptich*0.1f;
				mpu_filter.roll = mpu_filter.roll*0.9f + mpu_angle.roll*0.1f;
			}
		}
	}
}


/* USER CODE END 1 */

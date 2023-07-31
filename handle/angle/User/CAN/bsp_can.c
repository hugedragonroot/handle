/**
  ******************************************************************************
  * @file    bsp_debug_usart.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   can驱动（回环模式/双机通讯）
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F103-霸道 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 

#include ".\can\bsp_can.h"
#include "app_loopqueue.h"


CanStreamDef  CanStr1;
CanTxMsg      TxMsgFIFO;  /* 发送缓存 */
CanRxMsg      RxMsgFMP0;  /* FIFO0接收缓存 */
CanRxMsg      RxMsgFMP1;  /* FIFO1接收缓存 */

static uint8_t loopBufCAN1[MsgLen];


/**
  * @brief  配置CAN的GPIO
  * @param  无
  * @retval 无
  */
static void CAN_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | CAN_TX_GPIO_CLK | CAN_RX_GPIO_CLK, ENABLE);
  #if CAN_PinRemap
  GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);  /* 重映射引脚 */
  #endif
  /* Configure CAN TX pins */
  GPIO_InitStructure.GPIO_Pin   = CAN_TX_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(CAN_TX_GPIO_PORT, &GPIO_InitStructure);
  /* Configure CAN RX  pins */
  GPIO_InitStructure.GPIO_Pin   = CAN_RX_PIN ;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(CAN_RX_GPIO_PORT, &GPIO_InitStructure);
}

/**
  * @brief  配置CAN通信接收嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
static void CAN_NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  /* CAN_FIFO0 Receive Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel                    = USB_LP_CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority         = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  /* CAN_FIFO1 Receive Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel                    = CAN1_RX1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority         = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  配置CAN通信模式
  * @param  无
  * @retval 无
  */
static void CAN_Mode_Config(void)
{
  CAN_InitTypeDef CAN_InitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
  CAN_DeInit(CAN1);   /* CAN寄存器初始化 */
  CAN_StructInit(&CAN_InitStructure);
  /* CAN单元初始化 */
  CAN_InitStructure.CAN_TTCM      = DISABLE;            /* MCR-TTCM  关闭时间触发通信模式使能 */
  CAN_InitStructure.CAN_ABOM      = ENABLE;             /* MCR-ABOM  自动离线管理 */
  CAN_InitStructure.CAN_AWUM      = ENABLE;             /* MCR-AWUM  使用自动唤醒模式 */
  CAN_InitStructure.CAN_NART      = DISABLE;            /* MCR-NART  禁止报文自动重传	  DISABLE-自动重传 */
  CAN_InitStructure.CAN_RFLM      = DISABLE;            /* MCR-RFLM  接收FIFO 锁定模式  DISABLE-溢出时新报文会覆盖原有报文 */
  CAN_InitStructure.CAN_TXFP      = DISABLE;            /* MCR-TXFP  发送FIFO优先级 DISABLE-优先级取决于报文标示符 */
  CAN_InitStructure.CAN_Mode      = CAN_Mode_Normal;    /* 正常工作模式 */
//CAN_InitStructure.CAN_Mode      = CAN_Mode_LoopBack;  /* 回环工作模式 */
  /* ss=1 bs1=5 bs2=3 位时间宽度为(1+5+3) 波特率即为时钟周期tq*(1+3+5) */
  CAN_InitStructure.CAN_SJW       = CAN_SJW_1tq;        /* BTR-SJW 重新同步跳跃宽度 1个时间单元 */
  CAN_InitStructure.CAN_BS1       = CAN_BS1_5tq;        /* BTR-TS1 时间段1 占用了5个时间单元 */
  CAN_InitStructure.CAN_BS2       = CAN_BS2_3tq;        /* BTR-TS1 时间段2 占用了3个时间单元 */
  /* CAN Baudrate = 1 MBps (1MBps已为stm32的CAN最高速率) (CAN 时钟频率为 APB1 = 36 MHz) */
  CAN_InitStructure.CAN_Prescaler = 8;                  /* BTR-BRP 波特率分频器  定义了时间单元的时间长度 36/(1+5+3)/8 = 500kbps */
  CAN_Init(CAN1, &CAN_InitStructure);
}

/**
  * @brief  配置CAN接收筛选器
  * @param  无
  * @retval 无
  */
static void CAN_Filter_Config(void)
{
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;
  /* CAN筛选器0初始化,筛选组x和FIFOx无关联,两设备不要用相同地址 */
  CAN_FilterInitStructure.CAN_FilterNumber          = 0;                      /* 筛选器组0 */
  CAN_FilterInitStructure.CAN_FilterMode            = CAN_FilterMode_IdMask;  /* 工作在掩码模式 */
  CAN_FilterInitStructure.CAN_FilterScale           = CAN_FilterScale_32bit;  /* 筛选器位宽为单个32位 */
  /* 使能筛选器,按照标志的内容进行比对筛选,扩展ID不是如下的就抛弃掉,是的话存入FIFO0 */
  /* 一个32位过滤器,包括:STD[10:0],EXTID[17:0],IDE和RTR位 */
  /* 两个16位过滤器,包括:STD[10:0],IDE,RTR和EXTID[17:15]位 */
  CAN_FilterInitStructure.CAN_FilterIdHigh          = ScreenExtId0 >> 16;     /* 要筛选的ID高位 */
  CAN_FilterInitStructure.CAN_FilterIdLow           = ScreenExtId0 & 0xFFFF;  /* 要筛选的ID低位 */
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh      = 0xFFE0;                 /* 筛选器高16位 */
  CAN_FilterInitStructure.CAN_FilterMaskIdLow       = 0x0000;                 /* 筛选器低16位 */
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment  = CAN_Filter_FIFO0;       /* 筛选器被关联到FIFO0 */
  CAN_FilterInitStructure.CAN_FilterActivation      = ENABLE;                 /* 使能筛选器 */
  CAN_FilterInit(&CAN_FilterInitStructure);
  /* CAN筛选器1初始化 */
  CAN_FilterInitStructure.CAN_FilterNumber          = 1;
  CAN_FilterInitStructure.CAN_FilterMode            = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale           = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh          = ScreenExtId1 >> 16;
  CAN_FilterInitStructure.CAN_FilterIdLow           = ScreenExtId1 & 0xFFFF;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh      = 0xFFE0;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow       = 0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment  = CAN_Filter_FIFO0 ;
  CAN_FilterInitStructure.CAN_FilterActivation      = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);
  /* CAN筛选器2初始化 */
  CAN_FilterInitStructure.CAN_FilterNumber          = 2;
  CAN_FilterInitStructure.CAN_FilterMode            = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale           = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh          = ScreenExtId2 >> 16;
  CAN_FilterInitStructure.CAN_FilterIdLow           = ScreenExtId2 & 0xFFFF;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh      = 0xFFE0;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow       = 0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment  = CAN_Filter_FIFO1 ;
  CAN_FilterInitStructure.CAN_FilterActivation      = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);
  /* CAN筛选器3初始化 */
  CAN_FilterInitStructure.CAN_FilterNumber          = 3;
  CAN_FilterInitStructure.CAN_FilterMode            = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale           = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh          = ScreenExtId3 >> 16;
  CAN_FilterInitStructure.CAN_FilterIdLow           = ScreenExtId3 & 0xFFFF;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh      = 0xFFE0;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow       = 0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment  = CAN_Filter_FIFO1 ;
  CAN_FilterInitStructure.CAN_FilterActivation      = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);
  /* CAN通信FIFO接收中断使能 */
  CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
  CAN_ITConfig(CAN1, CAN_IT_FMP1, ENABLE);
}

/**
  * @brief  CAN初始化
  * @param  无
  * @retval 无
  */
void CAN_Config(void)
{
  LoopQueue_Init(&loopPointCAN1, loopBufCAN1, sizeof(loopBufCAN1));
  CAN_GPIO_Config();
  CAN_NVIC_Config();
  CAN_Mode_Config();
  CAN_Filter_Config();   
}

/**
  * @brief  初始化 Rx Message数据结构体
  * @param  RxMessage: 指向要初始化的数据结构体
  * @retval None
  */
void Init_RxMes(CanRxMsg *RxMessage)
{
  /* 把接收结构体清零 */
  RxMessage->StdId    = 0x00;
  RxMessage->ExtId    = 0x00;
  RxMessage->IDE      = CAN_ID_STD;
  RxMessage->DLC      = 0;
  RxMessage->FMI      = 0;
  RxMessage->Data[0]  = 0x00;
  RxMessage->Data[1]  = 0x00;
  RxMessage->Data[2]  = 0x00;
  RxMessage->Data[3]  = 0x00;
  RxMessage->Data[4]  = 0x00;
  RxMessage->Data[5]  = 0x00;
  RxMessage->Data[6]  = 0x00;
  RxMessage->Data[7]  = 0x00;
}

/**
  * @brief  CAN1发送标准数据帧
  * @param  无
  * @retval 无
  */
void CAN1_SendStdFrame(CanTxMsg* TxMessage, uint32_t CanID, uint8_t *buf, uint8_t num)
{
  static uint8_t i;
  TxMessage->StdId  = CanID;          /* 使用标准ID */
  TxMessage->IDE    = CAN_ID_STD;     /* 标准模式 */
  TxMessage->RTR    = CAN_RTR_DATA;   /* 发送的是数据 */
  TxMessage->DLC    = num;            /* 数据长度 */
  for(i=0; i<num; i++) {              /* 设置要发送的数据0-7 */
    TxMessage->Data[i] = buf[i];
  }
  CAN_Transmit(CAN1, TxMessage);
}

/**
  * @brief  CAN1发送扩展数据帧
  * @param  无
  * @retval 无
  */
void CAN1_SendExtFrame(CanTxMsg* TxMessage, uint32_t CanID, uint8_t *buf, uint8_t num)
{
  static uint8_t i;
  TxMessage->ExtId  = CanID;          /* 使用的扩展ID */
  TxMessage->IDE    = CAN_ID_EXT;     /* 扩展模式 */
  TxMessage->RTR    = CAN_RTR_DATA;   /* 发送的是数据 */
  TxMessage->DLC    = num;            /* 数据长度 */
  for(i=0; i<num; i++) {              /* 设置要发送的数据0-7 */
    TxMessage->Data[i] = buf[i];
  }
  CAN_Transmit(CAN1, TxMessage);
}

/**
  * @brief  CAN扩展协议转换
  * @param  无
  * @retval 无
  */
void CAN_ProtocolExt(CanRxMsg* RxMessage, CanStreamDef *CanStrx)
{
  CanStrx->RxBuf[0] = 0xA5;                   /* 帧头 */
  CanStrx->RxBuf[1] = 0x5E;
  CanStrx->RxBuf[2] = RxMessage->DLC + 4;     /* 数据长度 */
  CanStrx->RxBuf[3] = 0x00;                   /* 计数位 */
  CanStrx->RxBuf[4] = RxMessage->ExtId&0xff;  /* 命令位 */
  for(CanStrx->RxCNT=0; CanStrx->RxCNT<RxMessage->DLC; CanStrx->RxCNT++) {
    CanStrx->RxBuf[CanStrx->RxCNT+5] = RxMessage->Data[CanStrx->RxCNT];
  }
  CanStrx->RxBuf[CanStrx->RxCNT+5] = (RxMessage->ExtId >> 24)&0xff;
  CanStrx->RxBuf[CanStrx->RxCNT+6] = (RxMessage->ExtId >> 16)&0xff;
  CanStrx->RxBuf[CanStrx->RxCNT+7] = (RxMessage->ExtId >> 8)&0xff;
  CanStrx->RxBuf[CanStrx->RxCNT+8] = (RxMessage->ExtId >> 0)&0xff;
  CanStrx->RxBuf[CanStrx->RxCNT+9] = 0x5A;    /* 帧尾 */
  CanStrx->RxCNT = RxMessage->DLC + 10;
}

/**************************END OF FILE************************************/

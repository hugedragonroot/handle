/**
  ******************************************************************************
  * @file    bsp_debug_usart.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   can�������ػ�ģʽ/˫��ͨѶ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F103-�Ե� ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 

#include ".\can\bsp_can.h"
#include "app_loopqueue.h"


CanStreamDef  CanStr1;
CanTxMsg      TxMsgFIFO;  /* ���ͻ��� */
CanRxMsg      RxMsgFMP0;  /* FIFO0���ջ��� */
CanRxMsg      RxMsgFMP1;  /* FIFO1���ջ��� */

static uint8_t loopBufCAN1[MsgLen];


/**
  * @brief  ����CAN��GPIO
  * @param  ��
  * @retval ��
  */
static void CAN_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | CAN_TX_GPIO_CLK | CAN_RX_GPIO_CLK, ENABLE);
  #if CAN_PinRemap
  GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);  /* ��ӳ������ */
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
  * @brief  ����CANͨ�Ž���Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
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
  * @brief  ����CANͨ��ģʽ
  * @param  ��
  * @retval ��
  */
static void CAN_Mode_Config(void)
{
  CAN_InitTypeDef CAN_InitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
  CAN_DeInit(CAN1);   /* CAN�Ĵ�����ʼ�� */
  CAN_StructInit(&CAN_InitStructure);
  /* CAN��Ԫ��ʼ�� */
  CAN_InitStructure.CAN_TTCM      = DISABLE;            /* MCR-TTCM  �ر�ʱ�䴥��ͨ��ģʽʹ�� */
  CAN_InitStructure.CAN_ABOM      = ENABLE;             /* MCR-ABOM  �Զ����߹��� */
  CAN_InitStructure.CAN_AWUM      = ENABLE;             /* MCR-AWUM  ʹ���Զ�����ģʽ */
  CAN_InitStructure.CAN_NART      = DISABLE;            /* MCR-NART  ��ֹ�����Զ��ش�	  DISABLE-�Զ��ش� */
  CAN_InitStructure.CAN_RFLM      = DISABLE;            /* MCR-RFLM  ����FIFO ����ģʽ  DISABLE-���ʱ�±��ĻḲ��ԭ�б��� */
  CAN_InitStructure.CAN_TXFP      = DISABLE;            /* MCR-TXFP  ����FIFO���ȼ� DISABLE-���ȼ�ȡ���ڱ��ı�ʾ�� */
  CAN_InitStructure.CAN_Mode      = CAN_Mode_Normal;    /* ��������ģʽ */
//CAN_InitStructure.CAN_Mode      = CAN_Mode_LoopBack;  /* �ػ�����ģʽ */
  /* ss=1 bs1=5 bs2=3 λʱ����Ϊ(1+5+3) �����ʼ�Ϊʱ������tq*(1+3+5) */
  CAN_InitStructure.CAN_SJW       = CAN_SJW_1tq;        /* BTR-SJW ����ͬ����Ծ��� 1��ʱ�䵥Ԫ */
  CAN_InitStructure.CAN_BS1       = CAN_BS1_5tq;        /* BTR-TS1 ʱ���1 ռ����5��ʱ�䵥Ԫ */
  CAN_InitStructure.CAN_BS2       = CAN_BS2_3tq;        /* BTR-TS1 ʱ���2 ռ����3��ʱ�䵥Ԫ */
  /* CAN Baudrate = 1 MBps (1MBps��Ϊstm32��CAN�������) (CAN ʱ��Ƶ��Ϊ APB1 = 36 MHz) */
  CAN_InitStructure.CAN_Prescaler = 8;                  /* BTR-BRP �����ʷ�Ƶ��  ������ʱ�䵥Ԫ��ʱ�䳤�� 36/(1+5+3)/8 = 500kbps */
  CAN_Init(CAN1, &CAN_InitStructure);
}

/**
  * @brief  ����CAN����ɸѡ��
  * @param  ��
  * @retval ��
  */
static void CAN_Filter_Config(void)
{
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;
  /* CANɸѡ��0��ʼ��,ɸѡ��x��FIFOx�޹���,���豸��Ҫ����ͬ��ַ */
  CAN_FilterInitStructure.CAN_FilterNumber          = 0;                      /* ɸѡ����0 */
  CAN_FilterInitStructure.CAN_FilterMode            = CAN_FilterMode_IdMask;  /* ����������ģʽ */
  CAN_FilterInitStructure.CAN_FilterScale           = CAN_FilterScale_32bit;  /* ɸѡ��λ��Ϊ����32λ */
  /* ʹ��ɸѡ��,���ձ�־�����ݽ��бȶ�ɸѡ,��չID�������µľ�������,�ǵĻ�����FIFO0 */
  /* һ��32λ������,����:STD[10:0],EXTID[17:0],IDE��RTRλ */
  /* ����16λ������,����:STD[10:0],IDE,RTR��EXTID[17:15]λ */
  CAN_FilterInitStructure.CAN_FilterIdHigh          = ScreenExtId0 >> 16;     /* Ҫɸѡ��ID��λ */
  CAN_FilterInitStructure.CAN_FilterIdLow           = ScreenExtId0 & 0xFFFF;  /* Ҫɸѡ��ID��λ */
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh      = 0xFFE0;                 /* ɸѡ����16λ */
  CAN_FilterInitStructure.CAN_FilterMaskIdLow       = 0x0000;                 /* ɸѡ����16λ */
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment  = CAN_Filter_FIFO0;       /* ɸѡ����������FIFO0 */
  CAN_FilterInitStructure.CAN_FilterActivation      = ENABLE;                 /* ʹ��ɸѡ�� */
  CAN_FilterInit(&CAN_FilterInitStructure);
  /* CANɸѡ��1��ʼ�� */
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
  /* CANɸѡ��2��ʼ�� */
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
  /* CANɸѡ��3��ʼ�� */
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
  /* CANͨ��FIFO�����ж�ʹ�� */
  CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
  CAN_ITConfig(CAN1, CAN_IT_FMP1, ENABLE);
}

/**
  * @brief  CAN��ʼ��
  * @param  ��
  * @retval ��
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
  * @brief  ��ʼ�� Rx Message���ݽṹ��
  * @param  RxMessage: ָ��Ҫ��ʼ�������ݽṹ��
  * @retval None
  */
void Init_RxMes(CanRxMsg *RxMessage)
{
  /* �ѽ��սṹ������ */
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
  * @brief  CAN1���ͱ�׼����֡
  * @param  ��
  * @retval ��
  */
void CAN1_SendStdFrame(CanTxMsg* TxMessage, uint32_t CanID, uint8_t *buf, uint8_t num)
{
  static uint8_t i;
  TxMessage->StdId  = CanID;          /* ʹ�ñ�׼ID */
  TxMessage->IDE    = CAN_ID_STD;     /* ��׼ģʽ */
  TxMessage->RTR    = CAN_RTR_DATA;   /* ���͵������� */
  TxMessage->DLC    = num;            /* ���ݳ��� */
  for(i=0; i<num; i++) {              /* ����Ҫ���͵�����0-7 */
    TxMessage->Data[i] = buf[i];
  }
  CAN_Transmit(CAN1, TxMessage);
}

/**
  * @brief  CAN1������չ����֡
  * @param  ��
  * @retval ��
  */
void CAN1_SendExtFrame(CanTxMsg* TxMessage, uint32_t CanID, uint8_t *buf, uint8_t num)
{
  static uint8_t i;
  TxMessage->ExtId  = CanID;          /* ʹ�õ���չID */
  TxMessage->IDE    = CAN_ID_EXT;     /* ��չģʽ */
  TxMessage->RTR    = CAN_RTR_DATA;   /* ���͵������� */
  TxMessage->DLC    = num;            /* ���ݳ��� */
  for(i=0; i<num; i++) {              /* ����Ҫ���͵�����0-7 */
    TxMessage->Data[i] = buf[i];
  }
  CAN_Transmit(CAN1, TxMessage);
}

/**
  * @brief  CAN��չЭ��ת��
  * @param  ��
  * @retval ��
  */
void CAN_ProtocolExt(CanRxMsg* RxMessage, CanStreamDef *CanStrx)
{
  CanStrx->RxBuf[0] = 0xA5;                   /* ֡ͷ */
  CanStrx->RxBuf[1] = 0x5E;
  CanStrx->RxBuf[2] = RxMessage->DLC + 4;     /* ���ݳ��� */
  CanStrx->RxBuf[3] = 0x00;                   /* ����λ */
  CanStrx->RxBuf[4] = RxMessage->ExtId&0xff;  /* ����λ */
  for(CanStrx->RxCNT=0; CanStrx->RxCNT<RxMessage->DLC; CanStrx->RxCNT++) {
    CanStrx->RxBuf[CanStrx->RxCNT+5] = RxMessage->Data[CanStrx->RxCNT];
  }
  CanStrx->RxBuf[CanStrx->RxCNT+5] = (RxMessage->ExtId >> 24)&0xff;
  CanStrx->RxBuf[CanStrx->RxCNT+6] = (RxMessage->ExtId >> 16)&0xff;
  CanStrx->RxBuf[CanStrx->RxCNT+7] = (RxMessage->ExtId >> 8)&0xff;
  CanStrx->RxBuf[CanStrx->RxCNT+8] = (RxMessage->ExtId >> 0)&0xff;
  CanStrx->RxBuf[CanStrx->RxCNT+9] = 0x5A;    /* ֡β */
  CanStrx->RxCNT = RxMessage->DLC + 10;
}

/**************************END OF FILE************************************/

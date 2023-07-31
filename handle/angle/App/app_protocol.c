#include "app_protocol.h"
#include "main.h"
#include <string.h>


#define SlaveCOM      CCOM1

TcomData              CommunDataUp;     /* ������ >> ���� */
TcomData              CommunDataDown;   /* ���� >> ������ */
TREMOTE_COM           Remote;           /* ң��ͨ�� */
TREMOTE_COM           Slave;            /* �ӻ�ͨ�� */

/**
  * @brief  ����У���
  * @param  ��
  * @retval ��
  */
uint8_t ProtocolCheckSum(uint8_t *pbuf, uint16_t len)
{
	static uint16_t sum, i;
  sum = 0;
	for (i=0; i<len; i++) sum += pbuf[i];
	return (uint8_t)(sum&0xff);
}

/**
  * @brief  ��֡�������
  * @param  ��
  * @retval ��
  */
static void ProtocolReplyUp(uint8_t UCOM, uint8_t cmd, uint8_t data_len)
{
  CommunDataUp.Commun.HeadH = FrameHeadUpH;
  CommunDataUp.Commun.HeadL = FrameHeadUpL;
  CommunDataUp.Commun.DataLen = data_len;
  CommunDataUp.Commun.Count ++;
  CommunDataUp.Commun.Cmd = cmd;
  CommunDataUp.Data[data_len + Frame_InforLen - 1] = ProtocolCheckSum(CommunDataUp.Data, data_len + Frame_InforLen - 1);
  if(UCOM == UCOM2)       USART2_DMA_Send(CommunDataUp.Data, data_len + Frame_InforLen);
  else if(UCOM == UCOM3)  USART3_DMA_Send(CommunDataUp.Data, data_len + Frame_InforLen);
//else if(UCOM == CCOM1) {
//  CAN1_SendExtFrame(&TxMsgFIFO, ((0xA2<<18)|(0xD4<<8)|0x07), CommunDataUp.Commun.Data, 8);
////USART1_DMA_Send(CommunDataUp.Data, data_len + Frame_InforLen);
//}
}

/**
  * @brief  ˢ�¹�����Ϣ
  * @param  ��
  * @retval ��
  */
void ProtocolFaultEvent(uint8_t state)
{
  CommunDataUp.Commun.Data[0] = state;
  CommunDataUp.Commun.Data[1] = 0x00;
  ProtocolReplyUp(UCOM2, CMD_FaultEvent, 2);
}

uint8_t souc_sum = 0, des_sum = 0;
/**
  * @brief  ����֡Э��(����->������)
  * @param  ��
  * @retval ����ͨ��
  */
static bool ProtocolAnalysis(uint8_t *pbuf, uint8_t len)
{
  memcpy(&CommunDataDown, pbuf, len);   /* ���ݽṹ�廯 */
  if(pbuf[len-1] < CCOM1) {
    souc_sum = pbuf[len - 2];             /* ���һλ��COM�˿�,�����ڶ�λ��У��� */
    des_sum = ProtocolCheckSum(pbuf, len - 2);
    if(souc_sum != des_sum) return false;
  } else {
    CommunDataUp.Commun.SourceAddr = ((pbuf[len-5]<<8) | pbuf[len-4]) & 0x3ff;
  }
  switch(CommunDataDown.Commun.Cmd)
  {
    case CMD_Handshake:         /* 0x01 ����Э�� */
      CommunDataUp.Commun.Data[0] = PARAMS_DriverModel;
      CommunDataUp.Commun.Data[1] = PARAMS_SoftwareVer / 10;
      CommunDataUp.Commun.Data[2] = PARAMS_SoftwareVer % 10;
      ProtocolReplyUp(CommunDataDown.Commun.Data[len - Frame_InforLen], CommunDataDown.Commun.Cmd, 3);
      break;
      
    case CMD_SetCtrl:           /* 0x06 �������� */
      Remote.RemoteConnect  = true;
      Remote.RemoteCount    = 0;
      Remote.CoordXH        = CommunDataDown.Commun.Data[0];  /* X �����8λ */
      Remote.CoordXL        = CommunDataDown.Commun.Data[1];  /* X �����8λ */
      Remote.CoordYH        = CommunDataDown.Commun.Data[2];  /* Y �����8λ */
      Remote.CoordYL        = CommunDataDown.Commun.Data[3];  /* Y �����8λ */
      Remote.ControlMode    = CommunDataDown.Commun.Data[4];  /* ����ģʽ */
      Remote.OperationMode  = CommunDataDown.Commun.Data[5];  /* ���з�ʽ */
      Remote.SpeedGear      = CommunDataDown.Commun.Data[6];  /* �ٶȵ�λ */
      Remote.Peripherals    = CommunDataDown.Commun.Data[7];  /* ����״̬ */
      Remote.Reserved       = CommunDataDown.Commun.Data[8];  /* ����λ */
      Remote.CoordX         = ((int16_t)(Remote.CoordXH << 8) | Remote.CoordXL) - Coord_Base;
      Remote.CoordY         = ((int16_t)(Remote.CoordYH << 8) | Remote.CoordYL) - Coord_Base;
      /* �ش� */
      CommunDataUp.Commun.Data[0] = Remote.ControlMode;
      CommunDataUp.Commun.Data[1] = Remote.OperationMode;
      CommunDataUp.Commun.Data[2] = Remote.SpeedGear;
      CommunDataUp.Commun.Data[3] = Remote.Peripherals;
      CommunDataUp.Commun.Data[4] = Remote.Battery;           /* ��ص��� */
      CommunDataUp.Commun.Data[5] = 0x00;
      CommunDataUp.Commun.Data[6] = 0x00;
      ProtocolReplyUp(CommunDataDown.Commun.Data[len - Frame_InforLen], CommunDataDown.Commun.Cmd, 7);
      /* �������ӻ� */
      Remote.Transmit = CMD_SetCtrl;
      break;
      
    case CMD_GetAngle:          /* 0x11    ��ȡ�Ƕ����� */
      MotoU.Pitch = CommunDataDown.Commun.Data[1] << 8 | CommunDataDown.Commun.Data[0];
      MotoU.Roll  = CommunDataDown.Commun.Data[3] << 8 | CommunDataDown.Commun.Data[2];
      MotoU.pTemp = CommunDataDown.Commun.Data[5] << 8 | CommunDataDown.Commun.Data[4];
      MotoU.Vbus  = CommunDataDown.Commun.Data[7] << 8 | CommunDataDown.Commun.Data[6];
      /* �ش� */
      CommunDataUp.Commun.Data[0] = MotoA.Pitch & 0xff;      /* ������ */
      CommunDataUp.Commun.Data[1] = MotoA.Pitch >> 8;
      CommunDataUp.Commun.Data[2] = MotoA.Roll & 0xff;       /* ����� */
      CommunDataUp.Commun.Data[3] = MotoA.Roll >> 8;
      CommunDataUp.Commun.Data[4] = 0x00;
      CommunDataUp.Commun.Data[5] = 0x00;
      CommunDataUp.Commun.Data[6] = MotoA.Vbus & 0xff;
      CommunDataUp.Commun.Data[7] = MotoA.Vbus >> 8;
      ProtocolReplyUp(CommunDataDown.Commun.Data[len - Frame_InforLen], CommunDataDown.Commun.Cmd, 8);
      break;
      
    case CMD_GetAngleRaw:       /* 0x12   ��ȡԭʼ���� */
      CommunDataUp.Commun.Data[0]   = ChipAccData[0];
      CommunDataUp.Commun.Data[1]   = ChipAccData[0];
      CommunDataUp.Commun.Data[2]   = ChipAccData[1];
      CommunDataUp.Commun.Data[3]   = ChipAccData[1];
      CommunDataUp.Commun.Data[4]   = ChipAccData[2];
      CommunDataUp.Commun.Data[5]   = ChipAccData[2];
      CommunDataUp.Commun.Data[6]   = ChipGyroData[0];
      CommunDataUp.Commun.Data[7]   = ChipGyroData[0];
      CommunDataUp.Commun.Data[8]   = ChipGyroData[1];
      CommunDataUp.Commun.Data[9]   = ChipGyroData[1];
      CommunDataUp.Commun.Data[10]  = ChipGyroData[2];
      CommunDataUp.Commun.Data[11]  = ChipGyroData[2];
    //CommunDataUp.Commun.Data[12]  = ChipMagData[0];
    //CommunDataUp.Commun.Data[13]  = ChipMagData[0];
    //CommunDataUp.Commun.Data[14]  = ChipMagData[1];
    //CommunDataUp.Commun.Data[15]  = ChipMagData[1];
    //CommunDataUp.Commun.Data[16]  = ChipMagData[2];
    //CommunDataUp.Commun.Data[17]  = ChipMagData[2];
      ProtocolReplyUp(CommunDataDown.Commun.Data[len - Frame_InforLen], CommunDataDown.Commun.Cmd, 12);
      break;
      
    case CMD_GetAngleConfig:    /* 0x13   ��ȡ�Ƕ�ģ�����ò��� */
      break;
      
    case CMD_SetAngleConfig:    /* 0x14   ���ýǶ�ģ����� */
      ChipCorrect = true;
      AngleData_Calibrate(20);
      InFlashMCU.Wbuf[0] = ChipAccBias[0];
      InFlashMCU.Wbuf[1] = ChipAccBias[1];
      InFlashMCU.Wbuf[2] = ChipAccBias[2];
      InFlashMCU.Wbuf[3] = ChipGyroBias[0];
      InFlashMCU.Wbuf[4] = ChipGyroBias[1];
      InFlashMCU.Wbuf[5] = ChipGyroBias[2];
      InFlash_WriteArray(FLASH_SAVE_ADDR, InFlashMCU.Wbuf, 6);
      ChipCorrect = false;
      break;
      
    default:
      break;
  }
  return true;
}

TcomData      TransDataUp;      /* ������ >> ���� */
TcomData      TransDataDown;    /* ���� >> ������ */
/**
  * @brief  ��֡�������
  * @param  ��
  * @retval ��
  */
static void ProtocolReplyTrans(uint8_t UCOM, uint8_t cmd, uint8_t data_len)
{
  TransDataUp.Commun.HeadH = FrameHeadUpH;
  TransDataUp.Commun.HeadL = FrameHeadUpL;
  TransDataUp.Commun.DataLen = data_len;
  TransDataUp.Commun.Count ++;
  TransDataUp.Commun.Cmd = cmd;
  TransDataUp.Data[data_len + Frame_InforLen - 1] = ProtocolCheckSum(TransDataUp.Data, data_len + Frame_InforLen - 1);
  if(UCOM == UCOM2)      USART2_DMA_Send(TransDataUp.Data, data_len + Frame_InforLen);
  else if(UCOM == UCOM3) USART3_DMA_Send(TransDataUp.Data, data_len + Frame_InforLen);
  else if(UCOM == CCOM1) {
    CAN1_SendExtFrame(&TxMsgFIFO, (uint32_t)((TransDataUp.Commun.SourceAddr<<18)|(TxExtId<<8)|cmd), TransDataUp.Commun.Data, 8);
  }
}

/**
  * @brief  ������ӻ�����
  * @param  ��
  * @retval ��
  */
void ProtocolSlaveConnect(void)
{
  TransDataUp.Commun.Data[0] = 0x00;
  TransDataUp.Commun.Data[1] = 0x00;
  TransDataUp.Commun.SourceAddr = 0xDF;
  ProtocolReplyTrans(SlaveCOM, CMD_Handshake, 2);
}

/**
  * @brief  ת������������ӻ�
  * @param  ��
  * @retval ��
  */
void ProtocolSlaveCtrl(void)
{
  if(Remote.Transmit == CMD_SetCtrl) {
    Remote.Transmit = 0x00;
    TransDataUp.Commun.Data[0] = Remote.CoordXH;
    TransDataUp.Commun.Data[1] = Remote.CoordXL;
    TransDataUp.Commun.Data[2] = Remote.CoordYH;
    TransDataUp.Commun.Data[3] = Remote.CoordYL;
    TransDataUp.Commun.Data[4] = Remote.ControlMode;
    TransDataUp.Commun.Data[5] = Remote.OperationMode;
    TransDataUp.Commun.Data[6] = Remote.SpeedGear;
    TransDataUp.Commun.Data[7] = Remote.Peripherals;
    TransDataUp.Commun.Data[8] = Remote.Reserved;
    TransDataUp.Commun.SourceAddr = 0xDF;
    ProtocolReplyTrans(SlaveCOM, CMD_SetCtrl, 9);
  }
}

/**
  * @brief  ����֡Э��(��оƬ->��оƬ)
  * @param  ��
  * @retval CANͨ��
  */
static bool ProtocolSlaveAnalysis(uint8_t *pbuf, uint8_t len)
{
  memcpy(&TransDataDown, pbuf, len);    /* ���ݽṹ�廯 */
  if(pbuf[len-1] < CCOM1) {
    souc_sum = pbuf[len - 2];           /* ���һλ��COM�˿�,�����ڶ�λ��У��� */
    des_sum = ProtocolCheckSum(pbuf, len - 2);
    if(souc_sum != des_sum) return false;
  } else {
    TransDataUp.Commun.SourceAddr = ((pbuf[len-5]<<8) | pbuf[len-4]) & 0x3ff;
  }
  switch(TransDataDown.Commun.Cmd) {
    case CMD_Handshake:         /* 0x01   ����Э�� */
      Slave.RemoteConnect = true;
      Slave.RemoteCount   = 0;
      break;
      
    case CMD_SetCtrl:           /* 0x06   �������� */
      Slave.ControlMode   = TransDataDown.Commun.Data[0];   /* ����ģʽ */
      Slave.OperationMode = TransDataDown.Commun.Data[1];   /* ���з�ʽ */
      Slave.SpeedGear     = TransDataDown.Commun.Data[2];   /* �ٶȵ�λ */
      Slave.Peripherals   = TransDataDown.Commun.Data[3];   /* ����״̬ */
      Slave.Battery       = TransDataDown.Commun.Data[4];   /* ��ص��� */
      Slave.RemoteConnect = true;
      Slave.RemoteCount   = 0;
      break;
      
    case CMD_GetAngle:          /* 0x11   ��ȡ�Ƕ����� */
      MotoU.Pitch = TransDataDown.Commun.Data[1] << 8 | TransDataDown.Commun.Data[0];
      MotoU.Roll  = TransDataDown.Commun.Data[3] << 8 | TransDataDown.Commun.Data[2];
      MotoU.pTemp = TransDataDown.Commun.Data[5] << 8 | TransDataDown.Commun.Data[4];
      MotoU.Vbus  = TransDataDown.Commun.Data[7] << 8 | TransDataDown.Commun.Data[6];
      /* �ش� */
      TransDataUp.Commun.Data[0] = MotoA.Pitch & 0xff;      /* ������ */
      TransDataUp.Commun.Data[1] = MotoA.Pitch >> 8;
      TransDataUp.Commun.Data[2] = MotoA.Roll & 0xff;       /* ����� */
      TransDataUp.Commun.Data[3] = MotoA.Roll >> 8;
      TransDataUp.Commun.Data[4] = 0x00;
      TransDataUp.Commun.Data[5] = 0x00;
      TransDataUp.Commun.Data[6] = MotoA.Vbus & 0xff;
      TransDataUp.Commun.Data[7] = MotoA.Vbus >> 8;
      ProtocolReplyTrans(SlaveCOM, TransDataDown.Commun.Cmd, 8);
      break;
      
    case CMD_GetAngleRaw:       /* 0x12   ��ȡԭʼ���� */
      TransDataUp.Commun.Data[0]  = ChipAccData[0];
      TransDataUp.Commun.Data[1]  = ChipAccData[0];
      TransDataUp.Commun.Data[2]  = ChipAccData[1];
      TransDataUp.Commun.Data[3]  = ChipAccData[1];
      TransDataUp.Commun.Data[4]  = ChipAccData[2];
      TransDataUp.Commun.Data[5]  = ChipAccData[2];
      TransDataUp.Commun.Data[6]  = ChipGyroData[0];
      TransDataUp.Commun.Data[7]  = ChipGyroData[0];
      TransDataUp.Commun.Data[8]  = ChipGyroData[1];
      TransDataUp.Commun.Data[9]  = ChipGyroData[1];
      TransDataUp.Commun.Data[10] = ChipGyroData[2];
      TransDataUp.Commun.Data[11] = ChipGyroData[2];
    //TransDataUp.Commun.Data[12] = ChipMagData[0];
    //TransDataUp.Commun.Data[13] = ChipMagData[0];
    //TransDataUp.Commun.Data[14] = ChipMagData[1];
    //TransDataUp.Commun.Data[15] = ChipMagData[1];
    //TransDataUp.Commun.Data[16] = ChipMagData[2];
    //TransDataUp.Commun.Data[17] = ChipMagData[2];
      ProtocolReplyTrans(SlaveCOM, TransDataDown.Commun.Cmd, 12);
      break;
      
    case CMD_GetAngleConfig:    /* 0x13   ��ȡ�Ƕ�ģ�����ò��� */
      break;
      
    case CMD_SetAngleConfig:    /* 0x14   ���ýǶ�ģ����� */
      break;
      
    default:
      break;
  }
  return true;
}

/**
  * @brief  ��������ʶ�� ����Ƶ�� 10ms
  * @param  ��
  * @retval ��
  */
void ProtocolIdentify(TLoopBuf* pLoopBuffer)
{
  static uint8_t buf[32];
  static uint16_t data_len;
  
  if(LoopQueue_DataLen(pLoopBuffer) >= Frame_ShortLen)
  {
    LoopQueue_ReadRelease(pLoopBuffer, &buf[0], 1);
    if(buf[0] == FrameHeadUpH) {
      LoopQueue_ReadRelease(pLoopBuffer, &buf[1], 1);
      if(buf[1] == FrameHeadUpL) {/* �˶�֡ͷ */
        LoopQueue_ReadRelease(pLoopBuffer, &buf[2], 1);
        data_len = buf[2] + 1;/* ���һλ����������Դ�˿� */
        LoopQueue_ReadRelease(pLoopBuffer, &buf[3], (data_len + Frame_InforLen - 3));/* ��ȡ���� */
        /* ����֡Э�� */
        if(buf[data_len + Frame_InforLen - 1] == SlaveCOM) {
          ProtocolSlaveAnalysis(buf, data_len + Frame_InforLen);
        } else {
          ProtocolAnalysis(buf, data_len + Frame_InforLen);
        }
      }
    }
  }
}

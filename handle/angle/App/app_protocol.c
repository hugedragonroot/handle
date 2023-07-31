#include "app_protocol.h"
#include "main.h"
#include <string.h>


#define SlaveCOM      CCOM1

TcomData              CommunDataUp;     /* 控制器 >> 主机 */
TcomData              CommunDataDown;   /* 主机 >> 控制器 */
TREMOTE_COM           Remote;           /* 遥控通信 */
TREMOTE_COM           Slave;            /* 从机通信 */

/**
  * @brief  计算校验和
  * @param  无
  * @retval 无
  */
uint8_t ProtocolCheckSum(uint8_t *pbuf, uint16_t len)
{
	static uint16_t sum, i;
  sum = 0;
	for (i=0; i<len; i++) sum += pbuf[i];
	return (uint8_t)(sum&0xff);
}

/**
  * @brief  组帧打包发送
  * @param  无
  * @retval 无
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
  * @brief  刷新故障信息
  * @param  无
  * @retval 无
  */
void ProtocolFaultEvent(uint8_t state)
{
  CommunDataUp.Commun.Data[0] = state;
  CommunDataUp.Commun.Data[1] = 0x00;
  ProtocolReplyUp(UCOM2, CMD_FaultEvent, 2);
}

uint8_t souc_sum = 0, des_sum = 0;
/**
  * @brief  解析帧协议(主机->驱动器)
  * @param  无
  * @retval 串口通信
  */
static bool ProtocolAnalysis(uint8_t *pbuf, uint8_t len)
{
  memcpy(&CommunDataDown, pbuf, len);   /* 数据结构体化 */
  if(pbuf[len-1] < CCOM1) {
    souc_sum = pbuf[len - 2];             /* 最后一位是COM端口,倒数第二位是校验和 */
    des_sum = ProtocolCheckSum(pbuf, len - 2);
    if(souc_sum != des_sum) return false;
  } else {
    CommunDataUp.Commun.SourceAddr = ((pbuf[len-5]<<8) | pbuf[len-4]) & 0x3ff;
  }
  switch(CommunDataDown.Commun.Cmd)
  {
    case CMD_Handshake:         /* 0x01 握手协议 */
      CommunDataUp.Commun.Data[0] = PARAMS_DriverModel;
      CommunDataUp.Commun.Data[1] = PARAMS_SoftwareVer / 10;
      CommunDataUp.Commun.Data[2] = PARAMS_SoftwareVer % 10;
      ProtocolReplyUp(CommunDataDown.Commun.Data[len - Frame_InforLen], CommunDataDown.Commun.Cmd, 3);
      break;
      
    case CMD_SetCtrl:           /* 0x06 控制命令 */
      Remote.RemoteConnect  = true;
      Remote.RemoteCount    = 0;
      Remote.CoordXH        = CommunDataDown.Commun.Data[0];  /* X 坐标高8位 */
      Remote.CoordXL        = CommunDataDown.Commun.Data[1];  /* X 坐标低8位 */
      Remote.CoordYH        = CommunDataDown.Commun.Data[2];  /* Y 坐标高8位 */
      Remote.CoordYL        = CommunDataDown.Commun.Data[3];  /* Y 坐标低8位 */
      Remote.ControlMode    = CommunDataDown.Commun.Data[4];  /* 控制模式 */
      Remote.OperationMode  = CommunDataDown.Commun.Data[5];  /* 运行方式 */
      Remote.SpeedGear      = CommunDataDown.Commun.Data[6];  /* 速度挡位 */
      Remote.Peripherals    = CommunDataDown.Commun.Data[7];  /* 外设状态 */
      Remote.Reserved       = CommunDataDown.Commun.Data[8];  /* 保留位 */
      Remote.CoordX         = ((int16_t)(Remote.CoordXH << 8) | Remote.CoordXL) - Coord_Base;
      Remote.CoordY         = ((int16_t)(Remote.CoordYH << 8) | Remote.CoordYL) - Coord_Base;
      /* 回传 */
      CommunDataUp.Commun.Data[0] = Remote.ControlMode;
      CommunDataUp.Commun.Data[1] = Remote.OperationMode;
      CommunDataUp.Commun.Data[2] = Remote.SpeedGear;
      CommunDataUp.Commun.Data[3] = Remote.Peripherals;
      CommunDataUp.Commun.Data[4] = Remote.Battery;           /* 电池电量 */
      CommunDataUp.Commun.Data[5] = 0x00;
      CommunDataUp.Commun.Data[6] = 0x00;
      ProtocolReplyUp(CommunDataDown.Commun.Data[len - Frame_InforLen], CommunDataDown.Commun.Cmd, 7);
      /* 发送至从机 */
      Remote.Transmit = CMD_SetCtrl;
      break;
      
    case CMD_GetAngle:          /* 0x11    获取角度数据 */
      MotoU.Pitch = CommunDataDown.Commun.Data[1] << 8 | CommunDataDown.Commun.Data[0];
      MotoU.Roll  = CommunDataDown.Commun.Data[3] << 8 | CommunDataDown.Commun.Data[2];
      MotoU.pTemp = CommunDataDown.Commun.Data[5] << 8 | CommunDataDown.Commun.Data[4];
      MotoU.Vbus  = CommunDataDown.Commun.Data[7] << 8 | CommunDataDown.Commun.Data[6];
      /* 回传 */
      CommunDataUp.Commun.Data[0] = MotoA.Pitch & 0xff;      /* 俯仰角 */
      CommunDataUp.Commun.Data[1] = MotoA.Pitch >> 8;
      CommunDataUp.Commun.Data[2] = MotoA.Roll & 0xff;       /* 侧倾角 */
      CommunDataUp.Commun.Data[3] = MotoA.Roll >> 8;
      CommunDataUp.Commun.Data[4] = 0x00;
      CommunDataUp.Commun.Data[5] = 0x00;
      CommunDataUp.Commun.Data[6] = MotoA.Vbus & 0xff;
      CommunDataUp.Commun.Data[7] = MotoA.Vbus >> 8;
      ProtocolReplyUp(CommunDataDown.Commun.Data[len - Frame_InforLen], CommunDataDown.Commun.Cmd, 8);
      break;
      
    case CMD_GetAngleRaw:       /* 0x12   获取原始数据 */
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
      
    case CMD_GetAngleConfig:    /* 0x13   获取角度模块配置参数 */
      break;
      
    case CMD_SetAngleConfig:    /* 0x14   设置角度模块参数 */
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

TcomData      TransDataUp;      /* 控制器 >> 主机 */
TcomData      TransDataDown;    /* 主机 >> 控制器 */
/**
  * @brief  组帧打包发送
  * @param  无
  * @retval 无
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
  * @brief  主机与从机握手
  * @param  无
  * @retval 无
  */
void ProtocolSlaveConnect(void)
{
  TransDataUp.Commun.Data[0] = 0x00;
  TransDataUp.Commun.Data[1] = 0x00;
  TransDataUp.Commun.SourceAddr = 0xDF;
  ProtocolReplyTrans(SlaveCOM, CMD_Handshake, 2);
}

/**
  * @brief  转发控制命令给从机
  * @param  无
  * @retval 无
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
  * @brief  解析帧协议(主芯片->从芯片)
  * @param  无
  * @retval CAN通信
  */
static bool ProtocolSlaveAnalysis(uint8_t *pbuf, uint8_t len)
{
  memcpy(&TransDataDown, pbuf, len);    /* 数据结构体化 */
  if(pbuf[len-1] < CCOM1) {
    souc_sum = pbuf[len - 2];           /* 最后一位是COM端口,倒数第二位是校验和 */
    des_sum = ProtocolCheckSum(pbuf, len - 2);
    if(souc_sum != des_sum) return false;
  } else {
    TransDataUp.Commun.SourceAddr = ((pbuf[len-5]<<8) | pbuf[len-4]) & 0x3ff;
  }
  switch(TransDataDown.Commun.Cmd) {
    case CMD_Handshake:         /* 0x01   握手协议 */
      Slave.RemoteConnect = true;
      Slave.RemoteCount   = 0;
      break;
      
    case CMD_SetCtrl:           /* 0x06   控制命令 */
      Slave.ControlMode   = TransDataDown.Commun.Data[0];   /* 控制模式 */
      Slave.OperationMode = TransDataDown.Commun.Data[1];   /* 运行方式 */
      Slave.SpeedGear     = TransDataDown.Commun.Data[2];   /* 速度挡位 */
      Slave.Peripherals   = TransDataDown.Commun.Data[3];   /* 外设状态 */
      Slave.Battery       = TransDataDown.Commun.Data[4];   /* 电池电量 */
      Slave.RemoteConnect = true;
      Slave.RemoteCount   = 0;
      break;
      
    case CMD_GetAngle:          /* 0x11   获取角度数据 */
      MotoU.Pitch = TransDataDown.Commun.Data[1] << 8 | TransDataDown.Commun.Data[0];
      MotoU.Roll  = TransDataDown.Commun.Data[3] << 8 | TransDataDown.Commun.Data[2];
      MotoU.pTemp = TransDataDown.Commun.Data[5] << 8 | TransDataDown.Commun.Data[4];
      MotoU.Vbus  = TransDataDown.Commun.Data[7] << 8 | TransDataDown.Commun.Data[6];
      /* 回传 */
      TransDataUp.Commun.Data[0] = MotoA.Pitch & 0xff;      /* 俯仰角 */
      TransDataUp.Commun.Data[1] = MotoA.Pitch >> 8;
      TransDataUp.Commun.Data[2] = MotoA.Roll & 0xff;       /* 侧倾角 */
      TransDataUp.Commun.Data[3] = MotoA.Roll >> 8;
      TransDataUp.Commun.Data[4] = 0x00;
      TransDataUp.Commun.Data[5] = 0x00;
      TransDataUp.Commun.Data[6] = MotoA.Vbus & 0xff;
      TransDataUp.Commun.Data[7] = MotoA.Vbus >> 8;
      ProtocolReplyTrans(SlaveCOM, TransDataDown.Commun.Cmd, 8);
      break;
      
    case CMD_GetAngleRaw:       /* 0x12   获取原始数据 */
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
      
    case CMD_GetAngleConfig:    /* 0x13   获取角度模块配置参数 */
      break;
      
    case CMD_SetAngleConfig:    /* 0x14   设置角度模块参数 */
      break;
      
    default:
      break;
  }
  return true;
}

/**
  * @brief  串口数据识别 调用频率 10ms
  * @param  无
  * @retval 无
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
      if(buf[1] == FrameHeadUpL) {/* 核对帧头 */
        LoopQueue_ReadRelease(pLoopBuffer, &buf[2], 1);
        data_len = buf[2] + 1;/* 最后一位加了数据来源端口 */
        LoopQueue_ReadRelease(pLoopBuffer, &buf[3], (data_len + Frame_InforLen - 3));/* 获取数据 */
        /* 解析帧协议 */
        if(buf[data_len + Frame_InforLen - 1] == SlaveCOM) {
          ProtocolSlaveAnalysis(buf, data_len + Frame_InforLen);
        } else {
          ProtocolAnalysis(buf, data_len + Frame_InforLen);
        }
      }
    }
  }
}

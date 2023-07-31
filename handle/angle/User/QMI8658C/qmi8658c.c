/**
  ******************************************************************************
  * @file    QMI8658C.c
  * @author  Rhui
  * @version V1.0
  * @date    2022-xx-xx
  * @brief   QMI8658C 应用驱动
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

#include ".\QMI8658C\qmi8658c.h"
#include ".\ATusart\bsp_ATusart.h"
#include ".\internal_flash\bsp_internal_flash.h"
#include "math.h"


/*
    Y
    ^
    .---------  -> X
    |*Z       |
    |         |
    |         |     右手拇指指向加速度正方向
    |         |     其余四指指向陀螺仪旋转正方向
     ---------
*/

#define FLASH_8658CONF_ADDR   0x08010000

I2CStreamDef            QMI8658CStr;
QMI8658C_CONF_STRUCT    QMI8658CReg;
int16_t QMI8658C_Temp, QMI8658CAcc[3], QMI8658CGyro[3], QMI8658CdQ[4], QMI8658CdV[3];  /* 0:X  1:Y  2:Z */


/**
  * @brief  获取设备ID,判断设备连接状态
  * @param  None
  * @retval None
  */
void QMI8658C_DataConfig(void)
{
  InFlash_ReadHalfWord(FLASH_8658CONF_ADDR, InFlashMCU.Rbuf, 9);
  if(InFlashMCU.Rbuf[0] == 0xA55A)
  {
    QMI8658CReg.CTRL1 = InFlashMCU.Rbuf[2];
    QMI8658CReg.CTRL2 = InFlashMCU.Rbuf[3];
    QMI8658CReg.CTRL3 = InFlashMCU.Rbuf[4];
    QMI8658CReg.CTRL4 = InFlashMCU.Rbuf[5];
    QMI8658CReg.CTRL5 = InFlashMCU.Rbuf[6];
    QMI8658CReg.CTRL6 = InFlashMCU.Rbuf[7];
    QMI8658CReg.CTRL7 = InFlashMCU.Rbuf[8];
    QMI8658CReg.CTRL8 = InFlashMCU.Rbuf[9];
    QMI8658CReg.CTRL9 = InFlashMCU.Rbuf[10];
  }
  else
  {
    QMI8658CReg.CTRL1 = 0x40;
    QMI8658CReg.CTRL2 = 0x84;
    QMI8658CReg.CTRL3 = 0xE4;
    QMI8658CReg.CTRL4 = 0x00;
    QMI8658CReg.CTRL5 = 0x33;
    QMI8658CReg.CTRL6 = 0x00;
    QMI8658CReg.CTRL7 = 0x43;
    QMI8658CReg.CTRL8 = 0x00;
    QMI8658CReg.CTRL9 = 0x00;
  }
}

/**
  * @brief  获取设备ID,判断设备连接状态
  * @param  None
  * @retval None
  */
uint8_t QMI8658C_ReadAddr(void)
{
  QMI8658CStr.BusSta = Sensors_I2C_ReadRegister(QMI8658C_DEFAULT_ADDRESS, QMI8658C_WHO_AM_I, 1, QMI8658CStr.RBuf);
  if(QMI8658CStr.RBuf[0] != 0x05 || QMI8658CStr.BusSta != 0)
  {
    ForPC_Usart("QMI8658C_Error:%2x,%2x\r\n", QMI8658CStr.RBuf[0],QMI8658CStr.BusSta);
    return QMI8658CStr.BusSta;
  }
  else
  {
    ForPC_Usart("QMI8658C_Addr:%x\r\n", QMI8658CStr.RBuf[0]);
  }
  QMI8658CStr.BusSta = 0x00;
  return 0x00;
}

/**
  * @brief  QMI8658C 初始化
  * @param  None
  * @retval Typical Sensor Mode
  */
uint8_t QMI8658C_Init(void)
{
  QMI8658C_DataConfig();
  QMI8658CStr.BusSta = QMI8658C_ReadAddr();
  if(QMI8658CStr.BusSta != 0) return QMI8658CStr.BusSta;
  /* Control power states,SPI communications */
  QMI8658CStr.BusSta = Sensors_I2C_WriteRegister(QMI8658C_DEFAULT_ADDRESS, QMI8658C_CTRL1, 1, &QMI8658CReg.CTRL1);
  if(QMI8658CStr.BusSta != 0) return QMI8658CStr.BusSta;
  /* Enable the AttitudeEngine */
  QMI8658CStr.BusSta = Sensors_I2C_WriteRegister(QMI8658C_DEFAULT_ADDRESS, QMI8658C_CTRL7, 1, &QMI8658CReg.CTRL7);
  if(QMI8658CStr.BusSta != 0) return QMI8658CStr.BusSta;
  /* Configure Full Scale,Enable Self Test;2g~500Hz */
  QMI8658CStr.BusSta = Sensors_I2C_WriteRegister(QMI8658C_DEFAULT_ADDRESS, QMI8658C_CTRL2, 1, &QMI8658CReg.CTRL2);
  if(QMI8658CStr.BusSta != 0) return QMI8658CStr.BusSta;
  /* Configure Full Scale,Enable Self Test;±1024dps~500Hz */
  QMI8658CStr.BusSta = Sensors_I2C_WriteRegister(QMI8658C_DEFAULT_ADDRESS, QMI8658C_CTRL3, 1, &QMI8658CReg.CTRL3);
  if(QMI8658CStr.BusSta != 0) return QMI8658CStr.BusSta;
  /* Configure Output Data Rate and choose device */
  QMI8658CStr.BusSta = Sensors_I2C_WriteRegister(QMI8658C_DEFAULT_ADDRESS, QMI8658C_CTRL4, 1, &QMI8658CReg.CTRL4);
  if(QMI8658CStr.BusSta != 0) return QMI8658CStr.BusSta;
  /* Sensor Data Processing Settings */
  QMI8658CStr.BusSta = Sensors_I2C_WriteRegister(QMI8658C_DEFAULT_ADDRESS, QMI8658C_CTRL5, 1, &QMI8658CReg.CTRL5);
  if(QMI8658CStr.BusSta != 0) return QMI8658CStr.BusSta;
  
  QMI8658CStr.BusSta = 0x00;
  return 0x00;
}

/**
  * @brief  获取FIFO数据
  * @param  None
  * @retval None
  */
uint8_t QMI8658C_ReadFifoData(void)
{
  QMI8658CStr.BusSta = Sensors_I2C_ReadRegister(QMI8658C_DEFAULT_ADDRESS, QMI8658C_FIFO_DATA, 1, QMI8658CStr.RBuf);
  if(QMI8658CStr.BusSta != 0) return QMI8658CStr.BusSta;
  else
  {
  //ForPC_Usart("FIFO:%d\r\n", QMI8658CStr.RBuf[0]);
  }
  QMI8658CStr.BusSta = 0x00;
  return 0x00;
}

/**
  * @brief  获取数据状态
  * @param  None
  * @retval None
  */
uint8_t QMI8658C_Status(void)
{
  QMI8658CStr.BusSta = Sensors_I2C_ReadRegister(QMI8658C_DEFAULT_ADDRESS, QMI8658C_I2CM_STATUS, 3, QMI8658CStr.RBuf);
  if(QMI8658CStr.BusSta != 0) return QMI8658CStr.BusSta;
  else
  {
  //ForPC_Usart("I2CM_STATUS:%2x STATUSINT:%2x STATUS0:%2x\r\n", QMI8658CStr.RBuf[0], QMI8658CStr.RBuf[1], QMI8658CStr.RBuf[2]);
  }
  QMI8658CStr.BusSta = 0x00;
  return 0x00;
}

/**
  * @brief  获取采样时间
  * @param  None
  * @retval None
  */
uint8_t QMI8658C_SampleTime(void)
{
  QMI8658CStr.BusSta = Sensors_I2C_ReadRegister(QMI8658C_DEFAULT_ADDRESS, QMI8658C_TIMESTAMP_LOW, 3, QMI8658CStr.RBuf);
  if(QMI8658CStr.BusSta != 0) return QMI8658CStr.BusSta;
  else
  {
  //ForPC_Usart("LOW:%2x MID:%2x HIG:%2x\r\n", QMI8658CStr.RBuf[0], QMI8658CStr.RBuf[1], QMI8658CStr.RBuf[2]);
  }
  QMI8658CStr.BusSta = 0x00;
  return 0x00;
}

/**
  * @brief  获取AE状态
  * @param  None
  * @retval None
  */
uint8_t QMI8658C_AEREG(void)
{
  QMI8658CStr.BusSta = Sensors_I2C_ReadRegister(QMI8658C_DEFAULT_ADDRESS, QMI8658C_AE_REG1, 2, QMI8658CStr.RBuf);
  if(QMI8658CStr.BusSta != 0) return QMI8658CStr.BusSta;
  else
  {
  //ForPC_Usart("I2CM_STATUS:%2x STATUSINT:%2x STATUS0:%2x\r\n", QMI8658CStr.RBuf[0], QMI8658CStr.RBuf[1], QMI8658CStr.RBuf[2]);
  }
  QMI8658CStr.BusSta = 0x00;
  return 0x00;
}

/**
  * @brief  获取数据
  * @param  None
  * @retval None
  */
uint8_t QMI8658C_ReadData(void)
{
  QMI8658CStr.BusSta = Sensors_I2C_ReadRegister(QMI8658C_DEFAULT_ADDRESS, QMI8658C_TEMP_L, 14, QMI8658CStr.RBuf);
  if(QMI8658CStr.BusSta != 0) return QMI8658CStr.BusSta;
  else
  {
    QMI8658C_Temp   = (QMI8658CStr.RBuf[1] << 8) | QMI8658CStr.RBuf[0];     /* 温度 */
    QMI8658CAcc[0]  = (QMI8658CStr.RBuf[3] << 8) | QMI8658CStr.RBuf[2];     /* 加速度 */
    QMI8658CAcc[1]  = (QMI8658CStr.RBuf[5] << 8) | QMI8658CStr.RBuf[4];
    QMI8658CAcc[2]  = (QMI8658CStr.RBuf[7] << 8) | QMI8658CStr.RBuf[6];
    QMI8658CGyro[0] = (QMI8658CStr.RBuf[9] << 8) | QMI8658CStr.RBuf[8];     /* 陀螺仪 */
    QMI8658CGyro[1] = (QMI8658CStr.RBuf[11] << 8) | QMI8658CStr.RBuf[10];
    QMI8658CGyro[2] = (QMI8658CStr.RBuf[13] << 8) | QMI8658CStr.RBuf[12];
  }
  QMI8658CStr.BusSta = 0x00;
  return 0x00;
}

/*********************************************END OF FILE**********************/

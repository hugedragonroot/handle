/**
  ******************************************************************************
  * @file    QMC5883L.c
  * @author  Rhui
  * @version V1.0
  * @date    2022-xx-xx
  * @brief   QMC5883L 应用驱动
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

#include ".\QMC5883L\qmc5883l.h"
#include ".\ATusart\bsp_ATusart.h"
#include "math.h"


/*
    .--------- → Y
    |*Z       |
    |         |
    |         |
    |         |
     ---------
    ↓
    X
*/


I2CStreamDef  QMC5883LStr;
int16_t QMC5883LMag[3];  /* 0:X  1:Y  2:Z */
int16_t QMC5883LMX, QMC5883LMY, QMC5883LMZ;


/**
  * @brief  获取设备ID,判断设备连接状态
  * @param  None
  * @retval None
  */
uint8_t QMC5883L_ReadAddr(void)
{
  QMC5883LStr.BusSta = Sensors_I2C_ReadRegister(QMC5883L_DEFAULT_ADDRESS, QMC5883L_CHIP_ID, 1, QMC5883LStr.RBuf);
  if(QMC5883LStr.RBuf[0] != 0xFF || QMC5883LStr.BusSta != 0)
  {
    ForPC_Usart("QMC5883L_Error:%2x,%2x\r\n", QMC5883LStr.RBuf[0], QMC5883LStr.BusSta);
    return QMC5883LStr.BusSta;
  }
  else
  {
    ForPC_Usart("QMC5883LL_Addr:%x\r\n", QMC5883LStr.RBuf[0]);
  }
  QMC5883LStr.BusSta = 0x00;
  return 0x00;
}

/**
  * @brief  QMI8658 初始化
  * @param  None
  * @retval None
  */
uint8_t QMC5883L_Init(void)
{
  QMC5883LStr.BusSta = QMC5883L_ReadAddr();
  if(QMC5883LStr.BusSta != 0) return QMC5883LStr.BusSta;
  
  QMC5883LStr.WBuf[0] = 0x8D;    /*  */
  QMC5883LStr.BusSta = Sensors_I2C_WriteRegister(QMC5883L_DEFAULT_ADDRESS, QMC5883L_CTRL1, 1, QMC5883LStr.WBuf);
  if(QMC5883LStr.BusSta != 0) return QMC5883LStr.BusSta;
  
  QMC5883LStr.WBuf[0] = 0x01;    /*  */
  QMC5883LStr.BusSta = Sensors_I2C_WriteRegister(QMC5883L_DEFAULT_ADDRESS, QMC5883L_CTRL2, 1, QMC5883LStr.WBuf);
  if(QMC5883LStr.BusSta != 0) return QMC5883LStr.BusSta;
  
  QMC5883LStr.WBuf[0] = 0x01;    /* SET/RESET Period is controlled */
  QMC5883LStr.BusSta = Sensors_I2C_WriteRegister(QMC5883L_DEFAULT_ADDRESS, QMC5883L_SE_RES, 1, QMC5883LStr.WBuf);
  if(QMC5883LStr.BusSta != 0) return QMC5883LStr.BusSta;
  
  QMC5883LStr.BusSta = 0x00;
  return 0x00;
}

/**
  * @brief  获取数据
  * @param  None
  * @retval None
  */
uint8_t QMC5883L_ReadData(void)
{
  QMC5883LStr.BusSta = Sensors_I2C_ReadRegister(QMC5883L_DEFAULT_ADDRESS, QMC5883L_XOUT_L, 6, QMC5883LStr.RBuf);
  if(QMC5883LStr.BusSta != 0) return QMC5883LStr.BusSta;
  else
  {
    QMC5883LMag[0] = (QMC5883LStr.RBuf[1] << 8) | QMC5883LStr.RBuf[0];  /* 磁场 */
    QMC5883LMag[1] = (QMC5883LStr.RBuf[3] << 8) | QMC5883LStr.RBuf[2];
    QMC5883LMag[2] = (QMC5883LStr.RBuf[5] << 8) | QMC5883LStr.RBuf[4];
  }
  QMC5883LStr.BusSta = 0x00;
  return 0x00;
}

/*********************************************END OF FILE**********************/

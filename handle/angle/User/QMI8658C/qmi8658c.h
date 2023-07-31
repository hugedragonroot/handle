#ifndef __QMI8658C_H
#define __QMI8658C_H

#include "stm32f10x.h"
#include ".\i2c\bsp_i2c.h"


#define QMI8658C_ADDRESS_SA0_LOW     0x6A  /* address pin low (GND), default for InvenSense evaluation board */
#define QMI8658C_ADDRESS_SA0_HIGH    0x6B  /* address pin high (VCC) */
#define QMI8658C_DEFAULT_ADDRESS     QMI8658C_ADDRESS_SA0_LOW
/* General Purpose Registers */
#define QMI8658C_WHO_AM_I            0x00    /* R */
#define QMI8658C_REVISION_ID         0x01    /* R */
/* Setup and Control Registers */
#define QMI8658C_CTRL1               0x02    /* RW */
#define QMI8658C_CTRL2               0x03    /* RW */
#define QMI8658C_CTRL3               0x04    /* RW */
#define QMI8658C_CTRL4               0x05    /* RW */
#define QMI8658C_CTRL5               0x06    /* RW */
#define QMI8658C_CTRL6               0x07    /* RW */
#define QMI8658C_CTRL7               0x08    /* RW */
#define QMI8658C_CTRL8               0x09    /* RW */
#define QMI8658C_CTRL9               0x0A    /* RW */
/* Host Controlled Calibration Registers */
#define QMI8658C_CAL1_L              0x0B    /* RW */
#define QMI8658C_CAL1_H              0x0C    /* RW */
#define QMI8658C_CAL2_L              0x0D    /* RW */
#define QMI8658C_CAL2_H              0x0E    /* RW */
#define QMI8658C_CAL3_L              0x0F    /* RW */
#define QMI8658C_CAL3_H              0x10    /* RW */
#define QMI8658C_CAL4_L              0x11    /* RW */
#define QMI8658C_CAL4_H              0x12    /* RW */
/* FIFO Registers */
#define QMI8658C_FIFO_WTM_TH         0x13    /* RW */
#define QMI8658C_FIFO_CTRL           0x14    /* RW */
#define QMI8658C_FIFO_SMPL_CNT       0x15    /* R */
#define QMI8658C_FIFO_STATUS         0x16    /* R */
#define QMI8658C_FIFO_DATA           0x17    /* R */
/* Status Registers */
#define QMI8658C_I2CM_STATUS         0x2C    /* R */
#define QMI8658C_STATUSINT           0x2D    /* R */
#define QMI8658C_STATUS0             0x2E    /* R */
#define QMI8658C_STATUS1             0x2F    /* R */
/* Timestamp Register */
#define QMI8658C_TIMESTAMP_LOW       0x30    /* R */
#define QMI8658C_TIMESTAMP_MID       0x31    /* R */
#define QMI8658C_TIMESTAMP_HIGH      0x32    /* R */
/* Data Output Registers */
#define QMI8658C_TEMP_L              0x33    /* R */
#define QMI8658C_TEMP_H              0x34    /* R */
#define QMI8658C_AX_L                0x35    /* R */
#define QMI8658C_AX_H                0x36    /* R */
#define QMI8658C_AY_L                0x37    /* R */
#define QMI8658C_AY_H                0x38    /* R */
#define QMI8658C_AZ_L                0x39    /* R */
#define QMI8658C_AZ_H                0x3A    /* R */
#define QMI8658C_GX_L                0x3B    /* R */
#define QMI8658C_GX_H                0x3C    /* R */
#define QMI8658C_GY_L                0x3D    /* R */
#define QMI8658C_GY_H                0x3E    /* R */
#define QMI8658C_GZ_L                0x3F    /* R */
#define QMI8658C_GZ_H                0x40    /* R */
#define QMI8658C_MX_L                0x41    /* R */
#define QMI8658C_MX_H                0x42    /* R */
#define QMI8658C_MY_L                0x43    /* R */
#define QMI8658C_MY_H                0x44    /* R */
#define QMI8658C_MZ_L                0x45    /* R */
#define QMI8658C_MZ_H                0x46    /* R */
#define QMI8658C_dQW_L               0x49    /* R */
#define QMI8658C_dQW_H               0x4A    /* R */
#define QMI8658C_dQX_L               0x4B    /* R */
#define QMI8658C_dQX_H               0x4C    /* R */
#define QMI8658C_dQY_L               0x4D    /* R */
#define QMI8658C_dQY_H               0x4E    /* R */
#define QMI8658C_dQZ_L               0x4F    /* R */
#define QMI8658C_dQZ_H               0x50    /* R */
#define QMI8658C_dVX_L               0x51    /* R */
#define QMI8658C_dVX_H               0x52    /* R */
#define QMI8658C_dVY_L               0x53    /* R */
#define QMI8658C_dVY_H               0x54    /* R */
#define QMI8658C_dVZ_L               0x55    /* R */
#define QMI8658C_dVZ_H               0x56    /* R */
#define QMI8658C_AE_REG1             0x57    /* R */
#define QMI8658C_AE_REG2             0x58    /* R */
/* Reset Register */
#define QMI8658C_RESET               0x60    /* W */

/* 配置QMI8658C参数结构体初始化 */
typedef struct
{
  uint8_t CTRL1;
  uint8_t CTRL2;
  uint8_t CTRL3;
  uint8_t CTRL4;
  uint8_t CTRL5;
  uint8_t CTRL6;
  uint8_t CTRL7;
  uint8_t CTRL8;
  uint8_t CTRL9;
} QMI8658C_CONF_STRUCT;


extern I2CStreamDef QMI8658CStr;
extern int16_t QMI8658C_Temp, QMI8658CAcc[3], QMI8658CGyro[3], QMI8658CdQ[4], QMI8658CdV[3];

void QMI8658C_DataConfig(void);
uint8_t QMI8658C_ReadAddr(void);
uint8_t QMI8658C_Init(void);
uint8_t QMI8658C_ReadFifoData(void);
uint8_t QMI8658C_Status(void);
uint8_t QMI8658C_SampleTime(void);
uint8_t QMI8658C_AEREG(void);
uint8_t QMI8658C_ReadData(void);

#endif  /*__QMI8658C_H*/

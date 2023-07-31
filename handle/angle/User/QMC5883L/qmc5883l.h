#ifndef __QMC5883L_H
#define __QMC5883L_H

#include "stm32f10x.h"
#include ".\i2c\bsp_i2c.h"


#define QMC5883L_ADDRESS_S1_HIGH    0x0D    /* address pin high (VCC) */
#define QMC5883L_DEFAULT_ADDRESS    QMC5883L_ADDRESS_S1_HIGH
/* Output Data Register */
#define QMC5883L_XOUT_L             0x00    /* R */
#define QMC5883L_XOUT_M             0x01    /* R */
#define QMC5883L_YOUT_L             0x02    /* R */
#define QMC5883L_YOUT_M             0x03    /* R */
#define QMC5883L_ZOUT_L             0x04    /* R */
#define QMC5883L_ZOUT_M             0x05    /* R */
/* Status Register */
#define QMC5883L_STATUS             0x06    /* R */
/* Temperature Data Register */
#define QMC5883L_TOUT_L             0x07    /* R */
#define QMC5883L_TOUT_M             0x08    /* R */
/* Control Register */
#define QMC5883L_CTRL1              0x09    /* RW */
#define QMC5883L_CTRL2              0x0A    /* RW */
/* SET/RESET Period Register */
#define QMC5883L_SE_RES             0x0B    /* RW */
/* Reserved */
#define QMC5883L_RESERVE            0x0C    /* R */
/* Chip ID Register */
#define QMC5883L_CHIP_ID            0x0D    /* R */


extern I2CStreamDef QMC5883LStr;
extern int16_t QMC5883LMag[3];
extern int16_t QMC5883LMX, QMC5883LMY, QMC5883LMZ;

uint8_t QMC5883L_ReadAddr(void);
uint8_t QMC5883L_Init(void);
uint8_t QMC5883L_ReadData(void);

#endif  /*__QMC5883L_H*/

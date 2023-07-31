#ifndef __QMI8658C_H
#define	__QMI8658C_H
#include "main.h"
#include "common.h"

#define i2c_nop           i2c_delay(150)   /* 330k */

#define i2c_scl_1         HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6,GPIO_PIN_SET)
#define i2c_scl_0         HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6,GPIO_PIN_RESET)

#define i2c_sda_1         HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7,GPIO_PIN_SET)
#define i2c_sda_0         HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7,GPIO_PIN_RESET)
#define i2c_sda_state     HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7)
#define i2c_sda_write(value)			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,value)
uint8_t QMI8658_init(void);
/*-----位带操作------*/
//IO口地址映射

#define	QMI8658_SA0_H		0x6A
#define	QMI8658_SA0_L		0x6B

#define	WHO_AM_I	0x00
#define	REVISION_ID	0x01
#define	CTL1		0x02
#define	CTL2		0x03
#define	CTL3		0x04
#define	CTL4		0x05
#define	CTL5		0x06
#define	CTL6		0x07
#define	CTL7		0x08
#define CTL8        0x09    /* RW */
#define CTL9        0x0A    /* RW */

#define	STATUSINT	0x2d
#define	STATUS0		0x2e
#define	STATUS1		0x2f
#define	TIMESTAMP_LOW	0x30	
#define	TIMESTAMP_MID	0x31
#define	TIMESTAMP_HIGH	0x32
#define	AX_L		0x35
#define	AX_H		0x36
#define	AY_L		0x37
#define	AY_H		0x38
#define	AZ_L		0x39
#define	AZ_H		0x3A
#define	GX_L		0x3B
#define	GX_H		0x3C
#define	GY_L		0x3D
#define	GY_H		0x3E
#define	GZ_L		0x3F
#define	GZ_H		0x40

		

#define	CTL1_INIT	0x40	
#define	CTL2_INIT	0x84	
#define	CTL3_INIT	0xE4	
#define	CTL4_INIT	0x00	
#define	CTL5_INIT	0x33
#define	CTL6_INIT	0x00
#define	CTL7_INIT	0x43
#define	CTL8_INIT	0x00
#define	CTL9_INIT	0x00	
		
void Read_Gyro(void);
void Read_Accele(void);
extern int16_t Roll,Pitch;
float Yijielvbo(float accel_m, float gyro_m);
int16_t Erjielvbo(float accel_m, float gyro_m);
void QMI_ReadData(void);
uint8_t QMI_Angle(uint8_t Flag);


#endif 	

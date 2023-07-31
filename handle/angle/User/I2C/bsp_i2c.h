#ifndef __BSP_I2C_H
#define	__BSP_I2C_H

#include "stm32f10x.h"
#include <stdbool.h>
//#include ".\ATusart\bsp_ATusart.h"


//����������꣬ʹ��Ӳ��iic������ʹ�����iic,
//ʹ��Ӳ��IICʱ������Һ�������ã���ΪFSMC��NADV��IIC1��SDA ��ͬһ�����ţ�����Ӱ����
//������FSMCʱ���Ƽ���Ӳ��IIC

//#define HARD_IIC
/* ���I2C,Լ200KHz,8M_SYSCLK */

/********************* ���IICʹ�õĺ� ****************************/
//#define Soft_I2C_INT 		GPIO_Pin_5
#define Soft_I2C_SCL 		GPIO_Pin_6
#define Soft_I2C_SDA 		GPIO_Pin_7
#define Soft_I2C_PORT   GPIOB
//
#define Soft_I2C_SCL_0 		GPIO_ResetBits(Soft_I2C_PORT, Soft_I2C_SCL)
#define Soft_I2C_SCL_1 		GPIO_SetBits(Soft_I2C_PORT, Soft_I2C_SCL)
#define Soft_I2C_SDA_0 		GPIO_ResetBits(Soft_I2C_PORT, Soft_I2C_SDA)
#define Soft_I2C_SDA_1   	GPIO_SetBits(Soft_I2C_PORT, Soft_I2C_SDA)


/************************** I2C�������壬I2C1��I2C2 ********************************/
#define             macI2Cx                                I2C1
#define             macI2C_APBxClock_FUN                   RCC_APB1PeriphClockCmd
#define             macI2C_CLK                             RCC_APB1Periph_I2C1
#define             macI2C_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             macI2C_GPIO_CLK                        RCC_APB2Periph_GPIOB     
#define             macI2C_SCL_PORT                        GPIOB   
#define             macI2C_SCL_PIN                         GPIO_Pin_6
#define             macI2C_SDA_PORT                        GPIOB 
#define             macI2C_SDA_PIN                         GPIO_Pin_7

/* �ȴ���ʱʱ�� */
#define mdelay                    SysTick_Delay_Ms
#define I2CT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT         ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))


/* I2C��д����ṹ�� */
typedef struct
{
  bool    WEN;
  bool    REN;
  uint8_t WCNT;
  uint8_t RCNT;
  uint8_t WBuf[32];
  uint8_t RBuf[32];
  uint8_t BusSta;
} I2CStreamDef;

void I2C_Bus_Init(void);
void Soft_I2C_Reset(void);
void Set_I2C_Retry(unsigned short ml_sec);
unsigned short Get_I2C_Retry(void);
uint8_t Soft_I2C_START(void);
void Soft_I2C_STOP(void);
void Soft_I2C_SendACK(void);
void Soft_I2C_SendNACK(void);
uint8_t Soft_I2C_Wait_Ack(void);
void Soft_I2C_SendByte(uint8_t anbt_i2c_data);
uint8_t Soft_I2C_ReceiveByte(void);
uint8_t Soft_I2C_ReceiveByte_WithACK(void);
uint8_t Soft_I2C_CheckDevice(uint8_t _Address);
uint8_t Soft_I2C_Write(uint8_t soft_dev_addr, uint8_t soft_reg_addr, uint8_t soft_i2c_len, uint8_t *soft_i2c_data_buf);
uint8_t Soft_I2C_Read(uint8_t soft_dev_addr, uint8_t soft_reg_addr, uint8_t soft_i2c_len, uint8_t *soft_i2c_data_buf);
int Sensors_I2C_ReadRegister(unsigned char Address, unsigned char RegisterAddr, 
                             unsigned short RegisterLen, unsigned char *RegisterValue);
int Sensors_I2C_WriteRegister(unsigned char Address, unsigned char RegisterAddr, 
                              unsigned short RegisterLen, const unsigned char *RegisterValue);

#endif /* __BSP_I2C_H */

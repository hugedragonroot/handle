#ifndef __GPIO_H
#define	__GPIO_H


#include "stm32f10x.h"


/* 定义LED连接的GPIO端口, 用户只需要修改下面的代码即可改变控制的LED引脚 */
/* R-红色 (板内指示灯) */
#define LED1_GPIO_PORT      GPIOA                   /* GPIO端口 */
#define LED1_GPIO_CLK       RCC_APB2Periph_GPIOA    /* GPIO端口时钟 */
#define LED1_GPIO_PIN       GPIO_Pin_6              /* 连接到SCL时钟线的GPIO */
/* G-绿色 (外部指示灯) */
#define LED2_GPIO_PORT      GPIOA
#define LED2_GPIO_CLK       RCC_APB2Periph_GPIOA
#define LED2_GPIO_PIN       GPIO_Pin_7
/* B-蓝色 (蜂鸣器) */
#define LED3_GPIO_PORT      GPIOC
#define LED3_GPIO_CLK       RCC_APB2Periph_GPIOC
#define LED3_GPIO_PIN       GPIO_Pin_13
/* 点动控制H(PB12) */
#define Hctr_GPIO_PORT      GPIOB
#define Hctr_GPIO_CLK       RCC_APB2Periph_GPIOB
#define Hctr_GPIO_PIN       GPIO_Pin_12
/* 方向控制R(PB13) */
#define Rctr_GPIO_PORT      GPIOB
#define Rctr_GPIO_CLK       RCC_APB2Periph_GPIOB
#define Rctr_GPIO_PIN       GPIO_Pin_13
/* 转速控制C(PB14) */
#define Cctr_GPIO_PORT      GPIOB
#define Cctr_GPIO_CLK       RCC_APB2Periph_GPIOB
#define Cctr_GPIO_PIN       GPIO_Pin_14


/** the macro definition to trigger the led on or off 
  * 1 - off
  *0 - on
  */
#define ON  0
#define OFF 1

/* 使用标准的固件库控制IO */
#define LED1(a)	if (a)  \
          GPIO_SetBits(LED1_GPIO_PORT,LED1_GPIO_PIN);\
          else  \
          GPIO_ResetBits(LED1_GPIO_PORT,LED1_GPIO_PIN)

#define LED2(a)	if (a)  \
          GPIO_SetBits(LED2_GPIO_PORT,LED2_GPIO_PIN);\
          else  \
          GPIO_ResetBits(LED2_GPIO_PORT,LED2_GPIO_PIN)

#define LED3(a)	if (a)  \
          GPIO_ResetBits(LED3_GPIO_PORT,LED3_GPIO_PIN);\
          else  \
          GPIO_SetBits(LED3_GPIO_PORT,LED3_GPIO_PIN)


/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)      {p->BSRR=i;}  //输出为高电平		
#define digitalLo(p,i)      {p->BRR=i;}   //输出低电平
#define digitalToggle(p,i)  {p->ODR ^=i;} //输出反转状态


/* 定义控制IO的宏 */
#define LED1_TOGGLE     digitalToggle(LED1_GPIO_PORT,LED1_GPIO_PIN)
#define LED1_OFF        digitalHi(LED1_GPIO_PORT,LED1_GPIO_PIN)
#define LED1_ON         digitalLo(LED1_GPIO_PORT,LED1_GPIO_PIN)

#define LED2_TOGGLE     digitalToggle(LED2_GPIO_PORT,LED2_GPIO_PIN)
#define LED2_OFF        digitalHi(LED2_GPIO_PORT,LED2_GPIO_PIN)
#define LED2_ON         digitalLo(LED2_GPIO_PORT,LED2_GPIO_PIN)

#define LED3_TOGGLE     digitalToggle(LED3_GPIO_PORT,LED3_GPIO_PIN)
#define LED3_OFF        digitalHi(LED3_GPIO_PORT,LED3_GPIO_PIN)
#define LED3_ON         digitalLo(LED3_GPIO_PORT,LED3_GPIO_PIN)
/* H_ctrl: OFF直通 ON停止 */
#define Hctr_OFF        GPIO_SetBits(Hctr_GPIO_PORT,Hctr_GPIO_PIN)
#define Hctr_ON         GPIO_ResetBits(Hctr_GPIO_PORT,Hctr_GPIO_PIN)
/* R_ctrl: OFF上楼 ON下楼 */
#define Rctr_OFF        GPIO_SetBits(Rctr_GPIO_PORT,Rctr_GPIO_PIN)
#define Rctr_ON         GPIO_ResetBits(Rctr_GPIO_PORT,Rctr_GPIO_PIN)
/* C_ctrl: OFF低速 ON高速 */
#define Cctr_OFF        GPIO_SetBits(Cctr_GPIO_PORT,Cctr_GPIO_PIN)
#define Cctr_ON         GPIO_ResetBits(Cctr_GPIO_PORT,Cctr_GPIO_PIN)


void LED_GPIO_Config(void);/* 初始化控制LED的IO */
void Ctrl_GPIO_Config(void);/* 初始化控制继电器的IO */
void All_GPIO_Config(void);

#endif /* __GPIO_H */

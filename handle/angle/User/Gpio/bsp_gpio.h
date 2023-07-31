#ifndef __GPIO_H
#define	__GPIO_H


#include "stm32f10x.h"


/* ����LED���ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����Ĵ��뼴�ɸı���Ƶ�LED���� */
/* R-��ɫ (����ָʾ��) */
#define LED1_GPIO_PORT      GPIOA                   /* GPIO�˿� */
#define LED1_GPIO_CLK       RCC_APB2Periph_GPIOA    /* GPIO�˿�ʱ�� */
#define LED1_GPIO_PIN       GPIO_Pin_6              /* ���ӵ�SCLʱ���ߵ�GPIO */
/* G-��ɫ (�ⲿָʾ��) */
#define LED2_GPIO_PORT      GPIOA
#define LED2_GPIO_CLK       RCC_APB2Periph_GPIOA
#define LED2_GPIO_PIN       GPIO_Pin_7
/* B-��ɫ (������) */
#define LED3_GPIO_PORT      GPIOC
#define LED3_GPIO_CLK       RCC_APB2Periph_GPIOC
#define LED3_GPIO_PIN       GPIO_Pin_13
/* �㶯����H(PB12) */
#define Hctr_GPIO_PORT      GPIOB
#define Hctr_GPIO_CLK       RCC_APB2Periph_GPIOB
#define Hctr_GPIO_PIN       GPIO_Pin_12
/* �������R(PB13) */
#define Rctr_GPIO_PORT      GPIOB
#define Rctr_GPIO_CLK       RCC_APB2Periph_GPIOB
#define Rctr_GPIO_PIN       GPIO_Pin_13
/* ת�ٿ���C(PB14) */
#define Cctr_GPIO_PORT      GPIOB
#define Cctr_GPIO_CLK       RCC_APB2Periph_GPIOB
#define Cctr_GPIO_PIN       GPIO_Pin_14


/** the macro definition to trigger the led on or off 
  * 1 - off
  *0 - on
  */
#define ON  0
#define OFF 1

/* ʹ�ñ�׼�Ĺ̼������IO */
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


/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)      {p->BSRR=i;}  //���Ϊ�ߵ�ƽ		
#define digitalLo(p,i)      {p->BRR=i;}   //����͵�ƽ
#define digitalToggle(p,i)  {p->ODR ^=i;} //�����ת״̬


/* �������IO�ĺ� */
#define LED1_TOGGLE     digitalToggle(LED1_GPIO_PORT,LED1_GPIO_PIN)
#define LED1_OFF        digitalHi(LED1_GPIO_PORT,LED1_GPIO_PIN)
#define LED1_ON         digitalLo(LED1_GPIO_PORT,LED1_GPIO_PIN)

#define LED2_TOGGLE     digitalToggle(LED2_GPIO_PORT,LED2_GPIO_PIN)
#define LED2_OFF        digitalHi(LED2_GPIO_PORT,LED2_GPIO_PIN)
#define LED2_ON         digitalLo(LED2_GPIO_PORT,LED2_GPIO_PIN)

#define LED3_TOGGLE     digitalToggle(LED3_GPIO_PORT,LED3_GPIO_PIN)
#define LED3_OFF        digitalHi(LED3_GPIO_PORT,LED3_GPIO_PIN)
#define LED3_ON         digitalLo(LED3_GPIO_PORT,LED3_GPIO_PIN)
/* H_ctrl: OFFֱͨ ONֹͣ */
#define Hctr_OFF        GPIO_SetBits(Hctr_GPIO_PORT,Hctr_GPIO_PIN)
#define Hctr_ON         GPIO_ResetBits(Hctr_GPIO_PORT,Hctr_GPIO_PIN)
/* R_ctrl: OFF��¥ ON��¥ */
#define Rctr_OFF        GPIO_SetBits(Rctr_GPIO_PORT,Rctr_GPIO_PIN)
#define Rctr_ON         GPIO_ResetBits(Rctr_GPIO_PORT,Rctr_GPIO_PIN)
/* C_ctrl: OFF���� ON���� */
#define Cctr_OFF        GPIO_SetBits(Cctr_GPIO_PORT,Cctr_GPIO_PIN)
#define Cctr_ON         GPIO_ResetBits(Cctr_GPIO_PORT,Cctr_GPIO_PIN)


void LED_GPIO_Config(void);/* ��ʼ������LED��IO */
void Ctrl_GPIO_Config(void);/* ��ʼ�����Ƽ̵�����IO */
void All_GPIO_Config(void);

#endif /* __GPIO_H */

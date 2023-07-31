/**
  ******************************************************************************
  * @file    bsp_gpio.c
  * @author  RX
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   led应用函数接口
  ******************************************************************************
  * @attention
  ******************************************************************************
  */
  
#include ".\gpio\bsp_gpio.h"

 /**
  * @brief  初始化控制LED的IO
  * @param  无
  * @retval 无
  */
void LED_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( LED1_GPIO_CLK | LED3_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    /* 推挽输出 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LED2_GPIO_PIN;
	GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LED3_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;    /* 开漏输出 */
	GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStructure);

	/* 关闭所有led灯 */
	LED1_OFF;
	LED2_OFF;
	LED3_OFF;
}

 /**
  * @brief  初始化控制继电器的IO
  * @param  无
  * @retval 无
  */
void Ctrl_GPIO_Config(void)
{
//GPIO_InitTypeDef GPIO_InitStructure;
//RCC_APB2PeriphClockCmd( Hctr_GPIO_CLK | Rctr_GPIO_CLK | Cctr_GPIO_CLK, ENABLE);
//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;    /* 开漏输出 */
//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
///* 点动控制H_ctrl */
//GPIO_InitStructure.GPIO_Pin = Hctr_GPIO_PIN;
//GPIO_Init(Hctr_GPIO_PORT, &GPIO_InitStructure);
///* 方向控制R_ctrl */
//GPIO_InitStructure.GPIO_Pin = Rctr_GPIO_PIN;
//GPIO_Init(Rctr_GPIO_PORT, &GPIO_InitStructure);
///* 转速控制C_ctrl */
//GPIO_InitStructure.GPIO_Pin = Cctr_GPIO_PIN;
//GPIO_Init(Cctr_GPIO_PORT, &GPIO_InitStructure);

///* 关闭所有继电器 */
//Hctr_OFF;
//Rctr_OFF;
//Cctr_OFF;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

 /**
  * @brief  不用的IO都关了
  * @param  无
  * @retval 无
  */
void All_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

  /*禁用JTAG,只使用SWD,解放PA15/PB3/PB4*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
//PWR_BackupAccessCmd(ENABLE);//允许修改RTC 和后备寄存器
//RCC_LSEConfig(RCC_LSE_OFF);//关闭外部低速外部时钟信号功能 后，PC13 PC14 PC15 才可以当普通IO用
//PWR_BackupAccessCmd(DISABLE);//禁止修改后备寄存器
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15|GPIO_Pin_14|GPIO_Pin_13|GPIO_Pin_12|GPIO_Pin_11|GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8
																|GPIO_Pin_7|GPIO_Pin_6|GPIO_Pin_5|GPIO_Pin_4|GPIO_Pin_3|GPIO_Pin_2|GPIO_Pin_1|GPIO_Pin_0;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15|GPIO_Pin_12|GPIO_Pin_11|GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7
																|GPIO_Pin_6|GPIO_Pin_5|GPIO_Pin_4|GPIO_Pin_3|GPIO_Pin_2|GPIO_Pin_1|GPIO_Pin_0;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}


void assert_failed(uint8_t* file, uint32_t line)
{
	// 断言错误时执行的代码
	LED1_ON;
}
/*********************************************END OF FILE**********************/

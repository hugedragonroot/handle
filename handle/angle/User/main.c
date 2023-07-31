/**
  ******************************************************************************
  * @file    main.c
  * @author  Rhui
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   
  ******************************************************************************
  * @attention
  * SYSCLK = 72M
  *
  ******************************************************************************
  */


#include "stm32f10x.h"
#include "main.h"


// 驱动器状态控制自定义变量结构体定义
Drive_CtrlDef     Drive;
Moto_CtrlDef      MotoA;
Moto_CtrlDef      MotoU;
Time_CtrlDef      Time;
CanTxMsg      TxMsg;  /* 发送缓存 */
#define CANID_SEND	0X590
uint8_t Access[20] = {0x03,0xFC};
uint16_t FlashRead2[16];
int16_t Flashtext1[6] = {-48, 1142, 1331, 226, 214, 15};
uint8_t abfle_raw[8] = {0};
//#define SIZE sizeof(Flashtext)		//数组长度


/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
  uint8_t data[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
  /* 使用HSI时,SYSCLK = 4M * RCC_PLLMul_x, x:[2,3,...16],最高是64MH */
//HSI_SetSysClock(RCC_PLLMul_2);    /* HSI_PLL_8M */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
/*-------------------------------  系统初始化  -------------------------------*/
  All_GPIO_Config();
  LED_GPIO_Config();
  Ctrl_GPIO_Config();
  AT_Equipment_Init();
  BASIC_TIM_Init();
  I2C_Bus_Init();
  CAN_Config();
  ADC_Config();
  AngleParam_Init();
  #ifdef RTC_Alarm
  RTC_CheckAndConfig(Initial_Time);
  #endif
  TIM_Cmd(BASIC_TIM, ENABLE);
  /* 初始化WWDG:配置计数器初始值,配置上窗口值,启动WWDG,使能提前唤醒中断 */
//  WWDG_Config(0X7F, 0X7F, WWDG_Prescaler_8);
  #ifdef DEBUG_USART
  USARTx_SendString(DEBUG_USARTx, "systerm init\r\n");
  USARTx_SendString(DEBUG_USARTx, "CAN Communication 500kbps\r\n");
  #endif
/*--------------------------------  复位判断  --------------------------------*/
  if(RCC_GetFlagStatus(RCC_FLAG_WWDGRST) != RESET)
  {
    #ifdef DEBUG_USART
    USARTx_SendString(DEBUG_USARTx, "窗口看门狗\r\n");
    #endif
    MotoA.Error |= WDGR;
  }
  RCC_ClearFlag();
  LED1_ON;
  if(MotoA.Error&WDGR)  TIMdelay_10Ms(150);
  else                  TIMdelay_10Ms(70);
/*--------------------------------  姿态识别  --------------------------------*/
  /* QMI8658C 初始化 */
  QMI8658C_Init();
  TIMdelay_10Ms(300);
//AngleData_Calibrate(20);
/*--------------------------------  系统启动  --------------------------------*/
  LED1_ON;
  LED2_OFF;
  Drive.WWDG_num = 0;
  Drive.MCU_State = 0x20;
//InFlash_WriteArray(FLASH_SAVE_ADDR, ((uint16_t*)Flashtext1), 6);
//InFlash_ReadHalfWord(FLASH_SAVE_ADDR, FlashRead2, 6);
  
  ChipAccBias[0]  = Flashtext1[0];
  ChipAccBias[1]  = Flashtext1[1];
  ChipAccBias[2]  = Flashtext1[2];
  ChipGyroBias[0] = Flashtext1[3];
  ChipGyroBias[1] = Flashtext1[4];
  ChipGyroBias[2] = Flashtext1[5];
  
  #ifdef DEBUG_USART
  USARTx_SendString(DEBUG_USARTx, "STM32正常运行\r\n");
  #endif
/*----------------------------------  END  -----------------------------------*/
  while(1)
  {
/*------------------------------  10ms定时功能  --------------------------------------------------*/
    if(Time.ms10 == 1)
    {
      adc_GetVoltage();
      adc_GetTemp();
    //MotoA.Vbus = AdcData.Vbus;
    //ProtocolSlaveCtrl();
      ProtocolIdentify(&loopPointCAN1);
      ProtocolIdentify(&loopPointUsart2);
    //ProtocolIdentify(&loopPointUsart3);
      Time.ms10 = 0;
    }/* Time.ms10 END */
/*------------------------------  20ms定时功能  --------------------------------------------------*/
    if(Time.ms20 == 1)
    {
      Time.ms20 = 0;
//			memcpy(&abfle_raw[0],(uint8_t *)&MotoA.Pitch,4);
//			memcpy(&abfle_raw[4],(uint8_t *)&MotoA.Roll,4);
			memcpy(&abfle_raw[0],(uint8_t *)&pitch_mpu,4);
			memcpy(&abfle_raw[4],(uint8_t *)&roll_mpu,4);
			CAN1_SendStdFrame(&TxMsg,CANID_SEND,abfle_raw,8);
    }/* Time.ms20 END */
/*------------------------------  50ms定时功能  --------------------------------------------------*/
    if(Time.ms50 == 1)
    {
      Time.ms50 = 0;
    }/* Time.ms50 END */
/*-----------------------------  100ms定时功能  --------------------------------------------------*/
    if(Time.ms100 == 1)
    {
      Time.ms100 = 0;
    }/* Time.ms100 END */
/*-----------------------------  1000ms定时功能  -------------------------------------------------*/
    if(Time.ms1000 == 1)
    {
      USART3_DMA_Send(data, 8);
      
      LED1_TOGGLE;
      LED2_TOGGLE;
      Remote.Battery = Slave.Battery;
      Time.ms1000 = 0;
    }/* Time.ms1000 END */
/*---------------------------------- 看门狗 ----------------------------------*/
    Drive.WWDG_num = 0;
/*----------------------------------- END ------------------------------------*/
  }
}

/**
  * @brief  IO口闪烁
  * @param  无
  * @retval 无
  */
void GPIO_shining(uint8_t Hight, uint8_t Low, uint8_t PinX)
{
  
}

/*********************************************** END OF FILE ************************************************/

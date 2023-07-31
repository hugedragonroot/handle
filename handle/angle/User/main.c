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


// ������״̬�����Զ�������ṹ�嶨��
Drive_CtrlDef     Drive;
Moto_CtrlDef      MotoA;
Moto_CtrlDef      MotoU;
Time_CtrlDef      Time;
CanTxMsg      TxMsg;  /* ���ͻ��� */
#define CANID_SEND	0X590
uint8_t Access[20] = {0x03,0xFC};
uint16_t FlashRead2[16];
int16_t Flashtext1[6] = {-48, 1142, 1331, 226, 214, 15};
uint8_t abfle_raw[8] = {0};
//#define SIZE sizeof(Flashtext)		//���鳤��


/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
  uint8_t data[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
  /* ʹ��HSIʱ,SYSCLK = 4M * RCC_PLLMul_x, x:[2,3,...16],�����64MH */
//HSI_SetSysClock(RCC_PLLMul_2);    /* HSI_PLL_8M */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
/*-------------------------------  ϵͳ��ʼ��  -------------------------------*/
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
  /* ��ʼ��WWDG:���ü�������ʼֵ,�����ϴ���ֵ,����WWDG,ʹ����ǰ�����ж� */
//  WWDG_Config(0X7F, 0X7F, WWDG_Prescaler_8);
  #ifdef DEBUG_USART
  USARTx_SendString(DEBUG_USARTx, "systerm init\r\n");
  USARTx_SendString(DEBUG_USARTx, "CAN Communication 500kbps\r\n");
  #endif
/*--------------------------------  ��λ�ж�  --------------------------------*/
  if(RCC_GetFlagStatus(RCC_FLAG_WWDGRST) != RESET)
  {
    #ifdef DEBUG_USART
    USARTx_SendString(DEBUG_USARTx, "���ڿ��Ź�\r\n");
    #endif
    MotoA.Error |= WDGR;
  }
  RCC_ClearFlag();
  LED1_ON;
  if(MotoA.Error&WDGR)  TIMdelay_10Ms(150);
  else                  TIMdelay_10Ms(70);
/*--------------------------------  ��̬ʶ��  --------------------------------*/
  /* QMI8658C ��ʼ�� */
  QMI8658C_Init();
  TIMdelay_10Ms(300);
//AngleData_Calibrate(20);
/*--------------------------------  ϵͳ����  --------------------------------*/
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
  USARTx_SendString(DEBUG_USARTx, "STM32��������\r\n");
  #endif
/*----------------------------------  END  -----------------------------------*/
  while(1)
  {
/*------------------------------  10ms��ʱ����  --------------------------------------------------*/
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
/*------------------------------  20ms��ʱ����  --------------------------------------------------*/
    if(Time.ms20 == 1)
    {
      Time.ms20 = 0;
//			memcpy(&abfle_raw[0],(uint8_t *)&MotoA.Pitch,4);
//			memcpy(&abfle_raw[4],(uint8_t *)&MotoA.Roll,4);
			memcpy(&abfle_raw[0],(uint8_t *)&pitch_mpu,4);
			memcpy(&abfle_raw[4],(uint8_t *)&roll_mpu,4);
			CAN1_SendStdFrame(&TxMsg,CANID_SEND,abfle_raw,8);
    }/* Time.ms20 END */
/*------------------------------  50ms��ʱ����  --------------------------------------------------*/
    if(Time.ms50 == 1)
    {
      Time.ms50 = 0;
    }/* Time.ms50 END */
/*-----------------------------  100ms��ʱ����  --------------------------------------------------*/
    if(Time.ms100 == 1)
    {
      Time.ms100 = 0;
    }/* Time.ms100 END */
/*-----------------------------  1000ms��ʱ����  -------------------------------------------------*/
    if(Time.ms1000 == 1)
    {
      USART3_DMA_Send(data, 8);
      
      LED1_TOGGLE;
      LED2_TOGGLE;
      Remote.Battery = Slave.Battery;
      Time.ms1000 = 0;
    }/* Time.ms1000 END */
/*---------------------------------- ���Ź� ----------------------------------*/
    Drive.WWDG_num = 0;
/*----------------------------------- END ------------------------------------*/
  }
}

/**
  * @brief  IO����˸
  * @param  ��
  * @retval ��
  */
void GPIO_shining(uint8_t Hight, uint8_t Low, uint8_t PinX)
{
  
}

/*********************************************** END OF FILE ************************************************/

/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI
  
  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/


#include "stm32f10x_it.h"
#include "main.h"


/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
	#ifdef DEBUG_USART
	USARTx_SendString(DEBUG_USARTx, "\r\n Ӳ������,�ڴ�|��ջ���/����Խ��\r\n");
	#endif
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
	#ifdef DEBUG_USART
	USARTx_SendString(DEBUG_USARTx, "\r\n �ڴ�����쳣\r\n");
	#endif
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
	#ifdef DEBUG_USART
	USARTx_SendString(DEBUG_USARTx, "\r\n ���߹���\r\n");
	#endif
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
	#ifdef DEBUG_USART
	USARTx_SendString(DEBUG_USARTx, "\r\n �����÷�����\r\n");
	#endif
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  USB��CAN����FIFO0�����ж�
  * @param  None
  * @retval None
  */
void USB_LP_CAN1_RX0_IRQHandler(void)
{
  if( CAN_GetITStatus(CAN1, CAN_IT_FMP0) )
  {
    CAN_Receive(CAN1, CAN_FIFO0, &RxMsgFMP0);/* �������ж������� */
    CAN_ProtocolExt(&RxMsgFMP0, &CanStr1);
    CanStr1.RxBuf[CanStr1.RxCNT] = CCOM1;
    LoopQueue_Write(&loopPointCAN1, CanStr1.RxBuf, CanStr1.RxCNT+1);
    memset(CanStr1.RxBuf, 0, 16);
    CanStr1.RxEN = true;
  }
}

/**
  * @brief  CAN����FIFO1�����ж�
  * @param  None
  * @retval None
  */
void CAN1_RX1_IRQHandler(void)
{
  if( CAN_GetITStatus(CAN1, CAN_IT_FMP1) )
  {
  //CAN_Receive(CAN1, CAN_FIFO1, &RxMsgFMP0);/* �������ж������� */
    CAN_Receive(CAN1, CAN_FIFO1, &RxMsgFMP1);/* �������ж������� */
    CAN_ProtocolExt(&RxMsgFMP1, &CanStr1);
    CanStr1.RxBuf[CanStr1.RxCNT] = CCOM1;
    LoopQueue_Write(&loopPointCAN1, CanStr1.RxBuf, CanStr1.RxCNT+1);
    memset(CanStr1.RxBuf, 0, 16);
    CanStr1.RxEN = true;
  }
}

/**
  * @brief  USART1�жϺ���
  * @param  ��
  * @retval ��
  */
void USART1_IRQHandler()
{
  if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
  {
    USART_ReceiveData(USART1);
    DMA_Cmd(DMA1_Channel5, DISABLE);
    UsartStr1.RxCNT = StreamLen - DMA_GetCurrDataCounter(DMA1_Channel5);
    UsartStr1.RxBuf[UsartStr1.RxCNT] = UCOM1;
    LoopQueue_Write(&loopPointUsart1, UsartStr1.RxBuf, UsartStr1.RxCNT+1);
    memset(UsartStr1.RxBuf, 0, StreamLen);
    DMA_SetCurrDataCounter(DMA1_Channel5, StreamLen);
    DMA_Cmd(DMA1_Channel5, ENABLE);
    UsartStr1.RxEN = true;
  }
}

/**
  * @brief  DMA1 Channel4 UART1_TX
  * @param  None
  * @retval None
  */
void DMA1_Channel4_IRQHandler(void)
{
  if(DMA_GetITStatus(DMA1_IT_TC4) != RESET)
  {
    DMA_Cmd(DMA1_Channel4, DISABLE);
    UsartStr1.TxEN = true;
    /* Clear the DMA1_IT_TC4 pending bit */
    DMA_ClearITPendingBit(DMA1_IT_TC4);
  }
}

/**
  * @brief  USART2�жϺ���
  * @param  ��
  * @retval ��
  */
void USART2_IRQHandler()
{
  if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
  {
    USART_ReceiveData(USART2);
    DMA_Cmd(DMA1_Channel6, DISABLE);
    UsartStr2.RxCNT = StreamLen - DMA_GetCurrDataCounter(DMA1_Channel6);
    UsartStr2.RxBuf[UsartStr2.RxCNT] = UCOM2;
    LoopQueue_Write(&loopPointUsart2, UsartStr2.RxBuf, UsartStr2.RxCNT+1);
    memset(UsartStr2.RxBuf, 0, StreamLen);
    DMA_SetCurrDataCounter(DMA1_Channel6, StreamLen);
    DMA_Cmd(DMA1_Channel6, ENABLE);
    UsartStr2.RxEN = true;
  }
}

/**
  * @brief  DMA1 Channel7 UART2_TX
  * @param  None
  * @retval None
  */
void DMA1_Channel7_IRQHandler(void)
{
  if(DMA_GetITStatus(DMA1_IT_TC7) != RESET)
  {
    DMA_Cmd(DMA1_Channel7, DISABLE);
    UsartStr2.TxEN = true;
    /* Clear the DMA1_IT_TC7 pending bit */
    DMA_ClearITPendingBit(DMA1_IT_TC7);
  }
}

/**
  * @brief  USART3�жϺ���
  * @param  ��
  * @retval ��
  */
void USART3_IRQHandler()
{
  if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
  {
    USART_ReceiveData(USART3);
    DMA_Cmd(DMA1_Channel3, DISABLE);
    UsartStr3.RxCNT = StreamLen - DMA_GetCurrDataCounter(DMA1_Channel3);
    UsartStr3.RxBuf[UsartStr3.RxCNT] = UCOM3;
    LoopQueue_Write(&loopPointUsart3, UsartStr3.RxBuf, UsartStr3.RxCNT+1);
  //memset(UsartStr3.RxBuf, 0, StreamLen);
    DMA_SetCurrDataCounter(DMA1_Channel3, StreamLen);
    DMA_Cmd(DMA1_Channel3, ENABLE);
    UsartStr3.RxEN = true;
    USART3_DMA_Send(UsartStr3.RxBuf, UsartStr3.RxCNT+1);
  }
}

/**
  * @brief  DMA1 Channel2 UART3_TX
  * @param  None
  * @retval None
  */
void DMA1_Channel2_IRQHandler(void)
{
  if(DMA_GetITStatus(DMA1_IT_TC2) != RESET)
  {
    DMA_Cmd(DMA1_Channel2, DISABLE);
    UsartStr3.TxEN = true;
    GPIO_ResetBits(GPIOB, GPIO_Pin_2);  /* USART3_DR */
    /* Clear the DMA1_IT_TC2 pending bit */
    DMA_ClearITPendingBit(DMA1_IT_TC2);
  }
}

/**
  * @brief  ������ʱ��TIM6�жϴ��� ( 10ms��ʱ )
  * @param  None
  * @retval None
  */
void  BASIC_TIM_IRQHandler (void)
{
  static uint8_t wwdg_num;
  
  if(TIM_GetITStatus(BASIC_TIM, TIM_IT_Update) != RESET)
  {
/*------------------------------  10ms��ʱ����  --------------------------------------------------*/
    TIMdelay_Decrement();
    Time.CNT ++;
    Time.ms10 = 1;
/*--------------------------------  ���½Ƕ�  --------------------------------*/        /* 1.1 */
    if(!(MotoA.Error&MPU6) && !ChipCorrect)
    {
      QMI8658C_ReadData();
      AngleData_Convert();
      /* ��������ת����ͬ��Ϊ��,����Ϊ�� */
      MotoA.Pitch = Kalman_Filter_X(ChipAccf[0], ChipGyrof[0]);
    //MotoA.Pitch = SecondOrderFiltering(&AngleP, ChipAccf[0], ChipGyrof[0]);
    //MotoA.Roll  = Kalman_Filter_Y(-ChipAccf[1], ChipGyrof[1]);
      MotoA.Roll  = SecondOrderFiltering(&AngleR, -ChipAccf[1], ChipGyrof[1]);
    }
/*------------------------------  20ms��ʱ����  --------------------------------------------------*/
    if(Time.CNT%2 == 0)
    {
      Time.ms20 = 1;
/*--------------------------------  �¶ȼ��  --------------------------------*/        /* 2.3 */
      MotoA.pTemp = ChipTemp;
      if(MotoA.pTemp > TEMP_Limit && (MotoA.Error&TEMP) != TEMP)
      {
        MotoA.Error |= TEMP;
        #ifdef DEBUG_USART
        USARTx_SendString( DEBUG_USARTx,"ERR_ADCI\r\n");
        #endif
      }
      else if(MotoA.pTemp < (TEMP_Limit-100) && MotoA.Error&TEMP)
      {
        MotoA.Error &= (~TEMP);
        LED3_OFF;
      }
/*--------------------------------  ���߻���  --------------------------------*/        /* 0.0 */
      if(Drive.MCU_State)
      {
        Access[2] = MotoA.Pitch&0xff;
        Access[3] = MotoA.Pitch>>8;
        Access[4] = MotoA.Roll&0xff;
        Access[5] = MotoA.Roll>>8;
        Access[6] = MotoA.Yaw&0xff;
        Access[7] = MotoA.Yaw>>8;
        Access[8] = MotoA.pTemp&0xff;
        Access[9] = MotoA.pTemp>>8;
        Access[10] = 0x00&0xff;
        Access[11] = 0x00>>8;
        Access[12] = 0x00&0xff;
        Access[13] = 0x00>>8;
        Access[14] = AdcData.Vbus&0xff;
        Access[15] = AdcData.Vbus>>8;
        Access[16] = AdcData.TfbS&0xff;
        Access[17] = AdcData.TfbS>>8;
        
//        Access[2]   = ChipAccData[0]&0xff;
//        Access[3]   = ChipAccData[0]>>8;
//        Access[4]   = ChipAccData[1]&0xff;
//        Access[5]   = ChipAccData[1]>>8;
//        Access[6]   = ChipAccData[2]&0xff;
//        Access[7]   = ChipAccData[2]>>8;
//        Access[8]   = ChipGyroData[0]&0xff;
//        Access[9]   = ChipGyroData[0]>>8;
//        Access[10]  = ChipGyroData[1]&0xff;;
//        Access[11]  = ChipGyroData[1]>>8;
//        Access[12]  = ChipGyroData[2]&0xff;;
//        Access[13]  = ChipGyroData[2]>>8;
//        Access[14]  = 0x00;
//        Access[15]  = 0x00;
//        Access[16]  = MotoA.pTemp&0xff;
//        Access[17]  = MotoA.pTemp>>8;
        
        Access[18] = 0xFC;
        Access[19] = 0x03;
        USART1_DMA_Send(Access, 20);
      }
    }
/*------------------------------  50ms��ʱ����  --------------------------------------------------*/
    if(Time.CNT%5 == 0)
    {
      Time.ms50 = 1;
    }
/*-----------------------------  100ms��ʱ����  --------------------------------------------------*/
    if(Time.CNT%10 == 0)
    {
      Time.ms100 = 1;
    }
/*-------------------------------  1000ms��ʱ  ---------------------------------------------------*/
    if(Time.CNT%100 == 0)
    {
      Time.ms1000 = 1;
      Time.CNT = 0;
    }
/*------------------------------  30ms��ʱι��  ------------------------------*/        /* 7.1 */
    wwdg_num ++;
    Drive.WWDG_num ++;                  /* ���Ź����� */
    if(wwdg_num >= 3 && Drive.WWDG_num < 1000)
    {
      WWDG_SetCounter( 0X7F );
      wwdg_num = 0;
    }
/*----------------------------------- END ------------------------------------*/
    TIM_ClearITPendingBit(BASIC_TIM, TIM_FLAG_Update);
  }
  else
    TIM_ClearFlag(BASIC_TIM, TIM_FLAG_Update);
}

#ifdef RTC_Alarm
/**
  * @brief  RTC�ж��¼�����
  * @param  None
  * @retval None
  */
void RTC_IRQHandler(void)
{
  if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
  {
    /* Clear the RTC Second interrupt */
    RTC_ClearITPendingBit(RTC_IT_SEC);
    /* Enable time update */
  //TimeDisplay = 1;
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
  }
  /*����*/
  if (RTC_GetITStatus(RTC_IT_ALR) != RESET)
  {
    #ifdef DEBUG_USART
    USARTx_SendString(DEBUG_USARTx, "RTC Time Alarm...\n");
    #endif
    /* �����ڼ���ʱ�����д�뵽RTC�����Ĵ��� */
    RTC_SetCounter(Initial_Time);
    /* �ȴ�ȷ����һ�β������ */
    RTC_WaitForLastTask();
  //TimeAlarm  = 1 ;
    /* Clear the RTC Alarm interrupt */
    RTC_ClearITPendingBit(RTC_IT_ALR);
  }
  else
    RTC_ClearITPendingBit(RTC_IT_ALR|RTC_IT_SEC);
}

/**
  * @brief  RTC�����¼��жϴ���
  * @param  None
  * @retval None
  */
void RTCAlarm_IRQHandler(void)
{
/*--------------------------  ȷ��RTC�����¼�EXTI  ---------------------------*/
  if( EXTI_GetITStatus(EXTI_Line17) != RESET )
  {
    #ifdef DEBUG_USART
    USARTx_SendString(DEBUG_USARTx, "RTC Alarm EXTI\n");
    #endif
/*----------------------------------- END ------------------------------------*/
    EXTI_ClearITPendingBit(EXTI_Line17);
  }
  else
    EXTI_ClearFlag(EXTI_Line17);
}
#endif

/**
  * @brief  WWDG �жϷ�����
  *         ��������˴��ж�,��ʾ�����Ѿ������˹���,
  *         ����һ����ǰ�ж�.�ڴ��жϷ��������Ӧ�ø�����Ҫ����,
  *         ���籣����Ҫ�����ݵ�,���ʱ������ж೤
  *         ��WDGTB��ֵ����:
  *         WDGTB:0   113us
  *         WDGTB:1   227us
  *         WDGTB:2   455us
  *         WDGTB:3   910us
  * @param  None
  * @retval None
  */
void WWDG_IRQHandler(void)
{
  /* LED1��˸,LEDֻ��ʾ���ԵĲ��� */
  /* ����ʹ�õ�ʱ��,����Ӧ����������Ҫ������ */
//if( Drive.EXTI_Function != ZERO )
//  WWDG_SetCounter( 0X7F );
//else
    LED1_ON; 
  /* ����жϱ�־λ */
  WWDG_ClearFlag();
}


/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

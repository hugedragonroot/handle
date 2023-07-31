/**
  ******************************************************************************
  * @file    bsp_rtc.c
  * @author  Rhui
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   RTC���ж�/�����ж�
  ******************************************************************************
  * @attention
  ******************************************************************************
  */ 
#include ".\rtc\bsp_rtc.h"
#include ".\ATusart\bsp_ATusart.h"


/* ���жϱ�־���������ж�ʱ��1����ʱ�䱻ˢ��֮����0 */
__IO uint32_t TimeDisplay = 0;
/*���������־�����ж��������¼���1*/
__IO uint32_t TimeAlarm = 0;


/**
  * @brief  RTC����EXTI�ж�����
  * @param  None
  * @retval None
  */
static void RTC_EXTI_INITIAL(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure; 
/*------------���� �ж�-------------------*/
	/* Configure one bit for preemption priority */
//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	/* Enable the RTC Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = RTCAlarm_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
/*------------EXTI ����-------------------*/
	EXTI_InitStructure.EXTI_Line = EXTI_Line17;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_ClearFlag(EXTI_Line17);
	EXTI_Init(&EXTI_InitStructure); 
/*----------------------------------------*/
}

/**
  * @brief  RTCǶ�������ж�����
  * @param  None
  * @retval None
  */
static void RTC_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	/* Configure one bit for preemption priority */
//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	/* Enable the RTC Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Configures the RTC.
  * @param  None
  * @retval None
  */
static void RTC_Configuration(void)
{
	/* ʹ�� PWR �� Backup ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	/* ������� Backup ���� */
	PWR_BackupAccessCmd(ENABLE);
	/* ��λ Backup ���� */
	BKP_DeInit();
	
/* ʹ���ⲿʱ�ӻ����ڲ�ʱ��(��bsp_rtc.h�ļ�����)
   ʹ���ⲿʱ��ʱ,����Щ����¾�������
   ������Ʒ��ʱ��,�����׳����ⲿ������������,��̫�ɿ� */
#ifdef 	RTC_CLOCK_SOURCE_LSE
	/* ʹ�� LSE */
	RCC_LSEConfig(RCC_LSE_ON);
	/* �ȴ� LSE ׼���� */
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
	/* ѡ�� LSE ��Ϊ RTC ʱ��Դ */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	/* ʹ�� RTC ʱ�� */
	RCC_RTCCLKCmd(ENABLE);
	/* �ȴ� RTC �Ĵ��� ͬ��
	 * ��ΪRTCʱ���ǵ��ٵ�,�ڻ�ʱ���Ǹ��ٵ�,����Ҫͬ��
	 */
	RTC_WaitForSynchro();
	/* ȷ����һ�� RTC �Ĳ������ */
	RTC_WaitForLastTask();
	/* ʹ�� RTC ���ж� */
	RTC_ITConfig(RTC_IT_SEC|RTC_IT_ALR, ENABLE);
	/* ȷ����һ�� RTC �Ĳ������ */
	RTC_WaitForLastTask();
	/* ���� RTC ��Ƶ: ʹ RTC ����Ϊ1s  */
	/* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) = 1HZ */
	RTC_SetPrescaler(32767); 
	/* ȷ����һ�� RTC �Ĳ������ */
	RTC_WaitForLastTask();
#else
	/* ʹ�� LSI */
	RCC_LSICmd(ENABLE);
	/* �ȴ� LSI ׼���� */
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
	/* ѡ�� LSI ��Ϊ RTC ʱ��Դ */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	/* ʹ�� RTC ʱ�� */
	RCC_RTCCLKCmd(ENABLE);
	/* �ȴ� RTC �Ĵ��� ͬ��
	 * ��ΪRTCʱ���ǵ��ٵ�,�ڻ�ʱ���Ǹ��ٵ�,����Ҫͬ��
	 */
	RTC_WaitForSynchro();
	/* ȷ����һ�� RTC �Ĳ������ */
	RTC_WaitForLastTask();
	/* ʹ�� RTC ���ж� */
	RTC_ITConfig(RTC_IT_SEC|RTC_IT_ALR, ENABLE);
	/* ȷ����һ�� RTC �Ĳ������ */
	RTC_WaitForLastTask();
	/* ���� RTC ��Ƶ: ʹ RTC ����Ϊ1s ,LSIԼΪ40KHz */
	/* RTC period = RTCCLK/RTC_PR = (40 KHz)/(40000-1+1) = 1HZ */	
	RTC_SetPrescaler(40000-1); 
	/* ȷ����һ�� RTC �Ĳ������ */
	RTC_WaitForLastTask();
#endif
}

/**
  * @brief  ��鲢����RTC
  * @param  None
  * @retval None
  */
void RTC_CheckAndConfig(uint32_t RTCtime)
{
	RTC_NVIC_Config();
/*������ʱ��鱸�ݼĴ���BKP_DR1,������ݲ���0xA5A5,
	������������ʱ�䲢ѯ���û�����ʱ��*/
	if (BKP_ReadBackupRegister(RTC_BKP_DRX) != RTC_BKP_DATA)
	{
		#ifdef DEBUG_USART
		USARTx_SendString( DEBUG_USARTx,"RTC not yet configured...\n");
		USARTx_SendString( DEBUG_USARTx,"RTC configured...\n");
		#endif
		/* RTC ���� */
		RTC_Configuration();
		RTC_SetCounter(RTCtime);
		/* �ȴ�ȷ����һ�β������ */
		RTC_WaitForLastTask();
		/*��BKP_DR1�Ĵ���д���־��˵��RTC��������*/
		BKP_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
	}
	else
	{
		/* ʹ�� PWR �� Backup ʱ�� */
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
		/* ������� Backup ���� */
	  PWR_BackupAccessCmd(ENABLE);
	  /*LSE��������������ʱ��*/
		#ifdef RTC_CLOCK_SOURCE_LSI		
		/* ʹ�� LSI */
		RCC_LSICmd(ENABLE);
		/* �ȴ� LSI ׼���� */
		while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
		#endif
		/*����Ƿ��������*/
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
		{
			#ifdef DEBUG_USART
			USARTx_SendString( DEBUG_USARTx,"Power On Reset occurred...\n");
			#endif
		}
		/*����Ƿ�Reset��λ*/
		else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
		{
			#ifdef DEBUG_USART
			USARTx_SendString( DEBUG_USARTx,"External Reset occurred...\n");
			#endif
		}
		#ifdef DEBUG_USART
		USARTx_SendString( DEBUG_USARTx,"No need to configure RTC...\n");
		#endif
		/*�ȴ��Ĵ���ͬ��*/
		RTC_WaitForSynchro();
		/*����RTC���ж�,�����ж�*/
	//RTC_ITConfig(RTC_IT_SEC|RTC_IT_ALR, ENABLE);
		RTC_ITConfig(RTC_IT_ALR, ENABLE);
		/*�ȴ��ϴ�RTC�Ĵ���д�������*/
		RTC_WaitForLastTask();
	}
	/*������ʱ������꣬������У��ʱ�������PC13*/
	#ifdef RTCClockOutput_Enable
	/* ��ֹ Tamper ���� */
	/* Ҫ��� RTCCLK/64 �� Tamper ����,  tamper ���ܱ����ֹ */	
	BKP_TamperPinCmd(DISABLE); 
	/* ʹ�� RTC ʱ������� Tamper ���� */
	BKP_RTCOutputConfig(BKP_RTCOutputSource_CalibClock);
	#endif
	/* �����λ��־ flags */
	RCC_ClearFlag();
	/*�������ӼĴ���*/
	RTC_SetAlarm(Alarm_Time);
	/* RTC����EXTI�ж����� */
	RTC_EXTI_INITIAL();
}

/************************END OF FILE***************************************/

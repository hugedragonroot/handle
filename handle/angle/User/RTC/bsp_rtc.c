/**
  ******************************************************************************
  * @file    bsp_rtc.c
  * @author  Rhui
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   RTC秒中断/闹钟中断
  ******************************************************************************
  * @attention
  ******************************************************************************
  */ 
#include ".\rtc\bsp_rtc.h"
#include ".\ATusart\bsp_ATusart.h"


/* 秒中断标志，进入秒中断时置1，当时间被刷新之后清0 */
__IO uint32_t TimeDisplay = 0;
/*闹钟响铃标志，在中断中闹钟事件致1*/
__IO uint32_t TimeAlarm = 0;


/**
  * @brief  RTC闹钟EXTI中断配置
  * @param  None
  * @retval None
  */
static void RTC_EXTI_INITIAL(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure; 
/*------------设置 中断-------------------*/
	/* Configure one bit for preemption priority */
//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	/* Enable the RTC Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = RTCAlarm_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
/*------------EXTI 配置-------------------*/
	EXTI_InitStructure.EXTI_Line = EXTI_Line17;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_ClearFlag(EXTI_Line17);
	EXTI_Init(&EXTI_InitStructure); 
/*----------------------------------------*/
}

/**
  * @brief  RTC嵌套向量中断配置
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
	/* 使能 PWR 和 Backup 时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	/* 允许访问 Backup 区域 */
	PWR_BackupAccessCmd(ENABLE);
	/* 复位 Backup 区域 */
	BKP_DeInit();
	
/* 使用外部时钟还是内部时钟(在bsp_rtc.h文件定义)
   使用外部时钟时,在有些情况下晶振不起振
   批量产品的时候,很容易出现外部晶振不起振的情况,不太可靠 */
#ifdef 	RTC_CLOCK_SOURCE_LSE
	/* 使能 LSE */
	RCC_LSEConfig(RCC_LSE_ON);
	/* 等待 LSE 准备好 */
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
	/* 选择 LSE 作为 RTC 时钟源 */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	/* 使能 RTC 时钟 */
	RCC_RTCCLKCmd(ENABLE);
	/* 等待 RTC 寄存器 同步
	 * 因为RTC时钟是低速的,内环时钟是高速的,所以要同步
	 */
	RTC_WaitForSynchro();
	/* 确保上一次 RTC 的操作完成 */
	RTC_WaitForLastTask();
	/* 使能 RTC 秒中断 */
	RTC_ITConfig(RTC_IT_SEC|RTC_IT_ALR, ENABLE);
	/* 确保上一次 RTC 的操作完成 */
	RTC_WaitForLastTask();
	/* 设置 RTC 分频: 使 RTC 周期为1s  */
	/* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) = 1HZ */
	RTC_SetPrescaler(32767); 
	/* 确保上一次 RTC 的操作完成 */
	RTC_WaitForLastTask();
#else
	/* 使能 LSI */
	RCC_LSICmd(ENABLE);
	/* 等待 LSI 准备好 */
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
	/* 选择 LSI 作为 RTC 时钟源 */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	/* 使能 RTC 时钟 */
	RCC_RTCCLKCmd(ENABLE);
	/* 等待 RTC 寄存器 同步
	 * 因为RTC时钟是低速的,内环时钟是高速的,所以要同步
	 */
	RTC_WaitForSynchro();
	/* 确保上一次 RTC 的操作完成 */
	RTC_WaitForLastTask();
	/* 使能 RTC 秒中断 */
	RTC_ITConfig(RTC_IT_SEC|RTC_IT_ALR, ENABLE);
	/* 确保上一次 RTC 的操作完成 */
	RTC_WaitForLastTask();
	/* 设置 RTC 分频: 使 RTC 周期为1s ,LSI约为40KHz */
	/* RTC period = RTCCLK/RTC_PR = (40 KHz)/(40000-1+1) = 1HZ */	
	RTC_SetPrescaler(40000-1); 
	/* 确保上一次 RTC 的操作完成 */
	RTC_WaitForLastTask();
#endif
}

/**
  * @brief  检查并配置RTC
  * @param  None
  * @retval None
  */
void RTC_CheckAndConfig(uint32_t RTCtime)
{
	RTC_NVIC_Config();
/*在启动时检查备份寄存器BKP_DR1,如果内容不是0xA5A5,
	则需重新配置时间并询问用户调整时间*/
	if (BKP_ReadBackupRegister(RTC_BKP_DRX) != RTC_BKP_DATA)
	{
		#ifdef DEBUG_USART
		USARTx_SendString( DEBUG_USARTx,"RTC not yet configured...\n");
		USARTx_SendString( DEBUG_USARTx,"RTC configured...\n");
		#endif
		/* RTC 配置 */
		RTC_Configuration();
		RTC_SetCounter(RTCtime);
		/* 等待确保上一次操作完成 */
		RTC_WaitForLastTask();
		/*向BKP_DR1寄存器写入标志，说明RTC已在运行*/
		BKP_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
	}
	else
	{
		/* 使能 PWR 和 Backup 时钟 */
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
		/* 允许访问 Backup 区域 */
	  PWR_BackupAccessCmd(ENABLE);
	  /*LSE启动无需设置新时钟*/
		#ifdef RTC_CLOCK_SOURCE_LSI		
		/* 使能 LSI */
		RCC_LSICmd(ENABLE);
		/* 等待 LSI 准备好 */
		while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
		#endif
		/*检查是否掉电重启*/
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
		{
			#ifdef DEBUG_USART
			USARTx_SendString( DEBUG_USARTx,"Power On Reset occurred...\n");
			#endif
		}
		/*检查是否Reset复位*/
		else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
		{
			#ifdef DEBUG_USART
			USARTx_SendString( DEBUG_USARTx,"External Reset occurred...\n");
			#endif
		}
		#ifdef DEBUG_USART
		USARTx_SendString( DEBUG_USARTx,"No need to configure RTC...\n");
		#endif
		/*等待寄存器同步*/
		RTC_WaitForSynchro();
		/*允许RTC秒中断,闹钟中断*/
	//RTC_ITConfig(RTC_IT_SEC|RTC_IT_ALR, ENABLE);
		RTC_ITConfig(RTC_IT_ALR, ENABLE);
		/*等待上次RTC寄存器写操作完成*/
		RTC_WaitForLastTask();
	}
	/*定义了时钟输出宏，则配置校正时钟输出到PC13*/
	#ifdef RTCClockOutput_Enable
	/* 禁止 Tamper 引脚 */
	/* 要输出 RTCCLK/64 到 Tamper 引脚,  tamper 功能必须禁止 */	
	BKP_TamperPinCmd(DISABLE); 
	/* 使能 RTC 时钟输出到 Tamper 引脚 */
	BKP_RTCOutputConfig(BKP_RTCOutputSource_CalibClock);
	#endif
	/* 清除复位标志 flags */
	RCC_ClearFlag();
	/*设置闹钟寄存器*/
	RTC_SetAlarm(Alarm_Time);
	/* RTC闹钟EXTI中断配置 */
	RTC_EXTI_INITIAL();
}

/************************END OF FILE***************************************/

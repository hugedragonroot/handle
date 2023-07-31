/**
  ******************************************************************************
  * @file    bsp_TiMbase.c
  * @author  Rhui
  * @version V1.0
  * @date    2018-01-18 : ��������,��һ��У��
  * @brief   ������ʱ����ʱ�ж�Ӧ��bsp
  ******************************************************************************
  */

#include ".\TiMbase\bsp_TiMbase.h" 

static __IO uint16_t TIMdelay;

/**
  * @brief  ���û�����ʱ��Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
static void BASIC_TIM_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	/* �����ж���Ϊ2 */
//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	/* �����ж���Դ */
	NVIC_InitStructure.NVIC_IRQChannel = BASIC_TIM_IRQ ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*
 * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
 * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
 * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
 *-----------------------------------------------------------------------------
 *typedef struct
 *{ TIM_Prescaler            ����
 *	TIM_CounterMode			     TIMx,x[6,7]û�У���������
 *  TIM_Period               ����
 *  TIM_ClockDivision        TIMx,x[6,7]û�У���������
 *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]����
 *}TIM_TimeBaseInitTypeDef;
 *-----------------------------------------------------------------------------
 */

/**
  * @brief  ������ʱ����������
  * @param  ��
  * @retval ��
  */
static void BASIC_TIM_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	/* ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M */
	BASIC_TIM_APBxClock_FUN(BASIC_TIM_CLK, ENABLE);
	/* �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж� */
	TIM_TimeBaseStructure.TIM_Period = BASIC_TIM_Period;
	/* ʱ��Ԥ��Ƶ��Ϊ */
	TIM_TimeBaseStructure.TIM_Prescaler= BASIC_TIM_Prescaler;
#ifdef  BASIC_TIM6 /* ʹ�û�����ʱ��TIM6 */
#else              /* ʹ��ͨ�ö�ʱ��TIM4 */
	/* ʹ��ͨ�ö�ʱ������������������,�������ڸ��Ż��ȼ��»�ʧЧ */
	/* ʱ�ӷ�Ƶ����,������ʱ��û��,���ù� */
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	/* ����������ģʽ,������ʱ��ֻ�����ϼ���,û�м���ģʽ������ */
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	/* �ظ���������ֵ,������ʱ��û��,���ù� */
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
#endif
	/* ��ʼ����ʱ�� */
	TIM_TimeBaseInit(BASIC_TIM, &TIM_TimeBaseStructure);
	/* ����������жϱ�־λ */
	TIM_ClearFlag(BASIC_TIM, TIM_FLAG_Update);
	/* �����������ж� */
	TIM_ITConfig(BASIC_TIM,TIM_IT_Update,ENABLE);
	/* ���������� */
//TIM_Cmd(BASIC_TIM, ENABLE);
	/* �رռ����� */
	TIM_Cmd(BASIC_TIM, DISABLE);
}

/**
  * @brief  ��ʼ��������ʱ��
  * @param  ��
  * @retval ��
  */
void BASIC_TIM_Init(void)
{
	BASIC_TIM_NVIC_Config();
	BASIC_TIM_Mode_Config();
}

/**
  * @brief  ��ȡTIM�жϽ���
  * @param  ��
  * @retval ��
  * @attention  �жϺ���BASIC_TIM_IRQHandler����
  */
void TIMdelay_Decrement(void)
{
	if(TIMdelay != 0x00)
		TIMdelay--;
	else
	if(TIMdelay > 60000)
		TIMdelay = 100;
}

/**
  * @brief  TIM��ʱ����ʱ
  * @param  ��
  * @retval ��
  */
void TIMdelay_10Ms(uint16_t ms)
{
	TIMdelay = ms;	
	while(TIMdelay != 0);
}

/*********************************************END OF FILE**********************/

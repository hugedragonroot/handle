/**
  ******************************************************************************
  * @file    bsp_adc.c
  * @author  Rhui
  * @version V1.0
  * @date    2018-03-16
  * @brief   ADC应用bsp
  ******************************************************************************
  */

#include ".\adc\bsp_adc.h"


TADC_DATA AdcData;
__IO uint16_t adcDMAvalue[ADC_SampleNum][ADC_ChannelNum];


/**
  * @brief  ADC初始化
  * @param  无
  * @retval 无
  */
void ADC_Config(void)
{
  ADC_InitTypeDef   ADC_InitStructure;
  DMA_InitTypeDef   DMA_InitStructure;
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  RCC_ADCCLKConfig(RCC_PCLK2_Div6);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  ADC_StructInit(&ADC_InitStructure);
  ADC_InitStructure.ADC_Mode                = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode        = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode  = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv    = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign           = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel        = ADC_ChannelNum;
  ADC_Init(ADC1, &ADC_InitStructure);
  /* 配置ADCx_n通道的转换顺序和采样时间  Tconv = 采样时间 + 12.5个周期 */
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0,  1, ADC_SampleTime_239Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 2, ADC_SampleTime_239Cycles5);
  ADC_TempSensorVrefintCmd(ENABLE);
  
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr  = (uint32_t)&(ADC1->DR);
  DMA_InitStructure.DMA_MemoryBaseAddr      = (uint32_t)&adcDMAvalue;
  DMA_InitStructure.DMA_DIR                 = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize          = ADC_SampleNum * ADC_ChannelNum;
  DMA_InitStructure.DMA_PeripheralInc       = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc           = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize  = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize      = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode                = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority            = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M                 = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  DMA_Cmd(DMA1_Channel1 , ENABLE);
  
  ADC_DMACmd(ADC1, ENABLE);
  ADC_Cmd(ADC1, ENABLE);
  ADC_ResetCalibration(ADC1);
  while(ADC_GetResetCalibrationStatus(ADC1));
  ADC_StartCalibration(ADC1);
  while(ADC_GetCalibrationStatus(ADC1));
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/**
  * @brief  获取单通道采样值
  * @param  None
  * @retval None
  */
uint16_t adc_ReadAvgRaw(uint8_t channel)
{
  uint32_t sum = 0;
  uint8_t index = 0;
  
  for(index = 0; index < ADC_SampleNum; index++) {
    sum += adcDMAvalue[index][channel];
  }
  return (uint16_t)(sum / ADC_SampleNum);
}

/**
  * @brief  获取电压值
  * @param  None
  * @retval None
  */
void adc_GetVoltage(void)
{
//AdcData.Vbus = (1056*33 * adc_ReadAvgRaw(ADC_VBUS)) / 56*4096;
  AdcData.Vbus = (1089 * adc_ReadAvgRaw(ADC_VBUS)) / 7168;
}

/**
  * @brief  获取温度值
  * @param  None
  * @retval None
  */
void adc_GetTemp(void)
{
  AdcData.TfbS = (((TempV25 - adc_ReadAvgRaw(ADC_TEMP)) * 100) / AvgSlope) + 250;
}

/*********************************************END OF FILE**********************/

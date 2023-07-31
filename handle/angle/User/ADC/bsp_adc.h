#ifndef __BSP_ADC_H
#define	__BSP_ADC_H


#include "stm32f10x.h"


#define TempV25           0x6EF
#define AvgSlope          53

#define ADC_SampleNum     10   /* 单通道采样点数 */
#define ADC_ChannelNum    2    /* 采样通道数 */

#define ADC_VBUS          0
#define ADC_TEMP          1

typedef struct adc_data
{
  int16_t Vbus;           /* 母线电压 */
  int16_t TfbS;           /* 芯片温度 */
} TADC_DATA;

extern TADC_DATA AdcData;
extern __IO uint16_t adcDMAvalue[ADC_SampleNum][ADC_ChannelNum];

void ADC_Config (void);
uint16_t adc_ReadAvgRaw(uint8_t channel);
void adc_GetVoltage(void);
void adc_GetTemp(void);

#endif /* __BSP_ADC_H */

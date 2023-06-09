/**********************************************************/
//@Autor:Raw
//@Version:0.0.1
//@End Time:2023/3/20
//@Function:adc_callback.c
/**********************************************************/

#include "adc_callback.h"
#include "adc.h"

ADC_Data_t ADC_Data = {0};

#define current_para 3.3f/(4096.0f*0.002f*14.0f)


#define Rp 10.0f
#define T2 (273.15f+25.0f)
#define Bx 3987.0f
#define Ka 273.15f

static uint16_t adc_ReadAvgRaw_2(uint8_t channel)
{
	 uint16_t i,j,temp;
   for (j=0;j<ADC2_SAMPLING_NUMBER-1;j++)
   {
      for (i=1;i<ADC2_SAMPLING_NUMBER-j;i++)
      {
         if ( adcDMAvalue_adc2[i][channel]>adcDMAvalue_adc2[i-1][channel] )
         {
            temp = adcDMAvalue_adc2[i][channel];
            adcDMAvalue_adc2[i][channel] = adcDMAvalue_adc2[i-1][channel];
            adcDMAvalue_adc2[i-1][channel] = temp;
         }
      }
   }
	 return adcDMAvalue_adc2[(ADC2_SAMPLING_NUMBER-1)/2][channel];
}
static uint16_t adc_ReadAvgRaw_0(uint8_t channel)
{
	 uint16_t i,j,temp;
   for (j=0;j<ADC0_SAMPLING_NUMBER-1;j++)
   {
      for (i=1;i<ADC0_SAMPLING_NUMBER-j;i++)
      {
         if ( adcDMAvalue_adc0[i][channel]>adcDMAvalue_adc0[i-1][channel] )
         {
            temp = adcDMAvalue_adc0[i][channel];
            adcDMAvalue_adc0[i][channel] = adcDMAvalue_adc0[i-1][channel];
            adcDMAvalue_adc0[i-1][channel] = temp;
         }
      }
   }
	 return adcDMAvalue_adc0[(ADC0_SAMPLING_NUMBER-1)/2][channel];
}

uint16_t adc_ReadAvgRaw(uint8_t channel,PMSM_Num num)
{
	uint16_t i,j,temp;
	if(num == PMSM_A)
	{
	  for (j=0;j<ADC0_SAMPLING_NUMBER-1;j++)
   {
      for (i=1;i<ADC0_SAMPLING_NUMBER-j;i++)
      {
         if ( adcDMAvalue_adc0[i][channel]>adcDMAvalue_adc0[i-1][channel] )
         {
            temp = adcDMAvalue_adc0[i][channel];
            adcDMAvalue_adc0[i][channel] = adcDMAvalue_adc0[i-1][channel];
            adcDMAvalue_adc0[i-1][channel] = temp;
         }
      }
   }
	 return adcDMAvalue_adc0[(ADC0_SAMPLING_NUMBER-1)/2][channel];
	}
	else if(num == PMSM_U)
	{
	 for (j=0;j<ADC2_SAMPLING_NUMBER-1;j++)
   {
      for (i=1;i<ADC2_SAMPLING_NUMBER-j;i++)
      {
         if ( adcDMAvalue_adc2[i][channel]>adcDMAvalue_adc2[i-1][channel] )
         {
            temp = adcDMAvalue_adc2[i][channel];
            adcDMAvalue_adc2[i][channel] = adcDMAvalue_adc2[i-1][channel];
            adcDMAvalue_adc2[i-1][channel] = temp;
         }
      }
   }
	 return adcDMAvalue_adc2[(ADC2_SAMPLING_NUMBER-1)/2][channel];
	}
}

static float myln(float a)
{
   int N = 15;//我们取了前15+1项来估算
   int k,nk;
   float x,xx,y;
   x = (a-1.0f)/(a+1.0f);
   xx = x*x;
   nk = 2.0f*N+1.0f;
   y = 1.0f/nk;
   for(k=N;k>0;k--)
   {
     nk = nk - 2;
     y = 1.0f/nk+xx*y;
     
   }
   return 2.0f*x*y;
}

void GetVoltage_MOSTEMP(void)
{
  ADC_Data.Bus_Volt = 3.3f*adc_ReadAvgRaw_0(V_BUS)/4096.0f/56.0f*1056.0f;	//Vbus
	float Rt=0.0f;
	float v_t =0.0f;
	float N1,N2,N3;
	v_t = (float)(adc_ReadAvgRaw_2(MOS_TEMP)/4096.0f*3.3f);
//	Rt = v_t*10.0f/(3.3f-v_t)*1000.0f;
//	AdcData.Mos_Temp = 1/(1/T2+log(Rt/Rp)/Bx) -Ka;
	Rt = v_t*10.0f/(3.3f-v_t);
	N1 = (myln(Rt)-myln(Rp))/Bx;
	N2 = 1.0f/T2 - N1;
	N3 = 1.0f/N2;
	ADC_Data.mos_temp = N3-Ka;
}
//放大器偏置值采集
//
uint32_t offset_a = 0;
uint32_t offset_u = 0;
void Brush_ADC_OffsetSample(void)
{
	for(int i = 0;i<1000;i++)
	{
		offset_a += adc_ReadAvgRaw_2(Brush_adc_A);
		offset_u += adc_ReadAvgRaw_2(Brush_adc_U);
	}
	ADC_Data.Brush_adc[Brush_A].adc_offer = offset_a/1000;
	ADC_Data.Brush_adc[Brush_U].adc_offer= offset_u/1000;
	GetVoltage_MOSTEMP();
}

void Brush_GetCurrent(void)
{

		ADC_Data.Brush_adc[Brush_A].adc_current = (adc_ReadAvgRaw_2(Brush_adc_A) - ADC_Data.Brush_adc[Brush_A].adc_offer)*current_para;

		ADC_Data.Brush_adc[Brush_U].adc_current = (adc_ReadAvgRaw_2(Brush_adc_U) - ADC_Data.Brush_adc[Brush_U].adc_offer)*current_para;

}





float GildeAverageValueFilter_MAG(int16_t NewValue,int16_t *Data)
{
	float max,min;
	float sum;
	uint16_t i;
	Data[0]=NewValue;
	max=Data[0];
	min=Data[0];
	sum=Data[0];
	for(i=Nx-1;i!=0;i--)
	{
		if(Data[i]>max) max=Data[i];
		else if(Data[i]<min) min=Data[i];
		sum+=Data[i];
		Data[i]=Data[i-1];
	}
	i=Nx-2;
	sum=sum-max-min;
	sum=sum/i;
	return(sum);
}














#include "joystick_ext.h"
#include "main.h"
#include "stdlib.h"

uint16_t	adc1_val_buf[2];

#define ADC_VAL_X		adc1_val_buf[0]
#define ADC_VAL_Y  	adc1_val_buf[1]

JOYSTICK joyetick_adc;

void adc_config(void)
{

		rcu_periph_clock_enable(RCU_ADC0);
	
		adc_deinit(ADC0);
    /* ADC mode config */
    adc_mode_config(ADC_MODE_FREE); 
    /* 开启连续转换 */
    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, ENABLE);
    /* 开启扫描模式 */
    adc_special_function_config(ADC0, ADC_SCAN_MODE, ENABLE);
    /*数据右对齐 */
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);

    /* ADC channel length config */
    adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 2);

    /* ADC regular channel config */ 
    adc_regular_channel_config(ADC0, 0, ADC_CHANNEL_0, ADC_SAMPLETIME_55POINT5);
    adc_regular_channel_config(ADC0, 1, ADC_CHANNEL_1, ADC_SAMPLETIME_55POINT5);

    /* ADC 软件触发（规则组） */
    adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_REGULAR_NONE);
    adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, ENABLE);

    /* ADC DMA function enable */
    adc_dma_mode_enable(ADC0);
    /* enable ADC interface */
    adc_enable(ADC0);


    /* ADC校准和复位校准 */
    adc_calibration_enable(ADC0);
		
		adc_software_trigger_enable(ADC0,ADC_REGULAR_CHANNEL);
}

void dma_config(void)
{
	
    /* ADC_DMA_channel configuration */
    dma_parameter_struct dma_data_parameter;

		rcu_periph_clock_enable(RCU_DMA0);
    /*  ADC DMA0_0初始化 */
    dma_deinit(DMA0, DMA_CH0);

    /* initialize DMA single data mode */
    dma_data_parameter.periph_addr  = (uint32_t)(&ADC_RDATA(ADC0));//外设基地址
    dma_data_parameter.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;//内存地址增量模式
    dma_data_parameter.memory_addr  = (uint32_t)(&adc1_val_buf);//数据存放地址
    dma_data_parameter.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;//内存地址增量模式
    dma_data_parameter.periph_width = DMA_PERIPHERAL_WIDTH_16BIT;//dma外设宽度16位，半字
    dma_data_parameter.memory_width = DMA_MEMORY_WIDTH_16BIT;  
    dma_data_parameter.direction    = DMA_PERIPHERAL_TO_MEMORY;//传输模式，外设到存储（接收）
    dma_data_parameter.number       = 2;//长度
    dma_data_parameter.priority     = DMA_PRIORITY_HIGH;//优先级高
    dma_init(DMA0, DMA_CH0, &dma_data_parameter);

    dma_circulation_enable(DMA0, DMA_CH0);//循环模式开启dma_circulation_enable(DMA0, DMA_CH0)//dma_circulation_disable

/* enable DMA channel */
    dma_channel_enable(DMA0, DMA_CH0);    /* DMA内存到内存模式不开启 */
}


void ADC_EXT_Init(void)
{
	dma_config();
	adc_config();
}
 

uint32_t joy_x_sum;
uint32_t joy_y_sum;

#define ADC_LEN		50
uint16_t adc_buff_x[ADC_LEN];
uint16_t adc_buff_y[ADC_LEN];
/*
函数功能：滑动窗口滤波
函数形参：输入值，窗口数组
函数返回值：滤波后的值
*/
uint16_t GildeAverageValueFilter_MAG(uint16_t NewValue,uint16_t *Data)
{
	uint16_t max,min;
	float sum;
	unsigned char i;
	Data[0]=NewValue;
	max=Data[0];
	min=Data[0];
	sum=Data[0];
	for(i=ADC_LEN-1;i!=0;i--)
	{
		if(Data[i]>max) max=Data[i];
		else if(Data[i]<min) min=Data[i];
		sum+=Data[i];
		Data[i]=Data[i-1];
	}
	i=ADC_LEN-2;
	sum=sum-max-min;
	sum=sum/i;
	return(sum);
}

/*去除死区函数*/
uint16_t deadband(uint16_t value, const uint16_t midval, const uint16_t threshold)
{
	if(value<(midval-threshold))
		value = value + threshold;
	else if(value>(midval+threshold))
		value = value - threshold;
	else
		value = midval;
	return value;
}


void scan_joyxy(void)
{
	uint16_t temp_x;
	uint16_t temp_y;	
	//static uint8_t t=0;
	
	
	temp_x = ADC_VAL_X;
	temp_y = ADC_VAL_Y;


	temp_x = deadband(temp_x,Coord_Base<<4,Coord_Dead<<4);
	temp_y = deadband(temp_y,Coord_Base<<4,Coord_Dead<<4);
		
	temp_x =GildeAverageValueFilter_MAG(temp_x,adc_buff_x);
	temp_y = GildeAverageValueFilter_MAG(temp_y,adc_buff_y);
		
	temp_x = (temp_x>>4)&0xff;
	temp_y = (temp_y>>4)&0xff;		
#if DEBUG_MODECHANGE	
	if(Remote_setting_para.Control_change)return;	
#endif	
	if(joyetick_adc.valX != temp_x)
	{
		joyetick_adc.valX = temp_x;
	}
	#if DEBUG_MODE
		if(joyetick_adc.valY != temp_y)
		{
			joyetick_adc.valY = temp_y;
		}			
	#else
	// if(getdisplay_ui() !=ePAGE_UPSTAIR && getdisplay_ui() !=ePAGE_DOWNSTAIR )
	{
		if(joyetick_adc.valY != temp_y)
		{
			joyetick_adc.valY = temp_y;
		}		
	}		
	
	#endif

}


//void scan_joyxy(void)
//{
//	uint16_t temp_x;
//	uint16_t temp_y;	
//	static uint8_t t=0;
//	
//	
//	temp_x = ADC_VAL_X;
//	temp_y = ADC_VAL_Y;


//	temp_x = deadband(temp_x,Coord_Base<<4,Coord_Dead<<4);
//	temp_y = deadband(temp_y,Coord_Base<<4,Coord_Dead<<4);
//		
//	joy_x_sum += temp_x;
//	joy_y_sum += temp_y;
//	
//	if(t==(ADC_LEN-1))
//	{
//		t=0;
//		temp_x = joy_x_sum/ADC_LEN;
//		temp_y = joy_y_sum/ADC_LEN;
//		
//		joy_x_sum=0;
//		joy_y_sum=0;
//		
//		temp_x = (temp_x>>4)&0xff;
//		temp_y = (temp_y>>4)&0xff;		
//			
//		if(Remote_setting_para.Control_change)return;
//		
//		if(joyetick_adc.valX != temp_x)
//		{
//			joyetick_adc.valX = temp_x;
//		}
//		#if DEBUG_MODE
//			if(joyetick_adc.valY != temp_y)
//			{
//				joyetick_adc.valY = temp_y;
//			}			
//		#else
//		if(getdisplay_ui() !=ePAGE_UPSTAIR && getdisplay_ui() !=ePAGE_DOWNSTAIR )
//		{
//			if(joyetick_adc.valY != temp_y)
//			{
//				joyetick_adc.valY = temp_y;
//			}		
//		}		
//		
//		#endif
//	}
//	else
//	{
//		t++;
//	}	
//}


void joystick_init(void)
{
	Remote_setting_para.CoordX=Coord_Base;
	Remote_setting_para.CoordY=Coord_Base;

	ADC_EXT_Init();
}



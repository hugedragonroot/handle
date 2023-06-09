#include "filter.h"

#define M_PI_F 3.141592653589793f
#define Sampling_Freq  20000
biquadFilter_t accFilterLPF[5];
float ACCEL_LPF_CUTOFF_FREQ = 5.0f;

/*
	函数功能：一阶低通滤波
	函数输入：input（滤波前的值），out（滤波后的值），ratio（滤波系数）
	函数输出：无
*/

void First_Order_Low_Pass_filtering(float *input, float *out,float ratio)
{
	*out =  *out*(1.0f-ratio) + *input*ratio; 
}

//这里用在循环读取传感器数据之后用，每次都进行滤波
void FilterInit(float accUpdateRate)
{
	biquadFilterInitLPF(&accFilterLPF[0],accUpdateRate, ACCEL_LPF_CUTOFF_FREQ);//x轴用
	biquadFilterInitLPF(&accFilterLPF[1],accUpdateRate, ACCEL_LPF_CUTOFF_FREQ);

}
  //二阶低通滤波器
void biquadFilterInitLPF(biquadFilter_t *filter, uint16_t samplingFreq, uint16_t filterFreq)
  {
      biquadFilterInit(filter,samplingFreq, filterFreq, BIQUAD_Q);
  }
  //二阶滤波器
//samplingFreq:采样频率   filterFreq：截止频率   Q：品质因数
  void biquadFilterInit(biquadFilter_t *filter,uint16_t samplingFreq, uint16_t filterFreq, float Q)
  {
	  if(filterFreq<(samplingFreq/2))
	  {
		  const float sampleRate = (float)samplingFreq;
          const float omega = 2.0f* 3.1415926f * ((float)filterFreq) /((float)sampleRate);
          const float sn = sinf(omega);
          const float cs = cosf(omega);
          const float alpha = sn / (2 * Q);

          float b0 = 0,b1=0,b2=0,a0=0,a1=0,a2=0;
          b0 = (1-cs)/2;
          b1 = 1-cs;
          b2 = (1-cs)/2;
          a0 = 1+ alpha;
          a1 = -2*cs;
          a2 = 1-alpha;

          filter->b0 = b0/a0;
          filter->b1 = b1/a0;
          filter->b2 = b2/a0;
          filter->a1 = a1/a0;
          filter->a2 = a2/a0;
	  }
	  else
	  {
	 	 filter->b0 = 1.0f;
	   	 filter->b1 = 0.0f;
	   	 filter->b2 = 0.0f;
	   	 filter->a1 = 0.0f;
	   	 filter->a2 = 0.0f;
	  }
	  filter->d1 = filter->d2 = 0;
  }
  float biquadFilterApply(biquadFilter_t *filter,float input)
  {
  const float result = filter->b0*input + filter->d1;
  filter->d1 = filter->b1 * input - filter->a1*result + filter->d2;
  filter->d2 = filter->b2*input - filter->a2*result;
  return result;
  }
  int Ce[10];
  //-----Butterworth变量-----//
  Butter_Parameter Butter_80HZ_Parameter_Acce,Butter_60HZ_Parameter_Acce,Butter_51HZ_Parameter_Acce,
  Butter_30HZ_Parameter_Acce,Butter_20HZ_Parameter_Acce,Butter_15HZ_Parameter_Acce,
  Butter_10HZ_Parameter_Acce,Butter_5HZ_Parameter_Acce,Butter_2HZ_Parameter_Acce;
  Butter_BufferData Butter_Buffer[3];
  Butter_BufferData Butter_Buffer_Feedback[3];
  Butter_BufferData Butter_Buffer_SINS[3];
	Butter_BufferData  Butter_10HZ_buffer,Butter_5HZ_buffer,Butter_2HZ_buffer;
  /****************************************
  Butterworth低通滤波器参数初始化：http://blog.csdn.net/u011992534/article/details/73743955
  ***************************************/
  /***********************************************************
  @函数名：Butterworth_Parameter_Init
  @入口参数：无
  @出口参数：无
  功能描述：巴特沃斯低通滤波器初始化
  *************************************************************/
  void Butterworth_Parameter_Init(void)
  {
//    Set_Cutoff_Frequency(Sampling_Freq, 80,&Butter_80HZ_Parameter_Acce);
//    Set_Cutoff_Frequency(Sampling_Freq, 60,&Butter_60HZ_Parameter_Acce);
//    Set_Cutoff_Frequency(Sampling_Freq, 51,&Butter_51HZ_Parameter_Acce);
//    Set_Cutoff_Frequency(Sampling_Freq, 30,&Butter_30HZ_Parameter_Acce);
//    Set_Cutoff_Frequency(Sampling_Freq, 20,&Butter_20HZ_Parameter_Acce);
//    Set_Cutoff_Frequency(Sampling_Freq, 15,&Butter_15HZ_Parameter_Acce);
    Set_Cutoff_Frequency(Sampling_Freq, 10,&Butter_10HZ_Parameter_Acce);
    Set_Cutoff_Frequency(Sampling_Freq, 5 ,&Butter_5HZ_Parameter_Acce);
    Set_Cutoff_Frequency(Sampling_Freq, 2 ,&Butter_2HZ_Parameter_Acce);
  }

  /*************************************************
  函数名:	float LPButterworth(float curr_input,Butter_BufferData *Buffer,Butter_Parameter *Parameter)
  说明:	加速度计低通滤波器
  入口:	float curr_input 当前输入加速度计,滤波器参数，滤波器缓存
  出口:	无
  备注:	2阶Butterworth低通滤波器
  *************************************************/
  float LPButterworth(float curr_input,Butter_BufferData *Buffer,Butter_Parameter *Parameter)
  {
    /* 加速度计Butterworth滤波 */
    /* 获取最新x(n) */
    Buffer->Input_Butter[2]=curr_input;
    /* Butterworth滤波 */
    Buffer->Output_Butter[2]=
      Parameter->b[0] * Buffer->Input_Butter[2]
        +Parameter->b[1] * Buffer->Input_Butter[1]
          +Parameter->b[2] * Buffer->Input_Butter[0]
            -Parameter->a[1] * Buffer->Output_Butter[1]
              -Parameter->a[2] * Buffer->Output_Butter[0];
    /* x(n) 序列保存 */
    Buffer->Input_Butter[0]=Buffer->Input_Butter[1];
    Buffer->Input_Butter[1]=Buffer->Input_Butter[2];
    /* y(n) 序列保存 */
    Buffer->Output_Butter[0]=Buffer->Output_Butter[1];
    Buffer->Output_Butter[1]=Buffer->Output_Butter[2];
    return Buffer->Output_Butter[2];
  }


  /***********************************************************
  @函数名：Set_Cutoff_Frequency
  @入口参数：float sample_frequent, float cutoff_frequent,
  Butter_Parameter *LPF
  @出口参数：无
  功能描述：巴特沃斯低通滤波器初始化
  *************************************************************/
void Set_Cutoff_Frequency(float sample_frequent, float cutoff_frequent,Butter_Parameter *LPF)
{
	float fr = sample_frequent / cutoff_frequent;
	float ohm = tanf(M_PI_F / fr);
	float c = 1.0f + 2.0f * cosf(M_PI_F / 4.0f) * ohm + ohm * ohm;
	if (cutoff_frequent <= 0.0f) {
		// no filtering
		return;
	}
	LPF->b[0] = ohm * ohm / c;
	LPF->b[1] = 2.0f * LPF->b[0];
	LPF->b[2] = LPF->b[0];
	LPF->a[0]=1.0f;
	LPF->a[1] = 2.0f * (ohm * ohm - 1.0f) / c;
	LPF->a[2] = (1.0f - 2.0f * cosf(M_PI_F / 4.0f) * ohm + ohm * ohm) / c;
}

#define k_gain	0.00005f
#define b_gain  0.05f
void Linear_function(float x_input,float *y_out)
{

	#if 1
	if(x_input>=300.0f&&x_input<1000.0f)
	{
		*y_out = k_gain*300.0f + b_gain;
//		*lim_out = *y_out*k_lim + b_lim;
		return;
	}
	if(x_input<300.0f)
	{
		*y_out = k_gain*50.0f + b_gain;
//		*lim_out = *y_out*k_lim + b_lim;
		return;
	}
		*y_out = k_gain*x_input + b_gain;
//		*lim_out = *y_out*k_lim + b_lim;
	#endif
}



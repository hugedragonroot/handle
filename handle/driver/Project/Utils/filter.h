#ifndef __FILTER_H__
#define __FILTER_H__

#include "stdint.h"
#include "string.h"
#include "stdbool.h"
#include "stdio.h"
#include <math.h>
#define BIQUAD_Q		1.0f/sqrt(2.0f)
 typedef struct
 {
      float b0, b1, b2, a1, a2;
      float d1, d2;
      bool fil_flag;
  } biquadFilter_t;
//������׵�ͨ�˲���������λHz��
extern biquadFilter_t accFilterLPF[5];


void FilterInit(float accUpdateRate);
float biquadFilterApply(biquadFilter_t *filter,float input);
void biquadFilterInit(biquadFilter_t *filter,uint16_t samplingFreq, uint16_t filterFreq, float Q);
void biquadFilterInitLPF(biquadFilter_t *filter, uint16_t samplingFreq, uint16_t filterFreq);
//������˹�˲�������
typedef struct
{
    int N;          //������˹�˲�����Сʵ�ֽ���
    int length;     //�˲���ϵͳ����ϵ������ĳ���
    float fc;       //������˹�˲�����ֹƵ��
    float cosW0;    //����Ƶ�ʣ���ͨ����ʱ�õ�
    float fs;       //����Ƶ��
    int filterType; //��Ҫ��Ƶ������˲�������
	  float pass[2];
	  float stop[2];
	  float rp ;//ͨ��˥��������ֵ2dB
	  float rs ;//���˥��������ֵ20dB
	  float sbvalue[11];
	  float num[11];
	  float den[11];
	  float input[11];
	  float output[11];
	  bool isFOK;
}ButterFilterStruct;
typedef struct
{
  float a[3];
  float b[3];
}Butter_Parameter;
typedef struct
{
 float Input_Butter[3];
 float Output_Butter[3];
}Butter_BufferData;

extern Butter_Parameter Bandstop_Filter_Parameter_30_98,Bandstop_Filter_Parameter_30_94;

extern Butter_Parameter Butter_80HZ_Parameter_Acce,Butter_60HZ_Parameter_Acce,Butter_51HZ_Parameter_Acce,
                 Butter_30HZ_Parameter_Acce,Butter_20HZ_Parameter_Acce,Butter_15HZ_Parameter_Acce,
                 Butter_10HZ_Parameter_Acce,Butter_5HZ_Parameter_Acce,Butter_2HZ_Parameter_Acce;


float set_lpf_alpha(int16_t cutoff_frequency, float time_step);
void Acce_Control_Filter(void);
float LPButterworth(float curr_input,Butter_BufferData *Buffer,Butter_Parameter *Parameter);


void Set_Cutoff_Frequency(float sample_frequent, float cutoff_frequent,Butter_Parameter *LPF);
void Butterworth_Parameter_Init(void);
void Test_Filter(void);
float BPF_Butterworth(float curr_input,Butter_BufferData *Buffer,Butter_Parameter *Parameter);


void pascalTriangle(int N,int symbol,int *vector);
bool filter(ButterFilterStruct butterValue,float *numerator,float *denominator,float *xVector,int length,float *yVector);
bool BTfilter(ButterFilterStruct *butterValue,float data);

extern ButterFilterStruct Gyro_X_Butter_Filter,Gyro_Y_Butter_Filter,Gyro_Z_Butter_Filter;
extern ButterFilterStruct Accel_X_Butter_Filter,Accel_Y_Butter_Filter,Accel_Z_Butter_Filter;
extern Butter_Parameter Bandstop_Filter_Parameter_30_98;
extern Butter_Parameter Bandstop_Filter_Parameter_30_94;

void First_Order_Low_Pass_filtering(float *input, float *out,float ratio);
void Linear_function(float x_input,float *y_out);
#endif

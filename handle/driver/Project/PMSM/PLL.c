#include "PLL.h"

//PllSpeed_t PllSpeed[2]={
//	{
//	.Kp = 190.0f,
//	.Ki = 12.0f,
//	.Ts = 0.0001f,
//	},
//	{
//	.Kp = 190.0f,
//	.Ki = 12.0f,
//	.Ts = 0.0001f,
//	}
//};
/**@struct PllSpeed_t 
* @brief ���໷���ٽṹ��
*/

/**@struct ForceAngle_t
* @brief Vfǿ����Ƕȵ���ز���
*/
typedef struct{
	float ts;					///< �������õ�ʱ�� s
	float freq_hz;				///< Vfǿ��Ƶ��
	float angle_delta_factor;	///< ��Ӧʱ���Ӧ��Ƕ�Ƶ�ʵļ��
	float angle_delta;			///< ���ε����ĽǶ�
	float angle;				///< ����Ƕ�
}ForceAngle_t;



typedef struct
{
        float kp;
        float ki;
        float integ;
        float err;
        float theta;
        float speed_omega;
        float Ui;
        float Wn;
        float SpdLftGain;
} QPLL_t;
typedef struct 
{
	float Ts;		///< PPL���໷ ����ʱ��
	float Kp;		///< wn * att	wn���� att���� 0.95f
	float Ki;       ///< wn * wn * ts
	float error;	///< ��ǰ���
	float intg;		///< �������
	float Speed_Hz;	///< ���յõ����ٶ� --> Hz 
	float theta;	///< ����õ��ĵ�Ƕ�
	float out;		///< PLL ��PI������
	
}PllSpeed_t;
PllSpeed_t PllSpeed[2]={0};
void PLL_init(PLL_Num num,float kp,float ki,float ts)
{
	PllSpeed[num].Kp = kp;
	PllSpeed[num].Ki = ki;
	PllSpeed[num].Ts = ts;
}
/**@brief PllGetSpeed     ʹ�� PLL���໷���в���,���ݵ�Ƕȼ���õ��ĵ�Ƶ��  ���ת�����ٶ�Ӧ���� Hz * 60 / ������  = rpm
* @param[in]  p_PllSpeed	 ����Pll�ṹ��ָ��,���������� @PllSpeed_t
* @param[in]  Theta	 		��Ƕ�
* @return  �����ĵ�Ƕ�
*/
float temp_t=0.3f;
float PllGetSpeed(PLL_Num num, float Theta)
{
	static float Pll_PI_Out = 0.0f;
	static float Speed_HzTmp=0.0f;
	//p_PllSpeed->error =  HallNormalized_Beta * cosf(p_PllSpeed->theta) - HallNormalized_Alpha * sinf(p_PllSpeed->theta);

	//QPLL
	PllSpeed[num].error = Theta - PllSpeed[num].theta;
	
	/*Pll ��Ƕ���� �޶� -PI ~ PI*/
	if(PllSpeed[num].error>PI_1)
	{
		PllSpeed[num].error -= 2*PI_1;
	}
	else if(PllSpeed[num].error<-PI_1)
	{
		PllSpeed[num].error += 2*PI_1;
	}
	
	PllSpeed[num].intg += PllSpeed[num].Ki * PllSpeed[num].error;
	PllSpeed[num].out = PllSpeed[num].Kp * PllSpeed[num].error +PllSpeed[num].intg;
	Pll_PI_Out = PllSpeed[num].out;
	
	PllSpeed[num].theta += Pll_PI_Out * PllSpeed[num].Ts;
	Speed_HzTmp = Pll_PI_Out / PI_2;
	PllSpeed[num].Speed_Hz += (Speed_HzTmp - PllSpeed[num].Speed_Hz) * temp_t;
//	p_PllSpeed->Speed_Hz = Speed_HzTmp*temp_t + p_PllSpeed->Speed_Hz *(1.0f-temp_t);
	
	/*Pll ��Ƕ� �޶� -PI ~ PI*/
	if(PllSpeed[num].theta>PI_1)
	{
		PllSpeed[num].theta -= 2*PI_1;
	}
	else if(PllSpeed[num].theta<-PI_1)
	{
		PllSpeed[num].theta += 2*PI_1;
	}	
	
	return PllSpeed[num].Speed_Hz;
}


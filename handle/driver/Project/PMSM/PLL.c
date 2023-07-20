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
* @brief 锁相环测速结构体
*/

/**@struct ForceAngle_t
* @brief Vf强拉电角度的相关参数
*/
typedef struct{
	float ts;					///< 函数调用的时间 s
	float freq_hz;				///< Vf强拖频率
	float angle_delta_factor;	///< 相应时间对应电角度频率的间隔
	float angle_delta;			///< 单次递增的角度
	float angle;				///< 输出角度
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
	float Ts;		///< PPL锁相环 计算时间
	float Kp;		///< wn * att	wn带宽 att阻尼 0.95f
	float Ki;       ///< wn * wn * ts
	float error;	///< 当前误差
	float intg;		///< 积分误差
	float Speed_Hz;	///< 最终得到的速度 --> Hz 
	float theta;	///< 计算得到的电角度
	float out;		///< PLL 经PI计算结果
	
}PllSpeed_t;
PllSpeed_t PllSpeed[2]={0};
void PLL_init(PLL_Num num,float kp,float ki,float ts)
{
	PllSpeed[num].Kp = kp;
	PllSpeed[num].Ki = ki;
	PllSpeed[num].Ts = ts;
}
/**@brief PllGetSpeed     使用 PLL锁相环进行测速,根据电角度计算得到的电频率  最后转换成速度应该是 Hz * 60 / 极对数  = rpm
* @param[in]  p_PllSpeed	 输入Pll结构体指针,具体参数详见 @PllSpeed_t
* @param[in]  Theta	 		电角度
* @return  计算后的电角度
*/
float temp_t=0.3f;
float PllGetSpeed(PLL_Num num, float Theta)
{
	static float Pll_PI_Out = 0.0f;
	static float Speed_HzTmp=0.0f;
	//p_PllSpeed->error =  HallNormalized_Beta * cosf(p_PllSpeed->theta) - HallNormalized_Alpha * sinf(p_PllSpeed->theta);

	//QPLL
	PllSpeed[num].error = Theta - PllSpeed[num].theta;
	
	/*Pll 电角度误差 限定 -PI ~ PI*/
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
	
	/*Pll 电角度 限定 -PI ~ PI*/
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


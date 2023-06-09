//***************************************************************************************

//***************************************************************************************
#include "FOC.h"

typedef struct
{
	float *Theta;
	Phase3_Curr *Ia_Ib_Ic;
	Phase2_Curr *Id_Iq;
	Phase2_Volt *Vd_Vq;
	Phase3_value *PWM;
	Phase2_Curr Ialpha_Ibeta;
	Phase2_Volt Valpha_Vbeta;
	float SinThetaValue;
	float CosThetaValue;
}sFocCtrlTypeDef;

sFocCtrlTypeDef FOC_Motor[FOC_NUMBER]={0};

void FOC_Config(FOC_num NUM,foc_para_t *foc)
{
	FOC_Motor[NUM].Theta = &foc->Theta;
	FOC_Motor[NUM].Ia_Ib_Ic = &foc->Ia_Ib_Ic;
	FOC_Motor[NUM].Id_Iq = &foc->Id_Iq;	
	FOC_Motor[NUM].Vd_Vq = &foc->Vd_Vq;
	FOC_Motor[NUM].PWM = &foc->PWM;
}
void SinCosF(float Theta, float *SinfValue,float *CosfValue)//输入值为弧度，ST需要减90度
{
		*CosfValue = arm_cos_f32(Theta); 
	  *SinfValue = arm_sin_f32(Theta); 
}
void Clarke(Phase3_Curr *In_Ia_Ib_Ic, Phase2_Curr *Out_Ialpha_Ibeta)
{
	
  Out_Ialpha_Ibeta->I1 = 0.333333f*(In_Ia_Ib_Ic->I1*2.0f - In_Ia_Ib_Ic->I2 - In_Ia_Ib_Ic->I3);
  Out_Ialpha_Ibeta->I2 = (In_Ia_Ib_Ic->I2  - In_Ia_Ib_Ic->I3) * 0.57735f;
	
}
void Park(Phase2_Curr *In_Ialpha_Ibeta, Phase2_Curr *Out_Id_Iq, float sinTh,float cosTh)
{
	
  Out_Id_Iq->I1 = In_Ialpha_Ibeta->I1 * cosTh + In_Ialpha_Ibeta->I2 * sinTh;
  Out_Id_Iq->I2 = In_Ialpha_Ibeta->I2 * cosTh - In_Ialpha_Ibeta->I1 * sinTh;
 
}
void Rev_Park(Phase2_Volt *In_Vd_Vq, Phase2_Volt *Out_Valpha_Vbeta, float sinTh, float cosTh)
{ 	
	Out_Valpha_Vbeta->V1 = (In_Vd_Vq->V1 * cosTh - In_Vd_Vq->V2 * sinTh)/2.0f;
  Out_Valpha_Vbeta->V2 = (In_Vd_Vq->V2 * cosTh + In_Vd_Vq->V1 * sinTh)/2.0f;
}
void FOC_SVPWM(float Va,float Vb,Phase3_value *pwm_value)
{
  float Vmax,Vmin,Vcom;
  float Vectora,Vectorb,Vectorc;
	float T1,T2,T3;
	float Va_tmp ,Vb_tmp;	
	
  Va_tmp = (Va * 0.5f);
  Vb_tmp = 0.8660254038f * Vb;
  
  Vectora = Va;  
  Vectorb = -Va_tmp + Vb_tmp;
  Vectorc = -Va_tmp - Vb_tmp;
 
	
  Vmax=0;
  Vmin=0;

  if (Vectora > Vectorb)   
  {
    Vmax = Vectora;
    Vmin = Vectorb;
  }
  else
  {
    Vmax = Vectorb;
    Vmin = Vectora;
  }

  if (Vectorc > Vmax)
  {
    Vmax = Vectorc;
  }
  else if (Vectorc < Vmin)
  {
    Vmin = Vectorc;
  }
  
	
  Vcom = (Vmax + Vmin) * 0.5f;
	
	T1 = (Vectora - Vcom);
  T2 = (Vectorb - Vcom);
  T3 = (Vectorc - Vcom);

	pwm_value->value_a 	= T1 +0.5f;
	pwm_value->value_b  = T2 +0.5f;
	pwm_value->value_c  = T3 +0.5f;
}


void Foc_Handle(FOC_num NUM)
{
	SinCosF(*FOC_Motor[NUM].Theta,&FOC_Motor[NUM].SinThetaValue,&FOC_Motor[NUM].CosThetaValue);
	Clarke(FOC_Motor[NUM].Ia_Ib_Ic,&FOC_Motor[NUM].Ialpha_Ibeta);
	Park(&FOC_Motor[NUM].Ialpha_Ibeta,FOC_Motor[NUM].Id_Iq,FOC_Motor[NUM].SinThetaValue,FOC_Motor[NUM].CosThetaValue);
	Rev_Park(FOC_Motor[NUM].Vd_Vq,&FOC_Motor[NUM].Valpha_Vbeta, FOC_Motor[NUM].SinThetaValue,FOC_Motor[NUM].CosThetaValue);
	FOC_SVPWM(FOC_Motor[NUM].Valpha_Vbeta.V1,FOC_Motor[NUM].Valpha_Vbeta.V2,FOC_Motor[NUM].PWM);
}

//***************************************************************************************
//电流钳设置10mV,示波器用直流设置探头100倍，测出来即为电机电流
//***************************************************************************************
#ifndef _FOC_H_
#define _FOC_H_
#include "arm_math.h"

#define FOC_NUMBER 2
typedef enum
{
	FOC_1 = 0,
	FOC_2 = 1,
}FOC_num;

typedef struct 
{
	float V1;
	float V2;
} Phase2_Volt;

typedef struct 
{
	float I1;
	float I2;
} Phase2_Curr;

typedef struct
{
	float I1;
	float I2;
	float I3;
} Phase3_Curr;
typedef struct
{
	float value_a;
	float value_b;
	float value_c;
}Phase3_value;

typedef struct
{
	float Theta;
	Phase3_Curr Ia_Ib_Ic;
	Phase2_Curr Id_Iq;
	Phase2_Volt Vd_Vq;
	Phase3_value PWM;
}foc_para_t;
void FOC_Config(FOC_num NUM,foc_para_t *foc);
void Foc_Handle(FOC_num NUM);
#endif

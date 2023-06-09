/**********************************************************/
//@Autor:Raw
//@Version:0.0.1
//@End Time:2022-11-8
/**********************************************************/
#ifndef __PLL_H__
#define __PLL_H__
#include "common.h"
#ifndef PI_1
#define PI_1  3.1415926f    //1pi
#endif

#ifndef PI_2
#define PI_2  6.2831852f    //2pi
#endif
typedef enum
{
	PLL_A,
	PLL_U,
}PLL_Num;
void PLL_init(PLL_Num num,float kp,float ki,float ts);
float PllGetSpeed(PLL_Num num, float Theta);
#endif

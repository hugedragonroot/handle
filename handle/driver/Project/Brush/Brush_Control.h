/**********************************************************/
//@Autor:Raw
//@Version:0.0.1
//@End Time:2023/3/20
/**********************************************************/
#ifndef __BRUSH_CONTROL_H__
#define __BRUSH_CONTROL_H__
#include "common.h"


void BRUSH_SPEED_ANGLE_U(void);
void BRUSH_SPEED_ANGLE_A(void);
void brush_Cmd_handle(Brush_Num num);
void ADC_CURRENT_HANDLE(void);
void Brush_STATUS_LOOP(void);
void test_BRUSH(void);
void RESET_BRUSH_PARA(Brush_Num num);
#endif

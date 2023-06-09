/**********************************************************/
//@Autor:Raw
//@Version:0.0.1
//@End Time:2023/4/21
/**********************************************************/
#ifndef __STATUS_CHECK_H__
#define __STATUS_CHECK_H__
#include "common.h"

void Status_Check_Loop(void);
void online_loop(void);
void KEY_ZERO_CHECK_LOOP(void);
void ZERO_HANDLE_CHECK_LOOP(void);
void Brush_Fold_Check(Brush_Num num);
void MPU_PROTECT_CHECK(void);
#endif

/**********************************************************/
//@Autor:Raw
//@Version:0.0.1
//@End Time:2023/3/21
/**********************************************************/
#ifndef __PMSM_CONTROL_H__
#define __PMSM_CONTROL_H__
#include "common.h"
void adc0_callback_handle(void *parm);
void adc2_callback_handle(void *parm);

void PMSM_Status_Loop(void);
void PMSM_Mode_Loop(PMSM_Num num);
void PMSM_APP_LOOP(void);
void PMSM_TEST_LOOP(void);
void app_angel_handle(void);
#endif



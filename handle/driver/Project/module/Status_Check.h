/**********************************************************/
//@Autor:Raw
//@Version:0.0.1
//@End Time:2023/4/21
/**********************************************************/
#ifndef __STATUS_CHECK_H__
#define __STATUS_CHECK_H__
#include "common.h"
typedef struct
{
	int16_t pitch_return;
	int16_t roll_return;
	int16_t local_pitch;
	int16_t abs_angel;
}IMU_T;
extern IMU_T imu_mpu;
void Status_Check_Loop(void);
void online_loop(void);
void KEY_ZERO_CHECK_LOOP(void);
void ZERO_HANDLE_CHECK_LOOP(void);
void Brush_Fold_Check(Brush_Num num);
void MPU_PROTECT_CHECK(void);
void MPU_ZERO_CHECK_HANDLE(void);
#endif

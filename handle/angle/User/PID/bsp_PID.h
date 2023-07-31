#ifndef __PID_H
#define __PID_H


#include "stm32f10x.h"

/* A0 = Kp + Ki + Kd */
#define A0_DATA    8
/* A1 = (-Kp ) - (2 * Kd ) */
#define A1_DATA   -1.2
/* A2 = Kd */
#define A2_DATA    0

#define OUT_Sync_CHG          25
#define OUT_Async_CHG         -25

#define OUT_Sync_MAX          360
#define OUT_Async_MAX         0

/**
 * @brief Instance structure for the Q31 PID Control.
 */
typedef struct
{
	float     A0;            /**< The derived gain, A0 = Kp + Ki + Kd . */
	float     A1;            /**< The derived gain, A1 = -Kp - 2Kd. */
	float     A2;            /**< The derived gain, A2 = Kd . */
	int16_t  state[3];       /**< The state array of length 3.
                                state[0] = in_error;
                                state[1] = last_state[0];
                                state[2] = out_ctrl; */
} ARM_PID_Remove;

void ARM_PID_Init_q31(ARM_PID_Remove * S);
void ARM_PID_Reset_q31(ARM_PID_Remove * S);
int16_t ARM_PID_q31( ARM_PID_Remove * S, int16_t in);

#endif	/* __PID_H */

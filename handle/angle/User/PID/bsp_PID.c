/**
  ******************************************************************************
  * @file    bsp_PID.c
  * @author  Rhui
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   PID控制器应用函数接口
  ******************************************************************************
  */
	
#include ".\PID\bsp_PID.h"


/**
 * @brief  Initialization function for the Q31 PID Control.
 * @param[in,out] *S points to an instance of the Q15 PID structure.
 * @param[in]     resetStateFlag  flag to reset the state. 0 = no change in state 1 = reset the state.
 * @return none.
 */
void ARM_PID_Init_q31(ARM_PID_Remove * S)
{
	S->A0 = A0_DATA;
	S->A1 = A1_DATA;
	S->A2 = A2_DATA;
	S->state[0] = 0;
	S->state[1] = 0;
	S->state[2] = 0;
}


/**
 * @brief  Reset function for the Q31 PID Control.
 * @param[in,out] *S points to an instance of the Q31 PID Control structure
 * @return none
 */
void ARM_PID_Reset_q31(ARM_PID_Remove * S)
{
	S->state[0] = 0;
	S->state[1] = 0;
	S->state[2] = 0;
}


/**
 * @defgroup PID PID Motor Control
 *
 * A Proportional Integral Derivative (PID) controller is a generic feedback control
 * loop mechanism widely used in industrial control systems.
 * A PID controller is the most commonly used type of feedback controller.
 *
 * This set of functions implements (PID) controllers
 * for Q15, Q31, and floating-point data types.  The functions operate on a single sample
 * of data and each call to the function returns a single processed value.
 * <code>S</code> points to an instance of the PID control data structure.  <code>in</code>
 * is the input sample value. The functions return the output value.
 *
 * \par Algorithm:
 * <pre>
 *    y[n] = y[n-1] + A0 * x[n] + A1 * x[n-1] + A2 * x[n-2]
 *    A0 = Kp + Ki + Kd
 *    A1 = (-Kp ) - (2 * Kd )
 *    A2 = Kd  </pre>
 *
 * \par
 * where \c Kp is proportional constant, \c Ki is Integral constant and \c Kd is Derivative constant
 *
 * \par
 * \image html PID.gif "Proportional Integral Derivative Controller"
 *
 * \par
 * The PID controller calculates an "error" value as the difference between
 * the measured output and the reference input.
 * The controller attempts to minimize the error by adjusting the process control inputs.
 * The proportional value determines the reaction to the current error,
 * the integral value determines the reaction based on the sum of recent errors,
 * and the derivative value determines the reaction based on the rate at which the error has been changing.
 *
 * \par Instance Structure
 * The Gains A0, A1, A2 and state variables for a PID controller are stored together in an instance data structure.
 * A separate instance structure must be defined for each PID Controller.
 * There are separate instance structure declarations for each of the 3 supported data types.
 *
 * \par Reset Functions
 * There is also an associated reset function for each data type which clears the state array.
 *
 * \par Initialization Functions
 * There is also an associated initialization function for each data type.
 * The initialization function performs the following operations:
 * - Initializes the Gains A0, A1, A2 from Kp,Ki, Kd gains.
 * - Zeros out the values in the state buffer.
 *
 * \par
 * Instance structure cannot be placed into a const data section and it is recommended to use the initialization function.
 *
 * \par Fixed-Point Behavior
 * Care must be taken when using the fixed-point versions of the PID Controller functions.
 * In particular, the overflow and saturation behavior of the accumulator used in each function must be considered.
 * Refer to the function specific documentation below for usage guidelines.
 */

/**
 * @brief  Process function for the Q31 PID Control.
 * @param[in,out] *S points to an instance of the Q31 PID Control structure
 * @param[in] in input sample to process
 * @return out processed output sample.
 *
 * <b>Scaling and Overflow Behavior:</b>
 * \par
 * The function is implemented using an internal 64-bit accumulator.
 * The accumulator has a 2.62 format and maintains full precision of the intermediate multiplication results but provides only a single guard bit.
 * Thus, if the accumulator result overflows it wraps around rather than clip.
 * In order to avoid overflows completely the input signal must be scaled down by 2 bits as there are four additions.
 * After all multiply-accumulates are performed, the 2.62 accumulator is truncated to 1.32 format and then saturated to 1.31 format.
 */

int16_t ARM_PID_q31( ARM_PID_Remove * S, int16_t in)
{
	float   acc;
	int16_t out;

	/* acc = A0 * x[n]  */
	acc = S->A0 * in;

	/* acc += A1 * x[n-1] */
	acc += S->A1 * S->state[0];

	/* acc += A2 * x[n-2]  */
	acc += S->A2 * S->state[1];

	/* convert output to 1.31 format to add y[n-1] */
	out = (int16_t)acc;
/*---------------------------------  添加段  ---------------------------------*/
	if( out >= OUT_Sync_CHG )                 // 限制正向变化幅度
		out = OUT_Sync_CHG;
	
	if( out <= OUT_Async_CHG )                 // 限制反向变化幅度
		out = OUT_Async_CHG;
/*----------------------------------- END ------------------------------------*/

	/* out += y[n-1] */
	out += S->state[2];

/*---------------------------------  添加段  ---------------------------------*/
	if( out >= OUT_Sync_MAX )                 // 限制正向幅度
		out = OUT_Sync_MAX;
	
	if( out <= OUT_Async_MAX )                 // 限制反向幅度
		out = OUT_Async_MAX;
/*----------------------------------- END ------------------------------------*/

	/* Update state */
	S->state[1] = S->state[0];
	S->state[0] = in;
	S->state[2] = out;

	/* return to application */
	return (out);

}

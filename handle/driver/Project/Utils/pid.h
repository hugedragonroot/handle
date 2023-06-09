/**********************************************************/
//@Autor:Raw
//@Version:0.0.1
//@End Time:2022/12/12
/**********************************************************/
#ifndef __PID_H__
#define __PID_H__
#include "stdio.h"
#include "stdint.h"
typedef struct _PIDFloat_Obj_
{
	volatile float Kp;                     //!< the proportional gain for the PID controller
	volatile float Ki;                     //!< the integral gain for the PID controller
	volatile float Kd;                     //!< the derivative gain for the PID controller

	volatile float Ui;                     //!< the integrator start value for the PID controller
	volatile float PreErr;

	volatile float refValue;               //!< the reference input value
	volatile float fbackValue;             //!< the feedback input value
	volatile float Error;		

	volatile float outMin;                 //!< the minimum output value allowed for the PID controller
	volatile float outMax;                 //!< the maximum output value allowed for the PID controller

	volatile float PIDOut;  
	
} PIDFloat_Obj;


void pid_init(PIDFloat_Obj *Config_pid,float kp,float ki,float kd,float outmax,float outmin);
void clear_pid(PIDFloat_Obj *Clear_pid);
void PID_run_FloatspdVolt(PIDFloat_Obj* handle, float refValue, float fbackValue);
#endif

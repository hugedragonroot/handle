#include "pid.h"

void pid_init(PIDFloat_Obj *Config_pid,float kp,float ki,float kd,float outmax,float outmin)
{
	
	Config_pid->Kp = kp;
	Config_pid->Ki = ki;
	Config_pid->Kd = kd;
	Config_pid->outMax = outmax;
	Config_pid->outMin = outmin;
	
}


void clear_pid(PIDFloat_Obj *Clear_pid)
{
	Clear_pid->Error = 0.0f;
	Clear_pid->Ui = 0.0f;
	Clear_pid->PreErr = 0.0f;
	Clear_pid->PIDOut = 0.0f;
}




#define EableDT
void PID_run_FloatspdVolt(PIDFloat_Obj* handle, float refValue, float fbackValue)
{
#ifdef EableDT
  float D_term , D_Error;
#endif
  float Up;
  
 handle->Error = refValue - fbackValue;
  
 Up = handle->Kp*handle->Error;
 
 handle->Ui = (handle->Ui + handle->Ki*handle->Error);
	
	
 
 if(handle->Ui > handle->outMax)
  handle->Ui = handle->outMax;
// else if(handle->Ui < -handle->outMax)
//  handle->Ui = -handle->outMax;
 else if(handle->Ui < handle->outMin) //TODO
  handle->Ui = handle->outMin;
 
#ifdef EableDT

 D_Error = handle->Error - handle->PreErr;
 D_term  = handle->Kd*D_Error;
 handle->PreErr = handle->Error;

 handle->PIDOut = Up + handle->Ui + D_term;   
 
#else 
 handle->PIDOut = Up + handle->Ui;             // Saturate the output
#endif
 if(handle->PIDOut > handle->outMax)
  handle->PIDOut = handle->outMax;
 else if(handle->PIDOut < handle->outMin)
  handle->PIDOut = handle->outMin;
}









/**********************************************************/
//@Autor:Raw
//@Version:0.0.1
//@End Time:
//@Function:
/**********************************************************/
#include "Brush_Driver.h"
#include "gpio.h"
#include "tim.h"
#include "adc_callback.h"
#include "Brush_Calib.h"
Brush_Motor_t Brushx;
Brush_Motor_t Brush[2];

/****************************************
  函数名称：Brush_para_init
  函数表述：电机结构体参数初始化
  函数参数：无
  函数返回值：无
***************************************/
void Brush_para_init(void)
{
	
	memset(&Brush[Brush_A], 0, sizeof(Brush[Brush_A]));
	memset(&Brush[Brush_U], 0, sizeof(Brush[Brush_U]));
	
	Brush_Data_Check();
	Brush_Read_Para(Brush_A);
//	Motor_Data_Read();//读取flash的电机参数数据
	Brush[Brush_A].MAX_speed =	BRUSH_MAX_SPEED;//速度最大值
	Brush[Brush_A].Error_range_angle = 11;//误差范围±8个脉冲
	Brush[Brush_A].AngelAcc = 600;//角加速度
	Brush[Brush_A].SpeedAcc = 30.0f;//速度加速度
	Brush[Brush_A].brush_Cmd=Brush_Cmd_Stop;//停止模式
	Brush[Brush_A].Current_MAX = 100.0f;
//	Brush[Brush_A].Total_Current = 200.0f;
	Brush[Brush_A].Trip_Calib_Current = 80.0f;
	Brush[Brush_A].Voltage_acc = 0.01f;
	Brush[Brush_A].MPU_acc = 0.5f;
	Brush[Brush_A].MPU_Set = 0.0f;
	Brush[Brush_A].MPU_New = 0.0f; 

	
	Brush_Read_Para(Brush_U);
	Brush[Brush_U].MAX_speed =	BRUSH_MAX_SPEED;
	Brush[Brush_U].Error_range_angle = 8;
	Brush[Brush_U].AngelAcc = 600;
	Brush[Brush_U].SpeedAcc = 30.0f;
	Brush[Brush_U].brush_Cmd=Brush_Cmd_Stop;	
	Brush[Brush_U].Current_MAX = 100.0f;
//	Brush[Brush_U].Total_Current = 200.0f;
	Brush[Brush_U].Trip_Calib_Current = 80.0f;
	Brush[Brush_U].Voltage_acc = 0.01f;
	Brush[Brush_U].MPU_acc = 0.5f;
	Brush[Brush_U].MPU_Set = 0.0f;
	Brush[Brush_U].MPU_New = 0.0f; 
	

	pid_init(&Brush[Brush_A].ANGLE_PID,3.0f,0.0f,0.0f,Brush[Brush_A].MAX_speed,-Brush[Brush_A].MAX_speed);
	
	pid_init(&Brush[Brush_A].SPEED_PID,0.1f,0.05f,0.0f,0.99f,-0.99f);
	
	pid_init(&Brush[Brush_A].CURRENT_PID,20.0f,5.0f,0.0f,0.99f,-0.99f);
	
//	pid_init(&Brush[Brush_A].MPU_PID,0.25f,0.0005f,0.0f,0.99f,-0.99f);

	pid_init(&Brush[Brush_A].MPU_PID,500.0f,0.0000f,0.0f,Brush[Brush_A].MAX_speed,-Brush[Brush_A].MAX_speed);
	
	pid_init(&Brush[Brush_U].ANGLE_PID,0.9f,0.0f,0.0f,Brush[Brush_U].MAX_speed,-Brush[Brush_U].MAX_speed);
	
	pid_init(&Brush[Brush_U].SPEED_PID,1.0f,0.1f,0.0f,0.99f,-0.99f);
	
	pid_init(&Brush[Brush_U].CURRENT_PID,20.0f,5.0f,0.0f,0.99f,-0.99f);
	
//	pid_init(&Brush[Brush_U].MPU_PID,0.25f,0.0005f,0.0f,0.99f,-0.99f);
//	pid_init(&Brush[Brush_U].MPU_PID,250.0f,0.0000f,0.0f,Brush[Brush_U].MAX_speed,-Brush[Brush_U].MAX_speed);
	pid_init(&Brush[Brush_U].MPU_PID,200.0f,0.0f,0.0f,150.0f,-150.0f);
	
}
/****************************************************************************
 * 函数名：          Brush_Stop
 * 参  数：          无
 * 返回值:           无
 * 描述：            停止波形输出，关闭电机，将转速设定为 0 
 ***************************************************************************/
void Brush_Stop(Brush_Num num)
{
	if(num == Brush_A)
	{
		Brush1_A_OFF();
		Brush1_B_OFF();
	}
	else if(num == Brush_U)
	{
		Brush2_A_OFF();
		Brush2_B_OFF();
	}

}
/****************************************************************************
 * 函数名：          Brush_Star
 * 参  数：          MotorDir 电机方向
 * 返回值:           无
 * 描述：            停止波形输出，关闭电机，将转速设定为 0 
 ***************************************************************************/
void Brush_Star(Brush_Num num,float value)
{
	Brush_Control_Mode(num,value);
	if(num == Brush_A)
	{
		Brush1_A_ON();
		Brush1_B_ON();
	}
	else if(num == Brush_U)
	{
		Brush2_A_ON();
		Brush2_B_ON();

	}
}

/****************************************************************************
 * 函数名：          Brush_Control_Mode
 * 参  数：          FinalOut	最终量输出 正负代表方向 -0.5 ~ 0.5
 * 返回值:           无
 * 描述：            占空比的设定，使用双极性控制时使用
 ***************************************************************************/
void Brush_Control_Mode(Brush_Num num,float FinalOut)
{
	if(num == Brush_A)
	{
		BrushA_PWM_A((uint16_t)((0.5f + FinalOut/2) * DCMOTOR_DUTY_FULL_A));
		BrushA_PWM_B((uint16_t)((0.5f - FinalOut/2) * DCMOTOR_DUTY_FULL_A));
//		gpio_bit_set(GPIOE, GPIO_PIN_5);
//		gpio_bit_reset(GPIOE, GPIO_PIN_6);
	}
	else if(num == Brush_U)
	{
		BrushU_PWM_A((uint16_t)((0.5f + FinalOut/2) * DCMOTOR_DUTY_FULL_U));
		BrushU_PWM_B((uint16_t)((0.5f - FinalOut/2) * DCMOTOR_DUTY_FULL_U));
	}
}


void Brush_Start_Filter(Brush_Num num)
{
	switch(Brush[num].brush_Cmd_Pre)
	{
		case Brush_Cmd_Star:
			if(Brush[num].brush_Cmd != Brush_Cmd_Run&&Brush[num].first_start == false)
			{
				Brush[num].brush_Cmd = Brush_Cmd_Star;
				Brush[num].first_start = true;
			}
			Brush[num].first_stop = false;
			break;
		case Brush_Cmd_Stop:
			if(Brush[num].brush_Cmd != Brush_Cmd_Standby&&Brush[num].first_stop == false)
			{
				Brush[num].brush_Cmd = Brush_Cmd_Stop;
				Brush[num].first_stop = true;
			}
			Brush[num].first_start = false;
			break;
		default:
			break;
	}


}










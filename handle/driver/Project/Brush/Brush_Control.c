/**********************************************************/
//@Autor:Raw
//@Version:0.0.1
//@End Time:2023/3/20
//@Function:Brush_Control.c
/**********************************************************/

#include "Brush_Control.h"
#include "Brush_Driver.h"
#include "tim.h"
#include "adc_callback.h"
#include "can.h"
#include "QMI8658C.h"

/****************************************
  函数名称：RESET_BRUSH_PARA
  函数表述：复位电机设置参数
  函数参数：电机序号
  函数返回值：无
***************************************/
void RESET_BRUSH_PARA(Brush_Num num)
{

	Brush[num].AngelNew = 0.0f;
	Brush[num].MPU_New = 0.0f;
	Brush[num].Voltage_New = 0.0f;
	Brush[num].SpeedlNew =0.0f;
	
	Brush[num].set_speed = 0.0f;
	Brush[num].MPU_Set = 0.0f;
	Brush[num].AngelSet = 0.0f;
	Brush[num].Current_set = 0.0f;
	Brush[num].VOLTAGE_set = 0.0f;
	
}
/****************************************
  函数名称：BRUSH_SPEED_ANGLE_U
  函数表述：电机U速度，角度计算(T法测速)
  函数参数：无
  函数返回值：无
***************************************/
uint8_t flage_brush = 4;
void test_BRUSH(void)
{
	if(flage_brush == 0)
	{
		Brush[Brush_A].brush_Cmd_Pre = Brush_Cmd_Stop;
		Brush[Brush_U].brush_Cmd_Pre = Brush_Cmd_Stop;
	}
#if 0
	if(flage_brush == 1)
	{
		Brush[Brush_A].brush_Cmd_Pre = Brush_Cmd_Star;
		Brush[Brush_U].brush_Cmd_Pre = Brush_Cmd_Star;
		Brush[Brush_A].Control_mode = speed_mode;
		Brush[Brush_A].SpeedlNew = 300;
		Brush[Brush_U].Control_mode = mpu_mode;
		Brush[Brush_U].set_speed = -150.0f;
	}
	
	if(flage_brush == 2)
	{
		Brush[Brush_A].brush_Cmd_Pre = Brush_Cmd_Star;
		Brush[Brush_U].brush_Cmd_Pre = Brush_Cmd_Star;
		Brush[Brush_A].Control_mode = speed_mode;
		Brush[Brush_A].SpeedlNew = -300;
		Brush[Brush_U].Control_mode = mpu_mode;
		Brush[Brush_U].set_speed = 150.0f;
	}
#endif
#if 1
	if(flage_brush == 1)
	{
		Brush[Brush_A].brush_Cmd_Pre = Brush_Cmd_Star;
		Brush[Brush_U].brush_Cmd_Pre = Brush_Cmd_Star;
		Brush[Brush_A].Control_mode = fold_mode;
		Brush[Brush_A].SpeedlNew = 300;
		Brush[Brush_U].Control_mode = fold_mode;
		Brush[Brush_U].SpeedlNew = -300.0f;
	}
	
	if(flage_brush == 2)
	{
		Brush[Brush_A].brush_Cmd_Pre = Brush_Cmd_Star;
		Brush[Brush_U].brush_Cmd_Pre = Brush_Cmd_Star;
		Brush[Brush_A].Control_mode = speed_mode;
		Brush[Brush_A].SpeedlNew = -300;
		Brush[Brush_U].Control_mode = mpu_mode;
		Brush[Brush_U].set_speed = 150.0f;
	}
	if(flage_brush == 3)
	{
		Brush[Brush_A].brush_Cmd_Pre = Brush_Cmd_Star;
		Brush[Brush_U].brush_Cmd_Pre = Brush_Cmd_Star;
		Brush[Brush_A].Control_mode = speed_mode;
		Brush[Brush_A].SpeedlNew = 300;
		Brush[Brush_U].Control_mode = mpu_mode;
		Brush[Brush_U].set_speed = -150.0f;
	}
#endif
}
void BRUSH_SPEED_ANGLE_U(void)
{
	/*计算高电平与周期，单位us*/
	BrushSpeed[Brush_U].TimeUs = (uint32_t)((float)BrushSpeed[Brush_U].u32_SumCount * (1.0f / 84.0f));
	//1.0f/36.0f A相相邻两次的计数次数

	if(BrushSpeed[Brush_U].TimeUs == 0x00000000) BrushSpeed[Brush_U].TimeUs = 0xffffffff;
	//13是电机霍尔的极对数Z
	//28.0f
	BrushSpeed[Brush_U].i16_RealSpeed = (int16_t)((1000.0f * 1000.0f * 60.0f) / ((float)BrushSpeed[Brush_U].TimeUs * 28.0f)); // ==> us/r  * 1000 ==> ms/r  * 1000 = S/r  *60  ==> min/r   
	Brush[Brush_U].return_speed = BrushSpeed[Brush_U].i16_RealSpeed ;
	//4.0f是人为设置的极对数，因为电机没有文档，只能人为设置大概转速
	
	//公式：N=60*f/ZM,f脉冲频率，z表示一圈产生的脉冲数（11，1倍频，极对数13），M采样周期内产生脉冲数
	if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim4) == 1)//方向标志
	{
		Brush[Brush_U].real_speed = (float)BrushSpeed[Brush_U].i16_RealSpeed;//速度赋值
	}
	else
	{
		Brush[Brush_U].real_speed = (float)BrushSpeed[Brush_U].i16_RealSpeed* (-1.0f) ;//速度赋值
	}
	
	BrushSpeed[Brush_U].PosCountPHA =(BrushSpeed[Brush_U].PosWRIFCount - 1)*65536 + Brush_U_CNT();//计算A相脉冲个数，就是角度脉冲值

	Brush[Brush_U].LineRealPullLenth = BrushSpeed[Brush_U].PosCountPHA - BrushSpeed[Brush_U].PosBase;//记录A相位置

	Brush[Brush_U].AngelReal =  Brush[Brush_U].LineRealPullLenth;//换算电机真实角度
}

/****************************************
  函数名称：MOTOR_SPEED_ANGLE_A
  函数表述：电机A速度，角度计算（T法测速）
  函数参数：无
  函数返回值：无
***************************************/
void BRUSH_SPEED_ANGLE_A(void)
{
	/*计算高电平与周期，单位us*/
	BrushSpeed[Brush_A].TimeUs = (uint32_t)((float)BrushSpeed[Brush_A].u32_SumCount * (1.0f / 84.0f));
//	BrushSpeed[Brush_A].TimeUs = (uint32_t)((float)BrushSpeed[Brush_A].u32_SumCount * (1.0f / 2.0f));
	//1.0f/36.0f A相相邻两次的计数次数

	if(BrushSpeed[Brush_A].TimeUs == 0x00000000) BrushSpeed[Brush_A].TimeUs = 0xffffffff;
	//13是电机霍尔的极对数Z
	BrushSpeed[Brush_A].i16_RealSpeed = (int16_t)((1000.0f * 1000.0f * 60.0f) / ((float)BrushSpeed[Brush_A].TimeUs * 1.0f)); // ==> us/r  * 1000 ==> ms/r  * 1000 = S/r  *60  ==> min/r   24
	Brush[Brush_A].return_speed = BrushSpeed[Brush_A].i16_RealSpeed ;
	//4.0f是人为设置的极对数，因为电机没有文档，只能人为设置大概转速
	
	//公式：N=60*f/ZM,f脉冲频率，z表示一圈产生的脉冲数（11，1倍频，极对数13），M采样周期内产生脉冲数
	if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim3) == 0)//方向标志
	{
		Brush[Brush_A].real_speed = (float)BrushSpeed[Brush_A].i16_RealSpeed;//速度赋值
	}
	else
	{
		Brush[Brush_A].real_speed = (float)BrushSpeed[Brush_A].i16_RealSpeed* (-1.0f) ;//速度赋值
	}
	
	BrushSpeed[Brush_A].PosCountPHA =(BrushSpeed[Brush_A].PosWRIFCount - 1)*65536 + Brush_A_CNT();//计算A相脉冲个数，就是角度脉冲值

	Brush[Brush_A].LineRealPullLenth = BrushSpeed[Brush_A].PosCountPHA - BrushSpeed[Brush_A].PosBase;//记录A相位置
	Brush[Brush_A].AngelReal = Brush[Brush_A].LineRealPullLenth;//换算电机真实角度
}

/****************************************
  函数名称：Brush_Angel_Acc_handle
  函数表述：角度模式角加速度坡度跟随
  函数参数：电机序号
  函数返回值：无
***************************************/
void Brush_Angel_Acc_handle(Brush_Num num)
{
	if(Brush[num].AngelSet < Brush[num].AngelNew)//正向坡度
	{
			
		Brush[num].AngelSet = Brush[num].AngelSet + Brush[num].AngelAcc ;
		if(Brush[num].AngelSet > Brush[num].AngelNew)
		{
			Brush[num].AngelSet = Brush[num].AngelNew ;
		}
	}
	else if(Brush[num].AngelSet > Brush[num].AngelNew)//反向坡度
	{
		Brush[num].AngelSet = Brush[num].AngelSet - Brush[num].AngelAcc ;
		if(Brush[num].AngelSet < Brush[num].AngelNew)
		{
			Brush[num].AngelSet = Brush[num].AngelNew ;
		}
	}
	
	
}
/****************************************
  函数名称：Brush_Speed_Acc_handle
  函数表述：速度模式加速度坡度跟随
  函数参数：电机序号
  函数返回值：无
***************************************/
void Brush_Speed_Acc_handle(Brush_Num num)
{
	if(Brush[num].set_speed < Brush[num].SpeedlNew)
	{
			
		Brush[num].set_speed = Brush[num].set_speed + Brush[num].SpeedAcc ;//正向速度坡度
		if(Brush[num].set_speed > Brush[num].SpeedlNew)
		{
			Brush[num].set_speed = Brush[num].SpeedlNew ;
		}
	}
	else if(Brush[num].set_speed > Brush[num].SpeedlNew)
	{
		Brush[num].set_speed = Brush[num].set_speed - Brush[num].SpeedAcc ;//反向速度坡度
		if(Brush[num].set_speed < Brush[num].SpeedlNew)
		{
			Brush[num].set_speed = Brush[num].SpeedlNew ;
		}
	}
	
	
}
/****************************************
  函数名称：Brush_Voltage_handle
  函数表述：电压模式处理
  函数参数：电机序号
  函数返回值：无
***************************************/
void Brush_Voltage_handle(Brush_Num num)
{
	if(Brush[num].VOLTAGE_set < Brush[num].Voltage_New)
	{
			
		Brush[num].VOLTAGE_set = Brush[num].VOLTAGE_set + Brush[num].Voltage_acc ;//正向速度坡度
		if(Brush[num].VOLTAGE_set > Brush[num].Voltage_New)
		{
			Brush[num].VOLTAGE_set = Brush[num].Voltage_New ;
		}
	}
	else if(Brush[num].VOLTAGE_set > Brush[num].Voltage_New)
	{
		Brush[num].VOLTAGE_set = Brush[num].VOLTAGE_set - Brush[num].Voltage_acc ;//反向速度坡度
		if(Brush[num].VOLTAGE_set < Brush[num].Voltage_New)
		{
			Brush[num].VOLTAGE_set = Brush[num].Voltage_New ;
		}
	}
	Brush_Control_Mode(num,Brush[num].VOLTAGE_set);//双极性控制
	
}

void Brush_MPU_ACC_handle(Brush_Num num)
{
	if(Brush[num].MPU_Set < (float)Brush[num].MPU_New)
	{
			
		Brush[num].MPU_Set = Brush[num].MPU_Set + Brush[num].MPU_acc ;//正向速度坡度
		if(Brush[num].MPU_Set > (float)Brush[num].MPU_New)
		{
			Brush[num].MPU_Set = (float)Brush[num].MPU_New ;
		}
	}
	else if(Brush[num].MPU_Set > (float)Brush[num].MPU_New)
	{
		Brush[num].MPU_Set = Brush[num].MPU_Set - Brush[num].MPU_acc ;//反向速度坡度
		if(Brush[num].MPU_Set < (float)Brush[num].MPU_New)
		{
			Brush[num].MPU_Set = (float)Brush[num].MPU_New ;
		}
	}
	
}
/****************************************
  函数名称：Brush_Speed_Handle
  函数表述：速度模式处理
  函数参数：电机序号
  函数返回值：无
***************************************/
void Brush_Speed_Handle(Brush_Num num)
{
	Brush_Speed_Acc_handle(num);//加速度坡度
	if(Brush[num].SpeedlNew==0&&fabs(Brush[num].real_speed)<2.0f)//因为编码器分辨率低，太低速要清除积分误差
	{
		Brush[num].SPEED_PID.Ui = 0.0f;
	}
	PID_run_FloatspdVolt(&Brush[num].SPEED_PID,Brush[num].set_speed/Brush[num].MAX_speed,Brush[num].real_speed/Brush[num].MAX_speed);//速度pid，标幺
	Brush_Control_Mode(num,Brush[num].SPEED_PID.PIDOut);//双极性控制
}



/****************************************
  函数名称：Brush_Angle_Handle
  函数表述：角度模式处理
  函数参数：电机序号
  函数返回值：无
***************************************/
void Brush_Angle_Handle(Brush_Num num)
{
//	Angel_Acc_handle(NUM);
	Brush[num].AngelSet = Brush[num].AngelNew;
	if(fabs((float)Brush[num].AngelReal - (float)Brush[num].AngelNew)>Brush[num].Error_range_angle )//角度在误差范围外继续闭环
	{
		if(++Brush[num].angle_count>=2)//外环周期是内环的两倍
		{
			Brush[num].angle_count = 0;
			PID_run_FloatspdVolt(&Brush[num].ANGLE_PID,Brush[num].AngelSet,Brush[num].AngelReal);//角度闭环
			Brush[num].set_speed = Brush[num].ANGLE_PID.PIDOut;//输出给定速度环
		}
		PID_run_FloatspdVolt(&Brush[num].SPEED_PID,Brush[num].set_speed/Brush[num].MAX_speed,Brush[num].real_speed/Brush[num].MAX_speed);//速度闭环
		Brush_Control_Mode(num,Brush[num].SPEED_PID.PIDOut);	
	}
	else
	{
		clear_pid(&Brush[num].ANGLE_PID);//角度在误差范围内清除pid参数
//		Brush[num].brush_Cmd =Brush_Cmd_Stop ;
	}

}

void Brush_MPU_Handle(Brush_Num num)
{
//	Angel_Acc_handle(NUM);
	if(num == Brush_A)
	{
		Brush[num].MPU_REAL = mpu_filter.ptich - Brush[num].MPU_Basic;
		Brush[num].return_mpu = (int16_t)Brush[num].MPU_REAL;
		Brush_MPU_ACC_handle(num);
		if(fabs(Brush[num].MPU_REAL- (float)Brush[num].MPU_New)>1.0f )//角度在误差范围外继续闭环
		{
			if(++Brush[num].angle_count>=2)//外环周期是内环的两倍
			{
				Brush[num].angle_count = 0;
//				Brush[num].Tem_MPU = (Brush[num].MPU_REAL - Brush[num].Last_MPU)/0.02f*100.0f;
//				Brush[num].Last_MPU = Brush[num].MPU_REAL;
				if(Brush[num].MPU_REAL>0.0f) Brush[num].set_speed = -200.0f;
				if(Brush[num].MPU_REAL<0.0f) Brush[num].set_speed = 200.0f;
				PID_run_FloatspdVolt(&Brush[num].MPU_PID,Brush[num].MPU_Set,Brush[num].MPU_REAL);//角度闭环
//				Brush[num].set_speed = Brush[num].MPU_PID.PIDOut;//输出给定速度环
			}
			PID_run_FloatspdVolt(&Brush[num].SPEED_PID,(Brush[num].set_speed+ Brush[num].MPU_PID.PIDOut)/Brush[num].MAX_speed,Brush[num].real_speed/Brush[num].MAX_speed);//速度闭环
		}
		else
		{
//			Brush[num].Last_MPU = Brush[num].MPU_REAL;
			clear_pid(&Brush[num].MPU_PID);//角度在误差范围内清除pid参数
			clear_pid(&Brush[num].SPEED_PID);
//		Brush[num].brush_Cmd =Brush_Cmd_Stop ;
		}
//		Brush_Control_Mode(num,Brush[num].MPU_PID.PIDOut);	
		Brush_Control_Mode(num,Brush[num].SPEED_PID.PIDOut);	
	}
	else if(num == Brush_U)
	{
		Brush[num].MPU_REAL = mpu_filter.ptich - Brush[num].MPU_Basic;
		Brush[num].return_mpu = (int16_t)Brush[num].MPU_REAL;
		if(Brush[Brush_A].SpeedlNew >0.0f)Brush[num].set_speed = -150;
		if(Brush[Brush_A].SpeedlNew <0.0f)Brush[num].set_speed = 150;
		if(++Brush[num].angle_count>=2)//外环周期是内环的两倍
		{
			Brush[num].angle_count = 0;
			PID_run_FloatspdVolt(&Brush[num].MPU_PID,Brush[num].MPU_Set,Brush[num].MPU_REAL);//角度闭环
		}
		PID_run_FloatspdVolt(&Brush[num].SPEED_PID,(Brush[num].set_speed+ Brush[num].MPU_PID.PIDOut)/Brush[num].MAX_speed,Brush[num].real_speed/Brush[num].MAX_speed);
		Brush_Control_Mode(num,Brush[num].SPEED_PID.PIDOut);
	}

}


void Brush_Flod_Handle(Brush_Num num)
{
	Brush_Speed_Acc_handle(num);//加速度坡度
	if(Brush[num].SpeedlNew==0&&fabs(Brush[num].real_speed)<2.0f)//因为编码器分辨率低，太低速要清除积分误差
	{
		Brush[num].SPEED_PID.Ui = 0.0f;
	}
	PID_run_FloatspdVolt(&Brush[num].SPEED_PID,Brush[num].set_speed/Brush[num].MAX_speed,Brush[num].real_speed/Brush[num].MAX_speed);//速度pid，标幺
	Brush_Control_Mode(num,Brush[num].SPEED_PID.PIDOut);//双极性控制
}

void Brush_Zero_Handle(Brush_Num num)
{
	if(num == Brush_A)
	{
		Brush_Speed_Acc_handle(num);//加速度坡度
		if(Brush[num].SpeedlNew==0&&fabs(Brush[num].real_speed)<2.0f)//因为编码器分辨率低，太低速要清除积分误差
		{
			Brush[num].SPEED_PID.Ui = 0.0f;
		}
		PID_run_FloatspdVolt(&Brush[num].SPEED_PID,Brush[num].set_speed/Brush[num].MAX_speed,Brush[num].real_speed/Brush[num].MAX_speed);//速度pid，标幺
		Brush_Control_Mode(num,Brush[num].SPEED_PID.PIDOut);//双极性控制
	}
	else if(num == Brush_U)
	{
		Brush[num].MPU_REAL = mpu_filter.ptich - Brush[num].MPU_Basic;
		Brush[num].return_mpu = (int16_t)Brush[num].MPU_REAL;
		Brush_MPU_ACC_handle(num);
		if(fabs(Brush[num].MPU_REAL- (float)Brush[num].MPU_New)>0.5f )//角度在误差范围外继续闭环
		{
			if(++Brush[num].angle_count>=2)//外环周期是内环的两倍
			{
				Brush[num].angle_count = 0;
//				Brush[num].Tem_MPU = (Brush[num].MPU_REAL - Brush[num].Last_MPU)/0.02f*100.0f;
//				Brush[num].Last_MPU = Brush[num].MPU_REAL;
				PID_run_FloatspdVolt(&Brush[num].MPU_PID,Brush[num].MPU_Set,Brush[num].MPU_REAL);//角度闭环
				Brush[num].set_speed = Brush[num].MPU_PID.PIDOut;//输出给定速度环
			}
			PID_run_FloatspdVolt(&Brush[num].SPEED_PID,Brush[num].set_speed/Brush[num].MAX_speed,Brush[num].real_speed/Brush[num].MAX_speed);//速度闭环
			Brush[num].brush_state.bit.FINISH_calibration_One = 0;
		}
		else
		{
//			Brush[num].Last_MPU = Brush[num].MPU_REAL;
			clear_pid(&Brush[num].MPU_PID);//角度在误差范围内清除pid参数
			clear_pid(&Brush[num].SPEED_PID);
			Brush[num].brush_state.bit.FINISH_calibration_One = 1;
//		Brush[num].brush_Cmd =Brush_Cmd_Stop ;
		}
//		Brush_Control_Mode(num,Brush[num].MPU_PID.PIDOut);	
		Brush_Control_Mode(num,Brush[num].SPEED_PID.PIDOut);
	}
}
/****************************************
  函数名称：Brush_Current_Handle
  函数表述：电流模式处理
  函数参数：电机序号
  函数返回值：无
***************************************/
void Brush_Current_Handle(Brush_Num num)
{
	if(Brush[Brush_A].Current_set==0.0f&&fabs(Brush[num].Current_real)<0.1f)//采样电阻有差异，太低电流有偏差，直接清除pid参数
	{
		Brush[num].CURRENT_PID.Ui = 0.0f;
	}
	PID_run_FloatspdVolt(&Brush[num].CURRENT_PID,Brush[num].Current_set/Brush[num].Current_MAX,Brush[num].Current_real/Brush[num].Current_MAX);//电流环pid
	Brush_Control_Mode(num,Brush[num].CURRENT_PID.PIDOut);
}

/****************************************
  函数名称：Brush_Run_Task
  函数表述：电机运行模式下的任务状态机
  函数参数：电机序号
  函数返回值：无
***************************************/
void Brush_Run_Task(Brush_Num num)
{

	switch(Brush[num].Control_mode)
	{
		case normal_mode://上电默认模式
			break;
		case angle_mode://角度模式
			Brush_Angle_Handle(num);
			break;
		case speed_mode://速度模式
			Brush_Speed_Handle(num);
			break;
		case Voltage_mode://电压模式
			Brush_Voltage_handle(num);
			break;
		case current_mode://电流模式
			Brush_Current_Handle(num);
			break;
		case calib_mode:
			break;
		case mpu_mode:
			Brush_MPU_Handle(num);
			break;
		case fold_mode:
			Brush_Flod_Handle(num);
			break;
		case zero_mode:
			Brush_Zero_Handle(num);
			break;
		default:
			break;
	}
	
}
/****************************************
  函数名称：Over_Current
  函数表述：过流监测，保护
  函数参数：无
  函数返回值：无
***************************************/
uint16_t CurrentFaultNumber[3]={0};		
void Brush_Over_Current(void)
{
	if(Brush[Brush_A].Control_mode == calib_mode&&Brush[Brush_U].Control_mode == calib_mode)//校准模式下
	{
		if(fabs(Brush[Brush_A].Current_real)>Brush[Brush_A].Trip_Calib_Current||fabs(Brush[Brush_U].Current_real)>Brush[Brush_U].Trip_Calib_Current)//校准电流0.5a
		{
			CurrentFaultNumber[0]++;
			if(CurrentFaultNumber[0]>10)//1s
			{
				CurrentFaultNumber[0]=0;
				Brush[Brush_A].brush_Cmd = Brush_Cmd_Stop;//停止电机
				Brush[Brush_U].brush_Cmd = Brush_Cmd_Stop;//停止电机
			}
		}
		else
		{
			CurrentFaultNumber[0]=0;
		}
	}	
	if(Brush[Brush_A].brush_Cmd == Brush_Cmd_Run&&Brush[Brush_A].Control_mode != calib_mode)//A电机其他运行模式下
	{
		if(fabs(Brush[Brush_A].Current_real)>Brush[Brush_A].Total_Current)//总过流值1a
		{
			CurrentFaultNumber[1]++;
			if(CurrentFaultNumber[1]>10)//1s
			{
				CurrentFaultNumber[1]=0;
//				Brush[Brush_A].AngelKeep = Brush[Brush_A].AngelReal;
				Brush[Brush_A].brush_state.bit.OverCur = 1;//过流标志制1
			}
		}
		else
		{
			CurrentFaultNumber[1]=0;
		}
	}
	
	if(Brush[Brush_U].brush_Cmd == Brush_Cmd_Run&&Brush[Brush_U].Control_mode != calib_mode)//U电机其他运行模式下
	{
		if(fabs(Brush[Brush_U].Current_real)>Brush[Brush_U].Total_Current)//总过流值1a
		{
			CurrentFaultNumber[2]++;
			if(CurrentFaultNumber[2]>10)//1s
			{
				CurrentFaultNumber[2]=0;
//				Brush[Brush_U].AngelKeep = Brush[Brush_U].AngelReal;
				Brush[Brush_U].brush_state.bit.OverCur = 1;//过流标志制1
			}
		}
		else
		{
			CurrentFaultNumber[2]=0;
		}
	}

}
/****************************************
  函数名称：ADC_CURRENT_HANDLE
  函数表述：adc电流处理
  函数参数：无
  函数返回值：无
***************************************/
void ADC_CURRENT_HANDLE(void)
{
	Brush_GetCurrent();/* 读取电流AD值 */
  /* A电机电流 */
  Brush[Brush_A].Current_ADC = 0.9f*Brush[Brush_A].Current_ADC + ADC_Data.Brush_adc[Brush_A].adc_current*0.1f;//一阶低通滤波
	if(Brush1_A_PWM_READ()>Brush1_B_PWM_READ())//电流方向
	{
		Brush[Brush_A].Current_real = Brush[Brush_A].Current_ADC;
	}
	else
	{
		Brush[Brush_A].Current_real = -Brush[Brush_A].Current_ADC;
	}
	Brush[Brush_A].return_current = (int16_t)Brush[Brush_A].Current_real;
  /* U电机电流 */
    Brush[Brush_U].Current_ADC  = 0.9f*Brush[Brush_U].Current_ADC  + ADC_Data.Brush_adc[Brush_U].adc_current*0.1f;//一阶低通滤波
	if(Brush2_A_PWM_READ()>Brush2_B_PWM_READ())//电流方向
	{
		Brush[Brush_U].Current_real = Brush[Brush_U].Current_ADC ;
	}
	
	else
	{
		Brush[Brush_U].Current_real = -Brush[Brush_U].Current_ADC ;
	}
	Brush[Brush_U].return_current = (int16_t)Brush[Brush_U].Current_real;
	
//	Over_Current();
}
/****************************************
  函数名称：STATUS_UPDATE
  函数表述：电机母线电压，mos，限位开关的状态更新
  函数参数：无
  函数返回值：无
***************************************/
static uint8_t vd_count = 0;
uint32_t ms_counts = 0;
void STATUS_UPDATE(void)
{
	ms_counts++;
	if(++vd_count>=5)
	{
		vd_count = 0;
		GetVoltage_MOSTEMP();//获取母线电压
	}
	if(ADC_Data.Bus_Volt>=30.0f)//过压30v
	{
		Brush[Brush_A].brush_state.bit.OverVoltag = 1;
		Brush[Brush_U].brush_state.bit.OverVoltag = 1;
	}
	else if(ADC_Data.Bus_Volt<16)//欠压16v
	{
		Brush[Brush_A].brush_state.bit.Undervoltag = 1;
		Brush[Brush_U].brush_state.bit.Undervoltag = 1;
	}
	else if(ADC_Data.Bus_Volt>16.0f&&ADC_Data.Bus_Volt<30.0f)//正常电压范围内
	{
		
		Brush[Brush_A].brush_state.bit.OverVoltag = 0;
		Brush[Brush_U].brush_state.bit.OverVoltag = 0;
		Brush[Brush_A].brush_state.bit.Undervoltag = 0;
		Brush[Brush_U].brush_state.bit.Undervoltag = 0;
	}
	if(ADC_Data.mos_temp > 90.0f)//过温
	{
		Brush[Brush_A].brush_state.bit.OverTemp = 1;
		Brush[Brush_U].brush_state.bit.OverTemp = 1;
	}
	
}

/****************************************
  函数名称：brush_Cmd_handle
  函数表述：电机状态机
  函数参数：电机序号
  函数返回值：无
***************************************/
void brush_Cmd_handle(Brush_Num num)
{
		switch(Brush[num].brush_Cmd)
		{
			case Brush_Cmd_Stop://停止模式
				Brush_Stop(num);//关pwm
				if(++Brush[num].stop_count>=5)//计数50ms开抱闸
				{
					Brush[num].stop_count = 0;
					Brush[num].brush_Cmd = Brush_Cmd_Standby;//空闲状态
					RESET_BRUSH_PARA(num);//复位电机设置参数
				}
				break;
			case Brush_Cmd_Standby://空闲模式
				clear_pid(&Brush[num].ANGLE_PID);//清除应用pid参数
				clear_pid(&Brush[num].SPEED_PID);
				clear_pid(&Brush[num].CURRENT_PID);
				clear_pid(&Brush[num].MPU_PID);
				RESET_BRUSH_PARA(num);
				break;
			case Brush_Cmd_Star://电机启动模式
				if(++Brush[num].start_count>=5)//计数50ms运行电机
				{
					Brush_Star(num,0.0f);//开pwm
					Brush[num].start_count = 0;
					Brush[num].brush_Cmd = Brush_Cmd_Run;//运行模式
				}

				break;
			case Brush_Cmd_Run://运行模式
				Brush_Run_Task(num);//电机模式状态机
				break;
			default:
				break;
		}
}


void Brush_Mode_Loop(Brush_Num num)
{
	if(Brush[num].brush_state.bit.Current_Error != 1)
	{
		Brush_Start_Filter(num);
	}

}	




























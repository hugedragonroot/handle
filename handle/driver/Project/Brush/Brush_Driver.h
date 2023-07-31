/**********************************************************/
//@Autor:Raw
//@Version:0.0.1
//@End Time:2023/3/20
/**********************************************************/
#ifndef __BRUSH_DRIVER__
#define __BRUSH_DRIVER__
#include "common.h"
#include "pid.h"
#define BRUSH_MAX_SPEED	300.0f
typedef union 
{
  struct
  {
		     
		uint16_t OverCur:1;                 //过流
		uint16_t Undervoltag:1;             //欠压
		uint16_t OverVoltag:1;          		//过压 
		uint16_t OverTemp:1;								//过温		
		
		uint16_t FINISH_calibration_One:1;  //夹紧校准完成
		uint16_t Bandbrake:1;           		//抱闸状态
		uint16_t Current_Error:1;						//采样电流异常
		uint16_t pitch_error:1;							//俯仰角阈值
		
		uint16_t rol_error:1;								//圆滚角阈值
  }bit;
  uint16_t all;
}Brush_Status_t;

typedef enum 
{
	Brush_Cmd_Stop = 0, //电机停止
	Brush_Cmd_Star, //电机启动
	Brush_Cmd_Run,	//电机运转
	Brush_Cmd_Calib,	//电机校准
	Brush_Cmd_Standby,//待机
}Brush_Cmd_e_t;


typedef enum
{
	normal_mode = 0,
	angle_mode = 1,	//角度模式
	speed_mode = 2,	//速度模式
	Voltage_mode = 3,	//电压模式
	current_mode = 4,	//电流模式
	calib_mode = 5,		//校准模式（速度模式）
	mpu_mode = 6,		//陀螺仪模式
	fold_mode = 7,	//
	zero_mode = 8,	//
	stop_mode=0xff,
}Brush_mode_t;


typedef struct
{
	Brush_Status_t brush_state;	//电机状态
	Brush_Cmd_e_t brush_Cmd;	//电机的控制命令
	Brush_Cmd_e_t brush_Cmd_Pre;	//电机的控制命令
	Brush_mode_t Control_mode;		//	控制模式
	
	float MOS_temp;					//MOS管温度值
	uint16_t Trip_Calib_Current;	//行程过流值，设定
	uint16_t Total_Current;				//总过流值，设定
	int16_t return_current;
	float Current_set;	//电流环设定值
	float VOLTAGE_set;	//电压环设定值
	float Current_real; //电流环读取值
	float Current_ADC;	
	uint16_t Current_MAX;	//最大电流
	float speed_base;	//速度基准
	float set_speed;	//设定速度
	float real_speed;	//真实速度
	int16_t return_speed;
	float MAX_speed;	//速度最大值
	int16_t SpeedlNew;	//应用层设定新速度值
	float SpeedAcc;			//加速度
	float Voltage_New;
	float Voltage_acc;
	
	
	float AngelSet;		//角度设定
	int16_t AngelReal;	//实际角度
	int16_t AngelBase;	//角度的零点
	float AngelMAX;		//角度最大行程,正负区分
	int16_t AngelNew;		//单控设定新角度值
	
	int16_t MPU_New;
	float MPU_Set;
	float MPU_acc;
	float MPU_Basic;
	float roll_basic;
	float MPU_REAL;
	float Last_MPU;
	float Tem_MPU;
	int16_t return_mpu;
	uint16_t AngelTrip;
	uint16_t AngelAcc;	//角加速度
	uint8_t angle_count;	//位置环计数
	uint8_t calib_count;
	uint8_t zero_count;
	
	
	int32_t LineRealPullLenth;	//圈数实际长度
	int8_t Error_range_angle; //位置环设定误差允许范围
	
	uint16_t start_count;	//电机启动计数，为了延时抱闸
	uint16_t stop_count;	//电机停止计数，为了延时抱闸
	uint16_t fold_speed_count;
	bool first_start;
	bool first_stop;
	
	PIDFloat_Obj MPU_PID;	//速度pid
	PIDFloat_Obj SPEED_PID;	//速度pid
	PIDFloat_Obj ANGLE_PID;	//位置pid
	PIDFloat_Obj CURRENT_PID;	//电流pid
}Brush_Motor_t;


extern Brush_Motor_t Brush[2];

void Brush_para_init(void);
void Brush_Stop(Brush_Num num);
void Brush_Star(Brush_Num num,float value);
void Brush_Control_Mode(Brush_Num num,float FinalOut);
void Brush_Mode_Loop(Brush_Num num);
void Brush_Start_Filter(Brush_Num num);
#endif

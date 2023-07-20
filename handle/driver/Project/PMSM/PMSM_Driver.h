/**********************************************************/
//@Autor:Raw
//@Version:0.0.1
//@End Time:2023/3/21
/**********************************************************/
#ifndef __PMSM_DRIVER_H__
#define __PMSM_DRIVER_H__
#include "common.h"
#include "FOC.h"
#include "pid.h"


#define IQ_MAX 30.0f
#define Speed_MAX 3000.0f

typedef union 
{
  struct
  {
		     
		uint16_t OverCur:1;                 //过流
		uint16_t Undervoltag:1;             //欠压
		uint16_t OverVoltag:1;          		//过压 
		uint16_t OverTemp:1;								//过温		
		
		uint16_t hall_Error:1; 							//霍尔数据异常
		uint16_t Bandbrake:1;           		//抱闸状态
		uint16_t Current_Error:1;						//采样电流异常
		uint16_t off_link;									//设备离线
		
		uint16_t hall_calib_error:1;				//hall校准异常
  }bit;
  uint16_t all;
}PMSM_Status_t;
typedef enum
{
	MegCaliNone = 0,	//未校准
	MegCaliPositive,		//正校准
	MegCaliReverse,		//反校准
	MegCaliOK			//校准完毕
}sCaliStatusDef;

typedef enum
{
	MotorStop,	//电机停止
	MotorStart,	//电机初始化
	MotorWatit,	//电机等待
	MotorCail,	//零位校准
	MotorTest,	//电机测试模式
	MotorRun,	//电机运行
	MotorPrecharge,//电机充电
}sStateTypeDef;

typedef enum
{
	DragMode_e = 0,	 	//强拖模式
	HallCaliMode_e=1,		//霍尔校准模式
	SpeedCtrl_e=2,  		//转速模式
	VoltCtrl_e=3,    	//电压模式
	TorqueCtrl_e=4,    	//电流模式
	AngelCtrl_e=5,    	//角度模式
	
	Speed_TorqueCtrl = 6,	//速度，电流环
	angel_current = 7,
	StartMotor_e=9,    		//启动电机
	StopMotor_e=10			//停止电机
	
}sCtrlMode_e;

typedef enum
{
	start_acc = 0,
	run_acc = 1,
	dec_acc = 2,
}Acc_state;
typedef enum
{
	hight_hight = 0x00,
	hight_low = 0x01,
	low_low = 0x02,
	low_hight = 0x04,
	zero_zero = 0x08,
}speed_state;
typedef enum
{
	wheel_first = 0x00,
	wheel_sec = 0x01,
	dir_first = 0x02,
	dir_sec = 0x04,
	rise_first = 0x05,
	null_break = 0x08,
}Second_SPEED_STATUE;

typedef enum
{
	firt_q = 1,
	sec_q = 2,
	third_q = 3,
	four_q = 4,
}quadrant_speed;

typedef enum
{
	null_null = 0,
	rise_speed = 1,
	rise_mid_speed = 2,
	low_speed = 3,
	low_mid_speed = 4,
}rise_low_t;

typedef struct
{
	int16_t angle;
	int16_t last_angle;
	uint8_t Gear;
	uint8_t count_angle;
	rise_low_t rise_low;
	int16_t new_speed_l;
	int16_t new_speed_r;
	int16_t x_new;
	int16_t y_new;
	float SET_COORANGLE;
	float SET_COORANGLE_10X;
	float cosf_angle;
	float l_speed;
	float r_speed;
	float CoordSqrt;
	float CoordSpeed;
	float CoordSpeed_new;
	float a_temp;
	float a;
	float b_temp;
	float c;
	float d;
	
	float a_temp_new;
	float a_new;
	float b_temp_new;
	float c_new;
	float d_new;
	uint8_t	axis_x;
	float axis_y_filter;
	uint8_t axis_y;
	uint8_t start_first;
	quadrant_speed quadrant_t;
	uint8_t count_axisy ;
	float c_acc_d;
	float c_dec_2b;
	float temp_tanf;
	float vector;
}APP_PMSM_DRIVER;

extern APP_PMSM_DRIVER APP_PMSM;
typedef struct
{
	sStateTypeDef MotorState;						//当前状态
	
	sStateTypeDef MotorPreState;				//上一次电机状态
	sCaliStatusDef	MotorCali;				//电机校准状态
	sCtrlMode_e  ControlMode;				//控制模式
	PMSM_Status_t PMSM_Status;
	Second_SPEED_STATUE sec_speed_handle;
	speed_state speed_status;
	quadrant_speed quadrant;
	Acc_state acc_status;
	rise_low_t rise_lows;
	
	float SpeedCmd;		//速度控制
	float SpeedSet;		//速度设定
	float SpeedReal;	//真实速度
	float SpeedFiler;	//滤波速度
	float SpeedNew;		//新速度设定
	int16_t appspeed_l;
	int16_t appspeed_r;
	int16_t LastSpeedNew;
	int16_t SpeedNew_100MS;
	int16_t LastSpeedNew_100MS;
	int16_t SpeedTemp;
	uint8_t count_100ms;
	float speed_temp ;
	float speed_forward;
	float dec_ratio;
	
	float SpeedNow;		//速度中间变量
	float S_low_acc;	//启动加速度
	float S_Add_acc;	//速度加速度
	float S_Run_acc;
	float SpeedFIL;
	
	float AngleCmd;		//角度控制
	float AngleSet;		//角度设定
	float AngleReal;	//真实角度
	float AngleNew;		//新角度设定

	float OpenLoopVoltCmd; 	//开换电压控制
	float OpenLoopVoltSet;	//开环电压设定
	int32_t turns;		//圈数，用于位置控制

	float CurrentCmd; 	//电流控制
	float CurrentSet;	//电流设定
	float Current_over;//过流值

	float VoltVin;				//输入电压
	float MaxCurrent;			//最大电流，限流
	float MaxSpeed;
	float pairs;
	float Mostemp;
	
	uint16_t count_buffers;
	uint16_t count_put ;
	uint8_t count_10ms;
	float variance;
	float mean_value;
	
	
	float CurrOffset[3];		//A  B  C 三相的偏置电流
	
	float f_IqSet;   		//Iq设定值
	float f_BusVoltRatio;   //母线电压比例  用于电压环输出补偿
	float Theta_acc;
	uint8_t Start_time;
	uint8_t stop_time;
	
	uint8_t speed_pid_count;
	uint8_t current_pid_count;
	uint16_t count_acc;
	
	bool first_offer;
	bool first_start;
	bool first_stop;
	bool wheel_start;
	
	uint16_t i16_OverCurrentCount;	//过流次数
	PIDFloat_Obj angle_pid;
	PIDFloat_Obj speed_pid;
	PIDFloat_Obj current_id_pid;
	PIDFloat_Obj current_iq_pid;
	
	foc_para_t foc;	
	uint8_t foc_count;
}sMotorCtrlTypeDef;

void PMSM_para_init(void);
void PMSM_para_clear(PMSM_Num num);
void pmsm_start(PMSM_Num num);
void pmsm_stop(PMSM_Num num);
void pmsm_break_io(PMSM_Num num,bool value);
void PMSM_PWM_RUN(PMSM_Num num,Phase3_value *PWM);
void PMSM_Start_Filter(PMSM_Num num);
extern sMotorCtrlTypeDef PMSM[2];
#endif

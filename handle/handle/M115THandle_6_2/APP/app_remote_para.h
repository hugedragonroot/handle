#ifndef __APP_REMOTE_PARA_H
#define __APP_REMOTE_PARA_H

#include <stdint.h>
#include "main.h"
/************************* 座椅角度 *************************/
/*
							-90
						*	|
					*		|
				*			|
			*				|
		*					|
	*						|
0	* * * * * *	0
	*						|
		*					|
			*				|
				*			|
					*		|
						*	|
							90
*/	

/************************* 底盘角度 *************************/
/*
							90
						*	|
					*		|
				*			|
			*				|
		*					|
	*						|
0	* * * * * *	0
	*						|
		*					|
			*				|
				*			|
					*		|
						*	|
							-90
*/	


/************************* 摇杆控制 *************************/

#define MOVE_SPEEDDEF_ZERO				0
#define BACK_SPEEDDEF_UP					300
#define BACK_SPEEDDEF_DOWN				-300
#define FORNT_SPEEDDEF_UP					BACK_SPEEDDEF_UP
#define FORNT_SPEEDDEF_DOWN				BACK_SPEEDDEF_DOWN
#define SEAT_SPEEDDEF_UP					BACK_SPEEDDEF_UP
#define SEAT_SPEEDDEF_DOWN				BACK_SPEEDDEF_DOWN

#define PUSH_ORD_SPEED_ADD 300 
#define PUSH_ORD_SPEED_SUB -300 
#define PUSH_ORD_SPEED_ZERO 0 

#define Walking_SpeedMAX		3600

#define Tracks_SpeedMAX			3000    			//高速档速度
#define Support_elongationMAX			3000    //伸长量最大值 //实测前支撑2900 后支撑2870
#define BACK_ULT_TOP_X_1M			1000						//超声波距离
#define BACK_ULT_TOP_X			700						//超声波检测楼梯顶部的最短距离
#define BACK_ULT_MIN_X			100						//超声波最短距离

#define DISTANCE_GAP	50	//超声波检测两次距离差
#define DISTANCE_GAP1	10	//超声波检测两次距离差
#define DISTANCE_GAP2	20	//超声波检测两次距离差
#define DISTANCE_GAP3	30	//超声波检测两次距离差
#define DISTANCE_GAP4	40	//超声波检测两次距离差
#define DISTANCE_GAP5	70	//超声波检测两次距离差

#define DISTANCE_LAND	55	//超声波检测两次距离差


#if DRIVE_TYPE_NEW
#define BACK_ULT_Y_DEF			100						//后超声波默认距离
#else
#define BACK_ULT_Y_DEF			50						//后超声波默认距离
#endif
#define STAIRS_Y_DEF				50						//楼梯高度距离
#define STAIRS_X_DEF				50						//楼梯宽度距离

#define BACK_ULT_MIN_Y			(BACK_ULT_Y_DEF + STAIRS_Y_DEF)	//超声波下楼1级台阶
#define BACK_ULT_MIN_2Y			(BACK_ULT_MIN_Y + STAIRS_Y_DEF)						//超声波下楼2级台阶

#define DISTANCE_HALL_1MM		(370.0/200)	
#define DISTANCE_HALL_150MM		(DISTANCE_HALL_1MM*100)	
#define DISTANCE_HALL_200MM		(DISTANCE_HALL_1MM*200)						//霍尔距离
#define DISTANCE_HALL_300MM		(DISTANCE_HALL_1MM*300)						//霍尔距离
#define DISTANCE_HALL_400MM		(DISTANCE_HALL_1MM*400)						//霍尔距离
#define DISTANCE_HALL_650MM		(DISTANCE_HALL_1MM*800)						//霍尔距离
#define DISTANCE_HALL_900MM		(DISTANCE_HALL_1MM*900)						//霍尔距离
#if DRIVE_TYPE_NEW
 #define ANGLE_DEFUALT				-1000//10度
#else
#define ANGLE_DEFUALT				1000//10度
#endif
#define BODY_SAFE_ANGLE			5000

#define ANGLE_MAX						6000

#define COORD_SQRT_MAX 100

#define BATTERY_MAX_VOL 28.5f
// #define BATTERY_MIN_VOL 19.6f
#define BATTERY_MIN_VOL 20.6f


/* 控制模式 */
typedef enum {
																			//CT200			//CT420	
  eWalking                = 0x00,     //平地行走
  eSupportBack						= 0x01,     //后支撑轮	//后支撑轮	
  eSupportFornt           = 0x02,    	//前支撑轮	//载物板
  eBackrest               = 0x03,     //靠背			
  eTracks                 = 0x04,     //履带			//履带
  eUpstairs               = 0x05,     //上楼
  eDownstairs							= 0x06,     //下楼

} EControlMode;

/* 运行方式 */
typedef enum {
  eManual                 = 0x00,     //手动模式
  eAutoLoading            = 0x01,     //载物板自动
  eAutoUP                 = 0x02,     //自动上楼
  eAutoDOWN               = 0x03,     //自动下楼
} EOperationMode;

/* 速度挡位 */
typedef enum {
  eGearOne                = 0x01,     //一档
  eGearTwo                = 0x02,     //二档
  eGearThree              = 0x03,     //三档
  eGearFore               = 0x04,     //四档
  eGearFive               = 0x05,     //五档
} ESpeedGear;

/*后轮控制方式 */
typedef enum {
	eSpeedMode									= 0x00,     //通过速度控制
  eElongationMode							= 0x01,     //通过伸长量控制
	eAngleMode	                = 0x02,     //角度闭环控制
} ESupportMode;

	
/* 外设状态 */
typedef enum {
	eHorn                   = 0x80,     //喇叭
  eHeadLamp               = 0x40,     //前大灯
  eLeftLamp               = 0x20,     //左转灯
  eRightLamp              = 0x10,     //右转灯
  eBackLamp               = 0x08,     //后尾灯
	eSupportup							=	0x02,			//支撑升
	eSupportdown						= 0x01,			//支撑降
} EPeripherals;

typedef enum{
	eUnLock,//解锁状态
	eLock,//锁定状态
}ELockState;

/* 外设状态 */
typedef enum {
  eOffline                = 0,     //离线
  eWire					= 1,     //有线连接
  eBlubtooth              = 2,     //蓝牙连接

} ERemoteConnect;

/* 开关机状态 */
typedef enum {
  ePowerOff								= 0,     //关机
  ePowerOn 								= 1,     //开机
	
} EPower;

/* 支撑轮状态 */
typedef enum {
  eSupportManl							= 0,    //手动
  eSupportAuto 							= 1,    //自动
	eSupportUp								=	2,		//上升	
	eSupportDown							=	3,		//下降
} ESupport;

#define HANDLE_ERROR_NUM 3
typedef union {
	struct {
		// struct{
			uint8_t joyXyError:1;
			uint8_t CANDisconnectError:1;
			uint8_t angleError:1;
		// }handle_bit;
		// struct {
			uint8_t leftWheelError:1;
			uint8_t rightWheelError:1;
			uint8_t pushRodError:1;
			uint8_t seatRodError:1;
			uint8_t breakError:1;
		// }control_bit;
	}bit;
	uint8_t all[1];
}APP_Error_t;



/* 遥控通信 */
typedef struct remote_setting
{
  uint8_t   RemoteBTCmd;    /* 遥控连接指令 */	
  uint8_t   RemoteBTConnect;    /* 遥控连接标志 */
  uint8_t   RemoteBTCount;			//遥控连接计时
	
  uint8_t   RemoteConnect;    /* 遥控连接标志 */
  uint8_t   RemoteCount;			//遥控连接计时
  uint8_t   PowerStatus;			/* 遥控开关标志 */
	
//   uint8_t   CoordXH;          /* 摇杆 X 坐标高8位 */
//   uint8_t   CoordXL;          /* 摇杆 X 坐标低8位 */
  int16_t   CoordX;           /* 摇杆 X 坐标 */
  
//   uint8_t   CoordYH;          /* 摇杆 Y 坐标高8位 */
//   uint8_t   CoordYL;          /* 摇杆 Y 坐标低8位 */
  int16_t   CoordY;           /* 摇杆 Y 坐标 */

	
#if USING_XY_TO_SPEED
  int16_t   CoordSqrt;        /* 坐标平方根 */	
	float 		CoordAngle;				/* 摇杆角度（-180< x <= 180）*/
	// int16_t 		CoordIntAngle;				/* 摇杆角度（-180< x <= 180）*/
  int16_t   CoordSpeed;       /* 目标最大速度 */
#endif

  uint8_t   ControlMode;      /* 控制模式 */
  uint8_t   ControlPre;
	uint8_t   Control_change;		/*控制模式切换*/
  uint8_t   M115CtrlMode;      /* 控制模式 */
  uint8_t   M115CtrlModePre;

  uint8_t   OperationMode;    /* 运行方式 *///自动手动
  uint8_t   OperationPre;
  uint8_t   SpeedGear;        /* 速度挡位 */
  uint8_t   SpeedPre;
  uint8_t   Peripherals;      /* 外设状态 */
  uint8_t   PeripheryPre;
	uint8_t   HandleLock:1;				/* 锁定状态 */
//   uint8_t   Reserved;         /* 保留位 */ 
	uint8_t folding_time; //折叠时间

	uint8_t   Battery;          /* 电池电量 */

  uint8_t   Model;				//设备类型
	uint32_t  totalkm;			//总里程
	uint32_t  speed_kmh;			//速度	
//  int16_t   aPitch;           /* 俯仰角 */
//  int16_t   aRoll;            /* 滚转角 */
//  int16_t   aYaw;             /* 偏航角 */	
	uint8_t   LampSwitch;				//灯光开关		
	uint8_t		VoiceLevel;				//音量
	uint8_t  	VoiceSwitch;			//提示开关
	uint8_t		AlarmSwitch;			//警报提示
	uint8_t		CruiseCtrlSwitch;	//定速巡航
	// uint8_t		CurrentMusic;	//按下喇叭键要播放的音乐
	int8_t angleAlarmPrecent;//角度报警百分比 随座椅角度变化
	uint8_t	ev1527_user_id[EV1527_USER_NUM][2] ;//无线遥控器ID
	uint8_t CANQueueLenMax:4;
	uint8_t CANQueueLenCur:4;
	uint16_t CANBufferQueueLenMax;
	uint16_t CANBufferQueueLenCur;

} TREMOTE_SETTING_PARA;
extern TREMOTE_SETTING_PARA   Remote_setting_para;


typedef struct {
	int16_t 	pcbAngle;//板载角度 下降大
	uint16_t 	ultrasonic[3];//超声波距离
	int16_t		pitch_angle; // 俯仰角,上坡大			//水平值 -18000< x < 18000  默认0 
	int16_t		roll_angle;	//横滚角,左倾大 	//水平值 -18000< x < 18000  默认0
}ATTITUDE_PARA;


typedef struct remote_trans
{
	
	uint8_t		Drivermodel;			//驱动器型号	
	uint8_t		Driversoftware;		//主软件版本号
	uint8_t		Driversoftwarepre;//副软件版本号		

	int16_t	  walking_speed_L;					//左行走电机速度(当前)
	int16_t	  walking_speed_R;					//右行走电机速度(当前)
	int16_t  push_rod_speed; // M115升降推杆速度
	int16_t  seat_rod_speed; // M115座椅推杆速度

	uint8_t folding_state:1;//折叠状态
	// uint16_t push_rod_state;//推杆状态

#if 0
	int16_t		pitch_angle; // 俯仰角,上坡大			//水平值 -18000< x < 18000  默认0 
	int16_t		roll_angle;	//横滚角,左倾大 	//水平值 -18000< x < 18000  默认0

	uint16_t ultrasonic[4];
#else
	ATTITUDE_PARA m115_attitude;
#endif

	APP_Error_t errorFlag;

#if 0

	uint16_t	fornt_ultrasonic_y;				//前置纵向超声		(mm)
	uint16_t	back_ultrasonic_x;				//后置横向超声
	uint16_t	back_ultrasonic_y;				//后置纵向超声
	uint8_t		sumcheck_err;							//累加和校验错误		
#endif

//   uint8_t   Battery;          /* 电池电量 */
	float BatteryVol;//电池电压

}TREMOTE_TRANS_PARA;

extern TREMOTE_TRANS_PARA			Remote_trans_para;
extern TREMOTE_TRANS_PARA			Remote_receive_para;


/**************************手机APP通讯协议************************/

typedef enum {
  ePExpand									= 0,     //展开
  ePfold 										= 1,     //折叠
} EP_M115mode;


	
/* 状态 */
typedef enum {
	ePLamp										= 0x01,			//开灯
	ePPower										= 0x02,    //关机
	ePGyroscopeCheck					=	0x04,			//陀螺仪校准
	ePVoice										= 0x08,		//语音提示开
	ePAlarm										=	0x10,		//语音警报
	ePCruiseCtrl							= 0x20,		//定速巡航
} EP_Peripherals;


	
/* 调节 */
typedef enum {
	ePAngleNone                 = 0x00,     //无动作
	ePAngleUp										= 0x01,			//升	
	ePAngleDown									= 0x02,			//降	
} EP_Angle;

/* 调节 */
typedef enum {
	ePSeatNone                  = 0x00,     //无动作
	ePSeatFornt									= 0x01,			//座椅前倾
	ePSeatBack									= 0x02,			//座椅后仰
	ePSeatUp										= 0x03,			//座椅升
	ePSeatDown									= 0x04,			//座椅降
} EP_Seat;


/* 速度挡位 */
typedef enum {
  ePGearOne                = 0x01,     //一档
  ePGearTwo                = 0x02,     //二档
  ePGearThree              = 0x03,     //三档
  ePGearFore               = 0x04,     //四档
  ePGearFive               = 0x05,     //五档
} EP_SpeedGear;


	
/* 音量 */
typedef enum {
	ePVoiceLevel_1            = 0x01,     //低
	ePVoiceLevel_2						= 0x02,			//中
	ePVoiceLevel_3						= 0x03,			//高
} EP_VoiceLevel;
 
/*错误代码*/
typedef enum{
	ERROR_NONE = 0,//没有错误

	ERROR_JOY_XY,//开机摇杆不在原点
	ERROR_CAN_DISCONNECT ,//与控制器通信故障

	ERROR_LEFT_WHEEL,//左轮错误
	ERROR_RIGHT_WHEEL,//右轮错误
	ERROR_PUSH_ROB,//升降推杆错误
	ERROR_SEAT_ROB,//座椅推杆错误
	
}APP_ERROR_FLAG;



typedef struct Phone_trans
{
	
	uint8_t		Drivermodel;			//手柄型号	
	uint8_t		Driversoftware;		//主软件版本号
	uint8_t		Driversoftwarepre;//副软件版本号		

	
	uint8_t		M130CtrlMode;			//轮毂 模式选择
	uint8_t		M115CtrlMode;		//M115模式选择 展开 折叠
	uint8_t   LampSwitch;				//灯光开关	
	uint8_t   SeatCtrl;					//座椅调节
	uint8_t   ForntwheelCtrl;		//前轮调节
	uint8_t   BackwheelCtrl;		//后轮调节
	uint8_t   CoordX;
	uint8_t   CoordY;
	uint8_t		SpeedLevel;					//速度档位
	uint8_t   Power;						//开关机指令
	uint8_t   Gyroscopechk;			//陀螺仪校准
	uint8_t		VoiceLevel;				//音量
	uint8_t  	VoiceSwitch;			//提示开关
	uint8_t		AlarmSwitch;			//警报提示
	uint8_t		CruiseCtrlSwitch;	//定速巡航


}TPHONE_TRANS_PARA;

extern TPHONE_TRANS_PARA			Phone_trans_para;
extern TPHONE_TRANS_PARA			Phone_receive_para;


void Remote_para_init(void);
void Remote_para_default(void);
#endif


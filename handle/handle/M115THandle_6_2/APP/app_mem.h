#ifndef __APP_MEM_H
#define __APP_MEM_H
#include "main.h"


#define  WALKINGMODE	0
#define  TRACKMODE 		1
								

#if 0
typedef struct
{
	#if 0
	uint16_t	model:8;				//机器型号
	uint16_t 	ControlMode;			//控制模式
	int16_t		seatangle;		//座椅角度闭环设定值
	int16_t		bodyangle;		//机器底盘角度闭环设定值
	uint16_t	VoiceLevel:8;				//音量
	uint16_t	VoiceSwitch:8;			//提示开关
	uint16_t	AlarmSwitch:8;			//警报提示
	uint16_t	CruiseCtrlSwitch:8;	//定速巡航
	#endif
#if 1

	uint8_t 	folderState:1;//折叠状态
	uint8_t 	speedGear:3;//速度挡位
	uint8_t 	voiceLevel:2;//音量

#else
	uint8_t 	folderState;//折叠状态
	uint8_t 	speedGear;//速度挡位
	uint16_t 	reserve;
#endif
}MEM_STR;
#else
typedef union {
  
	struct {
		union {
			uint8_t id[4];//版本号
			uint32_t sum;
		}version;

		uint8_t voiceLevel:2;//音量
		uint8_t AlarmSwitch:1;//报警提示
		uint8_t HandleLock:1;//锁定状态

		uint8_t folderState:1;//折叠状态
		uint8_t	ev1527_user_id[EV1527_USER_NUM][2];
	}data;
	// uint32_t temp[2];
	uint32_t temp[1];
}MEM_STR;

#endif


extern MEM_STR Mem_Save;

void mem_init(void);
void mem_read(void);
void mem_save(void);

#endif


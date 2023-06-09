#ifndef __APP_MEM_H
#define __APP_MEM_H
#include "main.h"


#define  WALKINGMODE	0
#define  TRACKMODE 		1
								


typedef struct
{
	uint16_t	model:8;				//机器型号
	uint16_t 	ControlMode;			//控制模式
	int16_t		seatangle;		//座椅角度闭环设定值
	int16_t		bodyangle;		//机器底盘角度闭环设定值
	uint16_t	VoiceLevel:8;				//音量
	uint16_t	VoiceSwitch:8;			//提示开关
	uint16_t	AlarmSwitch:8;			//警报提示
	uint16_t	CruiseCtrlSwitch:8;	//定速巡航
}MEM_STR;

extern MEM_STR Mem_set;

void mem_init(void);
void mem_read(void);
void mem_save(void);

#endif


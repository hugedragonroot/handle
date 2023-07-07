#ifndef __APP_DISPLAY_H
#define __APP_DISPLAY_H
#include "main.h"

#include <stdint.h>

#define SPEED_DELAY		30// *50 ms
#define SURE_TIMES		10//3

#define YES		1
#define NO		2


typedef struct  
{
	uint8_t Driver_pause;		//暂停
	uint8_t Up_stairs_state;
	uint8_t Down_stairs_state;		

} Control_STR;

typedef enum 
{
	eUP_STAIRS_START,							//确认上楼
	eUP_STAIRS_READY,							//准备上楼
	eUP_STAIRS_BACK,							//正在上楼
	eUP_STAIRS_TOP_STAIRS,	
	eUP_STAIRS_TOP_UP,	
	eUP_STAIRS_TOP_ACK,						//确定是否到梯顶边缘
	eUP_STAIRS_TOP_ADJ,						//梯顶边缘调整
	eUP_STAIRS_BACK_SUPPORT_DOWN,	//下后支撑轮
	eUP_STAIRS_TOP_BACK,					//梯顶后退上楼
	eUP_STAIRS_TOP_LAND_ACK,			//确定是否到梯顶平台
	eUP_STAIRS_TOP_LAND_ADJ,			//到梯顶平台调整	
	eUP_STAIRS_TOP_LAND,					//到达梯顶平台			//10
	eUP_STAIRS_GO_BACK,						//超过安全角度退回平面
	eUP_STAIRS_EXTI,							//退出	
	eUP_STAIRS_FINISH,						//完成上楼退出
	eUP_STAIRS_OUT,						//退出	
	
	eDOWN_STAIRS_START,							//确认下楼
	eDOWN_STAIRS_READY,							//准备下楼
	eDOWN_STAIRS_FORNT,							//梯顶平台前进下楼
	eDOWN_STAIRS_FORNT_EXTI,				//梯顶平台前进下楼退出
	eDOWN_STAIRS_FORNT_ACK,					//确认到达楼梯边缘
	eDOWN_STAIRS_FORNT_GO,					//楼梯边缘再走
	
	eDOWN_STAIRS_FORNT_SUPPORT_DOWN,//前支撑下		//20
	eDOWN_STAIRS_TOP_FORNT,					//梯顶前进下楼直到平衡点
	eDOWN_STAIRS_TOP_FORNT_EXTI,		//梯顶前进下楼退出
	eDOWN_STAIRS_FORNT_SUPPORT_UP_ACK,	//收前支撑轮确认
	eDOWN_STAIRS_FORNT_SUPPORT_UP,	//收前支撑轮


	eDOWN_STAIRS_BACK_SUPPORT_DOWN,	//后支撑下
	eDOWN_STAIRS_GO_DOWN_ACK,
	eDOWN_STAIRS_GO_DOWN,
	eDOWN_STAIRS_BACK_SUPPORT_UP,	//后支撑收


	eDOWN_STAIRS_FORNT_DOWN,				//梯面前进下楼
	eDOWN_STAIRS_FORNT_DOWN_EXTI,		//梯面前进下楼退出	//30
	eDOWN_STAIRS_GO_BACK_FORNT_SUPPORT_DOWN,//返回只起到水平角
	eDOWN_STAIRS_TRACKS_GO_BACK,		//履带后退返回
	eDOWN_STAIRS_TRACKS_GO_BACK_ACK,//确认履带后退返回
	eDOWN_STAIRS_GO_BACK_FORNT_SUPPORT_UP,	//收起前轮
	eDOWN_STAIRS_TRACKS_GO_BACK_TOP,//履带后退返回平面
	eDOWN_STAIRS_FINISH,						//完成下楼退出	
	eDOWN_STAIRS_OUT,						//退出	
}Auto_Stairs_e;


typedef enum 
{
	ePAGE_MAIN,
	ePAGE_UPSTAIR,
	ePAGE_DOWNSTAIR,
	ePAGE_SETTING,
	ePAGE_PWROFF,
}ui_e ;


void display_init(void);
void setdisplay_ui(ui_e ui);
uint8_t getdisplay_ui(void);
void app_display(void);
#endif


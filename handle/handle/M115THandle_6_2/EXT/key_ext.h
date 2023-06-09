#ifndef __KEY_EXT_H
#define __KEY_EXT_H
#include <stdint.h>
#include "main.h"


#if USING_6KEY

#define HARD_KEY_NUM	   6									/* 实体按键个数 */
#else
#define HARD_KEY_NUM	   9									/* 实体按键个数 */
#endif

#define KEY_GROUP 1 //组合按键
#define KEY_COUNT   	 	(HARD_KEY_NUM + KEY_GROUP)	/* 个独立建 + 0个组合按键 */


/* 按键ID, 主要用于bsp_KeyState()函数的入口参数 */
typedef enum
{
#if	USING_6KEY
	KID_SPEED_ADD,		
	KID_SPEED_SUB,		
	KID_POWER,
	KID_PUSH_ORD_ADD,	
	KID_PUSH_ORD_SUB,	
	KID_HORN,			

#else
	KID_SPEED_ADD = 8,		KID_SPEED_SUB = 5,		KID_MENU_ENTER = 2,
	KID_PUSH_ORD_ADD = 7,	KID_PUSH_ORD_SUB = 4,	KID_MENU_ESC = 1,
	KID_BEEP = 6, 			KID_LOCK = 3,			KID_POWER = 0,
	
#endif	
}KEY_ID_E;

/*
	按键滤波时间40ms, 单位20ms。
	只有连续检测到40ms状态不变才认为有效，包括弹起和按下两种事件
	即使按键电路不做硬件滤波，该滤波机制也可以保证可靠地检测到按键事件
*/
#define KEY_FILTER_TIME   5
#define KEY_LONG_TIME     200			/* 单位10ms， 持续2秒，认为长按事件 */

/*
	每个按键对应1个全局的结构体变量。
*/
typedef struct
{
	/* 下面是一个函数指针，指向判断按键手否按下的函数 */
//	uint8_t (*IsKeyDownFunc)(void); /* 按键按下的判断函数,1表示按下 */

	uint8_t  Count;			/* 滤波器计数器 */
	uint16_t LongCount;		/* 长按计数器 */
	uint16_t LongTime;		/* 按键按下持续时间, 0表示不检测长按 */
	uint8_t  State;			/* 按键当前状态（按下还是弹起） */
	uint8_t  RepeatSpeed;	/* 连续按键周期 */
	uint8_t  RepeatCount;	/* 连续按键计数器 */
}KEY_T;

/*
	定义键值代码, 必须按如下次序定时每个键的按下、弹起和长按事件

	推荐使用enum, 不用#define，原因：
	(1) 便于新增键值,方便调整顺序，使代码看起来舒服点
	(2) 编译器可帮我们避免键值重复。
*/
typedef enum
{
	#if USING_6KEY
	KEY_NONE = 0,			/* 0 表示按键事件 */


	KEY_POWER_DOWN = KID_POWER*3+1,				/* 键按下 */
	KEY_POWER_UP = KID_POWER*3+2,					/* 键弹起 */
	KEY_POWER_LONG = KID_POWER*3+3,				/* 键长按 */

	KEY_HORN_DOWN = KID_HORN*3+1,				/* 键按下 */
	KEY_HORN_UP = KID_HORN*3+2,					/* 键弹起 */
	KEY_HORN_LONG = KID_HORN*3+3,				/* 键长按 */

	KEY_SPEED_ADD_DOWN = KID_SPEED_ADD*3+1,		/* 键按下 */
	KEY_SPEED_ADD_UP = KID_SPEED_ADD*3+2,			/* 键弹起 */
	KEY_SPEED_ADD_LONG = KID_SPEED_ADD*3+3,		/* 键长按 */


	KEY_SPEED_SUB_DOWN = KID_SPEED_SUB*3+1,		/* 键按下 */
	KEY_SPEED_SUB_UP = KID_SPEED_SUB*3+2,			/* 键弹起 */
	KEY_SPEED_SUB_LONG = KID_SPEED_SUB*3+3,		/* 键长按 */	

	KEY_PUSH_ORD_ADD_DOWN = KID_PUSH_ORD_ADD*3+1,
	KEY_PUSH_ORD_ADD_UP = KID_PUSH_ORD_ADD*3+2,
	KEY_PUSH_ORD_ADD_LONG = KID_PUSH_ORD_ADD*3+3,

	KEY_PUSH_ORD_SUB_DOWN = KID_PUSH_ORD_SUB*3+1,
	KEY_PUSH_ORD_SUB_UP = KID_PUSH_ORD_SUB*3+2,
	KEY_PUSH_ORD_SUB_LONG = KID_PUSH_ORD_SUB*3+3,

	//组合按键
	KEY_PUSH_ORD_ADD_SUB_DOWN = (HARD_KEY_NUM+0)*3+1,
	KEY_PUSH_ORD_ADD_SUB_UP = (HARD_KEY_NUM+0)*3+2,
	KEY_PUSH_ORD_ADD_SUB_LONG = (HARD_KEY_NUM+0)*3+3,

	#else
	KEY_NONE = 0,			/* 0 表示按键事件 */

	KEY_POWER_DOWN = KID_POWER*3+1,				/* 键按下 */
	KEY_POWER_UP = KID_POWER*3+2,					/* 键弹起 */
	KEY_POWER_LONG = KID_POWER*3+3,				/* 键长按 */

	KEY_LOCK_DOWN = KID_LOCK*3+1,				/* 键按下 */
	KEY_LOCK_UP = KID_LOCK*3+2,					/* 键弹起 */
	KEY_LOCK_LONG = KID_LOCK*3+3,				/* 键长按 */	

	KEY_HORN_DOWN = KID_BEEP*3+1,				/* 键按下 */
	KEY_HORN_UP = KID_BEEP*3+2,					/* 键弹起 */
	KEY_HORN_LONG = KID_BEEP*3+3,				/* 键长按 */
	
	KEY_SPEED_SUB_DOWN = KID_SPEED_SUB*3+1,		/* 键按下 */
	KEY_SPEED_SUB_UP = KID_SPEED_SUB*3+2,			/* 键弹起 */
	KEY_SPEED_SUB_LONG = KID_SPEED_SUB*3+3,		/* 键长按 */	

	KEY_SPEED_ADD_DOWN = KID_SPEED_ADD*3+1,		/* 键按下 */
	KEY_SPEED_ADD_UP = KID_SPEED_ADD*3+2,			/* 键弹起 */
	KEY_SPEED_ADD_LONG = KID_SPEED_ADD*3+3,		/* 键长按 */

	KEY_PUSH_ORD_ADD_DOWN = KID_PUSH_ORD_ADD*3+1,
	KEY_PUSH_ORD_ADD_UP = KID_PUSH_ORD_ADD*3+2,
	KEY_PUSH_ORD_ADD_LONG = KID_PUSH_ORD_ADD*3+3,

	KEY_PUSH_ORD_SUB_DOWN = KID_PUSH_ORD_SUB*3+1,
	KEY_PUSH_ORD_SUB_UP = KID_PUSH_ORD_SUB*3+2,
	KEY_PUSH_ORD_SUB_LONG = KID_PUSH_ORD_SUB*3+3,


	KEY_MENU_ENTER_DOWN = KID_MENU_ENTER*3+1,
	KEY_MENU_ENTER_UP = KID_MENU_ENTER*3+2,
	KEY_MENU_ENTER_LONG = KID_MENU_ENTER*3+3,

	KEY_MENU_ESC_DOWN = KID_MENU_ESC*3+1,
	KEY_MENU_ESC_UP = KID_MENU_ESC*3+2,
	KEY_MENU_ESC_LONG = KID_MENU_ESC*3+3,


	//组合按键
	KEY_PUSH_ORD_ADD_SUB_DOWN = (HARD_KEY_NUM+0)*3+1,
	KEY_PUSH_ORD_ADD_SUB_UP = (HARD_KEY_NUM+0)*3+2,
	KEY_PUSH_ORD_ADD_SUB_LONG = (HARD_KEY_NUM+0)*3+3,

	
#endif	
		
}KEY_ENUM;

/* 按键FIFO用到变量 */
#define KEY_FIFO_SIZE	10
typedef struct
{
	uint8_t Buf[KEY_FIFO_SIZE];		/* 键值缓冲区 */
	uint8_t Read;					/* 缓冲区读指针1 */
	uint8_t Write;					/* 缓冲区写指针 */
	uint8_t Read2;					/* 缓冲区读指针2 */
}KEY_FIFO_T;



/* 供外部调用的函数声明 */
void key_init(void);
void bsp_KeyScan10ms(void);
void bsp_PutKey(uint8_t _KeyCode);
  uint8_t bsp_GetKey(void);
uint8_t bsp_GetKey2(void);
uint8_t bsp_GetKeyState(KEY_ID_E _ucKeyID);
void bsp_SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t  _RepeatSpeed);
void bsp_ClearKey(void);


void TPAD_Init(void);

#endif

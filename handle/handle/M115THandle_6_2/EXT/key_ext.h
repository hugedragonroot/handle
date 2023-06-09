#ifndef __KEY_EXT_H
#define __KEY_EXT_H
#include <stdint.h>
#include "main.h"


#if USING_6KEY

#define HARD_KEY_NUM	   6									/* ʵ�尴������ */
#else
#define HARD_KEY_NUM	   9									/* ʵ�尴������ */
#endif

#define KEY_GROUP 1 //��ϰ���
#define KEY_COUNT   	 	(HARD_KEY_NUM + KEY_GROUP)	/* �������� + 0����ϰ��� */


/* ����ID, ��Ҫ����bsp_KeyState()��������ڲ��� */
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
	�����˲�ʱ��40ms, ��λ20ms��
	ֻ��������⵽40ms״̬�������Ϊ��Ч����������Ͱ��������¼�
	��ʹ������·����Ӳ���˲������˲�����Ҳ���Ա�֤�ɿ��ؼ�⵽�����¼�
*/
#define KEY_FILTER_TIME   5
#define KEY_LONG_TIME     200			/* ��λ10ms�� ����2�룬��Ϊ�����¼� */

/*
	ÿ��������Ӧ1��ȫ�ֵĽṹ�������
*/
typedef struct
{
	/* ������һ������ָ�룬ָ���жϰ����ַ��µĺ��� */
//	uint8_t (*IsKeyDownFunc)(void); /* �������µ��жϺ���,1��ʾ���� */

	uint8_t  Count;			/* �˲��������� */
	uint16_t LongCount;		/* ���������� */
	uint16_t LongTime;		/* �������³���ʱ��, 0��ʾ����ⳤ�� */
	uint8_t  State;			/* ������ǰ״̬�����»��ǵ��� */
	uint8_t  RepeatSpeed;	/* ������������ */
	uint8_t  RepeatCount;	/* �������������� */
}KEY_T;

/*
	�����ֵ����, ���밴���´���ʱÿ�����İ��¡�����ͳ����¼�

	�Ƽ�ʹ��enum, ����#define��ԭ��
	(1) ����������ֵ,�������˳��ʹ���뿴���������
	(2) �������ɰ����Ǳ����ֵ�ظ���
*/
typedef enum
{
	#if USING_6KEY
	KEY_NONE = 0,			/* 0 ��ʾ�����¼� */


	KEY_POWER_DOWN = KID_POWER*3+1,				/* ������ */
	KEY_POWER_UP = KID_POWER*3+2,					/* ������ */
	KEY_POWER_LONG = KID_POWER*3+3,				/* ������ */

	KEY_HORN_DOWN = KID_HORN*3+1,				/* ������ */
	KEY_HORN_UP = KID_HORN*3+2,					/* ������ */
	KEY_HORN_LONG = KID_HORN*3+3,				/* ������ */

	KEY_SPEED_ADD_DOWN = KID_SPEED_ADD*3+1,		/* ������ */
	KEY_SPEED_ADD_UP = KID_SPEED_ADD*3+2,			/* ������ */
	KEY_SPEED_ADD_LONG = KID_SPEED_ADD*3+3,		/* ������ */


	KEY_SPEED_SUB_DOWN = KID_SPEED_SUB*3+1,		/* ������ */
	KEY_SPEED_SUB_UP = KID_SPEED_SUB*3+2,			/* ������ */
	KEY_SPEED_SUB_LONG = KID_SPEED_SUB*3+3,		/* ������ */	

	KEY_PUSH_ORD_ADD_DOWN = KID_PUSH_ORD_ADD*3+1,
	KEY_PUSH_ORD_ADD_UP = KID_PUSH_ORD_ADD*3+2,
	KEY_PUSH_ORD_ADD_LONG = KID_PUSH_ORD_ADD*3+3,

	KEY_PUSH_ORD_SUB_DOWN = KID_PUSH_ORD_SUB*3+1,
	KEY_PUSH_ORD_SUB_UP = KID_PUSH_ORD_SUB*3+2,
	KEY_PUSH_ORD_SUB_LONG = KID_PUSH_ORD_SUB*3+3,

	//��ϰ���
	KEY_PUSH_ORD_ADD_SUB_DOWN = (HARD_KEY_NUM+0)*3+1,
	KEY_PUSH_ORD_ADD_SUB_UP = (HARD_KEY_NUM+0)*3+2,
	KEY_PUSH_ORD_ADD_SUB_LONG = (HARD_KEY_NUM+0)*3+3,

	#else
	KEY_NONE = 0,			/* 0 ��ʾ�����¼� */

	KEY_POWER_DOWN = KID_POWER*3+1,				/* ������ */
	KEY_POWER_UP = KID_POWER*3+2,					/* ������ */
	KEY_POWER_LONG = KID_POWER*3+3,				/* ������ */

	KEY_LOCK_DOWN = KID_LOCK*3+1,				/* ������ */
	KEY_LOCK_UP = KID_LOCK*3+2,					/* ������ */
	KEY_LOCK_LONG = KID_LOCK*3+3,				/* ������ */	

	KEY_HORN_DOWN = KID_BEEP*3+1,				/* ������ */
	KEY_HORN_UP = KID_BEEP*3+2,					/* ������ */
	KEY_HORN_LONG = KID_BEEP*3+3,				/* ������ */
	
	KEY_SPEED_SUB_DOWN = KID_SPEED_SUB*3+1,		/* ������ */
	KEY_SPEED_SUB_UP = KID_SPEED_SUB*3+2,			/* ������ */
	KEY_SPEED_SUB_LONG = KID_SPEED_SUB*3+3,		/* ������ */	

	KEY_SPEED_ADD_DOWN = KID_SPEED_ADD*3+1,		/* ������ */
	KEY_SPEED_ADD_UP = KID_SPEED_ADD*3+2,			/* ������ */
	KEY_SPEED_ADD_LONG = KID_SPEED_ADD*3+3,		/* ������ */

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


	//��ϰ���
	KEY_PUSH_ORD_ADD_SUB_DOWN = (HARD_KEY_NUM+0)*3+1,
	KEY_PUSH_ORD_ADD_SUB_UP = (HARD_KEY_NUM+0)*3+2,
	KEY_PUSH_ORD_ADD_SUB_LONG = (HARD_KEY_NUM+0)*3+3,

	
#endif	
		
}KEY_ENUM;

/* ����FIFO�õ����� */
#define KEY_FIFO_SIZE	10
typedef struct
{
	uint8_t Buf[KEY_FIFO_SIZE];		/* ��ֵ������ */
	uint8_t Read;					/* ��������ָ��1 */
	uint8_t Write;					/* ������дָ�� */
	uint8_t Read2;					/* ��������ָ��2 */
}KEY_FIFO_T;



/* ���ⲿ���õĺ������� */
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

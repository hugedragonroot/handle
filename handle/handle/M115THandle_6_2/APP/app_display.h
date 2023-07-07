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
	uint8_t Driver_pause;		//��ͣ
	uint8_t Up_stairs_state;
	uint8_t Down_stairs_state;		

} Control_STR;

typedef enum 
{
	eUP_STAIRS_START,							//ȷ����¥
	eUP_STAIRS_READY,							//׼����¥
	eUP_STAIRS_BACK,							//������¥
	eUP_STAIRS_TOP_STAIRS,	
	eUP_STAIRS_TOP_UP,	
	eUP_STAIRS_TOP_ACK,						//ȷ���Ƿ��ݶ���Ե
	eUP_STAIRS_TOP_ADJ,						//�ݶ���Ե����
	eUP_STAIRS_BACK_SUPPORT_DOWN,	//�º�֧����
	eUP_STAIRS_TOP_BACK,					//�ݶ�������¥
	eUP_STAIRS_TOP_LAND_ACK,			//ȷ���Ƿ��ݶ�ƽ̨
	eUP_STAIRS_TOP_LAND_ADJ,			//���ݶ�ƽ̨����	
	eUP_STAIRS_TOP_LAND,					//�����ݶ�ƽ̨			//10
	eUP_STAIRS_GO_BACK,						//������ȫ�Ƕ��˻�ƽ��
	eUP_STAIRS_EXTI,							//�˳�	
	eUP_STAIRS_FINISH,						//�����¥�˳�
	eUP_STAIRS_OUT,						//�˳�	
	
	eDOWN_STAIRS_START,							//ȷ����¥
	eDOWN_STAIRS_READY,							//׼����¥
	eDOWN_STAIRS_FORNT,							//�ݶ�ƽ̨ǰ����¥
	eDOWN_STAIRS_FORNT_EXTI,				//�ݶ�ƽ̨ǰ����¥�˳�
	eDOWN_STAIRS_FORNT_ACK,					//ȷ�ϵ���¥�ݱ�Ե
	eDOWN_STAIRS_FORNT_GO,					//¥�ݱ�Ե����
	
	eDOWN_STAIRS_FORNT_SUPPORT_DOWN,//ǰ֧����		//20
	eDOWN_STAIRS_TOP_FORNT,					//�ݶ�ǰ����¥ֱ��ƽ���
	eDOWN_STAIRS_TOP_FORNT_EXTI,		//�ݶ�ǰ����¥�˳�
	eDOWN_STAIRS_FORNT_SUPPORT_UP_ACK,	//��ǰ֧����ȷ��
	eDOWN_STAIRS_FORNT_SUPPORT_UP,	//��ǰ֧����


	eDOWN_STAIRS_BACK_SUPPORT_DOWN,	//��֧����
	eDOWN_STAIRS_GO_DOWN_ACK,
	eDOWN_STAIRS_GO_DOWN,
	eDOWN_STAIRS_BACK_SUPPORT_UP,	//��֧����


	eDOWN_STAIRS_FORNT_DOWN,				//����ǰ����¥
	eDOWN_STAIRS_FORNT_DOWN_EXTI,		//����ǰ����¥�˳�	//30
	eDOWN_STAIRS_GO_BACK_FORNT_SUPPORT_DOWN,//����ֻ��ˮƽ��
	eDOWN_STAIRS_TRACKS_GO_BACK,		//�Ĵ����˷���
	eDOWN_STAIRS_TRACKS_GO_BACK_ACK,//ȷ���Ĵ����˷���
	eDOWN_STAIRS_GO_BACK_FORNT_SUPPORT_UP,	//����ǰ��
	eDOWN_STAIRS_TRACKS_GO_BACK_TOP,//�Ĵ����˷���ƽ��
	eDOWN_STAIRS_FINISH,						//�����¥�˳�	
	eDOWN_STAIRS_OUT,						//�˳�	
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


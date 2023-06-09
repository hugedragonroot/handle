#ifndef __APP_MEM_H
#define __APP_MEM_H
#include "main.h"


#define  WALKINGMODE	0
#define  TRACKMODE 		1
								


typedef struct
{
	uint16_t	model:8;				//�����ͺ�
	uint16_t 	ControlMode;			//����ģʽ
	int16_t		seatangle;		//���νǶȱջ��趨ֵ
	int16_t		bodyangle;		//�������̽Ƕȱջ��趨ֵ
	uint16_t	VoiceLevel:8;				//����
	uint16_t	VoiceSwitch:8;			//��ʾ����
	uint16_t	AlarmSwitch:8;			//������ʾ
	uint16_t	CruiseCtrlSwitch:8;	//����Ѳ��
}MEM_STR;

extern MEM_STR Mem_set;

void mem_init(void);
void mem_read(void);
void mem_save(void);

#endif


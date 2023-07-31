#ifndef __APP_MEM_H
#define __APP_MEM_H
#include "main.h"


#define  WALKINGMODE	0
#define  TRACKMODE 		1
								

#if 0
typedef struct
{
	#if 0
	uint16_t	model:8;				//�����ͺ�
	uint16_t 	ControlMode;			//����ģʽ
	int16_t		seatangle;		//���νǶȱջ��趨ֵ
	int16_t		bodyangle;		//�������̽Ƕȱջ��趨ֵ
	uint16_t	VoiceLevel:8;				//����
	uint16_t	VoiceSwitch:8;			//��ʾ����
	uint16_t	AlarmSwitch:8;			//������ʾ
	uint16_t	CruiseCtrlSwitch:8;	//����Ѳ��
	#endif
#if 1

	uint8_t 	folderState:1;//�۵�״̬
	uint8_t 	speedGear:3;//�ٶȵ�λ
	uint8_t 	voiceLevel:2;//����

#else
	uint8_t 	folderState;//�۵�״̬
	uint8_t 	speedGear;//�ٶȵ�λ
	uint16_t 	reserve;
#endif
}MEM_STR;
#else
typedef union {
  
	struct {
		union {
			uint8_t id[4];//�汾��
			uint32_t sum;
		}version;

		uint8_t voiceLevel:2;//����
		uint8_t AlarmSwitch:1;//������ʾ
		uint8_t HandleLock:1;//����״̬

		uint8_t folderState:1;//�۵�״̬
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


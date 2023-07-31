#ifndef _MUSIC_EXT_H
#define _MUSIC_EXT_H

#include "stdint.h"
#include "app_loopqueue.h" 

#define PLAY_HEAD		0x7e	//֡ͷ
#define PLAY_ADDR		0xff	//��ַ
#define PLAY_LEN		0x06	//����
#define PLAY_TAIL		0xef	//֡β
                
#define PLAY_FB_Y		1	//��Ҫ����
#define PLAY_FB_N		0	//����Ҫ����

//��������

#define	PLAY_CMD_VOLUME_ADD	0x04	//����+
#define	PLAY_CMD_VOLUME_SUB	0x05	//����-
#define	PLAY_CMD_VOLUME		0x06	//ָ������
#define	PLAY_CMD_DEVICE		0x09	//ָ�������豸
#define	PLAY_CMD_BAUD		0x0b	//ָ��������
#define	PLAY_CMD_FILE		0x0f	//����ָ���ļ��е��ļ�
#define	PLAY_CMD_FILE_LOOP	0x17	//ѭ��˳�򲥷�ָ���ļ��е��ļ�

#define	PLAY_BAUD_115200	5		//115200������
#define	PLAY_VOLUME_MAX		30		//�������?
/*
typedef struct
{
	uint8_t head;
	uint8_t addr;
	uint8_t len;
	uint8_t cmd;
	uint8_t feedback;
	uint8_t datah;
	uint8_t datal;
	uint8_t sumh;
	uint8_t suml;
	uint8_t tail;
}PLAY_FRAME;

PLAY_FRAME play_buf = 
{
	0x7e,				//head
	0xff,				//addr
	0x06,				//len
	PLAY_CMD_DEVICE,	//cmd
	0,					//feedback
	0,					//data
	0,
	0,					//sum
	0,
	0xef				//tail
};
*/

typedef enum{
	MUSIC_HORN = 1,
	MUSIC_SLOPE_DANGER,
	MUSIC_BACK_ATTEN,
	MUSIC_ULRASONIC_ATTEN,
}e_music_num;


extern TLoopBuf music_FIFO;

void play_file_voice(uint8_t file,uint8_t num);
void play_set_volume(uint8_t volume);
void play_file_loop(uint8_t file);
void play_set_flash(void);
void music_play_task(void);

void music_init(void);

#endif

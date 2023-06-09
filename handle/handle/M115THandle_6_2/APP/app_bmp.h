#ifndef __APP_BMP_H
#define __APP_BMP_H

//���
#define BATTERY_X	0
#define BATTERY_Y	0
#define BATTERY_SIZE_X	34		
#define BATTERY_SIZE_Y	16

//����ͼ��
#define KEYLOCK_LOGO_X		42
#define KEYLOCK_LOGO_Y		0
#define KEYLOCK_SIZE_X		11
#define KEYLOCK_SIZE_Y		16

//�Զ����ֶ�
#define AUTO_X	60
#define AUTO_Y	0
#define AUTO_SIZE_X	24
#define AUTO_SIZE_Y	24


//����ͼ�� ���� ����
#define CONNECT_X	112
#define CONNECT_Y	0
#define CONNECT_SIZE_X	16
#define CONNECT_SIZE_Y	16

#define CONNECT_BT_X	96
#define CONNECT_BT_Y	0
#define CONNECT_BT_SIZE_X	16
#define CONNECT_BT_SIZE_Y	16

//���� 0-9
#define BMP_NUM_PLUS_X		10
#define BMP_NUM_PLUS_Y		16
#define NUM_PLUS_SIZE_X	20
#define NUM_PLUS_SIZE_Y	40


//Km/h
#define KNH_X	48
#define KNH_Y	48
#define KNH_SIZE_X	27		
#define KNH_SIZE_Y	16

//mode
#define MODE_X	80
#define MODE_Y	16
#define MODE_SIZE_X	48		
#define MODE_SIZE_Y	48


//�ٶȱ�
#define DIAL_X	45
#define DIAL_Y	40
#define DIAL_SIZE_X	24
#define DIAL_SIZE_Y	24



//����ģʽ
#define MODE_SW_X	80
#define MODE_SW_Y	40
#define MODE_SW_SIZE_X		24
#define MODE_SW_SIZE_Y		24

//���� 1-5
//�ٶȵ�λ
#define SPEED_X		130
#define SPEED_Y		16
#define NUM_SIZE_X	24
#define NUM_SIZE_Y	48

//ģʽlogo
#define MODELOGO_X	130
#define MODELOGO_Y	16
#define MODELOGO_SIZE_X	33
#define MODELOGO_SIZE_Y	40

//������ʾͼ��
#define KEY1_LOGO_X		56
#define KEY2_LOGO_X		128
#define KEY_LOGO_Y		48
#define KEY_SIZE_X		16
#define KEY_SIZE_Y		16





//֧����
#define SUPPORTLOGO_X	130
#define SUPPORTLOGO_Y	16
#define SUPPORTLOGO_SIZE_X	48
#define SUPPORTLOGO_SIZE_Y	40


//�Ĵ�
#define TRACKSLOGO_X	130
#define TRACKSLOGO_Y	24
#define TRACKSLOGO_SIZE_X	42
#define TRACKSLOGO_SIZE_Y	32

//�����
#define PUSHLOGO_X	130
#define PUSHLOGO_Y	24
#define PUSHLOGO_SIZE_X	53
#define PUSHLOGO_SIZE_Y	24

#define MODELOGO_MAXSIZE_X	53
#define MODELOGO_MAXSIZE_Y	48

//����ͼ XSTO
#define XSTOLOGO_X	30
#define XSTOLOGO_Y	24
#define XSTOLOGO_SIZE_X	67
#define XSTOLOGO_SIZE_Y	32


//����ͼ CT420��������
#define CT420LOGO_X	120
#define CT420LOGO_Y	16
#define CT420LOGO_SIZE_X	51
#define CT420LOGO_SIZE_Y	56

typedef enum
{
	KEY_POWER_ON=0,
	KEY_SPEED_ADD,
	KEY_SPEED_SUB,
	KEY_BEEP,
	KEY_AUTO,
	KEY_MODE,
	KEY_LEFT,
	KEY_RIGHT,	
	KEY_LAMP,		
}	KEY_NUM;	

extern const unsigned char BMP_Battery[6][BATTERY_SIZE_X*BATTERY_SIZE_Y/8];
extern const unsigned char BMP_Connect[3][CONNECT_SIZE_X*CONNECT_SIZE_Y/8];
extern const unsigned char BMP_Speed_Dial[DIAL_SIZE_X*DIAL_SIZE_Y/8];
extern const unsigned char BMP_Auto[2][AUTO_SIZE_X*AUTO_SIZE_Y/8];
extern const unsigned char BMP_Mode[MODE_SW_SIZE_X*MODE_SW_SIZE_Y/8];
extern const unsigned char BMP_NUM[6][NUM_SIZE_X*NUM_SIZE_Y/8];
extern const unsigned char BMP_MODE_LOGO[7][MODELOGO_SIZE_X*MODELOGO_SIZE_Y/8];
extern const unsigned char BMP_KEY[9][KEY_SIZE_X*KEY_SIZE_Y/8];
extern const unsigned char BMP_LOCK[KEYLOCK_SIZE_X * KEYLOCK_SIZE_Y/8];
extern const unsigned char BMP_SUPPORT[4][SUPPORTLOGO_SIZE_X*SUPPORTLOGO_SIZE_Y/8]; 
extern const unsigned char BMP_PUSH[2][PUSHLOGO_SIZE_X*PUSHLOGO_SIZE_Y/8];
extern const unsigned char BMP_TRACK[2][TRACKSLOGO_SIZE_X*TRACKSLOGO_SIZE_Y/8];
extern const unsigned char BMP_XSTO_1[XSTOLOGO_SIZE_X*XSTOLOGO_SIZE_Y/8];
extern const unsigned char BMP_CT420[CT420LOGO_SIZE_X*CT420LOGO_SIZE_Y/8];
extern const unsigned char BMP_NUM_PLUS[10][NUM_PLUS_SIZE_X*NUM_PLUS_SIZE_Y/8];
extern const unsigned char BMP_MODE[2][MODE_SIZE_X*MODE_SIZE_Y/8];
extern const unsigned char BMP_KNH[KNH_SIZE_X*KNH_SIZE_Y/8];
#endif

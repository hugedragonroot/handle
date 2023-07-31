#ifndef __APP_REMOTE_PARA_H
#define __APP_REMOTE_PARA_H

#include <stdint.h>
#include "main.h"
/************************* ���νǶ� *************************/
/*
							-90
						*	|
					*		|
				*			|
			*				|
		*					|
	*						|
0	* * * * * *	0
	*						|
		*					|
			*				|
				*			|
					*		|
						*	|
							90
*/	

/************************* ���̽Ƕ� *************************/
/*
							90
						*	|
					*		|
				*			|
			*				|
		*					|
	*						|
0	* * * * * *	0
	*						|
		*					|
			*				|
				*			|
					*		|
						*	|
							-90
*/	


/************************* ҡ�˿��� *************************/

#define MOVE_SPEEDDEF_ZERO				0
#define BACK_SPEEDDEF_UP					300
#define BACK_SPEEDDEF_DOWN				-300
#define FORNT_SPEEDDEF_UP					BACK_SPEEDDEF_UP
#define FORNT_SPEEDDEF_DOWN				BACK_SPEEDDEF_DOWN
#define SEAT_SPEEDDEF_UP					BACK_SPEEDDEF_UP
#define SEAT_SPEEDDEF_DOWN				BACK_SPEEDDEF_DOWN

#define PUSH_ORD_SPEED_ADD 300 
#define PUSH_ORD_SPEED_SUB -300 
#define PUSH_ORD_SPEED_ZERO 0 

#define Walking_SpeedMAX		3600

#define Tracks_SpeedMAX			3000    			//���ٵ��ٶ�
#define Support_elongationMAX			3000    //�쳤�����ֵ //ʵ��ǰ֧��2900 ��֧��2870
#define BACK_ULT_TOP_X_1M			1000						//����������
#define BACK_ULT_TOP_X			700						//���������¥�ݶ�������̾���
#define BACK_ULT_MIN_X			100						//��������̾���

#define DISTANCE_GAP	50	//������������ξ����
#define DISTANCE_GAP1	10	//������������ξ����
#define DISTANCE_GAP2	20	//������������ξ����
#define DISTANCE_GAP3	30	//������������ξ����
#define DISTANCE_GAP4	40	//������������ξ����
#define DISTANCE_GAP5	70	//������������ξ����

#define DISTANCE_LAND	55	//������������ξ����


#if DRIVE_TYPE_NEW
#define BACK_ULT_Y_DEF			100						//������Ĭ�Ͼ���
#else
#define BACK_ULT_Y_DEF			50						//������Ĭ�Ͼ���
#endif
#define STAIRS_Y_DEF				50						//¥�ݸ߶Ⱦ���
#define STAIRS_X_DEF				50						//¥�ݿ�Ⱦ���

#define BACK_ULT_MIN_Y			(BACK_ULT_Y_DEF + STAIRS_Y_DEF)	//��������¥1��̨��
#define BACK_ULT_MIN_2Y			(BACK_ULT_MIN_Y + STAIRS_Y_DEF)						//��������¥2��̨��

#define DISTANCE_HALL_1MM		(370.0/200)	
#define DISTANCE_HALL_150MM		(DISTANCE_HALL_1MM*100)	
#define DISTANCE_HALL_200MM		(DISTANCE_HALL_1MM*200)						//��������
#define DISTANCE_HALL_300MM		(DISTANCE_HALL_1MM*300)						//��������
#define DISTANCE_HALL_400MM		(DISTANCE_HALL_1MM*400)						//��������
#define DISTANCE_HALL_650MM		(DISTANCE_HALL_1MM*800)						//��������
#define DISTANCE_HALL_900MM		(DISTANCE_HALL_1MM*900)						//��������
#if DRIVE_TYPE_NEW
 #define ANGLE_DEFUALT				-1000//10��
#else
#define ANGLE_DEFUALT				1000//10��
#endif
#define BODY_SAFE_ANGLE			5000

#define ANGLE_MAX						6000

#define COORD_SQRT_MAX 100

#define BATTERY_MAX_VOL 28.5f
// #define BATTERY_MIN_VOL 19.6f
#define BATTERY_MIN_VOL 20.6f


/* ����ģʽ */
typedef enum {
																			//CT200			//CT420	
  eWalking                = 0x00,     //ƽ������
  eSupportBack						= 0x01,     //��֧����	//��֧����	
  eSupportFornt           = 0x02,    	//ǰ֧����	//�����
  eBackrest               = 0x03,     //����			
  eTracks                 = 0x04,     //�Ĵ�			//�Ĵ�
  eUpstairs               = 0x05,     //��¥
  eDownstairs							= 0x06,     //��¥

} EControlMode;

/* ���з�ʽ */
typedef enum {
  eManual                 = 0x00,     //�ֶ�ģʽ
  eAutoLoading            = 0x01,     //������Զ�
  eAutoUP                 = 0x02,     //�Զ���¥
  eAutoDOWN               = 0x03,     //�Զ���¥
} EOperationMode;

/* �ٶȵ�λ */
typedef enum {
  eGearOne                = 0x01,     //һ��
  eGearTwo                = 0x02,     //����
  eGearThree              = 0x03,     //����
  eGearFore               = 0x04,     //�ĵ�
  eGearFive               = 0x05,     //�嵵
} ESpeedGear;

/*���ֿ��Ʒ�ʽ */
typedef enum {
	eSpeedMode									= 0x00,     //ͨ���ٶȿ���
  eElongationMode							= 0x01,     //ͨ���쳤������
	eAngleMode	                = 0x02,     //�Ƕȱջ�����
} ESupportMode;

	
/* ����״̬ */
typedef enum {
	eHorn                   = 0x80,     //����
  eHeadLamp               = 0x40,     //ǰ���
  eLeftLamp               = 0x20,     //��ת��
  eRightLamp              = 0x10,     //��ת��
  eBackLamp               = 0x08,     //��β��
	eSupportup							=	0x02,			//֧����
	eSupportdown						= 0x01,			//֧�Ž�
} EPeripherals;

typedef enum{
	eUnLock,//����״̬
	eLock,//����״̬
}ELockState;

/* ����״̬ */
typedef enum {
  eOffline                = 0,     //����
  eWire					= 1,     //��������
  eBlubtooth              = 2,     //��������

} ERemoteConnect;

/* ���ػ�״̬ */
typedef enum {
  ePowerOff								= 0,     //�ػ�
  ePowerOn 								= 1,     //����
	
} EPower;

/* ֧����״̬ */
typedef enum {
  eSupportManl							= 0,    //�ֶ�
  eSupportAuto 							= 1,    //�Զ�
	eSupportUp								=	2,		//����	
	eSupportDown							=	3,		//�½�
} ESupport;

#define HANDLE_ERROR_NUM 3
typedef union {
	struct {
		// struct{
			uint8_t joyXyError:1;
			uint8_t CANDisconnectError:1;
			uint8_t angleError:1;
		// }handle_bit;
		// struct {
			uint8_t leftWheelError:1;
			uint8_t rightWheelError:1;
			uint8_t pushRodError:1;
			uint8_t seatRodError:1;
			uint8_t breakError:1;
		// }control_bit;
	}bit;
	uint8_t all[1];
}APP_Error_t;



/* ң��ͨ�� */
typedef struct remote_setting
{
  uint8_t   RemoteBTCmd;    /* ң������ָ�� */	
  uint8_t   RemoteBTConnect;    /* ң�����ӱ�־ */
  uint8_t   RemoteBTCount;			//ң�����Ӽ�ʱ
	
  uint8_t   RemoteConnect;    /* ң�����ӱ�־ */
  uint8_t   RemoteCount;			//ң�����Ӽ�ʱ
  uint8_t   PowerStatus;			/* ң�ؿ��ر�־ */
	
//   uint8_t   CoordXH;          /* ҡ�� X �����8λ */
//   uint8_t   CoordXL;          /* ҡ�� X �����8λ */
  int16_t   CoordX;           /* ҡ�� X ���� */
  
//   uint8_t   CoordYH;          /* ҡ�� Y �����8λ */
//   uint8_t   CoordYL;          /* ҡ�� Y �����8λ */
  int16_t   CoordY;           /* ҡ�� Y ���� */

	
#if USING_XY_TO_SPEED
  int16_t   CoordSqrt;        /* ����ƽ���� */	
	float 		CoordAngle;				/* ҡ�˽Ƕȣ�-180< x <= 180��*/
	// int16_t 		CoordIntAngle;				/* ҡ�˽Ƕȣ�-180< x <= 180��*/
  int16_t   CoordSpeed;       /* Ŀ������ٶ� */
#endif

  uint8_t   ControlMode;      /* ����ģʽ */
  uint8_t   ControlPre;
	uint8_t   Control_change;		/*����ģʽ�л�*/
  uint8_t   M115CtrlMode;      /* ����ģʽ */
  uint8_t   M115CtrlModePre;

  uint8_t   OperationMode;    /* ���з�ʽ *///�Զ��ֶ�
  uint8_t   OperationPre;
  uint8_t   SpeedGear;        /* �ٶȵ�λ */
  uint8_t   SpeedPre;
  uint8_t   Peripherals;      /* ����״̬ */
  uint8_t   PeripheryPre;
	uint8_t   HandleLock:1;				/* ����״̬ */
//   uint8_t   Reserved;         /* ����λ */ 
	uint8_t folding_time; //�۵�ʱ��

	uint8_t   Battery;          /* ��ص��� */

  uint8_t   Model;				//�豸����
	uint32_t  totalkm;			//�����
	uint32_t  speed_kmh;			//�ٶ�	
//  int16_t   aPitch;           /* ������ */
//  int16_t   aRoll;            /* ��ת�� */
//  int16_t   aYaw;             /* ƫ���� */	
	uint8_t   LampSwitch;				//�ƹ⿪��		
	uint8_t		VoiceLevel;				//����
	uint8_t  	VoiceSwitch;			//��ʾ����
	uint8_t		AlarmSwitch;			//������ʾ
	uint8_t		CruiseCtrlSwitch;	//����Ѳ��
	// uint8_t		CurrentMusic;	//�������ȼ�Ҫ���ŵ�����
	int8_t angleAlarmPrecent;//�Ƕȱ����ٷֱ� �����νǶȱ仯
	uint8_t	ev1527_user_id[EV1527_USER_NUM][2] ;//����ң����ID
	uint8_t CANQueueLenMax:4;
	uint8_t CANQueueLenCur:4;
	uint16_t CANBufferQueueLenMax;
	uint16_t CANBufferQueueLenCur;

} TREMOTE_SETTING_PARA;
extern TREMOTE_SETTING_PARA   Remote_setting_para;


typedef struct {
	int16_t 	pcbAngle;//���ؽǶ� �½���
	uint16_t 	ultrasonic[3];//����������
	int16_t		pitch_angle; // ������,���´�			//ˮƽֵ -18000< x < 18000  Ĭ��0 
	int16_t		roll_angle;	//�����,����� 	//ˮƽֵ -18000< x < 18000  Ĭ��0
}ATTITUDE_PARA;


typedef struct remote_trans
{
	
	uint8_t		Drivermodel;			//�������ͺ�	
	uint8_t		Driversoftware;		//������汾��
	uint8_t		Driversoftwarepre;//������汾��		

	int16_t	  walking_speed_L;					//�����ߵ���ٶ�(��ǰ)
	int16_t	  walking_speed_R;					//�����ߵ���ٶ�(��ǰ)
	int16_t  push_rod_speed; // M115�����Ƹ��ٶ�
	int16_t  seat_rod_speed; // M115�����Ƹ��ٶ�

	uint8_t folding_state:1;//�۵�״̬
	// uint16_t push_rod_state;//�Ƹ�״̬

#if 0
	int16_t		pitch_angle; // ������,���´�			//ˮƽֵ -18000< x < 18000  Ĭ��0 
	int16_t		roll_angle;	//�����,����� 	//ˮƽֵ -18000< x < 18000  Ĭ��0

	uint16_t ultrasonic[4];
#else
	ATTITUDE_PARA m115_attitude;
#endif

	APP_Error_t errorFlag;

#if 0

	uint16_t	fornt_ultrasonic_y;				//ǰ��������		(mm)
	uint16_t	back_ultrasonic_x;				//���ú�����
	uint16_t	back_ultrasonic_y;				//����������
	uint8_t		sumcheck_err;							//�ۼӺ�У�����		
#endif

//   uint8_t   Battery;          /* ��ص��� */
	float BatteryVol;//��ص�ѹ

}TREMOTE_TRANS_PARA;

extern TREMOTE_TRANS_PARA			Remote_trans_para;
extern TREMOTE_TRANS_PARA			Remote_receive_para;


/**************************�ֻ�APPͨѶЭ��************************/

typedef enum {
  ePExpand									= 0,     //չ��
  ePfold 										= 1,     //�۵�
} EP_M115mode;


	
/* ״̬ */
typedef enum {
	ePLamp										= 0x01,			//����
	ePPower										= 0x02,    //�ػ�
	ePGyroscopeCheck					=	0x04,			//������У׼
	ePVoice										= 0x08,		//������ʾ��
	ePAlarm										=	0x10,		//��������
	ePCruiseCtrl							= 0x20,		//����Ѳ��
} EP_Peripherals;


	
/* ���� */
typedef enum {
	ePAngleNone                 = 0x00,     //�޶���
	ePAngleUp										= 0x01,			//��	
	ePAngleDown									= 0x02,			//��	
} EP_Angle;

/* ���� */
typedef enum {
	ePSeatNone                  = 0x00,     //�޶���
	ePSeatFornt									= 0x01,			//����ǰ��
	ePSeatBack									= 0x02,			//���κ���
	ePSeatUp										= 0x03,			//������
	ePSeatDown									= 0x04,			//���ν�
} EP_Seat;


/* �ٶȵ�λ */
typedef enum {
  ePGearOne                = 0x01,     //һ��
  ePGearTwo                = 0x02,     //����
  ePGearThree              = 0x03,     //����
  ePGearFore               = 0x04,     //�ĵ�
  ePGearFive               = 0x05,     //�嵵
} EP_SpeedGear;


	
/* ���� */
typedef enum {
	ePVoiceLevel_1            = 0x01,     //��
	ePVoiceLevel_2						= 0x02,			//��
	ePVoiceLevel_3						= 0x03,			//��
} EP_VoiceLevel;
 
/*�������*/
typedef enum{
	ERROR_NONE = 0,//û�д���

	ERROR_JOY_XY,//����ҡ�˲���ԭ��
	ERROR_CAN_DISCONNECT ,//�������ͨ�Ź���

	ERROR_LEFT_WHEEL,//���ִ���
	ERROR_RIGHT_WHEEL,//���ִ���
	ERROR_PUSH_ROB,//�����Ƹ˴���
	ERROR_SEAT_ROB,//�����Ƹ˴���
	
}APP_ERROR_FLAG;



typedef struct Phone_trans
{
	
	uint8_t		Drivermodel;			//�ֱ��ͺ�	
	uint8_t		Driversoftware;		//������汾��
	uint8_t		Driversoftwarepre;//������汾��		

	
	uint8_t		M130CtrlMode;			//��� ģʽѡ��
	uint8_t		M115CtrlMode;		//M115ģʽѡ�� չ�� �۵�
	uint8_t   LampSwitch;				//�ƹ⿪��	
	uint8_t   SeatCtrl;					//���ε���
	uint8_t   ForntwheelCtrl;		//ǰ�ֵ���
	uint8_t   BackwheelCtrl;		//���ֵ���
	uint8_t   CoordX;
	uint8_t   CoordY;
	uint8_t		SpeedLevel;					//�ٶȵ�λ
	uint8_t   Power;						//���ػ�ָ��
	uint8_t   Gyroscopechk;			//������У׼
	uint8_t		VoiceLevel;				//����
	uint8_t  	VoiceSwitch;			//��ʾ����
	uint8_t		AlarmSwitch;			//������ʾ
	uint8_t		CruiseCtrlSwitch;	//����Ѳ��


}TPHONE_TRANS_PARA;

extern TPHONE_TRANS_PARA			Phone_trans_para;
extern TPHONE_TRANS_PARA			Phone_receive_para;


void Remote_para_init(void);
void Remote_para_default(void);
#endif


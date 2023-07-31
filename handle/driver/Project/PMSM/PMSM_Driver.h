/**********************************************************/
//@Autor:Raw
//@Version:0.0.1
//@End Time:2023/3/21
/**********************************************************/
#ifndef __PMSM_DRIVER_H__
#define __PMSM_DRIVER_H__
#include "common.h"
#include "FOC.h"
#include "pid.h"


#define IQ_MAX 30.0f
#define Speed_MAX 3000.0f

typedef union 
{
  struct
  {
		     
		uint16_t OverCur:1;                 //����
		uint16_t Undervoltag:1;             //Ƿѹ
		uint16_t OverVoltag:1;          		//��ѹ 
		uint16_t OverTemp:1;								//����		
		
		uint16_t hall_Error:1; 							//���������쳣
		uint16_t Bandbrake:1;           		//��բ״̬
		uint16_t Current_Error:1;						//���������쳣
		uint16_t off_link;									//�豸����
		
		uint16_t hall_calib_error:1;				//hallУ׼�쳣
  }bit;
  uint16_t all;
}PMSM_Status_t;

typedef enum
{
	MegCaliNone = 0,	//δУ׼
	MegCaliPositive,		//��У׼
	MegCaliReverse,		//��У׼
	MegCaliOK			//У׼���
}sCaliStatusDef;

typedef enum
{
	MotorStop,	//���ֹͣ
	MotorStart,	//�����ʼ��
	MotorWatit,	//����ȴ�
	MotorCail,	//��λУ׼
	MotorTest,	//�������ģʽ
	MotorRun,	//�������
	MotorPrecharge,//������
}sStateTypeDef;

typedef enum
{
	DragMode_e = 0,	 	//ǿ��ģʽ
	HallCaliMode_e=1,		//����У׼ģʽ
	SpeedCtrl_e=2,  		//ת��ģʽ
	VoltCtrl_e=3,    	//��ѹģʽ
	TorqueCtrl_e=4,    	//����ģʽ
	AngelCtrl_e=5,    	//�Ƕ�ģʽ
	
	Speed_TorqueCtrl = 6,	//�ٶȣ�������
	angel_current = 7,
	StartMotor_e=9,    		//�������
	StopMotor_e=10			//ֹͣ���
	
}sCtrlMode_e;




extern APP_PMSM_DRIVER APP_PMSM;
typedef struct
{
	sStateTypeDef MotorState;						//��ǰ״̬
	
	sStateTypeDef MotorPreState;				//��һ�ε��״̬
	sCaliStatusDef	MotorCali;				//���У׼״̬
	sCtrlMode_e  ControlMode;				//����ģʽ
	PMSM_Status_t PMSM_Status;


	uint8_t SpeedCmdCount;
	float SpeedCmd;		//�ٶȿ���
	float SpeedSet;		//�ٶ��趨
	float SpeedReal;	//��ʵ�ٶ�
	float SpeedFiler;	//�˲��ٶ�
	float SpeedNew;		//���ٶ��趨
	int16_t appspeed_l;
	int16_t appspeed_r;
	int16_t LastSpeedNew;
	int16_t SpeedTemp;
	uint8_t count_100ms;
	float speed_temp ;
	float speed_forward;
	
	float SpeedNow;		//�ٶ��м����
	float S_low_acc;	//�������ٶ�
	float S_Add_acc;	//�ٶȼ��ٶ�
	float S_Run_acc;
	
	float AngleCmd;		//�Ƕȿ���
	float AngleSet;		//�Ƕ��趨
	float AngleReal;	//��ʵ�Ƕ�
	float AngleNew;		//�½Ƕ��趨

	float OpenLoopVoltCmd; 	//������ѹ����
	float OpenLoopVoltSet;	//������ѹ�趨
	int32_t turns;		//Ȧ��������λ�ÿ���

	float CurrentCmd; 	//��������
	float CurrentSet;	//�����趨
	float Current_over;//����ֵ

	float VoltVin;				//�����ѹ
	float MaxCurrent;			//������������
	float MaxSpeed;
	float pairs;
	float Mostemp;
	
	uint16_t count_buffers;
	uint16_t count_put ;
	uint8_t count_10ms;
	float variance;
	float mean_value;
	
	
	float CurrOffset[3];		//A  B  C �����ƫ�õ���
	
	float f_IqSet;   		//Iq�趨ֵ
	float f_BusVoltRatio;   //ĸ�ߵ�ѹ����  ���ڵ�ѹ���������
	float Theta_acc;
	uint8_t Start_time;
	uint8_t stop_time;
	
	uint8_t speed_pid_count;
	uint8_t current_pid_count;
	uint16_t count_acc;
	float dec_ratio;
	
	
	bool first_offer;
	bool first_start;
	bool first_stop;
	
	uint16_t i16_OverCurrentCount;	//��������
	PIDFloat_Obj angle_pid;
	PIDFloat_Obj speed_pid;
	PIDFloat_Obj current_id_pid;
	PIDFloat_Obj current_iq_pid;
	
	foc_para_t foc;	
	uint8_t foc_count;
}sMotorCtrlTypeDef;

void PMSM_para_init(void);
void PMSM_para_clear(PMSM_Num num);
void pmsm_start(PMSM_Num num);
void pmsm_stop(PMSM_Num num);
void pmsm_break_io(PMSM_Num num,bool value);
void PMSM_PWM_RUN(PMSM_Num num,Phase3_value *PWM);
void PMSM_Start_Filter(PMSM_Num num);
extern sMotorCtrlTypeDef PMSM[2];
#endif

/**********************************************************/
//@Autor:Raw
//@Version:0.0.1
//@End Time:2023/3/20
/**********************************************************/
#ifndef __BRUSH_DRIVER__
#define __BRUSH_DRIVER__
#include "common.h"
#include "pid.h"
#define BRUSH_MAX_SPEED	300.0f
typedef union 
{
  struct
  {
		     
		uint16_t OverCur:1;                 //����
		uint16_t Undervoltag:1;             //Ƿѹ
		uint16_t OverVoltag:1;          		//��ѹ 
		uint16_t OverTemp:1;								//����		
		
		uint16_t FINISH_calibration_One:1;  //�н�У׼���
		uint16_t Bandbrake:1;           		//��բ״̬
		uint16_t Current_Error:1;						//���������쳣
		uint16_t pitch_error:1;							//��������ֵ
		
		uint16_t rol_error:1;								//Բ������ֵ
  }bit;
  uint16_t all;
}Brush_Status_t;

typedef enum 
{
	Brush_Cmd_Stop = 0, //���ֹͣ
	Brush_Cmd_Star, //�������
	Brush_Cmd_Run,	//�����ת
	Brush_Cmd_Calib,	//���У׼
	Brush_Cmd_Standby,//����
}Brush_Cmd_e_t;


typedef enum
{
	normal_mode = 0,
	angle_mode = 1,	//�Ƕ�ģʽ
	speed_mode = 2,	//�ٶ�ģʽ
	Voltage_mode = 3,	//��ѹģʽ
	current_mode = 4,	//����ģʽ
	calib_mode = 5,		//У׼ģʽ���ٶ�ģʽ��
	mpu_mode = 6,		//������ģʽ
	fold_mode = 7,	//
	zero_mode = 8,	//
	stop_mode=0xff,
}Brush_mode_t;


typedef struct
{
	Brush_Status_t brush_state;	//���״̬
	Brush_Cmd_e_t brush_Cmd;	//����Ŀ�������
	Brush_Cmd_e_t brush_Cmd_Pre;	//����Ŀ�������
	Brush_mode_t Control_mode;		//	����ģʽ
	
	float MOS_temp;					//MOS���¶�ֵ
	uint16_t Trip_Calib_Current;	//�г̹���ֵ���趨
	uint16_t Total_Current;				//�ܹ���ֵ���趨
	int16_t return_current;
	float Current_set;	//�������趨ֵ
	float VOLTAGE_set;	//��ѹ���趨ֵ
	float Current_real; //��������ȡֵ
	float Current_ADC;	
	uint16_t Current_MAX;	//������
	float speed_base;	//�ٶȻ�׼
	float set_speed;	//�趨�ٶ�
	float real_speed;	//��ʵ�ٶ�
	int16_t return_speed;
	float MAX_speed;	//�ٶ����ֵ
	int16_t SpeedlNew;	//Ӧ�ò��趨���ٶ�ֵ
	float SpeedAcc;			//���ٶ�
	float Voltage_New;
	float Voltage_acc;
	
	
	float AngelSet;		//�Ƕ��趨
	int16_t AngelReal;	//ʵ�ʽǶ�
	int16_t AngelBase;	//�Ƕȵ����
	float AngelMAX;		//�Ƕ�����г�,��������
	int16_t AngelNew;		//�����趨�½Ƕ�ֵ
	
	int16_t MPU_New;
	float MPU_Set;
	float MPU_acc;
	float MPU_Basic;
	float roll_basic;
	float MPU_REAL;
	float Last_MPU;
	float Tem_MPU;
	int16_t return_mpu;
	uint16_t AngelTrip;
	uint16_t AngelAcc;	//�Ǽ��ٶ�
	uint8_t angle_count;	//λ�û�����
	uint8_t calib_count;
	uint8_t zero_count;
	
	
	int32_t LineRealPullLenth;	//Ȧ��ʵ�ʳ���
	int8_t Error_range_angle; //λ�û��趨�������Χ
	
	uint16_t start_count;	//�������������Ϊ����ʱ��բ
	uint16_t stop_count;	//���ֹͣ������Ϊ����ʱ��բ
	uint16_t fold_speed_count;
	bool first_start;
	bool first_stop;
	
	PIDFloat_Obj MPU_PID;	//�ٶ�pid
	PIDFloat_Obj SPEED_PID;	//�ٶ�pid
	PIDFloat_Obj ANGLE_PID;	//λ��pid
	PIDFloat_Obj CURRENT_PID;	//����pid
}Brush_Motor_t;


extern Brush_Motor_t Brush[2];

void Brush_para_init(void);
void Brush_Stop(Brush_Num num);
void Brush_Star(Brush_Num num,float value);
void Brush_Control_Mode(Brush_Num num,float FinalOut);
void Brush_Mode_Loop(Brush_Num num);
void Brush_Start_Filter(Brush_Num num);
#endif

/**********************************************************/
//@Autor:Raw
//@Version:0.0.1
//@End Time:2023/3/20
//@Function:Brush_Control.c
/**********************************************************/

#include "Brush_Control.h"
#include "Brush_Driver.h"
#include "tim.h"
#include "adc_callback.h"
#include "can.h"
#include "QMI8658C.h"

/****************************************
  �������ƣ�RESET_BRUSH_PARA
  ������������λ������ò���
  ����������������
  ��������ֵ����
***************************************/
void RESET_BRUSH_PARA(Brush_Num num)
{

	Brush[num].AngelNew = 0.0f;
	Brush[num].MPU_New = 0.0f;
	Brush[num].Voltage_New = 0.0f;
	Brush[num].SpeedlNew =0.0f;
	
	Brush[num].set_speed = 0.0f;
	Brush[num].MPU_Set = 0.0f;
	Brush[num].AngelSet = 0.0f;
	Brush[num].Current_set = 0.0f;
	Brush[num].VOLTAGE_set = 0.0f;
	
}
/****************************************
  �������ƣ�BRUSH_SPEED_ANGLE_U
  �������������U�ٶȣ��Ƕȼ���(T������)
  ������������
  ��������ֵ����
***************************************/
uint8_t flage_brush = 4;
void test_BRUSH(void)
{
	if(flage_brush == 0)
	{
		Brush[Brush_A].brush_Cmd_Pre = Brush_Cmd_Stop;
		Brush[Brush_U].brush_Cmd_Pre = Brush_Cmd_Stop;
	}
#if 0
	if(flage_brush == 1)
	{
		Brush[Brush_A].brush_Cmd_Pre = Brush_Cmd_Star;
		Brush[Brush_U].brush_Cmd_Pre = Brush_Cmd_Star;
		Brush[Brush_A].Control_mode = speed_mode;
		Brush[Brush_A].SpeedlNew = 300;
		Brush[Brush_U].Control_mode = mpu_mode;
		Brush[Brush_U].set_speed = -150.0f;
	}
	
	if(flage_brush == 2)
	{
		Brush[Brush_A].brush_Cmd_Pre = Brush_Cmd_Star;
		Brush[Brush_U].brush_Cmd_Pre = Brush_Cmd_Star;
		Brush[Brush_A].Control_mode = speed_mode;
		Brush[Brush_A].SpeedlNew = -300;
		Brush[Brush_U].Control_mode = mpu_mode;
		Brush[Brush_U].set_speed = 150.0f;
	}
#endif
#if 1
	if(flage_brush == 1)
	{
		Brush[Brush_A].brush_Cmd_Pre = Brush_Cmd_Star;
		Brush[Brush_U].brush_Cmd_Pre = Brush_Cmd_Star;
		Brush[Brush_A].Control_mode = fold_mode;
		Brush[Brush_A].SpeedlNew = 300;
		Brush[Brush_U].Control_mode = fold_mode;
		Brush[Brush_U].SpeedlNew = -300.0f;
	}
	
	if(flage_brush == 2)
	{
		Brush[Brush_A].brush_Cmd_Pre = Brush_Cmd_Star;
		Brush[Brush_U].brush_Cmd_Pre = Brush_Cmd_Star;
		Brush[Brush_A].Control_mode = speed_mode;
		Brush[Brush_A].SpeedlNew = -300;
		Brush[Brush_U].Control_mode = mpu_mode;
		Brush[Brush_U].set_speed = 150.0f;
	}
	if(flage_brush == 3)
	{
		Brush[Brush_A].brush_Cmd_Pre = Brush_Cmd_Star;
		Brush[Brush_U].brush_Cmd_Pre = Brush_Cmd_Star;
		Brush[Brush_A].Control_mode = speed_mode;
		Brush[Brush_A].SpeedlNew = 300;
		Brush[Brush_U].Control_mode = mpu_mode;
		Brush[Brush_U].set_speed = -150.0f;
	}
#endif
}
void BRUSH_SPEED_ANGLE_U(void)
{
	/*����ߵ�ƽ�����ڣ���λus*/
	BrushSpeed[Brush_U].TimeUs = (uint32_t)((float)BrushSpeed[Brush_U].u32_SumCount * (1.0f / 84.0f));
	//1.0f/36.0f A���������εļ�������

	if(BrushSpeed[Brush_U].TimeUs == 0x00000000) BrushSpeed[Brush_U].TimeUs = 0xffffffff;
	//13�ǵ�������ļ�����Z
	//28.0f
	BrushSpeed[Brush_U].i16_RealSpeed = (int16_t)((1000.0f * 1000.0f * 60.0f) / ((float)BrushSpeed[Brush_U].TimeUs * 28.0f)); // ==> us/r  * 1000 ==> ms/r  * 1000 = S/r  *60  ==> min/r   
	Brush[Brush_U].return_speed = BrushSpeed[Brush_U].i16_RealSpeed ;
	//4.0f����Ϊ���õļ���������Ϊ���û���ĵ���ֻ����Ϊ���ô��ת��
	
	//��ʽ��N=60*f/ZM,f����Ƶ�ʣ�z��ʾһȦ��������������11��1��Ƶ��������13����M���������ڲ���������
	if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim4) == 1)//�����־
	{
		Brush[Brush_U].real_speed = (float)BrushSpeed[Brush_U].i16_RealSpeed;//�ٶȸ�ֵ
	}
	else
	{
		Brush[Brush_U].real_speed = (float)BrushSpeed[Brush_U].i16_RealSpeed* (-1.0f) ;//�ٶȸ�ֵ
	}
	
	BrushSpeed[Brush_U].PosCountPHA =(BrushSpeed[Brush_U].PosWRIFCount - 1)*65536 + Brush_U_CNT();//����A��������������ǽǶ�����ֵ

	Brush[Brush_U].LineRealPullLenth = BrushSpeed[Brush_U].PosCountPHA - BrushSpeed[Brush_U].PosBase;//��¼A��λ��

	Brush[Brush_U].AngelReal =  Brush[Brush_U].LineRealPullLenth;//��������ʵ�Ƕ�
}

/****************************************
  �������ƣ�MOTOR_SPEED_ANGLE_A
  �������������A�ٶȣ��Ƕȼ��㣨T�����٣�
  ������������
  ��������ֵ����
***************************************/
void BRUSH_SPEED_ANGLE_A(void)
{
	/*����ߵ�ƽ�����ڣ���λus*/
	BrushSpeed[Brush_A].TimeUs = (uint32_t)((float)BrushSpeed[Brush_A].u32_SumCount * (1.0f / 84.0f));
//	BrushSpeed[Brush_A].TimeUs = (uint32_t)((float)BrushSpeed[Brush_A].u32_SumCount * (1.0f / 2.0f));
	//1.0f/36.0f A���������εļ�������

	if(BrushSpeed[Brush_A].TimeUs == 0x00000000) BrushSpeed[Brush_A].TimeUs = 0xffffffff;
	//13�ǵ�������ļ�����Z
	BrushSpeed[Brush_A].i16_RealSpeed = (int16_t)((1000.0f * 1000.0f * 60.0f) / ((float)BrushSpeed[Brush_A].TimeUs * 1.0f)); // ==> us/r  * 1000 ==> ms/r  * 1000 = S/r  *60  ==> min/r   24
	Brush[Brush_A].return_speed = BrushSpeed[Brush_A].i16_RealSpeed ;
	//4.0f����Ϊ���õļ���������Ϊ���û���ĵ���ֻ����Ϊ���ô��ת��
	
	//��ʽ��N=60*f/ZM,f����Ƶ�ʣ�z��ʾһȦ��������������11��1��Ƶ��������13����M���������ڲ���������
	if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim3) == 0)//�����־
	{
		Brush[Brush_A].real_speed = (float)BrushSpeed[Brush_A].i16_RealSpeed;//�ٶȸ�ֵ
	}
	else
	{
		Brush[Brush_A].real_speed = (float)BrushSpeed[Brush_A].i16_RealSpeed* (-1.0f) ;//�ٶȸ�ֵ
	}
	
	BrushSpeed[Brush_A].PosCountPHA =(BrushSpeed[Brush_A].PosWRIFCount - 1)*65536 + Brush_A_CNT();//����A��������������ǽǶ�����ֵ

	Brush[Brush_A].LineRealPullLenth = BrushSpeed[Brush_A].PosCountPHA - BrushSpeed[Brush_A].PosBase;//��¼A��λ��
	Brush[Brush_A].AngelReal = Brush[Brush_A].LineRealPullLenth;//��������ʵ�Ƕ�
}

/****************************************
  �������ƣ�Brush_Angel_Acc_handle
  �����������Ƕ�ģʽ�Ǽ��ٶ��¶ȸ���
  ����������������
  ��������ֵ����
***************************************/
void Brush_Angel_Acc_handle(Brush_Num num)
{
	if(Brush[num].AngelSet < Brush[num].AngelNew)//�����¶�
	{
			
		Brush[num].AngelSet = Brush[num].AngelSet + Brush[num].AngelAcc ;
		if(Brush[num].AngelSet > Brush[num].AngelNew)
		{
			Brush[num].AngelSet = Brush[num].AngelNew ;
		}
	}
	else if(Brush[num].AngelSet > Brush[num].AngelNew)//�����¶�
	{
		Brush[num].AngelSet = Brush[num].AngelSet - Brush[num].AngelAcc ;
		if(Brush[num].AngelSet < Brush[num].AngelNew)
		{
			Brush[num].AngelSet = Brush[num].AngelNew ;
		}
	}
	
	
}
/****************************************
  �������ƣ�Brush_Speed_Acc_handle
  �����������ٶ�ģʽ���ٶ��¶ȸ���
  ����������������
  ��������ֵ����
***************************************/
void Brush_Speed_Acc_handle(Brush_Num num)
{
	if(Brush[num].set_speed < Brush[num].SpeedlNew)
	{
			
		Brush[num].set_speed = Brush[num].set_speed + Brush[num].SpeedAcc ;//�����ٶ��¶�
		if(Brush[num].set_speed > Brush[num].SpeedlNew)
		{
			Brush[num].set_speed = Brush[num].SpeedlNew ;
		}
	}
	else if(Brush[num].set_speed > Brush[num].SpeedlNew)
	{
		Brush[num].set_speed = Brush[num].set_speed - Brush[num].SpeedAcc ;//�����ٶ��¶�
		if(Brush[num].set_speed < Brush[num].SpeedlNew)
		{
			Brush[num].set_speed = Brush[num].SpeedlNew ;
		}
	}
	
	
}
/****************************************
  �������ƣ�Brush_Voltage_handle
  ������������ѹģʽ����
  ����������������
  ��������ֵ����
***************************************/
void Brush_Voltage_handle(Brush_Num num)
{
	if(Brush[num].VOLTAGE_set < Brush[num].Voltage_New)
	{
			
		Brush[num].VOLTAGE_set = Brush[num].VOLTAGE_set + Brush[num].Voltage_acc ;//�����ٶ��¶�
		if(Brush[num].VOLTAGE_set > Brush[num].Voltage_New)
		{
			Brush[num].VOLTAGE_set = Brush[num].Voltage_New ;
		}
	}
	else if(Brush[num].VOLTAGE_set > Brush[num].Voltage_New)
	{
		Brush[num].VOLTAGE_set = Brush[num].VOLTAGE_set - Brush[num].Voltage_acc ;//�����ٶ��¶�
		if(Brush[num].VOLTAGE_set < Brush[num].Voltage_New)
		{
			Brush[num].VOLTAGE_set = Brush[num].Voltage_New ;
		}
	}
	Brush_Control_Mode(num,Brush[num].VOLTAGE_set);//˫���Կ���
	
}

void Brush_MPU_ACC_handle(Brush_Num num)
{
	if(Brush[num].MPU_Set < (float)Brush[num].MPU_New)
	{
			
		Brush[num].MPU_Set = Brush[num].MPU_Set + Brush[num].MPU_acc ;//�����ٶ��¶�
		if(Brush[num].MPU_Set > (float)Brush[num].MPU_New)
		{
			Brush[num].MPU_Set = (float)Brush[num].MPU_New ;
		}
	}
	else if(Brush[num].MPU_Set > (float)Brush[num].MPU_New)
	{
		Brush[num].MPU_Set = Brush[num].MPU_Set - Brush[num].MPU_acc ;//�����ٶ��¶�
		if(Brush[num].MPU_Set < (float)Brush[num].MPU_New)
		{
			Brush[num].MPU_Set = (float)Brush[num].MPU_New ;
		}
	}
	
}
/****************************************
  �������ƣ�Brush_Speed_Handle
  �����������ٶ�ģʽ����
  ����������������
  ��������ֵ����
***************************************/
void Brush_Speed_Handle(Brush_Num num)
{
	Brush_Speed_Acc_handle(num);//���ٶ��¶�
	if(Brush[num].SpeedlNew==0&&fabs(Brush[num].real_speed)<2.0f)//��Ϊ�������ֱ��ʵͣ�̫����Ҫ����������
	{
		Brush[num].SPEED_PID.Ui = 0.0f;
	}
	PID_run_FloatspdVolt(&Brush[num].SPEED_PID,Brush[num].set_speed/Brush[num].MAX_speed,Brush[num].real_speed/Brush[num].MAX_speed);//�ٶ�pid������
	Brush_Control_Mode(num,Brush[num].SPEED_PID.PIDOut);//˫���Կ���
}



/****************************************
  �������ƣ�Brush_Angle_Handle
  �����������Ƕ�ģʽ����
  ����������������
  ��������ֵ����
***************************************/
void Brush_Angle_Handle(Brush_Num num)
{
//	Angel_Acc_handle(NUM);
	Brush[num].AngelSet = Brush[num].AngelNew;
	if(fabs((float)Brush[num].AngelReal - (float)Brush[num].AngelNew)>Brush[num].Error_range_angle )//�Ƕ�����Χ������ջ�
	{
		if(++Brush[num].angle_count>=2)//�⻷�������ڻ�������
		{
			Brush[num].angle_count = 0;
			PID_run_FloatspdVolt(&Brush[num].ANGLE_PID,Brush[num].AngelSet,Brush[num].AngelReal);//�Ƕȱջ�
			Brush[num].set_speed = Brush[num].ANGLE_PID.PIDOut;//��������ٶȻ�
		}
		PID_run_FloatspdVolt(&Brush[num].SPEED_PID,Brush[num].set_speed/Brush[num].MAX_speed,Brush[num].real_speed/Brush[num].MAX_speed);//�ٶȱջ�
		Brush_Control_Mode(num,Brush[num].SPEED_PID.PIDOut);	
	}
	else
	{
		clear_pid(&Brush[num].ANGLE_PID);//�Ƕ�����Χ�����pid����
//		Brush[num].brush_Cmd =Brush_Cmd_Stop ;
	}

}

void Brush_MPU_Handle(Brush_Num num)
{
//	Angel_Acc_handle(NUM);
	if(num == Brush_A)
	{
		Brush[num].MPU_REAL = mpu_filter.ptich - Brush[num].MPU_Basic;
		Brush[num].return_mpu = (int16_t)Brush[num].MPU_REAL;
		Brush_MPU_ACC_handle(num);
		if(fabs(Brush[num].MPU_REAL- (float)Brush[num].MPU_New)>1.0f )//�Ƕ�����Χ������ջ�
		{
			if(++Brush[num].angle_count>=2)//�⻷�������ڻ�������
			{
				Brush[num].angle_count = 0;
//				Brush[num].Tem_MPU = (Brush[num].MPU_REAL - Brush[num].Last_MPU)/0.02f*100.0f;
//				Brush[num].Last_MPU = Brush[num].MPU_REAL;
				if(Brush[num].MPU_REAL>0.0f) Brush[num].set_speed = -200.0f;
				if(Brush[num].MPU_REAL<0.0f) Brush[num].set_speed = 200.0f;
				PID_run_FloatspdVolt(&Brush[num].MPU_PID,Brush[num].MPU_Set,Brush[num].MPU_REAL);//�Ƕȱջ�
//				Brush[num].set_speed = Brush[num].MPU_PID.PIDOut;//��������ٶȻ�
			}
			PID_run_FloatspdVolt(&Brush[num].SPEED_PID,(Brush[num].set_speed+ Brush[num].MPU_PID.PIDOut)/Brush[num].MAX_speed,Brush[num].real_speed/Brush[num].MAX_speed);//�ٶȱջ�
		}
		else
		{
//			Brush[num].Last_MPU = Brush[num].MPU_REAL;
			clear_pid(&Brush[num].MPU_PID);//�Ƕ�����Χ�����pid����
			clear_pid(&Brush[num].SPEED_PID);
//		Brush[num].brush_Cmd =Brush_Cmd_Stop ;
		}
//		Brush_Control_Mode(num,Brush[num].MPU_PID.PIDOut);	
		Brush_Control_Mode(num,Brush[num].SPEED_PID.PIDOut);	
	}
	else if(num == Brush_U)
	{
		Brush[num].MPU_REAL = mpu_filter.ptich - Brush[num].MPU_Basic;
		Brush[num].return_mpu = (int16_t)Brush[num].MPU_REAL;
		if(Brush[Brush_A].SpeedlNew >0.0f)Brush[num].set_speed = -150;
		if(Brush[Brush_A].SpeedlNew <0.0f)Brush[num].set_speed = 150;
		if(++Brush[num].angle_count>=2)//�⻷�������ڻ�������
		{
			Brush[num].angle_count = 0;
			PID_run_FloatspdVolt(&Brush[num].MPU_PID,Brush[num].MPU_Set,Brush[num].MPU_REAL);//�Ƕȱջ�
		}
		PID_run_FloatspdVolt(&Brush[num].SPEED_PID,(Brush[num].set_speed+ Brush[num].MPU_PID.PIDOut)/Brush[num].MAX_speed,Brush[num].real_speed/Brush[num].MAX_speed);
		Brush_Control_Mode(num,Brush[num].SPEED_PID.PIDOut);
	}

}


void Brush_Flod_Handle(Brush_Num num)
{
	Brush_Speed_Acc_handle(num);//���ٶ��¶�
	if(Brush[num].SpeedlNew==0&&fabs(Brush[num].real_speed)<2.0f)//��Ϊ�������ֱ��ʵͣ�̫����Ҫ����������
	{
		Brush[num].SPEED_PID.Ui = 0.0f;
	}
	PID_run_FloatspdVolt(&Brush[num].SPEED_PID,Brush[num].set_speed/Brush[num].MAX_speed,Brush[num].real_speed/Brush[num].MAX_speed);//�ٶ�pid������
	Brush_Control_Mode(num,Brush[num].SPEED_PID.PIDOut);//˫���Կ���
}

void Brush_Zero_Handle(Brush_Num num)
{
	if(num == Brush_A)
	{
		Brush_Speed_Acc_handle(num);//���ٶ��¶�
		if(Brush[num].SpeedlNew==0&&fabs(Brush[num].real_speed)<2.0f)//��Ϊ�������ֱ��ʵͣ�̫����Ҫ����������
		{
			Brush[num].SPEED_PID.Ui = 0.0f;
		}
		PID_run_FloatspdVolt(&Brush[num].SPEED_PID,Brush[num].set_speed/Brush[num].MAX_speed,Brush[num].real_speed/Brush[num].MAX_speed);//�ٶ�pid������
		Brush_Control_Mode(num,Brush[num].SPEED_PID.PIDOut);//˫���Կ���
	}
	else if(num == Brush_U)
	{
		Brush[num].MPU_REAL = mpu_filter.ptich - Brush[num].MPU_Basic;
		Brush[num].return_mpu = (int16_t)Brush[num].MPU_REAL;
		Brush_MPU_ACC_handle(num);
		if(fabs(Brush[num].MPU_REAL- (float)Brush[num].MPU_New)>0.5f )//�Ƕ�����Χ������ջ�
		{
			if(++Brush[num].angle_count>=2)//�⻷�������ڻ�������
			{
				Brush[num].angle_count = 0;
//				Brush[num].Tem_MPU = (Brush[num].MPU_REAL - Brush[num].Last_MPU)/0.02f*100.0f;
//				Brush[num].Last_MPU = Brush[num].MPU_REAL;
				PID_run_FloatspdVolt(&Brush[num].MPU_PID,Brush[num].MPU_Set,Brush[num].MPU_REAL);//�Ƕȱջ�
				Brush[num].set_speed = Brush[num].MPU_PID.PIDOut;//��������ٶȻ�
			}
			PID_run_FloatspdVolt(&Brush[num].SPEED_PID,Brush[num].set_speed/Brush[num].MAX_speed,Brush[num].real_speed/Brush[num].MAX_speed);//�ٶȱջ�
			Brush[num].brush_state.bit.FINISH_calibration_One = 0;
		}
		else
		{
//			Brush[num].Last_MPU = Brush[num].MPU_REAL;
			clear_pid(&Brush[num].MPU_PID);//�Ƕ�����Χ�����pid����
			clear_pid(&Brush[num].SPEED_PID);
			Brush[num].brush_state.bit.FINISH_calibration_One = 1;
//		Brush[num].brush_Cmd =Brush_Cmd_Stop ;
		}
//		Brush_Control_Mode(num,Brush[num].MPU_PID.PIDOut);	
		Brush_Control_Mode(num,Brush[num].SPEED_PID.PIDOut);
	}
}
/****************************************
  �������ƣ�Brush_Current_Handle
  ��������������ģʽ����
  ����������������
  ��������ֵ����
***************************************/
void Brush_Current_Handle(Brush_Num num)
{
	if(Brush[Brush_A].Current_set==0.0f&&fabs(Brush[num].Current_real)<0.1f)//���������в��죬̫�͵�����ƫ�ֱ�����pid����
	{
		Brush[num].CURRENT_PID.Ui = 0.0f;
	}
	PID_run_FloatspdVolt(&Brush[num].CURRENT_PID,Brush[num].Current_set/Brush[num].Current_MAX,Brush[num].Current_real/Brush[num].Current_MAX);//������pid
	Brush_Control_Mode(num,Brush[num].CURRENT_PID.PIDOut);
}

/****************************************
  �������ƣ�Brush_Run_Task
  �����������������ģʽ�µ�����״̬��
  ����������������
  ��������ֵ����
***************************************/
void Brush_Run_Task(Brush_Num num)
{

	switch(Brush[num].Control_mode)
	{
		case normal_mode://�ϵ�Ĭ��ģʽ
			break;
		case angle_mode://�Ƕ�ģʽ
			Brush_Angle_Handle(num);
			break;
		case speed_mode://�ٶ�ģʽ
			Brush_Speed_Handle(num);
			break;
		case Voltage_mode://��ѹģʽ
			Brush_Voltage_handle(num);
			break;
		case current_mode://����ģʽ
			Brush_Current_Handle(num);
			break;
		case calib_mode:
			break;
		case mpu_mode:
			Brush_MPU_Handle(num);
			break;
		case fold_mode:
			Brush_Flod_Handle(num);
			break;
		case zero_mode:
			Brush_Zero_Handle(num);
			break;
		default:
			break;
	}
	
}
/****************************************
  �������ƣ�Over_Current
  ����������������⣬����
  ������������
  ��������ֵ����
***************************************/
uint16_t CurrentFaultNumber[3]={0};		
void Brush_Over_Current(void)
{
	if(Brush[Brush_A].Control_mode == calib_mode&&Brush[Brush_U].Control_mode == calib_mode)//У׼ģʽ��
	{
		if(fabs(Brush[Brush_A].Current_real)>Brush[Brush_A].Trip_Calib_Current||fabs(Brush[Brush_U].Current_real)>Brush[Brush_U].Trip_Calib_Current)//У׼����0.5a
		{
			CurrentFaultNumber[0]++;
			if(CurrentFaultNumber[0]>10)//1s
			{
				CurrentFaultNumber[0]=0;
				Brush[Brush_A].brush_Cmd = Brush_Cmd_Stop;//ֹͣ���
				Brush[Brush_U].brush_Cmd = Brush_Cmd_Stop;//ֹͣ���
			}
		}
		else
		{
			CurrentFaultNumber[0]=0;
		}
	}	
	if(Brush[Brush_A].brush_Cmd == Brush_Cmd_Run&&Brush[Brush_A].Control_mode != calib_mode)//A�����������ģʽ��
	{
		if(fabs(Brush[Brush_A].Current_real)>Brush[Brush_A].Total_Current)//�ܹ���ֵ1a
		{
			CurrentFaultNumber[1]++;
			if(CurrentFaultNumber[1]>10)//1s
			{
				CurrentFaultNumber[1]=0;
//				Brush[Brush_A].AngelKeep = Brush[Brush_A].AngelReal;
				Brush[Brush_A].brush_state.bit.OverCur = 1;//������־��1
			}
		}
		else
		{
			CurrentFaultNumber[1]=0;
		}
	}
	
	if(Brush[Brush_U].brush_Cmd == Brush_Cmd_Run&&Brush[Brush_U].Control_mode != calib_mode)//U�����������ģʽ��
	{
		if(fabs(Brush[Brush_U].Current_real)>Brush[Brush_U].Total_Current)//�ܹ���ֵ1a
		{
			CurrentFaultNumber[2]++;
			if(CurrentFaultNumber[2]>10)//1s
			{
				CurrentFaultNumber[2]=0;
//				Brush[Brush_U].AngelKeep = Brush[Brush_U].AngelReal;
				Brush[Brush_U].brush_state.bit.OverCur = 1;//������־��1
			}
		}
		else
		{
			CurrentFaultNumber[2]=0;
		}
	}

}
/****************************************
  �������ƣ�ADC_CURRENT_HANDLE
  ����������adc��������
  ������������
  ��������ֵ����
***************************************/
void ADC_CURRENT_HANDLE(void)
{
	Brush_GetCurrent();/* ��ȡ����ADֵ */
  /* A������� */
  Brush[Brush_A].Current_ADC = 0.9f*Brush[Brush_A].Current_ADC + ADC_Data.Brush_adc[Brush_A].adc_current*0.1f;//һ�׵�ͨ�˲�
	if(Brush1_A_PWM_READ()>Brush1_B_PWM_READ())//��������
	{
		Brush[Brush_A].Current_real = Brush[Brush_A].Current_ADC;
	}
	else
	{
		Brush[Brush_A].Current_real = -Brush[Brush_A].Current_ADC;
	}
	Brush[Brush_A].return_current = (int16_t)Brush[Brush_A].Current_real;
  /* U������� */
    Brush[Brush_U].Current_ADC  = 0.9f*Brush[Brush_U].Current_ADC  + ADC_Data.Brush_adc[Brush_U].adc_current*0.1f;//һ�׵�ͨ�˲�
	if(Brush2_A_PWM_READ()>Brush2_B_PWM_READ())//��������
	{
		Brush[Brush_U].Current_real = Brush[Brush_U].Current_ADC ;
	}
	
	else
	{
		Brush[Brush_U].Current_real = -Brush[Brush_U].Current_ADC ;
	}
	Brush[Brush_U].return_current = (int16_t)Brush[Brush_U].Current_real;
	
//	Over_Current();
}
/****************************************
  �������ƣ�STATUS_UPDATE
  �������������ĸ�ߵ�ѹ��mos����λ���ص�״̬����
  ������������
  ��������ֵ����
***************************************/
static uint8_t vd_count = 0;
uint32_t ms_counts = 0;
void STATUS_UPDATE(void)
{
	ms_counts++;
	if(++vd_count>=5)
	{
		vd_count = 0;
		GetVoltage_MOSTEMP();//��ȡĸ�ߵ�ѹ
	}
	if(ADC_Data.Bus_Volt>=30.0f)//��ѹ30v
	{
		Brush[Brush_A].brush_state.bit.OverVoltag = 1;
		Brush[Brush_U].brush_state.bit.OverVoltag = 1;
	}
	else if(ADC_Data.Bus_Volt<16)//Ƿѹ16v
	{
		Brush[Brush_A].brush_state.bit.Undervoltag = 1;
		Brush[Brush_U].brush_state.bit.Undervoltag = 1;
	}
	else if(ADC_Data.Bus_Volt>16.0f&&ADC_Data.Bus_Volt<30.0f)//������ѹ��Χ��
	{
		
		Brush[Brush_A].brush_state.bit.OverVoltag = 0;
		Brush[Brush_U].brush_state.bit.OverVoltag = 0;
		Brush[Brush_A].brush_state.bit.Undervoltag = 0;
		Brush[Brush_U].brush_state.bit.Undervoltag = 0;
	}
	if(ADC_Data.mos_temp > 90.0f)//����
	{
		Brush[Brush_A].brush_state.bit.OverTemp = 1;
		Brush[Brush_U].brush_state.bit.OverTemp = 1;
	}
	
}

/****************************************
  �������ƣ�brush_Cmd_handle
  �������������״̬��
  ����������������
  ��������ֵ����
***************************************/
void brush_Cmd_handle(Brush_Num num)
{
		switch(Brush[num].brush_Cmd)
		{
			case Brush_Cmd_Stop://ֹͣģʽ
				Brush_Stop(num);//��pwm
				if(++Brush[num].stop_count>=5)//����50ms����բ
				{
					Brush[num].stop_count = 0;
					Brush[num].brush_Cmd = Brush_Cmd_Standby;//����״̬
					RESET_BRUSH_PARA(num);//��λ������ò���
				}
				break;
			case Brush_Cmd_Standby://����ģʽ
				clear_pid(&Brush[num].ANGLE_PID);//���Ӧ��pid����
				clear_pid(&Brush[num].SPEED_PID);
				clear_pid(&Brush[num].CURRENT_PID);
				clear_pid(&Brush[num].MPU_PID);
				RESET_BRUSH_PARA(num);
				break;
			case Brush_Cmd_Star://�������ģʽ
				if(++Brush[num].start_count>=5)//����50ms���е��
				{
					Brush_Star(num,0.0f);//��pwm
					Brush[num].start_count = 0;
					Brush[num].brush_Cmd = Brush_Cmd_Run;//����ģʽ
				}

				break;
			case Brush_Cmd_Run://����ģʽ
				Brush_Run_Task(num);//���ģʽ״̬��
				break;
			default:
				break;
		}
}


void Brush_Mode_Loop(Brush_Num num)
{
	if(Brush[num].brush_state.bit.Current_Error != 1)
	{
		Brush_Start_Filter(num);
	}

}	




























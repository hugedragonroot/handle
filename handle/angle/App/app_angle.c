#include "app_angle.h"
#include ".\ATusart\bsp_ATusart.h"
#include ".\TiMbase\bsp_TiMbase.h" 
#include ".\QMI8658C\qmi8658c.h"
#include "math.h"


TAngle  AngleP;
TAngle  AngleR;
TAngle  AngleY;
uint8_t ChipCorrect;
int16_t ChipTemp;
int32_t ChipAccSum[3], ChipGyroSum[3], ChipMagSum[3];
int16_t ChipAccBias[3], ChipGyroBias[3], ChipGyroMag[3];
int16_t ChipAccC[3], ChipGyroC[3], ChipMagC[3];// calibrate
float   ChipAccf[3], ChipGyrof[3], ChipMagf[3];


/**
  * @brief  �Ƕ��˲�������ʼ��
  * @param  None
  * @retval None
  */
void AngleParam_Init(void)
{
  AngleP.akp      = 0.9;
  AngleP.adt      = 0.01;
  AngleP.state[3] = 0;
  AngleR.akp      = 0.9;
  AngleR.adt      = 0.01;
  AngleR.state[3] = 0;
  AngleY.akp      = 0.9;
  AngleY.adt      = 0.01;
  AngleY.state[3] = 0;
}

/**
  * @brief  �Ƕ�����У׼
  * @param  None
  * @retval None
  */
uint8_t AngleData_Calibrate(uint8_t num)
{
  static uint16_t i, ReadSta;
  
  ChipAccSum[0]   = 0;
  ChipAccSum[1]   = 0;
  ChipAccSum[2]   = 0;
  ChipGyroSum[0]  = 0;
  ChipGyroSum[1]  = 0;
  ChipGyroSum[2]  = 0;
  
  for(i = 0; i < num; i ++) {
    ReadSta = ChipReadData;
    if(!ReadSta) break;
    ChipDelay100ms;
  }
  
  for(i = 0; i < num; i ++) {
    ReadSta = ChipReadData;
    if(ReadSta) return ReadSta;
    else {
      ChipAccSum[0]   += ChipAccData[0];
      ChipAccSum[1]   += ChipAccData[1];
      ChipAccSum[2]   += (ChipAccData[2] - 16384);
      ChipGyroSum[0]  += ChipGyroData[0];
      ChipGyroSum[1]  += ChipGyroData[1];
      ChipGyroSum[2]  += ChipGyroData[2];
    }
    ChipDelay100ms;
  }
  
  ChipAccBias[0]  = ChipAccSum[0] / num;
  ChipAccBias[1]  = ChipAccSum[1] / num;
  ChipAccBias[2]  = ChipAccSum[2] / num;
  ChipGyroBias[0] = ChipGyroSum[0] / num;
  ChipGyroBias[1] = ChipGyroSum[1] / num;
  ChipGyroBias[2] = ChipGyroSum[2] / num;
  
  return 0x00;
}

float pitch_mpu = 0.0f;
float roll_mpu = 0.0f;
float Yijielvbo_pitch(float accel_m, float gyro_m)
{
	static float Angle_one_pitch,Angle_L1_pitch;
	
	Angle_L1_pitch = Angle_one_pitch;
	Angle_one_pitch = 0.04f*accel_m + (1.0f-0.04f)*(Angle_L1_pitch + gyro_m*0.01f);
	return Angle_one_pitch;
}

float Yijielvbo_roll(float accel_m, float gyro_m)
{
	static float Angle_one_roll,Angle_L1_roll;
	
	Angle_L1_roll = Angle_one_roll;
	Angle_one_roll = 0.04f*accel_m + (1.0f-0.04f)*(Angle_L1_roll + gyro_m*0.01f);
	return Angle_one_roll;
}
/**
  * @brief  �Ƕ�����ת��
  * @param  None
  * @retval None
  */
void AngleData_Convert(void)
{
  ChipTemp = ChipTempData*5 >> 7;
  
  ChipAccC[0]   = ChipAccData[0] - ChipAccBias[0];
  ChipAccC[1]   = ChipAccData[1] - ChipAccBias[1];
  ChipAccC[2]   = ChipAccData[2] - ChipAccBias[2];
  ChipGyroC[0]  = ChipGyroData[0] - ChipGyroBias[0];
  ChipGyroC[1]  = ChipGyroData[1] - ChipGyroBias[1];
  ChipGyroC[2]  = ChipGyroData[2] - ChipGyroBias[2];
  
  ChipAccf[0]   = atan2((float)ChipAccC[1],(float)(-ChipAccC[2]))*57.3;//57.29578
  ChipAccf[1]   = atan2((float)ChipAccC[0],(float)(-ChipAccC[2]))*57.3;
  ChipAccf[2]   = atan2((float)ChipAccC[0],(float)(-ChipAccC[1]))*57.3;
  
  ChipGyrof[0]  = (float)ChipGyroC[0]/32;  /* ����������ת��,Gyro/32 ~ 1024dps */
  ChipGyrof[1]  = (float)ChipGyroC[1]/32;
  ChipGyrof[2]  = (float)ChipGyroC[2]/32;
	
	pitch_mpu = Yijielvbo_pitch(ChipAccf[0],ChipGyrof[0]);
	roll_mpu = Yijielvbo_roll(ChipAccf[1],ChipGyrof[1]);
}


/**
  * @brief  ���׻����˲�
  * @param  angle_m: ���ٶȺϳɽ�
  * @param  gyro_m:  �����ǽ��ٶ�
  * @retval None
  */
int16_t SecondOrderFiltering(TAngle* Anglex, float accel_m, float gyro_m)
{
	static int16_t AngleOut;
	
	Anglex->state[0] = (accel_m - Anglex->state[3]) * (Anglex->akp * Anglex->akp);  /* Kp */
	Anglex->state[1] += Anglex->state[0] * Anglex->adt;  /* (Kp*t)/Ti */
	Anglex->state[2] = Anglex->state[1] + (2 * Anglex->akp) * (accel_m - Anglex->state[3]) + gyro_m; /* (Kp*Td)/t */
	Anglex->state[3] += Anglex->state[2] * Anglex->adt;
	AngleOut = Anglex->state[3] * 100;  /* �Ŵ�100�� */
	return AngleOut;
}

//float Q_bias_x;/*  */
//float GyroFinal;/* �˲�����ٶ� */
//float AngleErr;/* �����ǻ��ֽ�����ٶȼƺϳɽǵ�ƫ�� */
//float AngleFinal;/* �˲���Ƕ� */
///* ���б��� */
//char  C_0 = 1;
//float PCt_0, PCt_1, E;
//float K_0, K_1, t_0, t_1;
//float Pdot[4] = { 0,0,0,0 };
//float PP[2][2] = { { 1, 0 },{ 0, 1 } };
///**
//  * @brief  �������˲�
//  * @param  Accel: ���ٶȺϳɽ�
//  * @param  Gyro:  �����ǽ��ٶ�
//  * @retval None
//  */
//int16_t Kalman_Filter_X(float Accel, float Gyro) //����������
//{
//  static int16_t AngleOut;
//  
//  AngleFinal += (Gyro - Q_bias_x) * dt; //�������
//  
//  Pdot[0] = Q_angle - PP[0][1] - PP[1][0]; // Pk-����������Э�����΢��
//  Pdot[1] = -PP[1][1];
//  Pdot[2] = -PP[1][1];
//  Pdot[3] = Q_gyro;
//  
//  PP[0][0] += Pdot[0] * dt;   // Pk-����������Э����΢�ֵĻ���
//  PP[0][1] += Pdot[1] * dt;   // =����������Э����
//  PP[1][0] += Pdot[2] * dt;
//  PP[1][1] += Pdot[3] * dt;
//  
//  AngleErr = Accel - AngleFinal;	//zk-�������
//  
//  PCt_0 = C_0 * PP[0][0];
//  PCt_1 = C_0 * PP[1][0];
//  
//  E = R_angle + C_0 * PCt_0;
//  
//  K_0 = PCt_0 / E;
//  K_1 = PCt_1 / E;
//  
//  t_0 = PCt_0;
//  t_1 = C_0 * PP[0][1];
//  
//  PP[0][0] -= K_0 * t_0;		 //����������Э����
//  PP[0][1] -= K_0 * t_1;
//  PP[1][0] -= K_1 * t_0;
//  PP[1][1] -= K_1 * t_1;
//  
//  AngleFinal += K_0 * AngleErr;	 //�������
//  Q_bias_x += K_1 * AngleErr;	 //�������
//  GyroFinal = Gyro - Q_bias_x;	 //���ֵ(�������)��΢��=���ٶ�
//  
//  AngleOut = AngleFinal * 100;
//  
//  return AngleOut;
//}

/**
  ******************************************************************************
  * @file    mpu6050.c
  * @author  Rhui
  * @version V1.0
  * @date    2020-xx-xx
  * @brief   MPU6050 Ӧ������
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

#include ".\MPU6050\mpu6050.h"
#include ".\ATusart\bsp_ATusart.h"
#include ".\SysTick\bsp_SysTick.h"
#include ".\i2c\bsp_i2c.h"
#include "math.h"


/*
    Y
    ^
    .---------  -> X
    |*Z       |
    |         |
    |         |     ����Ĵָָ����ٶ�������
    |         |     ������ָָ����������ת������
     ---------
*/


#define PI    3.1415926

int16_t MPU_Temp, Accel[3], Gyro[3];  /* 0:X  1:Y  2:Z */
float   Accel_X, Accel_Y, Accel_Z, Gyro_X, Gyro_Y, Gyro_Z;  /* �Ƕ�ת������ */


/**
  * @brief  ��ȡ�豸ID,�ж��豸����״̬
  * @param  ��
  * @retval ��
  */
uint8_t MPU6050_ReadAddr(void)
{
	static uint8_t I2C_DATA, ret;
	
	ret = Sensors_I2C_ReadRegister(MPU6050_ADDRESS_AD0_LOW, MPU6050_RA_WHO_AM_I, 1, &I2C_DATA);
	if( I2C_DATA != MPU6050_DEFAULT_ADDRESS || ret != 0 )
	{
		ForPC_Usart( "Error : %2x,%2x\r\n",I2C_DATA,ret );
		return ret;
	}
	else
	{
		ForPC_Usart( "Addr : %x\r\n",I2C_DATA );
		return 0x00;
	}
}

/**
  * @brief  MPU6050 ��ʼ��
  * @param  ��
  * @retval ��
  */
uint8_t MPU6050_Init(void)
{
	static uint8_t I2C_DATA, ret;
	
	I2C_DATA = 0x80;    /* �豸��λ */
	ret = Sensors_I2C_WriteRegister(MPU6050_ADDRESS_AD0_LOW, MPU6050_RA_PWR_MGMT_1, 1, &I2C_DATA);
  if( ret != 0 ) return ret;
	
  delay_ms(150);      /* ��ʼ��ǰҪ��ʱһ��ʱ��,��û����ʱ,��ϵ�����ϵ����ݿ��ܻ���� */
	
	I2C_DATA = 0x00;    /* �������״̬ */
	ret = Sensors_I2C_WriteRegister(MPU6050_ADDRESS_AD0_LOW, MPU6050_RA_PWR_MGMT_1, 1, &I2C_DATA);
  if( ret != 0 ) return ret;
	
	/* ���豸ID */
	ret = MPU6050_ReadAddr();
  if( ret != 0 ) return ret;
	
	I2C_DATA = 0x07;    /* �����ǲ�����,1KHz */
	ret = Sensors_I2C_WriteRegister(MPU6050_ADDRESS_AD0_LOW, MPU6050_RA_SMPLRT_DIV, 1, &I2C_DATA);
  if( ret != 0 ) return ret;
	
	I2C_DATA = 0x06;    /* ��ͨ�˲���������,��ֹƵ����1K,������5K */
	ret = Sensors_I2C_WriteRegister(MPU6050_ADDRESS_AD0_LOW, MPU6050_RA_CONFIG, 1, &I2C_DATA);
  if( ret != 0 ) return ret;
	
	I2C_DATA = 0x00;    /* ���ü��ٶȴ�����������2Gģʽ,���Լ� */
	ret = Sensors_I2C_WriteRegister(MPU6050_ADDRESS_AD0_LOW, MPU6050_RA_ACCEL_CONFIG, 1, &I2C_DATA);
  if( ret != 0 ) return ret;
	
	I2C_DATA = 0x18;    /* �������Լ켰������Χ,����ֵ:0x18(���Լ�,2000deg/s) */
	ret = Sensors_I2C_WriteRegister(MPU6050_ADDRESS_AD0_LOW, MPU6050_RA_GYRO_CONFIG, 1, &I2C_DATA);
  if( ret != 0 ) return ret;
	
	return 0x00;
}

/**
  * @brief  ��ȡ����
  * @param  ��
  * @retval ��
  */
uint8_t MPU6050_ReadData(void)
{
	static uint8_t mpubuf[14], ret;
	
	ret = Sensors_I2C_ReadRegister(MPU6050_ADDRESS_AD0_LOW, MPU6050_RA_ACCEL_XOUT_H, 14, mpubuf);
	if( ret != 0 ) return ret;
	else
	{
		Accel[0] = (mpubuf[0] << 8) | mpubuf[1];  /* ���ٶ� */
		Accel[1] = (mpubuf[2] << 8) | mpubuf[3];
		Accel[2] = (mpubuf[4] << 8) | mpubuf[5];
		MPU_Temp = (mpubuf[6] << 8) | mpubuf[7];  /* �¶� */
		Gyro[0]  = (mpubuf[8] << 8) | mpubuf[9];  /* ������ */
		Gyro[1]  = (mpubuf[10] << 8) | mpubuf[11];
		Gyro[2]  = (mpubuf[12] << 8) | mpubuf[13];
	//MPU_Temp = (((double)(MPU_Temp + 13200)) / 280) - 13;
		#ifdef ZAXIS_up
		/* Z�ᳯ�� */
		Accel_X = atan2((float)Accel[1],(float)Accel[2])*57.29578;
		Accel_Y = atan2((float)Accel[0],(float)Accel[2])*57.29578;
		Accel_Z = atan2((float)Accel[0],(float)(-Accel[1]))*57.29578;
		#elif defined ZAXIS_down
		/* Z�ᳯ�� */
		Accel_X = atan2((float)Accel[1],(float)(-Accel[2]))*57.29578;
		Accel_Y = atan2((float)Accel[0],(float)(-Accel[2]))*57.29578;
		Accel_Z = atan2((float)Accel[1],(float)Accel[0])*57.29578;
		#elif defined YAXIS_up
		/* Y�ᳯ�� */
		Accel_X = atan2((float)Accel[2],(float)Accel[1])*57.29578;
		Accel_Z = atan2((float)Accel[0],(float)Accel[1])*57.29578;
		Accel_Y = atan2((float)Accel[0],(float)(-Accel[2]))*57.29578;
		#elif defined YAXIS_down
		/* Y�ᳯ�� */
		Accel_X = atan2((float)Accel[2],(float)(-Accel[1]))*57.29578;
		Accel_Z = atan2((float)Accel[0],(float)(-Accel[1]))*57.29578;
		Accel_Y = atan2((float)Accel[2],(float)Accel[0])*57.29578;
		#elif defined XAXIS_up
		/* X�ᳯ�� */
		Accel_Y = atan2((float)Accel[2],(float)Accel[0])*57.29578;
		Accel_Z = atan2((float)Accel[1],(float)Accel[0])*57.29578;
		Accel_X = atan2((float)Accel[1],(float)(-Accel[2]))*57.29578;
		#elif defined XAXIS_down
		/* X�ᳯ�� */
		Accel_Y = atan2((float)Accel[2],(float)(-Accel[0]))*57.29578;
		Accel_Z = atan2((float)Accel[1],(float)(-Accel[0]))*57.29578;
		Accel_X = atan2((float)Accel[2],(float)Accel[2])*57.29578;
		#endif
		Gyro_X  = (float)Gyro[0]*5/82;   /* ����������ת��,Gyro/16.4 ~ 2000deg/s */
		Gyro_Y  = (float)Gyro[1]*5/82;
		Gyro_Z  = (float)Gyro[2]*5/82;
		return 0x00;
	}
}


#define K1 0.02   /* һ���˲�ϵ�� */
#define K2 0.90   /* �����˲�ϵ��,ϵ��Խ������Խ�� */
#define dt 0.01   /* ����ʱ�� */

/**
  * @brief  һ�׻����˲�(��������,���ھ�̬���,������Ư��������)
  * @param  angle_m: ���ٶȺϳɽ�
  * @param  gyro_m:  �����ǽ��ٶ�
  * @retval ��
  */
int16_t Yijielvbo(float accel_m, float gyro_m)
{
	static int16_t Angle1;
	static float Angle_one,Angle_L1;
	
	Angle_L1 = Angle_one;
	Angle_one = K1*accel_m + (1-K1)*(Angle_L1 + gyro_m*dt);
	Angle1 = Angle_one*100;  /* �Ŵ�100�� */
	
//	static float Ox1, Ox2, Oy1, Angle_one, Angle_L1;
//	Angle_L1 = Angle_one;
//	Ox1 = (accel_m-Angle_L1) * (K2*K2);  /* Kp */
//	Oy1 = Oy1 + (Ox1*dt);  /* (Kp*t)/Ti */
//	Ox2 = Oy1 + (2*K2) * (accel_m-Angle_L1) + gyro_m; /* (Kp*Td)/t */
//	Angle_one = Angle_L1 + Ox2*dt;
//	Angle1 = Angle_one*100;  /* �Ŵ�100�� */
	
//Angle = Angle_one;
	return Angle1;
}

/**
  * @brief  ���׻����˲�(���ֿ���)
  * @param  angle_m: ���ٶȺϳɽ�
  * @param  gyro_m:  �����ǽ��ٶ�
  * @retval ��
  */
int16_t Erjielvbo(float accel_m, float gyro_m)
{
	static int16_t Angle2;
	static float Tx1, Tx2, Ty1, Angle_two, Angle_L2;
	
	Angle_L2 = Angle_two;
	Tx1 = (accel_m-Angle_L2) * (K2*K2);  /* Kp */
	Ty1 = Ty1 + (Tx1*dt);  /* (Kp*t)/Ti */
	Tx2 = Ty1 + (2*K2) * (accel_m-Angle_L2) + gyro_m; /* (Kp*Td)/t */
	Angle_two = Angle_L2 + Tx2*dt;
	Angle2 = Angle_two*100;  /* �Ŵ�100�� */
//Angle = Angle_two;
	return Angle2;
}

/*********************************************END OF FILE**********************/

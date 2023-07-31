/**
  ******************************************************************************
  * @file    mpu6050.c
  * @author  Rhui
  * @version V1.0
  * @date    2020-xx-xx
  * @brief   MPU6050 应用驱动
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
    |         |     右手拇指指向加速度正方向
    |         |     其余四指指向陀螺仪旋转正方向
     ---------
*/


#define PI    3.1415926

int16_t MPU_Temp, Accel[3], Gyro[3];  /* 0:X  1:Y  2:Z */
float   Accel_X, Accel_Y, Accel_Z, Gyro_X, Gyro_Y, Gyro_Z;  /* 角度转换变量 */


/**
  * @brief  获取设备ID,判断设备连接状态
  * @param  无
  * @retval 无
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
  * @brief  MPU6050 初始化
  * @param  无
  * @retval 无
  */
uint8_t MPU6050_Init(void)
{
	static uint8_t I2C_DATA, ret;
	
	I2C_DATA = 0x80;    /* 设备复位 */
	ret = Sensors_I2C_WriteRegister(MPU6050_ADDRESS_AD0_LOW, MPU6050_RA_PWR_MGMT_1, 1, &I2C_DATA);
  if( ret != 0 ) return ret;
	
  delay_ms(150);      /* 初始化前要延时一段时间,若没有延时,则断电后再上电数据可能会出错 */
	
	I2C_DATA = 0x00;    /* 解除休眠状态 */
	ret = Sensors_I2C_WriteRegister(MPU6050_ADDRESS_AD0_LOW, MPU6050_RA_PWR_MGMT_1, 1, &I2C_DATA);
  if( ret != 0 ) return ret;
	
	/* 读设备ID */
	ret = MPU6050_ReadAddr();
  if( ret != 0 ) return ret;
	
	I2C_DATA = 0x07;    /* 陀螺仪采样率,1KHz */
	ret = Sensors_I2C_WriteRegister(MPU6050_ADDRESS_AD0_LOW, MPU6050_RA_SMPLRT_DIV, 1, &I2C_DATA);
  if( ret != 0 ) return ret;
	
	I2C_DATA = 0x06;    /* 低通滤波器的设置,截止频率是1K,带宽是5K */
	ret = Sensors_I2C_WriteRegister(MPU6050_ADDRESS_AD0_LOW, MPU6050_RA_CONFIG, 1, &I2C_DATA);
  if( ret != 0 ) return ret;
	
	I2C_DATA = 0x00;    /* 配置加速度传感器工作在2G模式,不自检 */
	ret = Sensors_I2C_WriteRegister(MPU6050_ADDRESS_AD0_LOW, MPU6050_RA_ACCEL_CONFIG, 1, &I2C_DATA);
  if( ret != 0 ) return ret;
	
	I2C_DATA = 0x18;    /* 陀螺仪自检及测量范围,典型值:0x18(不自检,2000deg/s) */
	ret = Sensors_I2C_WriteRegister(MPU6050_ADDRESS_AD0_LOW, MPU6050_RA_GYRO_CONFIG, 1, &I2C_DATA);
  if( ret != 0 ) return ret;
	
	return 0x00;
}

/**
  * @brief  获取数据
  * @param  无
  * @retval 无
  */
uint8_t MPU6050_ReadData(void)
{
	static uint8_t mpubuf[14], ret;
	
	ret = Sensors_I2C_ReadRegister(MPU6050_ADDRESS_AD0_LOW, MPU6050_RA_ACCEL_XOUT_H, 14, mpubuf);
	if( ret != 0 ) return ret;
	else
	{
		Accel[0] = (mpubuf[0] << 8) | mpubuf[1];  /* 加速度 */
		Accel[1] = (mpubuf[2] << 8) | mpubuf[3];
		Accel[2] = (mpubuf[4] << 8) | mpubuf[5];
		MPU_Temp = (mpubuf[6] << 8) | mpubuf[7];  /* 温度 */
		Gyro[0]  = (mpubuf[8] << 8) | mpubuf[9];  /* 陀螺仪 */
		Gyro[1]  = (mpubuf[10] << 8) | mpubuf[11];
		Gyro[2]  = (mpubuf[12] << 8) | mpubuf[13];
	//MPU_Temp = (((double)(MPU_Temp + 13200)) / 280) - 13;
		#ifdef ZAXIS_up
		/* Z轴朝上 */
		Accel_X = atan2((float)Accel[1],(float)Accel[2])*57.29578;
		Accel_Y = atan2((float)Accel[0],(float)Accel[2])*57.29578;
		Accel_Z = atan2((float)Accel[0],(float)(-Accel[1]))*57.29578;
		#elif defined ZAXIS_down
		/* Z轴朝下 */
		Accel_X = atan2((float)Accel[1],(float)(-Accel[2]))*57.29578;
		Accel_Y = atan2((float)Accel[0],(float)(-Accel[2]))*57.29578;
		Accel_Z = atan2((float)Accel[1],(float)Accel[0])*57.29578;
		#elif defined YAXIS_up
		/* Y轴朝上 */
		Accel_X = atan2((float)Accel[2],(float)Accel[1])*57.29578;
		Accel_Z = atan2((float)Accel[0],(float)Accel[1])*57.29578;
		Accel_Y = atan2((float)Accel[0],(float)(-Accel[2]))*57.29578;
		#elif defined YAXIS_down
		/* Y轴朝下 */
		Accel_X = atan2((float)Accel[2],(float)(-Accel[1]))*57.29578;
		Accel_Z = atan2((float)Accel[0],(float)(-Accel[1]))*57.29578;
		Accel_Y = atan2((float)Accel[2],(float)Accel[0])*57.29578;
		#elif defined XAXIS_up
		/* X轴朝上 */
		Accel_Y = atan2((float)Accel[2],(float)Accel[0])*57.29578;
		Accel_Z = atan2((float)Accel[1],(float)Accel[0])*57.29578;
		Accel_X = atan2((float)Accel[1],(float)(-Accel[2]))*57.29578;
		#elif defined XAXIS_down
		/* X轴朝下 */
		Accel_Y = atan2((float)Accel[2],(float)(-Accel[0]))*57.29578;
		Accel_Z = atan2((float)Accel[1],(float)(-Accel[0]))*57.29578;
		Accel_X = atan2((float)Accel[2],(float)Accel[2])*57.29578;
		#endif
		Gyro_X  = (float)Gyro[0]*5/82;   /* 陀螺仪量程转换,Gyro/16.4 ~ 2000deg/s */
		Gyro_Y  = (float)Gyro[1]*5/82;
		Gyro_Z  = (float)Gyro[2]*5/82;
		return 0x00;
	}
}


#define K1 0.02   /* 一阶滤波系数 */
#define K2 0.90   /* 二阶滤波系数,系数越大收敛越快 */
#define dt 0.01   /* 采样时间 */

/**
  * @brief  一阶互补滤波(比例控制,存在静态误差,陀螺仪漂移量积分)
  * @param  angle_m: 加速度合成角
  * @param  gyro_m:  陀螺仪角速度
  * @retval 无
  */
int16_t Yijielvbo(float accel_m, float gyro_m)
{
	static int16_t Angle1;
	static float Angle_one,Angle_L1;
	
	Angle_L1 = Angle_one;
	Angle_one = K1*accel_m + (1-K1)*(Angle_L1 + gyro_m*dt);
	Angle1 = Angle_one*100;  /* 放大100倍 */
	
//	static float Ox1, Ox2, Oy1, Angle_one, Angle_L1;
//	Angle_L1 = Angle_one;
//	Ox1 = (accel_m-Angle_L1) * (K2*K2);  /* Kp */
//	Oy1 = Oy1 + (Ox1*dt);  /* (Kp*t)/Ti */
//	Ox2 = Oy1 + (2*K2) * (accel_m-Angle_L1) + gyro_m; /* (Kp*Td)/t */
//	Angle_one = Angle_L1 + Ox2*dt;
//	Angle1 = Angle_one*100;  /* 放大100倍 */
	
//Angle = Angle_one;
	return Angle1;
}

/**
  * @brief  二阶互补滤波(积分控制)
  * @param  angle_m: 加速度合成角
  * @param  gyro_m:  陀螺仪角速度
  * @retval 无
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
	Angle2 = Angle_two*100;  /* 放大100倍 */
//Angle = Angle_two;
	return Angle2;
}

/*********************************************END OF FILE**********************/

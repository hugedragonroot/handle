/**
  ******************************************************************************
  * @file    app_kalman.c
  * @author  Rhui
  * @version V1.0
  * @date    2022-xx-xx
  * @brief   Kalman
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

#include "app_kalman.h"
#include "math.h"


float Gyro_x;		 //X轴陀螺仪数据暂存
float Gyro_y;    //Y轴陀螺仪数据暂存
float Gyro_z;		 //Z轴陀螺仪数据暂存

float Angle_X_Final; //X最终倾斜角度
float Angle_Y_Final; //Y最终倾斜角度
float Angle_Z_Final; //Z最终倾斜角度

//卡尔曼参数
char  C_0 = 1;
float Q_bias_x, Q_bias_y, Q_bias_z;
float Angle_err_x, Angle_err_y, Angle_err_z;
float PCt_0, PCt_1, E;
float K_0, K_1, t_0, t_1;
float Pdot[4] = { 0,0,0,0 };
float PP[2][2] = { { 1, 0 },{ 0, 1 } };

int16_t Kalman_Filter_X(float Accel, float Gyro) //卡尔曼函数
{
	static int16_t AngleOut_X;
  
	Angle_X_Final += (Gyro - Q_bias_x) * dt; //先验估计

	Pdot[0] = Q_angle - PP[0][1] - PP[1][0]; // Pk-先验估计误差协方差的微分

	Pdot[1] = -PP[1][1];
	Pdot[2] = -PP[1][1];
	Pdot[3] = Q_gyro;

	PP[0][0] += Pdot[0] * dt;   // Pk-先验估计误差协方差微分的积分
	PP[0][1] += Pdot[1] * dt;   // =先验估计误差协方差
	PP[1][0] += Pdot[2] * dt;
	PP[1][1] += Pdot[3] * dt;

	Angle_err_x = Accel - Angle_X_Final;	//zk-先验估计

	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];

	E = R_angle + C_0 * PCt_0;

	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;

	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];

	PP[0][0] -= K_0 * t_0;		 //后验估计误差协方差
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;

	Angle_X_Final += K_0 * Angle_err_x;	 //后验估计
	Q_bias_x += K_1 * Angle_err_x;	 //后验估计
	Gyro_x = Gyro - Q_bias_x;	 //输出值(后验估计)的微分=角速度
  
  AngleOut_X = Angle_X_Final * 100;
  
	return AngleOut_X;
}

int16_t Kalman_Filter_Y(float Accel, float Gyro) //卡尔曼函数		
{
	static int16_t AngleOut_Y;
  
	Angle_Y_Final += (Gyro - Q_bias_y) * dt; //先验估计

	Pdot[0] = Q_angle - PP[0][1] - PP[1][0]; // Pk-先验估计误差协方差的微分

	Pdot[1] = -PP[1][1];
	Pdot[2] = -PP[1][1];
	Pdot[3] = Q_gyro;

	PP[0][0] += Pdot[0] * dt;   // Pk-先验估计误差协方差微分的积分
	PP[0][1] += Pdot[1] * dt;   // =先验估计误差协方差
	PP[1][0] += Pdot[2] * dt;
	PP[1][1] += Pdot[3] * dt;

	Angle_err_y = Accel - Angle_Y_Final;	//zk-先验估计

	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];

	E = R_angle + C_0 * PCt_0;

	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;

	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];

	PP[0][0] -= K_0 * t_0;		 //后验估计误差协方差
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;

	Angle_Y_Final += K_0 * Angle_err_y;	 //后验估计
	Q_bias_y += K_1 * Angle_err_y;	 //后验估计
	Gyro_y = Gyro - Q_bias_y;	 //输出值(后验估计)的微分=角速度
  
  AngleOut_Y = Angle_Y_Final * 100;
  
	return AngleOut_Y;
}

int16_t Kalman_Filter_Z(float Accel, float Gyro) //卡尔曼函数
{
	static int16_t AngleOut_Z;
  
	Angle_Z_Final += (Gyro - Q_bias_z) * dt; //先验估计

	Pdot[0] = Q_angle - PP[0][1] - PP[1][0]; // Pk-先验估计误差协方差的微分

	Pdot[1] = -PP[1][1];
	Pdot[2] = -PP[1][1];
	Pdot[3] = Q_gyro;

	PP[0][0] += Pdot[0] * dt;   // Pk-先验估计误差协方差微分的积分
	PP[0][1] += Pdot[1] * dt;   // =先验估计误差协方差
	PP[1][0] += Pdot[2] * dt;
	PP[1][1] += Pdot[3] * dt;

	Angle_err_z = Accel - Angle_Z_Final;	//zk-先验估计

	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];

	E = R_angle + C_0 * PCt_0;

	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;

	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];

	PP[0][0] -= K_0 * t_0;		 //后验估计误差协方差
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;

	Angle_Z_Final += K_0 * Angle_err_z;	 //后验估计
	Q_bias_z += K_1 * Angle_err_z;	 //后验估计
	Gyro_z = Gyro - Q_bias_z;	 //输出值(后验估计)的微分=角速度
  
  AngleOut_Z = Angle_Z_Final * 100;
  
	return AngleOut_Z;
}

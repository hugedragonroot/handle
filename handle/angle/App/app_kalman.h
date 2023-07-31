#ifndef __APP_KALMAN_H_
#define __APP_KALMAN_H_

#include <stdint.h>


//#define Pi 	3.1415926
//��������������
#define dt           0.01
#define R_angle      0.5
#define Q_angle      0.001
#define Q_gyro       0.003//Խ��Խ�ͺ�


//extern short aacx, aacy, aacz;		//���ٶȴ�����ԭʼ����
//extern short gyrox, gyroy, gyroz;	//������ԭʼ����

//double KalmanFilter(const double ResrcData, double ProcessNiose_Q, double MeasureNoise_R);
//void Angle_Calcu(void);
int16_t Kalman_Filter_X(float Accel,float Gyro);
int16_t Kalman_Filter_Y(float Accel,float Gyro);
int16_t Kalman_Filter_Z(float Accel, float Gyro);

#endif  /* __APP_KALMAN_H_ */

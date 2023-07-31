#ifndef __APP_ANGLE_H
#define __APP_ANGLE_H

#include <stdint.h>



#define ChipTempData    QMI8658C_Temp
#define ChipAccData     QMI8658CAcc
#define ChipGyroData    QMI8658CGyro
//#define ChipMagData     QMC5883LMag

#define ChipReadData    QMI8658C_ReadData()
#define ChipDelay100ms  TIMdelay_10Ms(10)

/* 卡尔曼噪声参数 */
#define dt              0.01
#define R_angle         0.5
#define Q_angle         0.001
#define Q_gyro          0.003   /* 越大越滞后 */


/* 角度滤波参数 */
typedef struct
{
  float akp;
  float adt;
  float state[4];
} TAngle;

extern TAngle   AngleP;
extern TAngle   AngleR;
extern TAngle   AngleY;
extern uint8_t  ChipCorrect;
extern int16_t  ChipTemp;
extern int16_t  ChipAccBias[3], ChipGyroBias[3], ChipGyroMag[3];
extern int16_t  ChipAccC[3], ChipGyroC[3], ChipMagC[3];
extern float    ChipAccf[3], ChipGyrof[3], ChipMagf[3];
extern float pitch_mpu;
extern float roll_mpu;
void AngleParam_Init(void);
uint8_t AngleData_Calibrate(uint8_t num);
void AngleData_Convert(void);
int16_t SecondOrderFiltering(TAngle* Anglex, float accel_m, float gyro_m);
float Yijielvbo(float accel_m, float gyro_m);
#endif  /* __APP_ANGLE_H */

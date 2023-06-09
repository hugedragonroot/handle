/**********************************************************/
//@Autor:Raw
//@Version:0.0.1
//@End Time:
/**********************************************************/
#ifndef __HALL_ENCODER_H__
#define __HALL_ENCODER_H__

#include "common.h"

#define my_sinf_1_3 0.8660254038f
#define my_cosf_1_3 0.5f
#define my_rad	57.29579f
#define my_pairs 0.25f
#define my_half_angle	180.0f
#define my_unit_angle	360.0f
#define my_rpm	60.0f
#define my_para_speed my_rpm/my_unit_angle
typedef struct
{
	int16_t HALL_U_VALUE;
	int16_t HALL_V_VALUE;
	int16_t HALL_W_VALUE;
}__attribute__((packed)) three_hall_t;
typedef struct
{
	three_hall_t RANGE_Amplitude;
	three_hall_t Median;
	float Theta_Beta;
	float Theta_Alpha;
	float Theta_Calib_Bias;
	float Theta_Compose;
	float Theta;
	float ElecAngle;
	float real_speed;
	float filter_speed;
	float last_Theta;
	float Theta_Interval;
	float PLL_realspeed;
	int16_t return_speed;
	int32_t turns;
	float AngleReal;
	float lastAngle;
	bool first_speed_flage;
	uint8_t speed_count;
}__attribute__((packed))hall_encoder_t;
extern hall_encoder_t hall_encoder[2];

void HALL_Encoder_init(void);
void Theta_Read(PMSM_Num num);
#endif

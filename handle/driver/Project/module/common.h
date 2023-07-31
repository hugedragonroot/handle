#ifndef COMMON_H_
#define COMMON_H_
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#define DEG2RAD (PI / 180.0f)
#define RAD2DEG (180.0f / PI)
#define PI_2  6.2831852f    //2pi
#define TRUE  true
#define FALSE false
//io
typedef enum
{
	pin_none = 0x0000,

	pin0     = 0x0001,
	pin1     = 0x0002,
	pin2     = 0x0004,
	pin3     = 0x0008,

	pin4     = 0x0010,
	pin5     = 0x0020,
	pin6     = 0x0040,
	pin7     = 0x0080,

	pin8     = 0x0100,
	pin9     = 0x0200,
	pin10    = 0x0400,
	pin11    = 0x0800,

	pin12    = 0x1000,
	pin13    = 0x2000,
	pin14    = 0x4000,
	pin15    = 0x8000,

	pin_all  = 0xffff
}pin1_t;

typedef struct
{
	uint32_t timer_1ms;
	uint32_t timer_2ms;
	uint32_t timer_5ms;
	uint32_t timer_10ms;
	uint32_t timer_100ms;
}task_time_t;
extern task_time_t task_time;
extern volatile  uint32_t system_times ;
typedef enum
{
	port_none = 0,
	port_a = 1,
	port_b = 2,
	port_c = 3,
	port_d = 4
}port_t;
typedef struct
{
	port_t port;
	pin1_t pins;
}pin_t;





//timer
typedef enum
{
	timing_mode_none = 0x00,
	timing_mode_once = 0x01,
	timing_mode_continuous = 0x02
}timing_mode_t;
typedef enum
{
	timer1 = 0,
	timer2 = 1,
	timer3 = 2,
	timer4 = 3,
	timer5 = 4,
	timer6 = 5,
	timer7 = 6,
	timer8 = 7,
}timers_t;
typedef enum
{
	afio_node =0x00,
	afio_part =0x01,
	afio_all =0x02,
}timer_afio_mode;
typedef enum
{
	channel_1  = 0x00000001,
	channel_2  = 0x00000002,
	channel_3  = 0x00000004,
	channel_4  = 0x00000008,

	channel_5  = 0x00000010,
	channel_6  = 0x00000020,
	channel_7  = 0x00000040,
	channel_8  = 0x00000080,

	channel_9  = 0x00000100,
	channel_10 = 0x00000200,
	channel_11 = 0x00000400,
	channel_12 = 0x00000800,

	channel_13 = 0x00001000,
	channel_14 = 0x00002000,
	channel_15 = 0x00004000,
	channel_16 = 0x00008000,

	channel_17 = 0x00010000,
	channel_18 = 0x00020000,
	channel_19 = 0x00040000,
	channel_20 = 0x00080000,

	channel_29 = 0x00100000,
	channel_30 = 0x00200000,
	channel_31 = 0x00400000,
	channel_32 = 0x00800000,
}channel_t;
typedef enum
{
	hight = 0x00,
	low = 0x01,
}measure_mode_t;
typedef enum
{
	e_speed_mode = 0x00,
	position_mode = 0x01,
}encoder_mode_t;

//gpio
typedef struct
{
    port_t port;
    pin1_t pin;
} gpio_pin_t;
typedef enum
{
    port_mode_none = 0x00,

    pull_down = 0x01,    //
    pull_up = 0x02,      //
    floating = 0x04,

    open_drain = 0x08,
    push_pull = 0x10,

	af_open_drain = 0x20,
	af_push_pull = 0x40,
	analog = 0x80,
} gpio_mode_t;

typedef enum
{
    adc1,
	adc2,
	adc3,
} adcx_t;

typedef enum
{
    gpio_rising = 0x01,
    gpio_falling = 0x02,
    gpio_bothedge = 0x04,
} gpio_event_type_t;

typedef enum
{
    gpio_8bit_value = 1,
    gpio_16bit_value = 2,
    gpio_32bit_value = 3,
} gpio_value_type_t;


typedef struct
{
    gpio_pin_t *gpio_mapping;
    gpio_value_type_t value_type;
} gpio_value_def_t;


typedef struct
{
	gpio_pin_t pin;//io脚
	uint32_t circle;//采样周期
	uint8_t value_count; //滤波次数
	uint16_t average; //平均值
	uint16_t *values;//数组指针
}adc_t;//adc的结构体


//led
typedef enum
{
    led_pow_red     = 0x0001,
    led_pow_green   = 0x0002,
    led_pow_yellow  = 0x0003,
    led_gps_red     = 0x0010,
    led_gps_green   = 0x0020,
    led_gps_yellow  = 0x0030,
    led_rf_red      = 0x0100,
    led_rf_green    = 0x0200,
    led_rf_yellow   = 0x0300,
    led_net_red     = 0x1000,
    led_net_green   = 0x2000,
    led_net_yellow  = 0x3000,
    led_all_red     = 0x1111,
    led_all_green   = 0x2222,
    led_all         = 0x3333
} led_hw_t;

//usart
typedef enum
{
//	serial_none=0,
	serial0,
	serial1,
	serial2,
	serial3,
	serial_max
}serialx_t;
typedef enum
{
	p_none = 0,
	p_even = 1,
	p_odd = 2,
}serial_mode;
typedef struct
{
	uint8_t *buffer;
	uint32_t size;
	uint16_t front;
	uint16_t rail;
	bool overwritable;//可重写
	bool overflow;
	bool full; //uint8_t  满
	bool empty; //uint8_t 空
}queue_t;
//freertos
typedef enum
{
	mutex,//互斥信号量
	counting_semaphore,//计数信号量
	recursive_semaphore,//递归互斥信号量
	binary_semaphore,//二值信号量
}semaphore_mode_t;

typedef struct{
	float ptich;
	float roll;
}rece_angle;

typedef  struct
{
	uint16_t u16_NewCount;					//本次计数，计数值
	uint32_t u32_SumCount;					//总的计数，包括溢出值。用于计算速度
	uint16_t u16_TimerOverflowCnt;			//定时器溢出次数
	uint32_t TimeUs;						//时间
	int16_t i16_RealSpeed;					//速度
	
	int32_t  PosBase;						//基位置，0点位置
	int32_t  PosCount;
	int32_t  PosWRIFCount;					//位置溢出次数  单次	65536
	int32_t  PosCountPHA;					//PHA 转动的圈数
	
}TimerInputCaptureSpeed_t;

typedef enum
{
	Brush_A = 0,//底盘
	Brush_U = 1//座椅
}Brush_Num;

typedef enum
{
	PMSM_A = 0,//左
	PMSM_U = 1//右
}PMSM_Num;

typedef struct
{
	uint16_t Current_A_B_C[3];
}adc_data;

typedef union
{
  struct
  {
		     
		uint8_t PMSM_A_ERROR:1;             //左轮毂报错
		uint8_t PMSM_U_ERROR:1;             //右轮毂报错
		uint8_t Brush_A_ERROR:1;          	//后推杆报错 
		uint8_t Brush_U_ERROR:1;						//前推杆报错		
		
		uint8_t BREAK_ERROR:1; 							//抱闸异常
  }bit;
  uint8_t all;
}APP_Error_t;

typedef struct
{
	int16_t angle;
	uint8_t Gear;
	float x_new;
	float y_new;

	float SET_COORANGLE_10X;
	float CoordSqrt;
	float CoordSpeed;

	uint8_t	axis_x;
	uint8_t axis_y;
	APP_Error_t APP_ERROR;
}APP_PMSM_DRIVER;

typedef struct
{
//	uint8_t ErrorTime;
//	uint8_t ErrorStatus;
//	uint8_t LimitStatus;
	float Roll;
	float Pitch;
	float bais_pitch;
}tQmi_type;
extern tQmi_type tQmi;
#endif /* COMMON_H_ */

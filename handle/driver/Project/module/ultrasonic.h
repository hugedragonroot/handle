/**********************************************************/
//@Autor:Raw
//@Version:0.0.1
//@End Time:
/**********************************************************/
#ifndef __ULTRASONIC_H__
#define __ULTRASONIC_H__
#include "common.h"

typedef enum
{
	RECE_IDLE   	 	= 0,
	RECE_DATA  	 	= 1,
	RECE_FINISH		= 2,
	DATA_DEAL		= 3
}MB_RX_STATUE;

typedef enum
{
	DEAL_IDLE		= 0,
	DEAL_ING  	 	= 1,
	DEAL_FINISH		= 2,
	DEAL_SEND		= 3,
	DEAL_LIQUID_LEVEL = 4,
}MB_DEAL_STATUE;
#define Max_Buffer 64
typedef struct
{
	uint8_t buffer[Max_Buffer];
	uint16_t sum;
	uint8_t len_in;
	uint8_t len_out;
	uint8_t data_len;
	MB_RX_STATUE mb_statue;
	MB_DEAL_STATUE deal_statue;
	bool BROADCAST_STATUE;
	uint8_t max_len;
}ultrasonic_pro_t;
extern ultrasonic_pro_t ultrasonic;
void modbus_recve(void);

#endif



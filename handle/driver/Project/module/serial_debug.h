#ifndef __SERIAL_DEBUG_H__
#define __SERIAL_DEBUG_H__


#include "common.h"
typedef struct 
{
	float data1;
	float data2;
	float data3;
	float data4;

	float data5;
	float data6;
	float data7;
	float data8;

	float data9;
	float data10;
	float data11;
	float data12;

	float data13;
	float data14;
	float data15;
	float data16;
}DebugData_t;


void serial_loop(void);
#endif

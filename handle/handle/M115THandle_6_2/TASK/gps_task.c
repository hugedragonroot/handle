#include <stdbool.h>
#include "gps_task.h"
/*FreeRtos includes*/
#include "FreeRTOS.h"
#include "task.h"
#include "key_ext.h"
#include "gps_ext.h"
#include <stdio.h>
void gpsTask(void* param)
{
	static uint16_t i;
	while(1)
	{
		vTaskDelay(1000);
		i++;

//		if(i==3)
//		{
//			i=0;
//			dtu_config_init(DTU_WORKMODE_GPS);
//		}		
//		else 
//			if(i==6)
//		{
//			i=0;
//			dtu_config_init(DTU_WORKMODE_ALIYUN);
//		}
			
	}
}

void GPSNoneOSTask(void){
	static u8 flag = DTU_WORKMODE_GPS;
    if(dtu_work_mode(flag) == DTU_SUCCEED){
		flag ^= DTU_WORKMODE_GPS^DTU_WORKMODE_XSTOYUN;
    }
	
}

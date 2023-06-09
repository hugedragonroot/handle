#ifndef __GPS_EXT_H
#define __GPS_EXT_H
#include "stdint.h"

#define DTU_RX_CMD_BUF_SIZE (512)

#define  PRODUCTKEY		"if31LbK8Cqi"
#define  DEVICENAME		"M130Ttest"
#define  DEVICESECRET	"88f59404404747beea850e4684161cb8"

typedef enum
{
    DTU_WORKMODE_ALIYUN,   /*������͸��ģʽ*/
    DTU_WORKMODE_ONENET,   /*OneNET͸��ģʽ*/
		DTU_WORKMODE_GPS, 
} _dtu_work_mode_eu;

typedef struct
{
    uint32_t timeout; /*ָ��ȴ���ʱʱ�䣬��λ��100ms*/
    char *set_cmd;  // ���ò���ָ��
    char *read_cmd;  //���ز���ָ��
} _dtu_atcmd_st;


typedef struct
{
	uint8_t lbs_vail_flag; //�Ƿ���Ч
	uint8_t lbs_type;				//���ͣ�����/��վ
	char lbs_longitude[16];	//����
	char lbs_latitude[16];	//γ��
}gps_lbs_st;

typedef struct
{
	char dev_id[16];	
	gps_lbs_st gps;
	uint8_t dev_type;
	uint8_t dev_M130Tmode;
	uint8_t dev_M115mode;
	uint8_t dev_lock;
	uint8_t dev_lampsw;
	uint8_t dev_voicesw;
	uint8_t dev_alarmsw;
	uint8_t dev_cruisesw;
	uint8_t dev_speed;
	int8_t dev_seat_angle;
	int8_t dev_body_angle;
	uint8_t dev_vol;
	uint8_t dev_battery;
	uint32_t dev_total_mil;
	uint8_t res_1;
	uint32_t res_2;
	uint8_t res_3[16];	
	uint8_t dev_version[16];	
}gps_data_st;

extern gps_data_st gps_data;

void gps_init(void);
int dtu_config_init(_dtu_work_mode_eu work_mode);
#endif

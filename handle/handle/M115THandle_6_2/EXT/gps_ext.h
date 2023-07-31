#ifndef __GPS_EXT_H
#define __GPS_EXT_H
#include "stdint.h"

#if 0
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
#else

#define DTU_RX_CMD_BUF_SIZE (128)
#define DTU_TX_CMD_BUF_SIZE (512)
#define  PRODUCTKEY		"if31LbK8Cqi"
#define  DEVICENAME		"M130T"
#define  DEVICESECRET	"88f59404404747beea850e4684161cb8"

typedef enum
{
    DTU_WORKMODE_ALIYUN,   /*������͸��ģʽ*/
    DTU_WORKMODE_ONENET,   /*OneNET͸��ģʽ*/
	DTU_WORKMODE_GPS, 
	DTU_WORKMODE_XSTOYUN,
} _dtu_work_mode_eu;

typedef enum{
	DTU_FAILE,
	DTU_SUCCEED = !DTU_FAILE,
}GPSDtuStatus;

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
	char* lbs_longitude;	//����
	char* lbs_latitude;	//γ��
}gps_lbs_st;

typedef struct
{
	char* dev_id;	
	gps_lbs_st gps;
	uint8_t dev_type;			//�豸����0-M130T 1-M115
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
	char* res_3;	
	char* dev_version;	
}gps_data_st;

extern gps_data_st gps_data;

void gps_init(void);
void gps_data_update(void);
// int dtu_work_mode(_dtu_work_mode_eu work_mode);

uint8_t dtu_work_mode(_dtu_work_mode_eu work_mode);
#endif

#endif

#ifndef __APP_PROTOCOL_H
#define __APP_PROTOCOL_H

#include <stdint.h>

#include "../PROTOCOL/protocol_427/protocol.h"

/*********************** 主机<>控制器 ***********************/
#define FrameHeadUpH          0xA5
#define FrameHeadUpL          0x5E
#define Frame_ShortLen        8       //最短帧长度
#define Frame_InforLen        6       //除数据外固定帧信息长度
#define Frame_OverTime        3      //帧接收超时时间 100ms

/************************** 命令字 **************************/
#define CMD_Idle 			        0x00    //默认
#define CMD_Handshake         0x01    //握手协议
#define CMD_GetConfig         0x02    //获取驱动器配置参数
#define CMD_GetParams         0x03    //获取实时值
#define CMD_GetChart          0x04    //获取实时曲线值
#define CMD_SetConfig         0x05    //设置参数
#define CMD_SetCtrl           0x06    //控制命令
#define CMD_GetCtrl           0x07    //控制命令
#define CMD_FaultEvent        0xff    //故障信息

/************************* 执行状态 *************************/
#define ExecState_Suc         0x01    //成功
#define ExecState_ErrParam    0x02    //参数范围错误
#define ExecState_ErrSave     0x03    //保存失败

/************************* 默认参数 *************************/

#define PARAMS_ChartLen       10      //单次采集曲线长度

#define ANGLE_BODY_N5			-500
#define ANGLE_BODY_N2P5		-250
#define ANGLE_BODY_N2			-200
#define ANGLE_BODY_N1			-100
#define ANGLE_BODY_P5			50
#define ANGLE_BODY_1			100
#define ANGLE_BODY_2			200
#define ANGLE_BODY_2P5		250
#define ANGLE_BODY_5			500
#define ANGLE_BODY_30			3000
#define ANGLE_BODY_40			4000

#define ANGLE_SEAT_NP5		-50
#define ANGLE_SEAT_P5			50
#define ANGLE_SEAT_1			150

#define ANGLE_REMOTE_N3			-30
#define ANGLE_REMOTE_N45			-450
#define ANGLE_REMOTE_N87			-870
#define ANGLE_REMOTE_N93			-930
#define ANGLE_REMOTE_N135			-1350
#define ANGLE_REMOTE_N177			-1770
#define ANGLE_REMOTE_3			30
#define ANGLE_REMOTE_42			420
#define ANGLE_REMOTE_45			450
#define ANGLE_REMOTE_87			870
#define ANGLE_REMOTE_90			900
#define ANGLE_REMOTE_93			930
#define ANGLE_REMOTE_135			1350
#define ANGLE_REMOTE_177			1770
#define ANGLE_REMOTE_180			1800

#define ELONGATION_0			40 //伸长量0
#define ELONGATION_MID			1500 //伸长量
#define ELONGATION_ERR			40 //伸长量0

/* 通用 */
typedef struct
{
  uint8_t   DataLen;
  uint8_t   Count;
  uint8_t   Cmd;
  uint8_t   Data[100];
} TCOMMUN;

/* 网络通讯数据协议 */
typedef union
{
  TCOMMUN   Commun;
  uint8_t   Data[128];
} TcomData;

extern int16_t walking_speed[5];

void Remote_buff_init(void);

void Set_Driver_default(void);
void Set_Driver_power( uint8_t power);
void Set_Driver_horn( uint8_t horn);

void ProtocolTransbuff(TCOMMUN* p);
void app_analysis(void);
void app_cltprocess(void);


void CANProtocolTransbuff(combine_write_t* combine_wirte,uint8_t *txbuffer);

#endif

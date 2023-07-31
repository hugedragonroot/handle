#ifndef __APP_PROTOCOL_H
#define __APP_PROTOCOL_H

#include "app_loopqueue.h"
#include <stdint.h>
#include <stdbool.h>


/*********************** 主机<>控制器 ***********************/
#define FrameHeadUpH          0xA5
#define FrameHeadUpL          0x5E
#define Frame_ShortLen        8       //最短帧长度
#define Frame_InforLen        6       //除数据外固定帧信息长度
#define Frame_OverTime        50      //帧接收超时时间 10ms

/************************** 命令字 **************************/
#define CMD_Handshake         0x01    //握手协议
#define CMD_GetConfig         0x02    //获取驱动器配置参数
#define CMD_GetParams         0x03    //获取实时值
#define CMD_GetChart          0x04    //获取实时曲线值
#define CMD_SetConfig         0x05    //设置参数
#define CMD_SetCtrl           0x06    //控制命令

#define CMD_GetAngle          0x11    //获取角度数据
#define CMD_GetAngleRaw       0x12    //获取原始数据
#define CMD_GetAngleConfig    0x13    //获取角度模块配置参数
#define CMD_SetAngleConfig    0x14    //设置角度模块参数

#define CMD_FaultEvent        0xff    //故障信息

/************************* 执行状态 *************************/
#define ExecState_Suc         0x01    //成功
#define ExecState_ErrParam    0x02    //参数范围错误
#define ExecState_ErrSave     0x03    //保存失败

/************************* 默认参数 *************************/
#define PARAMS_DriverModel    0x00    //驱动器型号
#define PARAMS_SoftwareVer    10      //软件版本 	1.0
#define PARAMS_ChartLen       10      //单次采集曲线长度

/************************* 摇杆控制 *************************/
#define Coord_MAX             255     //坐标最大值
#define Coord_MIN             0       //坐标最小值
#define Coord_Base            127     //坐标基线
#define Coord_Start           (Coord_MAX+Coord_MIN+1)/32

/* 通用 */
typedef struct
{
  uint8_t   HeadH;
  uint8_t   HeadL;
  uint8_t   DataLen;
  uint8_t   Count;
  uint8_t   Cmd;
  uint8_t   Data[100];
  uint16_t  SourceAddr;
} TCOMMUN;

/* 网络通讯数据协议 */
typedef union
{
  TCOMMUN   Commun;
  uint8_t   Data[128];
} TcomData;

/* 遥控通信 */
typedef struct remote_com
{
  bool      RemoteConnect;    /* 遥控连接标志 */
  uint8_t   RemoteCount;      /* 遥控连接计时 */
  uint8_t   Transmit;         /* 消息转发标志 */
  
  uint8_t   CoordXH;          /* 摇杆 X 坐标高8位 */
  uint8_t   CoordXL;          /* 摇杆 X 坐标低8位 */
  int16_t   CoordX;           /* 摇杆 X 坐标 */
  
  uint8_t   CoordYH;          /* 摇杆 Y 坐标高8位 */
  uint8_t   CoordYL;          /* 摇杆 Y 坐标低8位 */
  int16_t   CoordY;           /* 摇杆 Y 坐标 */
  
  int16_t   CoordSqrt;        /* 坐标平方根 */
  int16_t   CoordAngle;       /* 坐标角 */
  
  uint8_t   ControlMode;      /* 控制模式 */
  uint8_t   ControlPre;
  uint8_t   OperationMode;    /* 运行方式 */
  uint8_t   OperationPre;
  uint8_t   SpeedGear;        /* 速度挡位 */
  uint8_t   SpeedPre;
  uint8_t   Peripherals;      /* 外设状态 */
  uint8_t   PeripheryPre;
  uint8_t   Reserved;         /* 保留位 */
  
  uint8_t   Battery;          /* 电池电量 */
  int16_t   aPitch;           /* 俯仰角 */
  int16_t   aRoll;            /* 滚转角 */
  int16_t   aYaw;             /* 偏航角 */
} TREMOTE_COM;

extern TREMOTE_COM   Remote;
extern TREMOTE_COM   Slave;

uint8_t ProtocolCheckSum(uint8_t *pbuf, uint16_t len);
void ProtocolFaultEvent(uint8_t state);
void ProtocolSlaveConnect(void);
void ProtocolSlaveCtrl(void);
void ProtocolIdentify(TLoopBuf* pLoopBuffer);

#endif  /* __APP_PROTOCOL_H */

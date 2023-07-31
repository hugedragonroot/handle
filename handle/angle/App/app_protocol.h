#ifndef __APP_PROTOCOL_H
#define __APP_PROTOCOL_H

#include "app_loopqueue.h"
#include <stdint.h>
#include <stdbool.h>


/*********************** ����<>������ ***********************/
#define FrameHeadUpH          0xA5
#define FrameHeadUpL          0x5E
#define Frame_ShortLen        8       //���֡����
#define Frame_InforLen        6       //��������̶�֡��Ϣ����
#define Frame_OverTime        50      //֡���ճ�ʱʱ�� 10ms

/************************** ������ **************************/
#define CMD_Handshake         0x01    //����Э��
#define CMD_GetConfig         0x02    //��ȡ���������ò���
#define CMD_GetParams         0x03    //��ȡʵʱֵ
#define CMD_GetChart          0x04    //��ȡʵʱ����ֵ
#define CMD_SetConfig         0x05    //���ò���
#define CMD_SetCtrl           0x06    //��������

#define CMD_GetAngle          0x11    //��ȡ�Ƕ�����
#define CMD_GetAngleRaw       0x12    //��ȡԭʼ����
#define CMD_GetAngleConfig    0x13    //��ȡ�Ƕ�ģ�����ò���
#define CMD_SetAngleConfig    0x14    //���ýǶ�ģ�����

#define CMD_FaultEvent        0xff    //������Ϣ

/************************* ִ��״̬ *************************/
#define ExecState_Suc         0x01    //�ɹ�
#define ExecState_ErrParam    0x02    //������Χ����
#define ExecState_ErrSave     0x03    //����ʧ��

/************************* Ĭ�ϲ��� *************************/
#define PARAMS_DriverModel    0x00    //�������ͺ�
#define PARAMS_SoftwareVer    10      //����汾 	1.0
#define PARAMS_ChartLen       10      //���βɼ����߳���

/************************* ҡ�˿��� *************************/
#define Coord_MAX             255     //�������ֵ
#define Coord_MIN             0       //������Сֵ
#define Coord_Base            127     //�������
#define Coord_Start           (Coord_MAX+Coord_MIN+1)/32

/* ͨ�� */
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

/* ����ͨѶ����Э�� */
typedef union
{
  TCOMMUN   Commun;
  uint8_t   Data[128];
} TcomData;

/* ң��ͨ�� */
typedef struct remote_com
{
  bool      RemoteConnect;    /* ң�����ӱ�־ */
  uint8_t   RemoteCount;      /* ң�����Ӽ�ʱ */
  uint8_t   Transmit;         /* ��Ϣת����־ */
  
  uint8_t   CoordXH;          /* ҡ�� X �����8λ */
  uint8_t   CoordXL;          /* ҡ�� X �����8λ */
  int16_t   CoordX;           /* ҡ�� X ���� */
  
  uint8_t   CoordYH;          /* ҡ�� Y �����8λ */
  uint8_t   CoordYL;          /* ҡ�� Y �����8λ */
  int16_t   CoordY;           /* ҡ�� Y ���� */
  
  int16_t   CoordSqrt;        /* ����ƽ���� */
  int16_t   CoordAngle;       /* ����� */
  
  uint8_t   ControlMode;      /* ����ģʽ */
  uint8_t   ControlPre;
  uint8_t   OperationMode;    /* ���з�ʽ */
  uint8_t   OperationPre;
  uint8_t   SpeedGear;        /* �ٶȵ�λ */
  uint8_t   SpeedPre;
  uint8_t   Peripherals;      /* ����״̬ */
  uint8_t   PeripheryPre;
  uint8_t   Reserved;         /* ����λ */
  
  uint8_t   Battery;          /* ��ص��� */
  int16_t   aPitch;           /* ������ */
  int16_t   aRoll;            /* ��ת�� */
  int16_t   aYaw;             /* ƫ���� */
} TREMOTE_COM;

extern TREMOTE_COM   Remote;
extern TREMOTE_COM   Slave;

uint8_t ProtocolCheckSum(uint8_t *pbuf, uint16_t len);
void ProtocolFaultEvent(uint8_t state);
void ProtocolSlaveConnect(void);
void ProtocolSlaveCtrl(void);
void ProtocolIdentify(TLoopBuf* pLoopBuffer);

#endif  /* __APP_PROTOCOL_H */

#ifndef __CAN_H
#define	__CAN_H


#include "stm32f10x.h"
#include <stdbool.h>

#define CAN_PinRemap    0   /* 是否使用引脚重映射 */

#if CAN_PinRemap
#define CAN_TX_GPIO_CLK         RCC_APB2Periph_GPIOB
#define CAN_TX_GPIO_PORT        GPIOB
#define CAN_TX_PIN              GPIO_Pin_9
#define CAN_RX_GPIO_CLK         RCC_APB2Periph_GPIOB
#define CAN_RX_GPIO_PORT        GPIOB
#define CAN_RX_PIN              GPIO_Pin_8
#else
#define CAN_TX_GPIO_CLK         RCC_APB2Periph_GPIOA
#define CAN_TX_GPIO_PORT        GPIOA
#define CAN_TX_PIN              GPIO_Pin_12
#define CAN_RX_GPIO_CLK         RCC_APB2Periph_GPIOA
#define CAN_RX_GPIO_PORT        GPIOA
#define CAN_RX_PIN              GPIO_Pin_11
#endif

/**
  * 标准格式的ID有11个位:从ID28到ID18被依次发送,禁止高7位都为隐性(显性电平为"0",隐性电平为"1")
  * 扩展格式的ID有29个位:基本ID从ID28到ID18,扩展ID由ID17到ID0表示,基本ID和标准格式的ID相同
  * 禁止设定:基本ID = 1111111xxxx
  * 基本ID最大为0x7EF(2031),扩展ID最大为0x3FFFF(262343)
  */
#define TxStdId         0xCF
#define TxExtId         0xA1    /* 本机ID */

#define ScreenExtId0    ((((0xAF<<18) | 0x3FFFF) << 3) | CAN_ID_EXT | CAN_RTR_DATA)   /* 全系统广播地址 */
#define ScreenExtId1    ((((0xCF<<18) | 0x3FFFF) << 3) | CAN_ID_EXT | CAN_RTR_DATA)   /* 控制系统广播地址 */
#define ScreenExtId2    ((((0xDF<<18) | 0x3FFFF) << 3) | CAN_ID_EXT | CAN_RTR_DATA)   /* 驱动器广播地址 */
#define ScreenExtId3    ((((0xA1<<18) | 0x3FFFF) << 3) | CAN_ID_EXT | CAN_RTR_DATA)

#define MsgLen          128
/* 传输端口 */
typedef enum {
  CCOM1 = 0x0A,
  CCOM2 = 0x0B,
} CAN_COM;
/* CAN收发缓存结构体 */
typedef struct
{
  bool    TxEN;
  bool    RxEN;
  uint8_t TxCNT;
  uint8_t RxCNT;
  uint8_t TxBuf[MsgLen];
  uint8_t RxBuf[MsgLen];
} CanStreamDef;


extern CanStreamDef   CanStr1;
extern CanTxMsg       TxMsgFIFO;
extern CanRxMsg       RxMsgFMP0;
extern CanRxMsg       RxMsgFMP1;


void CAN_Config(void);
void Init_RxMes(CanRxMsg *RxMessage);
void CAN1_SendStdFrame(CanTxMsg* TxMessage, uint32_t CanID, uint8_t *buf, uint8_t num);
void CAN1_SendExtFrame(CanTxMsg* TxMessage, uint32_t CanID, uint8_t *buf, uint8_t num);
void CAN_ProtocolExt(CanRxMsg* RxMessage, CanStreamDef *CanStrx);

#endif /* __CAN_H */

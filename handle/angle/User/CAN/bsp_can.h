#ifndef __CAN_H
#define	__CAN_H


#include "stm32f10x.h"
#include <stdbool.h>

#define CAN_PinRemap    0   /* �Ƿ�ʹ��������ӳ�� */

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
  * ��׼��ʽ��ID��11��λ:��ID28��ID18�����η���,��ֹ��7λ��Ϊ����(���Ե�ƽΪ"0",���Ե�ƽΪ"1")
  * ��չ��ʽ��ID��29��λ:����ID��ID28��ID18,��չID��ID17��ID0��ʾ,����ID�ͱ�׼��ʽ��ID��ͬ
  * ��ֹ�趨:����ID = 1111111xxxx
  * ����ID���Ϊ0x7EF(2031),��չID���Ϊ0x3FFFF(262343)
  */
#define TxStdId         0xCF
#define TxExtId         0xA1    /* ����ID */

#define ScreenExtId0    ((((0xAF<<18) | 0x3FFFF) << 3) | CAN_ID_EXT | CAN_RTR_DATA)   /* ȫϵͳ�㲥��ַ */
#define ScreenExtId1    ((((0xCF<<18) | 0x3FFFF) << 3) | CAN_ID_EXT | CAN_RTR_DATA)   /* ����ϵͳ�㲥��ַ */
#define ScreenExtId2    ((((0xDF<<18) | 0x3FFFF) << 3) | CAN_ID_EXT | CAN_RTR_DATA)   /* �������㲥��ַ */
#define ScreenExtId3    ((((0xA1<<18) | 0x3FFFF) << 3) | CAN_ID_EXT | CAN_RTR_DATA)

#define MsgLen          128
/* ����˿� */
typedef enum {
  CCOM1 = 0x0A,
  CCOM2 = 0x0B,
} CAN_COM;
/* CAN�շ�����ṹ�� */
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

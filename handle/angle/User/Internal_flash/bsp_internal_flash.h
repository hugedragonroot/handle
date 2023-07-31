#ifndef __INTERNAL_FLASH_H
#define	__INTERNAL_FLASH_H

#include "stm32f10x.h"


/* ����ʵʵ����16λ�������ͺ͵������ݼ�2�ĵ�ַ���� */
#define FLASH_BASE_ADDR     0x08000000      /* STM32 FLASH����ʼ��ַ */
#define FLASH_SIZE          512             /* ��ѡSTM32��FLASH������С(��λΪK) */
#define FLASH_SAVE_ADDR     0x08008000      /* ����FLASH �����ַ(����Ϊż��,����ֵҪ���ڱ�������ռ��FLASH�Ĵ�С+0X08000000) */


typedef enum 
{
  FAILED = 0, 
  PASSED = !FAILED
} TestStatus;

typedef struct
{
  uint8_t   data;
  uint8_t   Wflag;
  uint16_t  Wbuf[32];
  uint16_t  Rbuf[32];
  uint16_t  i;
} inflash_StrDef;

extern inflash_StrDef InFlashMCU;

uint8_t InFlash_ReadByte(uint32_t ReadAddr, uint8_t *pBuff, uint8_t Bufflen);
uint8_t InFlash_ReadHalfWord(uint32_t ReadAddr, uint16_t *pBuff, uint8_t Bufflen);
uint8_t InFlash_WriteVerify(uint32_t WriteAddr, uint16_t *pBuff, uint8_t Bufflen);
uint8_t InFlash_WriteHalfWord(uint32_t WriteAddr, uint16_t *pBuff, uint8_t Bufflen);
void    InFlash_WriteArray(uint32_t WriteAddr, uint16_t *pBuff, uint8_t Bufflen);

#endif /* __INTERNAL_FLASH_H */

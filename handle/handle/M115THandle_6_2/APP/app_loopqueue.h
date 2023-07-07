#ifndef __APP_LOOPQUEUE_H
#define __APP_LOOPQUEUE_H

#include <stdint.h>


typedef struct  
{
  uint8_t   *nData;
  uint16_t  nLength;      /* ���ݳ��� */
  uint16_t  nReadIndex;   /* ��ָ���ַ */
  uint16_t  nWriteIndex;  /* дָ���ַ */
  uint16_t  nMaxLen;      /* ���г��� */
	uint8_t  	lock;
} TLoopBuf;




void LoopQueue_Init(TLoopBuf* pLoopBuf, uint8_t *pData, uint16_t pLen);
void LoopQueue_DeInit(TLoopBuf* pLoopBuf);
uint16_t LoopQueue_DataLen(TLoopBuf* pLoopBuf);
uint16_t LoopQueue_IdleLen(TLoopBuf* pLoopBuf);
uint8_t LoopQueue_Write(TLoopBuf* pLoopBuf, uint8_t *pData, uint16_t pLength);
void LoopQueue_ReadOnly(TLoopBuf* pLoopBuf, uint8_t *pData, uint16_t pLength);
uint8_t LoopQueue_ReadRelease(TLoopBuf* pLoopBuf, uint8_t *pData, uint16_t pLength);

#endif  /* __APP_LOOPQUEUE_H */


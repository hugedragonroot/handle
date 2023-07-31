#include "app_loopqueue.h"
#include <string.h>


TLoopBuf  loopPointCAN1;
TLoopBuf  loopPointUsart1;
TLoopBuf  loopPointUsart2;
TLoopBuf  loopPointUsart3;

/**
  * @brief  ��ʼ������
  * @param  pLoopBuf  : Ŀ��ṹ��
  * @param  pData     : ��������
  * @param  pLen      : ���г���
  * @retval None
  */
void LoopQueue_Init(TLoopBuf* pLoopBuf, uint8_t *pData, uint16_t pLen)
{
  memset(pLoopBuf, 0, sizeof(TLoopBuf));
  pLoopBuf->nData = pData;
  pLoopBuf->nMaxLen = pLen;
  pLoopBuf->nLength = 0;
  pLoopBuf->nReadIndex = 0;
  pLoopBuf->nWriteIndex = 0;
}

/**
  * @brief  ���ö���
  * @param  None
  * @retval None
  */
void LoopQueue_DeInit(TLoopBuf* pLoopBuf)
{
  pLoopBuf->nLength = 0;
  pLoopBuf->nReadIndex = 0;
  pLoopBuf->nWriteIndex = 0;
}

/**
  * @brief  ��ȡ���е����ݳ���
  * @param  None
  * @retval None
  */
uint16_t LoopQueue_DataLen(TLoopBuf* pLoopBuf)
{
  static uint16_t pLen;
  pLen = pLoopBuf->nLength;
  return pLen;
}

/**
  * @brief  ��ȡ���еĿ��г���
  * @param  None
  * @retval None
  */
uint16_t LoopQueue_IdleLen(TLoopBuf* pLoopBuf)
{
  static uint16_t pLen;
  pLen = pLoopBuf->nMaxLen - pLoopBuf->nLength;
  return pLen;
}

/**
  * @brief  ����д�����
  * @param  pLoopBuf  : Ŀ��ṹ��
  * @param  pData     : ����Դ
  * @param  pLength   : ���ݳ���
  * @retval nLen ���ݳ���  0 ʧ��
  */
void LoopQueue_Write(TLoopBuf* pLoopBuf, uint8_t *pData, uint16_t pLength)
{
  if(pLength <= (pLoopBuf->nMaxLen - pLoopBuf->nLength))  /* д�볤��ӦС�ڿ��г��� */
  {
    if((pLength + pLoopBuf->nWriteIndex) > pLoopBuf->nMaxLen) /* дָ���ƶ�������β */
    {
      memcpy(pLoopBuf->nData + pLoopBuf->nWriteIndex, pData, pLoopBuf->nMaxLen - pLoopBuf->nWriteIndex);
      pLoopBuf->nLength += pLoopBuf->nMaxLen - pLoopBuf->nWriteIndex;
      pLength -= pLoopBuf->nMaxLen - pLoopBuf->nWriteIndex;
      pData += pLoopBuf->nMaxLen - pLoopBuf->nWriteIndex;
      pLoopBuf->nWriteIndex = 0;
    }
    memcpy(pLoopBuf->nData + pLoopBuf->nWriteIndex, pData, pLength);
    pLoopBuf->nWriteIndex += pLength;
    pLoopBuf->nLength += pLength;
  }
}

/**
  * @brief  �Ӷ���ֻ������
  * @param  pLoopBuf  : Ŀ��ṹ��
  * @param  pData     : ��ȡ������
  * @param  pLength   : ��ȡ����
  * @retval None
  */
void LoopQueue_ReadOnly(TLoopBuf* pLoopBuf, uint8_t *pData, uint16_t pLength)
{
  static uint16_t pReadIndex;
  
  pReadIndex = pLoopBuf->nReadIndex;
  if(pLength <= pLoopBuf->nLength)  /* ��ȡ����Ӧ�������ݳ��� */
  {
    if((pLength + pLoopBuf->nReadIndex) > pLoopBuf->nMaxLen)  /* ��ָ���ƶ�������β */
    {
      memcpy(pData, pLoopBuf->nData + pLoopBuf->nReadIndex, pLoopBuf->nMaxLen - pLoopBuf->nReadIndex);
      pLength -= pLoopBuf->nMaxLen - pLoopBuf->nReadIndex;
      pData += pLoopBuf->nMaxLen - pLoopBuf->nReadIndex;
      pReadIndex = 0;
    }
    memcpy(pData, pLoopBuf->nData + pReadIndex, pLength);
  }
}

/**
  * @brief  �Ӷ���ȡ������,�ͷſռ�
  * @param  pLoopBuf  : Ŀ��ṹ��
  * @param  pData     : ��ȡ������
  * @param  nLength   : ��ȡ����
  * @retval None
  */
void LoopQueue_ReadRelease(TLoopBuf* pLoopBuf, uint8_t *pData, uint16_t pLength)
{
  if(pLength <= pLoopBuf->nLength)
  {
    if((pLength + pLoopBuf->nReadIndex) > pLoopBuf->nMaxLen)
    {
      memcpy(pData, pLoopBuf->nData + pLoopBuf->nReadIndex, pLoopBuf->nMaxLen - pLoopBuf->nReadIndex);
      pLoopBuf->nLength -= pLoopBuf->nMaxLen - pLoopBuf->nReadIndex;
      pLength -= pLoopBuf->nMaxLen - pLoopBuf->nReadIndex;
      pData += pLoopBuf->nMaxLen - pLoopBuf->nReadIndex;
      pLoopBuf->nReadIndex = 0;
    }
    memcpy(pData, pLoopBuf->nData + pLoopBuf->nReadIndex, pLength);
    pLoopBuf->nReadIndex += pLength;
    pLoopBuf->nLength -= pLength;
  }
}

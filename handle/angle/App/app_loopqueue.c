#include "app_loopqueue.h"
#include <string.h>


TLoopBuf  loopPointCAN1;
TLoopBuf  loopPointUsart1;
TLoopBuf  loopPointUsart2;
TLoopBuf  loopPointUsart3;

/**
  * @brief  初始化队列
  * @param  pLoopBuf  : 目标结构体
  * @param  pData     : 缓存数组
  * @param  pLen      : 队列长度
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
  * @brief  重置队列
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
  * @brief  读取队列的数据长度
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
  * @brief  读取队列的空闲长度
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
  * @brief  数据写入队列
  * @param  pLoopBuf  : 目标结构体
  * @param  pData     : 数据源
  * @param  pLength   : 数据长度
  * @retval nLen 数据长度  0 失败
  */
void LoopQueue_Write(TLoopBuf* pLoopBuf, uint8_t *pData, uint16_t pLength)
{
  if(pLength <= (pLoopBuf->nMaxLen - pLoopBuf->nLength))  /* 写入长度应小于空闲长度 */
  {
    if((pLength + pLoopBuf->nWriteIndex) > pLoopBuf->nMaxLen) /* 写指针移动超过队尾 */
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
  * @brief  从队列只读数据
  * @param  pLoopBuf  : 目标结构体
  * @param  pData     : 读取缓存区
  * @param  pLength   : 读取长度
  * @retval None
  */
void LoopQueue_ReadOnly(TLoopBuf* pLoopBuf, uint8_t *pData, uint16_t pLength)
{
  static uint16_t pReadIndex;
  
  pReadIndex = pLoopBuf->nReadIndex;
  if(pLength <= pLoopBuf->nLength)  /* 读取长度应大于数据长度 */
  {
    if((pLength + pLoopBuf->nReadIndex) > pLoopBuf->nMaxLen)  /* 读指针移动超过队尾 */
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
  * @brief  从队列取出数据,释放空间
  * @param  pLoopBuf  : 目标结构体
  * @param  pData     : 读取缓存区
  * @param  nLength   : 读取长度
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

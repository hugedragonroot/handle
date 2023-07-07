#include "app_loopqueue.h"
#include <string.h>




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
	pLoopBuf->lock=0;
	
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
  uint16_t pLen;
  
//  pLen = pLoopBuf->nLength;
	if(pLoopBuf->nReadIndex > pLoopBuf->nWriteIndex)
		pLoopBuf->nLength = pLoopBuf->nWriteIndex + pLoopBuf->nMaxLen - pLoopBuf->nReadIndex;
	else
		pLoopBuf->nLength  = pLoopBuf->nWriteIndex - pLoopBuf->nReadIndex;
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
  uint16_t pLen;
  
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
uint8_t LoopQueue_Write(TLoopBuf* pLoopBuf, uint8_t *pData, uint16_t pLength)
{
  uint16_t LenBuf = 0;
	
  #if 0
  if(LoopQueue_IdleLen(pLoopBuf) < pLength) return 0;
  while (LenBuf == pLength)
  {
    pLoopBuf->nData[pLoopBuf->nWriteIndex]=*(pData +LenBuf);
    if(++pLoopBuf->nWriteIndex >= pLoopBuf->nMaxLen)
      pLoopBuf->nWriteIndex = 0;
    ++LenBuf;
  }
  #else
  
 if(pLength <= (pLoopBuf->nMaxLen - pLoopBuf->nLength))  /* 写入长度应小于空闲长度 */
 {
	pLoopBuf->lock = 1;
   if((pLength + pLoopBuf->nWriteIndex) > pLoopBuf->nMaxLen) /* 写指针移动超过队尾 */
   {
     LenBuf = pLoopBuf->nMaxLen - pLoopBuf->nWriteIndex;
     memcpy(pLoopBuf->nData + pLoopBuf->nWriteIndex, pData, LenBuf);
     pLoopBuf->nWriteIndex = 0;
     pLoopBuf->nLength += LenBuf;
     pLength -= LenBuf;
     pData += LenBuf;
   }
   memcpy(pLoopBuf->nData + pLoopBuf->nWriteIndex, pData, pLength);
   pLoopBuf->nWriteIndex += pLength;
   pLoopBuf->nLength += pLength;

	pLoopBuf->lock = 0;
  return pLength;
 }
 #endif

return 0;

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
  //uint16_t pReadIndex, LenBuf;
	
	*pData = pLoopBuf->nData[pLoopBuf->nReadIndex];
  
//  pReadIndex = pLoopBuf->nReadIndex;
//  if(pLength <= pLoopBuf->nLength)  /* 读取长度应大于数据长度 */
//  {
//    if((pLength + pLoopBuf->nReadIndex) > pLoopBuf->nMaxLen)  /* 读指针移动超过队尾 */
//    {
//      LenBuf = pLoopBuf->nMaxLen - pLoopBuf->nReadIndex;
//      memcpy(pData, pLoopBuf->nData + pLoopBuf->nReadIndex, LenBuf);
//      pReadIndex = 0;
//      pLength -= LenBuf;
//      pData += LenBuf;
//    }
//    memcpy(pData, pLoopBuf->nData + pReadIndex, pLength);
//  }
}

/**
  * @brief  从队列取出数据,释放空间
  * @param  pLoopBuf  : 目标结构体
  * @param  pData     : 读取缓存区
  * @param  nLength   : 读取长度
  * @retval None
  */
uint8_t LoopQueue_ReadRelease(TLoopBuf* pLoopBuf, uint8_t *pData, uint16_t pLength)
{
  uint16_t LenBuf = 0;
#if 0
	if(LoopQueue_DataLen(pLoopBuf)<pLength || pLoopBuf->lock) {
		return 0;
	}
	
	*pData = pLoopBuf->nData[pLoopBuf->nReadIndex];
	pLoopBuf->nReadIndex++;
	if(pLoopBuf->nReadIndex >= pLoopBuf->nMaxLen)
		pLoopBuf->nReadIndex = 0;	
  
  return 1;
#else

if(LoopQueue_DataLen(pLoopBuf)<pLength || pLoopBuf->lock) {
		return 0;
	}
  
   if((pLength + pLoopBuf->nReadIndex) > pLoopBuf->nMaxLen)
   {
     LenBuf = pLoopBuf->nMaxLen - pLoopBuf->nReadIndex;
     memcpy(pData, pLoopBuf->nData + pLoopBuf->nReadIndex, LenBuf);
     pLoopBuf->nReadIndex = 0;
     pLoopBuf->nLength -= LenBuf;
     pLength -= LenBuf;
     pData += LenBuf;
   }
   memcpy(pData, pLoopBuf->nData + pLoopBuf->nReadIndex, pLength);
   pLoopBuf->nReadIndex += pLength;
   pLoopBuf->nLength -= pLength;
  return 1;

 #endif
}

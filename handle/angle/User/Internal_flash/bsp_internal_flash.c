/**
  ******************************************************************************
  * @file    bsp_internalFlash.c
  * @author  Rhui
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   内部FLASH读写测试
  ******************************************************************************
  */

#include ".\internal_flash\bsp_internal_flash.h"
#include ".\ATusart\bsp_ATusart.h"


inflash_StrDef  InFlashMCU;
uint32_t AddrBuf, AddrEND;
uint8_t RWnumINC;


/**
  * @brief  内部flash指定地址读取8位数据
  * @param  None
  * @retval None
  */
uint8_t InFlash_ReadByte(uint32_t ReadAddr, uint8_t *pBuff, uint8_t Bufflen)
{
  if(ReadAddr < FLASH_BASE_ADDR || (ReadAddr >= (FLASH_BASE_ADDR + FLASH_SIZE * 1024)))
    return FAILED;  /* 非法地址 */
  else
  {
    AddrEND = ReadAddr + Bufflen * 1;
    AddrBuf = ReadAddr;
    RWnumINC = 0;
    #ifdef DEBUG_USART
    USARTx_SendString(DEBUG_USARTx, "\r\nByteData:");
    #endif
    while(AddrBuf < AddrEND)
    {
      pBuff[RWnumINC] = (*(__IO uint8_t*)AddrBuf);
      #ifdef DEBUG_USART
      ForPC_Usart(" %x,", pBuff[RWnumINC]);
      #endif
      AddrBuf += 1;
      RWnumINC ++;
    }
    return PASSED;
  }
}

/**
  * @brief  内部flash指定地址读取16位数据
  * @param  None
  * @retval None
  */
uint8_t InFlash_ReadHalfWord(uint32_t ReadAddr, uint16_t *pBuff, uint8_t Bufflen)
{
  if(ReadAddr < FLASH_BASE_ADDR || (ReadAddr >= (FLASH_BASE_ADDR + FLASH_SIZE * 1024)))
    return FAILED;  /* 非法地址 */
  else
  {
    AddrEND = ReadAddr + Bufflen * 2;
    AddrBuf = ReadAddr;
    RWnumINC = 0;
    #ifdef DEBUG_USART
    USARTx_SendString(DEBUG_USARTx, "\r\nHalfWordData");
    #endif
    while(AddrBuf < AddrEND)
    {
      pBuff[RWnumINC] = (*(__IO uint16_t*)AddrBuf);
      #ifdef DEBUG_USART
      ForPC_Usart(" %x,", pBuff[RWnumINC]);
      #endif
      AddrBuf += 2;
      RWnumINC ++;
    }
    return PASSED;
  }
}

/**
  * @brief  写flash前进行校验
  * @param  None
  * @retval None
  */
uint8_t InFlash_WriteVerify(uint32_t WriteAddr, uint16_t *pBuff, uint8_t Bufflen)
{
  TestStatus MemoryProgramStatus = PASSED;    /* 记录整个测试结果 */
  if(WriteAddr < FLASH_BASE_ADDR || (WriteAddr >= (FLASH_BASE_ADDR + FLASH_SIZE * 1024)))
    return FAILED;  /* 非法地址 */
  else
  {
    AddrEND = WriteAddr + Bufflen * 2;
    AddrBuf = WriteAddr;
    RWnumINC = 0;
    #ifdef DEBUG_USART
    USARTx_SendString(DEBUG_USARTx, "\r\n写flash前进行校验:");
    #endif
    while((AddrBuf < AddrEND) && (MemoryProgramStatus != FAILED))
    {
      #ifdef DEBUG_USART
      ForPC_Usart(" %x,", (*(__IO uint16_t*)AddrBuf));
      #endif
      if((*(__IO uint16_t*)AddrBuf) != pBuff[RWnumINC])
      {
        MemoryProgramStatus = FAILED;
        #ifdef DEBUG_USART
        ForPC_Usart("\r\n不同的数据：%x \r\n", (*(__IO uint16_t*)AddrBuf));
        #endif
      }
      AddrBuf += 2;
      RWnumINC ++;
    }
    return MemoryProgramStatus;
  }
}

/**
  * @brief  向内部flash指定地址写入数据
  * @param  None
  * @retval None
	* @attention  只提供一页大小(2k/1k)做数据存储
	*             存一个数据地址+2
  */
uint8_t InFlash_WriteHalfWord(uint32_t WriteAddr, uint16_t *pBuff, uint8_t Bufflen)
{
  FLASH_Status FLASHStatus = FLASH_COMPLETE;    /* 记录每次擦除的结果 */
  TestStatus MemoryProgramStatus = PASSED;      /* 记录整个测试结果 */
  
  if(WriteAddr < FLASH_BASE_ADDR || (WriteAddr >= (FLASH_BASE_ADDR + FLASH_SIZE * 1024)))
    return FAILED;  /* 非法地址 */
  else
  {
    FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	
    FLASHStatus = FLASH_ErasePage(WriteAddr);   /* 擦除页 */
    #ifdef DEBUG_USART
    ForPC_Usart("擦除后的数据：%x \r\n", (*(__IO uint16_t*)WriteAddr));
    #endif
    /* 向内部FLASH写入数据 */
    AddrEND = WriteAddr + Bufflen * 2;
    AddrBuf = WriteAddr;
    RWnumINC = 0;
    while((AddrBuf < AddrEND) && (FLASHStatus == FLASH_COMPLETE))
    {
      #ifdef DEBUG_USART
      ForPC_Usart("%x:%x ", AddrBuf, pBuff[RWnumINC]);
      #endif
      FLASHStatus = FLASH_ProgramHalfWord(AddrBuf, pBuff[RWnumINC]);
      AddrBuf += 2;
      RWnumINC ++;
    }
    FLASH_Lock();
    #ifdef DEBUG_USART
    ForPC_Usart("\r\n");
    #endif
    /* 检查写入的数据是否正确 */
    AddrBuf = WriteAddr;
    RWnumINC = 0;
    while((AddrBuf < AddrEND) && (MemoryProgramStatus != FAILED))
    {
      #ifdef DEBUG_USART
      ForPC_Usart("%x, ", (*(__IO uint16_t*)AddrBuf));
      #endif
      if((*(__IO uint16_t*)AddrBuf) != (uint16_t)pBuff[RWnumINC])
      {
        MemoryProgramStatus = FAILED;
        #ifdef DEBUG_USART
        ForPC_Usart("\r\n错误的数据：%x \r\n", (*(__IO uint16_t*)AddrBuf));
        #endif
      }
      AddrBuf += 2;
      RWnumINC ++;
    }
    return MemoryProgramStatus;
  }
}

/**
  * @brief  写数组至内部flash指定地址
  * @param  None
  * @retval None
  */
void InFlash_WriteArray(uint32_t WriteAddr, uint16_t *pBuff, uint8_t Bufflen)
{
  if(InFlash_WriteVerify(WriteAddr, pBuff, Bufflen) == PASSED)
  {
    #ifdef DEBUG_USART
    USARTx_SendString(DEBUG_USARTx, "读写内部FLASH数据相同\r\n");
    #endif
  }
  else
  {
    if(InFlash_WriteHalfWord(WriteAddr, pBuff, Bufflen) == PASSED)
    {
      #ifdef DEBUG_USART
      USARTx_SendString(DEBUG_USARTx, "数组写内部FLASH成功\r\n");
      #endif
    }
    else
    {
      #ifdef DEBUG_USART
      USARTx_SendString(DEBUG_USARTx, "数组写内部FLASH失败\r\n");
      #endif
    }
  }
}

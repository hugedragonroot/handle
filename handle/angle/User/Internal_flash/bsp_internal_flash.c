/**
  ******************************************************************************
  * @file    bsp_internalFlash.c
  * @author  Rhui
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   �ڲ�FLASH��д����
  ******************************************************************************
  */

#include ".\internal_flash\bsp_internal_flash.h"
#include ".\ATusart\bsp_ATusart.h"


inflash_StrDef  InFlashMCU;
uint32_t AddrBuf, AddrEND;
uint8_t RWnumINC;


/**
  * @brief  �ڲ�flashָ����ַ��ȡ8λ����
  * @param  None
  * @retval None
  */
uint8_t InFlash_ReadByte(uint32_t ReadAddr, uint8_t *pBuff, uint8_t Bufflen)
{
  if(ReadAddr < FLASH_BASE_ADDR || (ReadAddr >= (FLASH_BASE_ADDR + FLASH_SIZE * 1024)))
    return FAILED;  /* �Ƿ���ַ */
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
  * @brief  �ڲ�flashָ����ַ��ȡ16λ����
  * @param  None
  * @retval None
  */
uint8_t InFlash_ReadHalfWord(uint32_t ReadAddr, uint16_t *pBuff, uint8_t Bufflen)
{
  if(ReadAddr < FLASH_BASE_ADDR || (ReadAddr >= (FLASH_BASE_ADDR + FLASH_SIZE * 1024)))
    return FAILED;  /* �Ƿ���ַ */
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
  * @brief  дflashǰ����У��
  * @param  None
  * @retval None
  */
uint8_t InFlash_WriteVerify(uint32_t WriteAddr, uint16_t *pBuff, uint8_t Bufflen)
{
  TestStatus MemoryProgramStatus = PASSED;    /* ��¼�������Խ�� */
  if(WriteAddr < FLASH_BASE_ADDR || (WriteAddr >= (FLASH_BASE_ADDR + FLASH_SIZE * 1024)))
    return FAILED;  /* �Ƿ���ַ */
  else
  {
    AddrEND = WriteAddr + Bufflen * 2;
    AddrBuf = WriteAddr;
    RWnumINC = 0;
    #ifdef DEBUG_USART
    USARTx_SendString(DEBUG_USARTx, "\r\nдflashǰ����У��:");
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
        ForPC_Usart("\r\n��ͬ�����ݣ�%x \r\n", (*(__IO uint16_t*)AddrBuf));
        #endif
      }
      AddrBuf += 2;
      RWnumINC ++;
    }
    return MemoryProgramStatus;
  }
}

/**
  * @brief  ���ڲ�flashָ����ַд������
  * @param  None
  * @retval None
	* @attention  ֻ�ṩһҳ��С(2k/1k)�����ݴ洢
	*             ��һ�����ݵ�ַ+2
  */
uint8_t InFlash_WriteHalfWord(uint32_t WriteAddr, uint16_t *pBuff, uint8_t Bufflen)
{
  FLASH_Status FLASHStatus = FLASH_COMPLETE;    /* ��¼ÿ�β����Ľ�� */
  TestStatus MemoryProgramStatus = PASSED;      /* ��¼�������Խ�� */
  
  if(WriteAddr < FLASH_BASE_ADDR || (WriteAddr >= (FLASH_BASE_ADDR + FLASH_SIZE * 1024)))
    return FAILED;  /* �Ƿ���ַ */
  else
  {
    FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	
    FLASHStatus = FLASH_ErasePage(WriteAddr);   /* ����ҳ */
    #ifdef DEBUG_USART
    ForPC_Usart("����������ݣ�%x \r\n", (*(__IO uint16_t*)WriteAddr));
    #endif
    /* ���ڲ�FLASHд������ */
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
    /* ���д��������Ƿ���ȷ */
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
        ForPC_Usart("\r\n��������ݣ�%x \r\n", (*(__IO uint16_t*)AddrBuf));
        #endif
      }
      AddrBuf += 2;
      RWnumINC ++;
    }
    return MemoryProgramStatus;
  }
}

/**
  * @brief  д�������ڲ�flashָ����ַ
  * @param  None
  * @retval None
  */
void InFlash_WriteArray(uint32_t WriteAddr, uint16_t *pBuff, uint8_t Bufflen)
{
  if(InFlash_WriteVerify(WriteAddr, pBuff, Bufflen) == PASSED)
  {
    #ifdef DEBUG_USART
    USARTx_SendString(DEBUG_USARTx, "��д�ڲ�FLASH������ͬ\r\n");
    #endif
  }
  else
  {
    if(InFlash_WriteHalfWord(WriteAddr, pBuff, Bufflen) == PASSED)
    {
      #ifdef DEBUG_USART
      USARTx_SendString(DEBUG_USARTx, "����д�ڲ�FLASH�ɹ�\r\n");
      #endif
    }
    else
    {
      #ifdef DEBUG_USART
      USARTx_SendString(DEBUG_USARTx, "����д�ڲ�FLASHʧ��\r\n");
      #endif
    }
  }
}

#ifndef __FLASH_EXT_H
#define __FLASH_EXT_H
#include "main.h"


#define GD32_FLASH_SIZE 512 	 	//��ѡGD32��FLASH������С(��λΪK)

#define GD32_FLASH_BASE 0x08000000 	//FLASH����ʼ��ַ


#define DATA_EEPROM_BASE ((uint32_t)0x08060000U)

#define FLASH_FOLDING_STATE_ADDR 0x00


void flash_write(uint32_t Addr,uint8_t *Buffer,uint8_t Length);
void flash_read(uint32_t Addr,uint8_t *Buffer,uint8_t Length);
void flash_test(void);
#endif

/**********************************************************/
//@Autor:Raw
//@Version:0.0.1
//@End Time:2023/3/14
//@Function:bsp_flash.c
/**********************************************************/
#include "bsp_flash.h"


uint16_t flash_ok = 0;
uint16_t crc = 0;
#define flash_page_size 0x1000



void fmc_erase_pages(uint32_t addr, uint32_t num)
{
	HAL_FLASH_Unlock();
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);//清除一些错误标志FLASH_Erase_Sector(1,FLASH_VOLTAGE_RANGE_3);
	FLASH_Erase_Sector(addr,FLASH_VOLTAGE_RANGE_3);
	HAL_FLASH_Lock();
}
void fmc_program(uint32_t addr, uint8_t *data, uint32_t num)
{
	uint32_t i=0;
	uint32_t *buffer = (uint32_t *)data;
	uint32_t temp_addr = addr;
	HAL_FLASH_Unlock();
	for (i = 0; i < num; i+=4)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,(uint32_t)(temp_addr + i) , *buffer);
		buffer++;
		__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
	}
	HAL_FLASH_Lock();
}


void fmc_read_data(uint32_t addr ,uint8_t *Data,uint32_t num)
{
	memcpy(Data, (uint8_t *)addr, num);
}






















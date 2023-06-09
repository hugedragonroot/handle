#include "flash_ext.h"
#include "string.h"

 
#if GD32_FLASH_SIZE<256
#define GD_SECTOR_SIZE 1024 //×Ö½Ú
#else 
#define GD_SECTOR_SIZE	2048
#endif

//
void flash_write(uint32_t Addr,uint8_t *Buffer,uint8_t Length)
{
	uint8_t i;
	uint32_t wAddr_temp;
	uint32_t * buffer = (uint32_t*)Buffer;
	
	wAddr_temp = Addr+DATA_EEPROM_BASE;	
	fmc_unlock();
	
	fmc_page_erase(wAddr_temp/GD_SECTOR_SIZE*GD_SECTOR_SIZE);
	
	for (i = 0; i < Length; i += 4)
	{
		fmc_word_program((uint32_t)(Addr+DATA_EEPROM_BASE + i) , *buffer);
		wAddr_temp = wAddr_temp + 4;
		buffer++;
	}	
	fmc_lock();
	
}	

//
void flash_read(uint32_t Addr,uint8_t *Buffer,uint8_t Length)
{
	//uint8_t i; 	
	uint32_t wAddr_temp;
	
	wAddr_temp = Addr+DATA_EEPROM_BASE;	
	
	memcpy(Buffer, (uint8_t *)wAddr_temp, Length);
	

}

void flash_test(void)
{
	uint8_t i,buf[11];
	
	buf[10]=0;
	
	for(i=0;i<10;i++)
	{
		buf[i]=i+0x30;
	}	
	flash_write(0,buf,10);
	flash_read(0,buf,10);
	printf("wr1 :%s\r\n",buf);
	
	for(i=0;i<10;i++)
	{
		buf[i]=0x30+9-i;
	}	
	flash_write(0,buf,10);
	flash_read(0,buf,10);	

	printf("wr2 :%s\r\n",buf);
	
	
	
}	


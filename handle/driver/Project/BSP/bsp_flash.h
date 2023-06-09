/**********************************************************/
//@Autor:Raw
//@Version:0.0.1
//@End Time:2023/3/14
/**********************************************************/
#ifndef __BSP_FLASH_H__
#define __BSP_FLASH_H__
#include "main.h"
#include "common.h"
#define  data_addr 0x8003e800

void fmc_read_data(uint32_t addr ,uint8_t *Data,uint32_t num);
void fmc_program(uint32_t addr, uint8_t *data, uint32_t num);
void fmc_erase_pages(uint32_t addr, uint32_t num);
#endif


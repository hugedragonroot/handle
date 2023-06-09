#ifndef __RF_315M_EXT_H
#define __RF_315M_EXT_H

#include "stdint.h"
typedef struct rf_315m_s
{
	uint8_t rf_d0;
	uint8_t rf_d1;
	uint8_t rf_d2;
	uint8_t rf_d3;
	
}RF_315M_DATA;


extern RF_315M_DATA rf_315m;
void rf_315m_init(void);
void rf_315m_scan(void);

#endif

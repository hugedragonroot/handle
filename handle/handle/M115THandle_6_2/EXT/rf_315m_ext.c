#include "rf_315m_ext.h"
#include "rf_315m_drv.h"
#include "gd32f30x.h"
RF_315M_DATA rf_315m;

void rf_315m_init(void)
{
	rf_315m.rf_d0=0;
	rf_315m.rf_d1=0;
	rf_315m.rf_d2=0;
	rf_315m.rf_d3=0;

}

void rf_315m_scan(void)
{
	static uint8_t t0=0,t1=0,t2=0,t3=0;
	
	if(READ_RF_D0())
	{
		t0++;
		if(t0>3)
		{
			rf_315m.rf_d0=1;
		}
	}
	else
	{
		t0=0;
	}
	
	if(READ_RF_D1())
	{
		t1++;
		if(t1>3)
		{
			rf_315m.rf_d1=1;
		}
	}
	else
	{
		t1=0;
	}

	if(READ_RF_D2())
	{
		t2++;
		if(t2>3)
		{
			rf_315m.rf_d2=1;
		}
	}
	else
	{
		t2=0;
	}
	
	if(READ_RF_D3())
	{
		t3++;
		if(t3>3)
		{
			rf_315m.rf_d3=1;
		}
	}
	else
	{
		t3=0;
	}	
	
		
}


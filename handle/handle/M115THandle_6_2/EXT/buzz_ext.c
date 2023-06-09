#include "buzz_ext.h"
#include "buzz_drv.h"


beep_str beep;

void scan_beep(void)
{
	static uint8_t i=0;
	
	if(i>100)i=0;
	i++;
	BUZZ_OFF(); 
	switch(beep.mode)
	{
	 case 0:
		 if(i==50)
			 BUZZ_ON();
		 break;
	 case 1:
		 if(i==40 || i==80)
			 BUZZ_ON();		 
		 break;
	}
}

void beep_set(uint8_t mode )
{
	beep.mode = mode;

}

void buzz_init(void)
{
	beep.mode = 0;
	beep.per = 0;
	beep.cnt = 0;	
}

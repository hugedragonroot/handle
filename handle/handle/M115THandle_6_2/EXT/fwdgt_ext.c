#include "fwdgt_ext.h"

#define FWDGT_OPEN 0

void fwdgt_init(void)
{
    #if FWDGT_OPEN
	rcu_osci_on(RCU_IRC40K);
	while(SUCCESS != rcu_osci_stab_wait(RCU_IRC40K)){};
	fwdgt_write_enable();
	fwdgt_config(2500,FWDGT_PSC_DIV32);//t=2500*32/40=2000ms
	fwdgt_write_disable();
	fwdgt_enable();	
    #endif
}

void fwdgt_feed(void)
{
    #if FWDGT_OPEN
	fwdgt_write_enable();
	fwdgt_counter_reload();
	fwdgt_write_disable();
    #endif
}

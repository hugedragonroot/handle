#include "app_mem.h"

MEM_STR Mem_set;
void mem_test(void);
void mem_init(void)
{
	Mem_set.model = DRIVER_MODEL;
	Mem_set.model = WALKINGMODE;
	Mem_set.seatangle =ANGLE_DEFUALT;
	Mem_set.bodyangle =ANGLE_DEFUALT;	
	Mem_set.VoiceLevel =1;
	Mem_set.VoiceSwitch =0;
	Mem_set.AlarmSwitch =0;
	Mem_set.CruiseCtrlSwitch =0;	

}

void mem_read(void)
{
//	flash_read(0,(uint8_t *)&Mem_set,sizeof(Mem_set));
	
	if(Mem_set.model > 1)Mem_set.model = DRIVER_MODEL;
	if(Mem_set.ControlMode>1)Mem_set.ControlMode = WALKINGMODE;
	if(Mem_set.seatangle > ANGLE_MAX || Mem_set.seatangle < -ANGLE_MAX)Mem_set.seatangle = ANGLE_DEFUALT;
	if(Mem_set.bodyangle > ANGLE_MAX || Mem_set.bodyangle < -ANGLE_MAX)Mem_set.bodyangle = ANGLE_DEFUALT;
	if(Mem_set.VoiceLevel > 5)	Mem_set.VoiceLevel = 1;
	if(Mem_set.VoiceSwitch > 1)Mem_set.VoiceSwitch = 0;
	if(Mem_set.AlarmSwitch > 1)Mem_set.AlarmSwitch = 0;
	if(Mem_set.CruiseCtrlSwitch > 1)Mem_set.CruiseCtrlSwitch = 0;
				
	Remote_setting_para.Model = Mem_set.model;
	Remote_setting_para.ControlMode = (Mem_set.ControlMode==WALKINGMODE)?eWalking:eTracks;

	Remote_setting_para.VoiceLevel = Mem_set.VoiceLevel  ;
	Remote_setting_para.VoiceSwitch=Mem_set.VoiceSwitch  ;	
	Remote_setting_para.AlarmSwitch=Mem_set.AlarmSwitch  ;
	Remote_setting_para.CruiseCtrlSwitch=Mem_set.CruiseCtrlSwitch  ;	

}

void mem_save(void)
{
	if(Remote_setting_para.ControlMode==eWalking)Mem_set.ControlMode=WALKINGMODE;
	if(Remote_setting_para.ControlMode==eTracks)Mem_set.ControlMode=TRACKMODE;
	Mem_set.VoiceLevel = Remote_setting_para.VoiceLevel;
	Mem_set.VoiceSwitch = Remote_setting_para.VoiceSwitch;	
	Mem_set.AlarmSwitch = Remote_setting_para.AlarmSwitch;
	Mem_set.CruiseCtrlSwitch = Remote_setting_para.CruiseCtrlSwitch;
	#if MEM_SAVE_EN
//	taskENTER_CRITICAL();	/*进入临界区*/
//	flash_write(0,(uint8_t *)&Mem_set,sizeof(Mem_set));
//	taskEXIT_CRITICAL();	/*退出临界区*/
	#endif
}


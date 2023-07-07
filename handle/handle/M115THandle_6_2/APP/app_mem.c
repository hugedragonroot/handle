#include "app_mem.h"

MEM_STR Mem_set;
void mem_test(void);
void mem_init(void)
{
	#if 0
	Mem_set.model = DRIVER_MODEL;
	Mem_set.model = WALKINGMODE;
	Mem_set.seatangle =ANGLE_DEFUALT;
	Mem_set.bodyangle =ANGLE_DEFUALT;	
	Mem_set.VoiceLevel =1;
	Mem_set.VoiceSwitch =0;
	Mem_set.AlarmSwitch =0;
	Mem_set.CruiseCtrlSwitch =0;	
	#endif

#if USING_FLASH
	flash_read(0x00,(uint8_t *)&Mem_set,sizeof(Mem_set));

	Remote_receive_para.folding_state = Mem_set.data.folderState;
	Remote_trans_para.folding_state = Mem_set.data.folderState;

	// if(Mem_set.data.speedGear > ePGearFive || Mem_set.data.speedGear < ePGearOne)
	// 	Remote_setting_para.SpeedGear = ePGearOne;
	// else
	// 	Remote_setting_para.SpeedGear = Mem_set.data.speedGear;

	Remote_setting_para.VoiceLevel = Mem_set.data.voiceLevel;

	ev1527_set_userID((uint8_t **)Mem_set.data.ev1527_user_id,EV1527_USER_NUM);
	memcpy(Remote_setting_para.ev1527_user_id,Mem_set.data.ev1527_user_id,
	sizeof(Mem_set.data.ev1527_user_id));

#endif

}

void mem_read(void)
{
//	flash_read(0,(uint8_t *)&Mem_set,sizeof(Mem_set));
	
	#if 0
	if(Mem_set.model > 1)Mem_set.model = DRIVER_MODEL;
	if(Mem_set.ControlMode>1)Mem_set.ControlMode = WALKINGMODE;
	if(Mem_set.seatangle > ANGLE_MAX || Mem_set.seatangle < -ANGLE_MAX)Mem_set.seatangle = ANGLE_DEFUALT;
	if(Mem_set.bodyangle > ANGLE_MAX || Mem_set.bodyangle < -ANGLE_MAX)Mem_set.bodyangle = ANGLE_DEFUALT;
	if(Mem_set.VoiceLevel > 5)	Mem_set.VoiceLevel = 1;
	if(Mem_set.VoiceSwitch > 1)Mem_set.VoiceSwitch = 0;
	if(Mem_set.AlarmSwitch > 1)Mem_set.AlarmSwitch = 0;
	if(Mem_set.CruiseCtrlSwitch > 1)Mem_set.CruiseCtrlSwitch = 0;
				
	// Remote_setting_para.Model = Mem_set.model;
	Remote_setting_para.ControlMode = (Mem_set.ControlMode==WALKINGMODE)?eWalking:eTracks;

	Remote_setting_para.VoiceLevel = Mem_set.VoiceLevel  ;
	Remote_setting_para.VoiceSwitch=Mem_set.VoiceSwitch  ;	
	Remote_setting_para.AlarmSwitch=Mem_set.AlarmSwitch  ;
	Remote_setting_para.CruiseCtrlSwitch=Mem_set.CruiseCtrlSwitch  ;	

	#endif
}

#define COMPARE_AND_ASSIGN(dst,src) {\
	if((dst) != (src)){\
		changeFlag = 1;\
		(dst) = (src);\
	}\
}

void mem_save(void)
{

	#if 0
	if(Remote_setting_para.ControlMode==eWalking)Mem_set.ControlMode=WALKINGMODE;
	if(Remote_setting_para.ControlMode==eTracks)Mem_set.ControlMode=TRACKMODE;
	Mem_set.VoiceLevel = Remote_setting_para.VoiceLevel;
	Mem_set.VoiceSwitch = Remote_setting_para.VoiceSwitch;	
	Mem_set.AlarmSwitch = Remote_setting_para.AlarmSwitch;
	Mem_set.CruiseCtrlSwitch = Remote_setting_para.CruiseCtrlSwitch;
	#endif

	uint8_t changeFlag = 0;
	COMPARE_AND_ASSIGN(Mem_set.data.folderState,Remote_receive_para.folding_state);
	// COMPARE_AND_ASSIGN(Mem_set.data.speedGear , Remote_setting_para.SpeedGear);
	COMPARE_AND_ASSIGN(Mem_set.data.voiceLevel , Remote_setting_para.VoiceLevel);
	COMPARE_AND_ASSIGN(Mem_set.data.AlarmSwitch , Remote_setting_para.AlarmSwitch);
	
	for(uint8_t i = 0; i<EV1527_USER_NUM;i++){
		COMPARE_AND_ASSIGN(Mem_set.data.ev1527_user_id[i][0],Remote_setting_para.ev1527_user_id[i][0]);
		COMPARE_AND_ASSIGN(Mem_set.data.ev1527_user_id[i][1],Remote_setting_para.ev1527_user_id[i][1]);
	}

#if USING_FLASH
	if(changeFlag){
		flash_write(0x00,(uint8_t *)&Mem_set,sizeof(Mem_set));
	}
#endif

	#if MEM_SAVE_EN
//	taskENTER_CRITICAL();	/*进入临界区*/
//	flash_write(0,(uint8_t *)&Mem_set,sizeof(Mem_set));
//	taskEXIT_CRITICAL();	/*退出临界区*/
	#endif
}


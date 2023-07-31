#include "app_mem.h"

MEM_STR Mem_Save;
const MEM_STR Mem_Init_para ={
	.data.version.id[0] = 0x00,
	.data.version.id[1] = 0x00,
	.data.version.id[2] = 0x00,
	.data.version.id[3] = 0x01,
	.data.voiceLevel = ePVoiceLevel_1,
	.data.AlarmSwitch = RESET,
	.data.HandleLock = eUnLock,
};
void mem_test(void);
void mem_init(void)
{
	#if 0
	Mem_Save.model = DRIVER_MODEL;
	Mem_Save.model = WALKINGMODE;
	Mem_Save.seatangle =ANGLE_DEFUALT;
	Mem_Save.bodyangle =ANGLE_DEFUALT;	
	Mem_Save.VoiceLevel =1;
	Mem_Save.VoiceSwitch =0;
	Mem_Save.AlarmSwitch =0;
	Mem_Save.CruiseCtrlSwitch =0;	
	#endif


#if USING_FLASH
	flash_read(0x00,(uint8_t *)&Mem_Save,sizeof(Mem_Save));

#if 1
	if(Mem_Save.data.version.sum != Mem_Init_para.data.version.sum){
		Mem_Save.data.version.sum = Mem_Init_para.data.version.sum;
		Remote_setting_para.VoiceLevel = Mem_Init_para.data.voiceLevel;
		Remote_setting_para.AlarmSwitch = Mem_Init_para.data.AlarmSwitch;
		Remote_setting_para.HandleLock = Mem_Init_para.data.HandleLock;
		// flash_write(0x00,(uint8_t *)&Mem_Save,sizeof(Mem_Save));
	}else
#endif
	{
		Remote_setting_para.VoiceLevel = Mem_Save.data.voiceLevel;
		Remote_setting_para.AlarmSwitch = Mem_Save.data.AlarmSwitch;
		Remote_setting_para.HandleLock = Mem_Save.data.HandleLock;
	}

	Remote_receive_para.folding_state = Mem_Save.data.folderState;
	Remote_trans_para.folding_state = Mem_Save.data.folderState;

	ev1527_set_userID((uint8_t **)Mem_Save.data.ev1527_user_id,EV1527_USER_NUM);
	memcpy(Remote_setting_para.ev1527_user_id,Mem_Save.data.ev1527_user_id,
	sizeof(Mem_Save.data.ev1527_user_id));

#endif

}

void mem_read(void)
{
//	flash_read(0,(uint8_t *)&Mem_Save,sizeof(Mem_Save));
	
	#if 0
	if(Mem_Save.model > 1)Mem_Save.model = DRIVER_MODEL;
	if(Mem_Save.ControlMode>1)Mem_Save.ControlMode = WALKINGMODE;
	if(Mem_Save.seatangle > ANGLE_MAX || Mem_Save.seatangle < -ANGLE_MAX)Mem_Save.seatangle = ANGLE_DEFUALT;
	if(Mem_Save.bodyangle > ANGLE_MAX || Mem_Save.bodyangle < -ANGLE_MAX)Mem_Save.bodyangle = ANGLE_DEFUALT;
	if(Mem_Save.VoiceLevel > 5)	Mem_Save.VoiceLevel = 1;
	if(Mem_Save.VoiceSwitch > 1)Mem_Save.VoiceSwitch = 0;
	if(Mem_Save.AlarmSwitch > 1)Mem_Save.AlarmSwitch = 0;
	if(Mem_Save.CruiseCtrlSwitch > 1)Mem_Save.CruiseCtrlSwitch = 0;
				
	// Remote_setting_para.Model = Mem_Save.model;
	Remote_setting_para.ControlMode = (Mem_Save.ControlMode==WALKINGMODE)?eWalking:eTracks;

	Remote_setting_para.VoiceLevel = Mem_Save.VoiceLevel  ;
	Remote_setting_para.VoiceSwitch=Mem_Save.VoiceSwitch  ;	
	Remote_setting_para.AlarmSwitch=Mem_Save.AlarmSwitch  ;
	Remote_setting_para.CruiseCtrlSwitch=Mem_Save.CruiseCtrlSwitch  ;	

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
	if(Remote_setting_para.ControlMode==eWalking)Mem_Save.ControlMode=WALKINGMODE;
	if(Remote_setting_para.ControlMode==eTracks)Mem_Save.ControlMode=TRACKMODE;
	Mem_Save.VoiceLevel = Remote_setting_para.VoiceLevel;
	Mem_Save.VoiceSwitch = Remote_setting_para.VoiceSwitch;	
	Mem_Save.AlarmSwitch = Remote_setting_para.AlarmSwitch;
	Mem_Save.CruiseCtrlSwitch = Remote_setting_para.CruiseCtrlSwitch;
	#endif

	uint8_t changeFlag = 0;
	COMPARE_AND_ASSIGN(Mem_Save.data.folderState,Remote_receive_para.folding_state);
	// COMPARE_AND_ASSIGN(Mem_Save.data.speedGear , Remote_setting_para.SpeedGear);
	COMPARE_AND_ASSIGN(Mem_Save.data.voiceLevel , Remote_setting_para.VoiceLevel);
	COMPARE_AND_ASSIGN(Mem_Save.data.AlarmSwitch , Remote_setting_para.AlarmSwitch);
	COMPARE_AND_ASSIGN(Mem_Save.data.HandleLock , Remote_setting_para.HandleLock);
	
	for(uint8_t i = 0; i<EV1527_USER_NUM;i++){
		COMPARE_AND_ASSIGN(Mem_Save.data.ev1527_user_id[i][0],Remote_setting_para.ev1527_user_id[i][0]);
		COMPARE_AND_ASSIGN(Mem_Save.data.ev1527_user_id[i][1],Remote_setting_para.ev1527_user_id[i][1]);
	}

#if USING_FLASH
	if(changeFlag){
		flash_write(0x00,(uint8_t *)&Mem_Save,sizeof(Mem_Save));
	}
#endif

	#if MEM_SAVE_EN
//	taskENTER_CRITICAL();	/*进入临界区*/
//	flash_write(0,(uint8_t *)&Mem_Save,sizeof(Mem_Save));
//	taskEXIT_CRITICAL();	/*退出临界区*/
	#endif
}


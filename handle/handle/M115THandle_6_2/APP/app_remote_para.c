#include "app_remote_para.h"
#include "string.h"
#include "ext.h"

 TREMOTE_SETTING_PARA		Remote_setting_para;	
 
 TREMOTE_TRANS_PARA			Remote_trans_para;
 TREMOTE_TRANS_PARA			Remote_receive_para;

 TPHONE_TRANS_PARA			Phone_trans_para;
 TPHONE_TRANS_PARA			Phone_receive_para;


void Remote_para_init(void)
{
	memset(&Remote_setting_para,0,sizeof(Remote_setting_para));
	
	memset(&Remote_trans_para,0,sizeof(Remote_trans_para));
	memset(&Remote_receive_para,0,sizeof(Remote_receive_para));
	
	memset(&Phone_trans_para,0,sizeof(Phone_trans_para));
	memset(&Phone_receive_para,0,sizeof(Phone_receive_para));	

	Remote_para_default();
}

void Remote_para_default(void)
{
	Remote_setting_para.RemoteConnect = eOffline;	
	Remote_setting_para.RemoteCount = 0;	
	Remote_setting_para.RemoteBTCmd = CMD_Idle;
	Remote_setting_para.RemoteBTConnect = eOffline;	
	Remote_setting_para.RemoteBTCount = 0;		
	
	Remote_setting_para.PowerStatus = ePowerOff;
	Remote_setting_para.CoordX = Coord_Base;
	Remote_setting_para.CoordY = Coord_Base;

	Remote_setting_para.ControlMode = eWalking;

	Remote_setting_para.ControlPre=Remote_setting_para.ControlMode;
	Remote_setting_para.OperationMode = eManual;
	// if(Remote_setting_para.SpeedGear == 0)
	Remote_setting_para.SpeedGear = eGearOne;
	Remote_setting_para.Peripherals = 0x00;
	Remote_setting_para.Battery =0x00;
	// Remote_setting_para.HandleLock = eUnLock;
	// Remote_setting_para.HandleLock = eLock;
	// Remote_setting_para.VoiceLevel = ePVoiceLevel_1;
	Remote_setting_para.VoiceSwitch = 0;
	// Remote_setting_para.AlarmSwitch = 1;
	Remote_setting_para.CruiseCtrlSwitch = 0;	
	// Remote_setting_para.CurrentMusic = 0;
	Remote_setting_para.Model = M115;
	Remote_setting_para.folding_time = 0;

	if(joyetick_adc.valX != Coord_Base || joyetick_adc.valY != Coord_Base){
		// Remote_setting_para.ErrorFlag |= 1<<ERROR_JOY_XY;
		Remote_trans_para.errorFlag.bit.joyXyError = 1;
	}

	Remote_trans_para.walking_speed_L = 0x0000;
	Remote_trans_para.walking_speed_R = 0x0000;	
	Remote_trans_para.push_rod_speed = 0x0000;	
	
	#if 0
	Remote_trans_para.seat_angle = ANGLE_DEFUALT;
	Remote_trans_para.body_angle = ANGLE_DEFUALT;	
	
	Remote_receive_para.back_ultrasonic_x = 100;	
	Remote_receive_para.back_ultrasonic_y = 0;	
	Remote_receive_para.fornt_ultrasonic_y = 0;
	#endif

	Remote_receive_para.Drivermodel = 0;
	Remote_receive_para.Driversoftware = 1;
	Remote_receive_para.Driversoftwarepre = 0;

	Remote_trans_para.folding_state = Remote_receive_para.folding_state;
	
	memset(&Phone_receive_para,0,sizeof(Phone_receive_para));		
	// Phone_receive_para.SpeedLevel = eGearOne;	
	Phone_receive_para.CoordX = Coord_Base;
	Phone_receive_para.CoordY = Coord_Base;
	// Phone_receive_para.M115CtrlMode = ePfold;
	// Phone_receive_para.VoiceLevel = ePVoiceLevel_1;
	// Phone_receive_para.VoiceSwitch = 0;
	// Phone_receive_para.AlarmSwitch = 0;
	Phone_receive_para.CruiseCtrlSwitch = 0;	
}





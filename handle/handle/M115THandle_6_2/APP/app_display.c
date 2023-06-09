#include "app_display.h"
#include "main.h"
#include "stdlib.h"

#if !USING_LED_POINT_DISPLAY

ui_e page_num=ePAGE_PWROFF;

Control_STR Control_para;

void display_init(void)
{
	Control_para.Up_stairs_state  = eUP_STAIRS_START;
	Control_para.Down_stairs_state = eDOWN_STAIRS_START;

}

void display_main_reflash(void)
{
	display_graphic(BATTERY_X,BATTERY_Y,BATTERY_SIZE_X,BATTERY_SIZE_Y,BMP_Battery[Remote_setting_para.Battery]);	
//	display_graphic(KEYLOCK_LOGO_X,KEYLOCK_LOGO_Y,KEYLOCK_SIZE_X,KEYLOCK_SIZE_Y,BMP_LOCK);	
	display_graphic(CONNECT_X,CONNECT_Y,CONNECT_SIZE_X,CONNECT_SIZE_Y,BMP_Connect[ Remote_setting_para.RemoteConnect ]);	
	display_graphic(CONNECT_BT_X,CONNECT_BT_Y,CONNECT_BT_SIZE_X,CONNECT_BT_SIZE_Y,BMP_Connect[ Remote_setting_para.RemoteBTConnect ]);	
	display_graphic(BMP_NUM_PLUS_X,BMP_NUM_PLUS_Y,NUM_PLUS_SIZE_X,NUM_PLUS_SIZE_Y,BMP_NUM_PLUS[Remote_setting_para.SpeedGear]);	

	display_graphic(MODE_X,MODE_Y,MODE_SIZE_X,MODE_SIZE_Y,BMP_MODE[Remote_setting_para.ControlMode == eWalking?0:1]);

}


void display_main_lock(void)
{
	static uint8_t lock;
	
	if(lock ==Remote_setting_para.HandleLock)
		return;
	lock = Remote_setting_para.HandleLock;
	if(Remote_setting_para.HandleLock)
		display_graphic(KEYLOCK_LOGO_X,KEYLOCK_LOGO_Y,KEYLOCK_SIZE_X,KEYLOCK_SIZE_Y,BMP_LOCK);	
	else
		clear_graphic(KEYLOCK_LOGO_X,KEYLOCK_LOGO_Y,KEYLOCK_SIZE_X,KEYLOCK_SIZE_Y);
	
	
}

void display_main_battery(void)
{
	static uint8_t Battery;
	
	if(Battery==Remote_setting_para.Battery)
		return;	
	Battery = Remote_setting_para.Battery;
	display_graphic(BATTERY_X,BATTERY_Y,BATTERY_SIZE_X,BATTERY_SIZE_Y,BMP_Battery[Remote_setting_para.Battery]);	
}

void display_main_connect(void)
{
	static uint8_t RemoteConnect;	

	if(RemoteConnect==Remote_setting_para.RemoteConnect)
		return;
	RemoteConnect = Remote_setting_para.RemoteConnect;
	display_graphic(CONNECT_X,CONNECT_Y,CONNECT_SIZE_X,CONNECT_SIZE_Y,BMP_Connect[ Remote_setting_para.RemoteConnect ]);
			
}

void display_main_btconnect(void)
{
	static uint8_t RemoteConnect;	

	if(RemoteConnect==Remote_setting_para.RemoteBTConnect)
		return;
	RemoteConnect = Remote_setting_para.RemoteBTConnect;
	display_graphic(CONNECT_BT_X,CONNECT_BT_Y,CONNECT_BT_SIZE_X,CONNECT_BT_SIZE_Y,BMP_Connect[ Remote_setting_para.RemoteBTConnect ]);
			
}
void display_main_auto(void)
{
	uint8_t temp;

	if(Remote_setting_para.OperationMode==eManual)	
		display_string_8x16(AUTO_X,AUTO_Y,0,(uint8_t*)"M ");
	else if(Remote_setting_para.OperationMode==eAutoLoading)	
		display_string_8x16(AUTO_X,AUTO_Y,0,(uint8_t*)"A ");
	else if(Remote_setting_para.OperationMode==eAutoUP)	
		display_string_8x16(AUTO_X,AUTO_Y,0,(uint8_t*)"AU");
	else if(Remote_setting_para.OperationMode==eAutoDOWN)	
		display_string_8x16(AUTO_X,AUTO_Y,0,(uint8_t*)"AD");	
}


void display_main_speed(void)
{
	display_graphic(BMP_NUM_PLUS_X,BMP_NUM_PLUS_Y,NUM_PLUS_SIZE_X,NUM_PLUS_SIZE_Y,BMP_NUM_PLUS[Remote_setting_para.SpeedGear]);
	
//	display_graphic(KNH_X,KNH_Y,KNH_SIZE_X,KNH_SIZE_Y,BMP_KNH);

}


void display_main_modelogo(void)
{	
	display_graphic(MODE_X,MODE_Y,MODE_SIZE_X,MODE_SIZE_Y,BMP_MODE[(Remote_setting_para.ControlMode == eWalking)?0:1 ]);
}


void display_exit_pause(void)
{	
//	display_graphic(KEY1_LOGO_X,KEY_LOGO_Y+8,KEY_SIZE_X,KEY_SIZE_Y,BMP_KEY[KEY_LEFT]);
//	display_string_5x8(KEY1_LOGO_X+18,KEY_LOGO_Y,0,(uint8_t*)":Exit");
//	display_graphic(KEY2_LOGO_X,KEY_LOGO_Y+8,KEY_SIZE_X,KEY_SIZE_Y,BMP_KEY[KEY_MODE]);
//	display_string_5x8(KEY2_LOGO_X+18,KEY_LOGO_Y,0,(uint8_t*)":Pause");
}

void display_select_enter(void)
{
//	display_graphic(KEY1_LOGO_X,KEY_LOGO_Y+8,KEY_SIZE_X,KEY_SIZE_Y,BMP_KEY[KEY_SPEED_ADD]);
//	display_string_5x8(KEY1_LOGO_X+18,KEY_LOGO_Y,0,(uint8_t*)":Select");
//	display_graphic(KEY2_LOGO_X,KEY_LOGO_Y+8,KEY_SIZE_X,KEY_SIZE_Y,BMP_KEY[KEY_LAMP]);
//	display_string_5x8(KEY2_LOGO_X+18,KEY_LOGO_Y,0,(uint8_t*)":Enter");	

}
void display_select_enter_exit(void)
{
//	display_graphic(KEY1_LOGO_X,KEY_LOGO_Y+8,KEY_SIZE_X,KEY_SIZE_Y,BMP_KEY[KEY_SPEED_ADD]);
//	display_string_5x8(KEY1_LOGO_X+16,KEY_LOGO_Y,0,(uint8_t*)":Select");
	
//	display_graphic(KEY2_LOGO_X,KEY_LOGO_Y+8,KEY_SIZE_X,KEY_SIZE_Y,BMP_KEY[KEY_LAMP]);
//	display_string_5x8(KEY2_LOGO_X+16,KEY_LOGO_Y,0,(uint8_t*)":Enter");	
	
//	display_graphic(0,56,KEY_SIZE_X,KEY_SIZE_Y,BMP_KEY[KEY_LEFT]);
//	display_string_5x8(16,KEY_LOGO_Y,0,(uint8_t*)":Exit");	

}


uint8_t display_message(uint8_t*q)
{
	clear_screen();
	display_string_8x16(0,0,0,q);
	return 0;
}

void display_sel_3(uint8_t*q,uint8_t*m,uint8_t*n,uint8_t*p,uint8_t sel)
{	
	display_string_8x16(0,0,0,q);
	if(p==NULL)
	{
		display_string_8x16(24,48,sel==0,m);	
		display_string_8x16(80,48,sel==1,n);		
	}
	else
	{
		display_string_8x16(0,48,sel==0,m);	
		display_string_8x16(30,48,sel==1,n);		
		display_string_8x16(60,48,sel==2,p);
	}		
}





uint8_t display_yes_no(uint8_t*q,uint8_t*m,uint8_t*n,uint8_t key)
{	
	static uint8_t sel=0;
	uint8_t ret=0;
	
	display_string_8x16(0,0,0,q);
	display_string_8x16(24,48,sel,m);	
	display_string_8x16(80,48,!sel,n);		
	
	if(key==KEY_SPEED_ADD_DOWN || key==KEY_SPEED_SUB_DOWN)
	{
		sel++;
		sel&=0x01;
	}
	else if(key==KEY_MENU_ENTER_DOWN )
	{
		if(sel&0x01)//YES
		{
			ret=YES;
		}
		else			//NO
		{
			ret=NO;
		}
		sel=0;
	}

	return ret;	
}



void display_main_ui(void)
{
	uint8_t keyState;
	static uint8_t delay;
	static uint16_t mode_delay;
	uint8_t buff[20];
	

	
	display_main_battery();	
	display_main_lock();
	display_main_auto();
	display_main_speed();
	display_main_modelogo();
	display_main_connect();
	display_main_btconnect();
	

#if APP_BT_TEST
	sprintf((char*)buff,"LS:%d",Remote_setting_para.LampSwitch);
	display_string_8x16(31,16,0,buff);
	sprintf((char*)buff,"VL:%d",Remote_setting_para.VoiceLevel);
		display_string_8x16(31,32,0,buff);
	sprintf((char*)buff,"VS:%d",Remote_setting_para.VoiceSwitch);
		display_string_8x16(31,48,0,buff);		
	sprintf((char*)buff,"AS:%d",Remote_setting_para.AlarmSwitch);
		display_string_8x16(66,32,0,buff);	
	sprintf((char*)buff,"CS:%d",Remote_setting_para.CruiseCtrlSwitch);
		display_string_8x16(66,48,0,buff);			
#else

	if(Remote_setting_para.ControlMode==eWalking)
	{
		sprintf((char*)buff,"LS:%5d",Remote_trans_para.walking_speed_L);
		display_string_8x16(34,16,0,buff);
		sprintf((char*)buff,"RS:%5d",Remote_trans_para.walking_speed_R);
		display_string_8x16(34,32,0,buff);
		sprintf((char*)buff,"AN:%3.1f",Remote_setting_para.CoordAngle/3.14*180);
		// display_string_8x16(0,48,0,"    ");	
		display_string_8x16(34,48,0,buff);			
	}
	#if 0
	else if(Remote_setting_para.ControlMode==eTracks)
	{
		sprintf((char*)buff,"LS:%5d",Remote_trans_para.track_speed_L);
		display_string_8x16(34,16,0,buff);
		sprintf((char*)buff,"RS:%5d",Remote_trans_para.track_speed_R);
		display_string_8x16(34,32,0,buff);
		sprintf((char*)buff,"BA:%5d",Remote_receive_para.body_angle);
		display_string_8x16(0,48,0,buff);			
		sprintf((char*)buff,"SA:%5d",Remote_receive_para.seat_angle);
		display_string_8x16(64,48,0,buff);	
	}
	#endif			
#endif

#if DEBUG_MODECHANGE	
	if(Remote_setting_para.Control_change==1)
	{
		mode_delay++;
		if(Remote_setting_para.ControlMode == eTracks)
		{
			if(Remote_receive_para.fornt_support_elongation<=ELONGATION_0 && Remote_receive_para.back_support_elongation<=ELONGATION_0)
			{
				mode_delay =0;
				Remote_setting_para.Control_change = 0;
				Remote_trans_para.back_wheel_mode = eSpeedMode;
				Remote_trans_para.fornt_wheel_mode = eSpeedMode;				
			}
			if(mode_delay>=400)//50*200
			{
				mode_delay =0;
				Remote_setting_para.Control_change = 0;
				Remote_trans_para.back_wheel_mode = eSpeedMode;
				Remote_trans_para.fornt_wheel_mode = eSpeedMode;
				Remote_setting_para.ControlMode = eWalking;
			}
			return;
			
		}
		else if(Remote_setting_para.ControlMode == eWalking)
		{
			if(abs(Remote_receive_para.fornt_support_elongation-ELONGATION_MID)<=ELONGATION_ERR && abs(Remote_receive_para.back_support_elongation-ELONGATION_MID)<=ELONGATION_ERR)
			{
				mode_delay =0;
				Remote_setting_para.Control_change = 0;
				Remote_trans_para.back_wheel_mode = eSpeedMode;
				Remote_trans_para.fornt_wheel_mode = eSpeedMode;				
			}
			if(mode_delay>=400)//50*200
			{
				mode_delay =0;
				Remote_setting_para.Control_change = 0;
				Remote_trans_para.back_wheel_mode = eSpeedMode;
				Remote_trans_para.fornt_wheel_mode = eSpeedMode;
				Remote_setting_para.ControlMode = eTracks;
			}
				return;			
		}
		
	}	
#endif
	
	if(Remote_setting_para.PowerStatus == ePowerOff)
	{
		Remote_para_default();	
		lcd_PowerOff();
		setdisplay_ui(ePAGE_PWROFF);
		return;
	}
	keyState = bsp_GetKey();	
	switch(keyState)
	{
		case KEY_LOCK_DOWN:		//锁定			
			Remote_setting_para.HandleLock++;
			Remote_setting_para.HandleLock &=0x01;	
			break;		
		case KEY_POWER_DOWN:			//关机
			Remote_setting_para.PowerStatus = ePowerOff;
			Remote_para_default();	
			lcd_PowerOff();
			setdisplay_ui(ePAGE_PWROFF);		
			break;
		case KEY_SPEED_ADD_DOWN://加速档	
			if(Remote_setting_para.HandleLock)break;			
			if(Remote_setting_para.SpeedGear<eGearFive)
				Remote_setting_para.SpeedGear++;							
			break;
		case KEY_SPEED_SUB_DOWN://减速档
			if(Remote_setting_para.HandleLock)break;			
			if(Remote_setting_para.SpeedGear>eGearOne)
				Remote_setting_para.SpeedGear--;			
			break;
		case KEY_HORN_DOWN://喇叭
			if(Remote_setting_para.HandleLock)break;			
			play_file_voice(1,Remote_setting_para.CurrentMusic);
			break;
		case KEY_HORN_LONG:
			if(Remote_setting_para.HandleLock)break;
			// setdisplay_ui(ePAGE_SETTING);//setting			
			break;
		case KEY_MENU_ENTER_DOWN://进入菜单
			if(Remote_setting_para.HandleLock)break;
			setdisplay_ui(ePAGE_SETTING);//setting			
			break;
		case KEY_PUSH_ORD_ADD_DOWN://重定义为推杆升
		// case KEY_PUSH_ORD_ADD_LONG://重定义为推杆升
			if(Remote_setting_para.HandleLock)break;
			Remote_trans_para.push_rod_speed=PUSH_ORD_SPEED_SUB;			
			break;		
		case KEY_PUSH_ORD_ADD_UP:
		case KEY_PUSH_ORD_SUB_UP:
			Remote_trans_para.push_rod_speed=PUSH_ORD_SPEED_ZERO;		
			break;
		case KEY_PUSH_ORD_SUB_DOWN:	//重定义为推杆降
		// case KEY_PUSH_ORD_SUB_LONG:	//重定义为推杆降
			if(Remote_setting_para.HandleLock)break;
			Remote_trans_para.push_rod_speed=PUSH_ORD_SPEED_ADD;//实际正速度座椅往下
			break;		
		//组合按键
		case KEY_PUSH_ORD_ADD_SUB_LONG:
			if(Remote_setting_para.HandleLock)break;
			if(Remote_receive_para.push_rod_speed == 0 && Remote_receive_para.seat_rod_speed == 0)
				Remote_trans_para.folding_state = !Remote_receive_para.folding_state;
			// flash_write(FLASH_FOLDING_STATE_ADDR,
						// &Remote_trans_para.folding_state,
						// sizeof(Remote_trans_para.folding_state));
		break;

#if 0
		case KEY_MODE_ADD_DOWN://座椅
			if(Remote_setting_para.HandleLock)break;
			Remote_trans_para.seat_wheel_mode=eSpeedMode;
			Remote_trans_para.seat_speed=SEAT_SPEEDDEF_UP;
			break;		
		case KEY_MODE_ADD_UP:
			Remote_trans_para.seat_speed=MOVE_SPEEDDEF_ZERO;
			break;		
		
		case KEY_MODE_SUB_DOWN://座椅
			if(Remote_setting_para.HandleLock)break;
			Remote_trans_para.seat_wheel_mode=eSpeedMode;
			Remote_trans_para.seat_speed=SEAT_SPEEDDEF_DOWN;		
			break;	
		case KEY_MODE_SUB_UP:	
			Remote_trans_para.seat_speed=MOVE_SPEEDDEF_ZERO;		
			break;
		
		case KEY_AUTO_ADD_DOWN:	//后支撑
			if(Remote_setting_para.HandleLock)break;
			Remote_trans_para.back_wheel_mode=eSpeedMode;
			Remote_trans_para.back_support_speed=BACK_SPEEDDEF_DOWN;//实际正速度座椅往下			
			break;
		case KEY_AUTO_ADD_UP:
			Remote_trans_para.back_support_speed=MOVE_SPEEDDEF_ZERO;		
			break;			
		case KEY_AUTO_SUB_DOWN:	//后支撑
			if(Remote_setting_para.HandleLock)break;
			Remote_trans_para.back_wheel_mode=eSpeedMode;
			Remote_trans_para.back_support_speed=BACK_SPEEDDEF_UP;//实际负速度座椅往上
			break;
		case KEY_AUTO_SUB_UP:
			Remote_trans_para.back_support_speed=MOVE_SPEEDDEF_ZERO;		
			break;		

		case KEY_COMF_ADD_DOWN:	//前支撑
			if(Remote_setting_para.HandleLock)break;
			Remote_trans_para.fornt_wheel_mode=eSpeedMode;
			Remote_trans_para.fornt_support_speed=FORNT_SPEEDDEF_UP;			
			break;
		case KEY_COMF_ADD_UP:
			Remote_trans_para.fornt_support_speed=MOVE_SPEEDDEF_ZERO;		
			break;				
		
		case KEY_COMF_SUB_DOWN:	//前支撑
			if(Remote_setting_para.HandleLock)break;
			Remote_trans_para.fornt_wheel_mode=eSpeedMode;
			Remote_trans_para.fornt_support_speed=FORNT_SPEEDDEF_DOWN;			
			break;
		case KEY_COMF_SUB_UP:
			Remote_trans_para.fornt_support_speed=MOVE_SPEEDDEF_ZERO;		
			break;			
		case KEY_LAMP_ADD_DOWN:	//前后支撑
			if(Remote_setting_para.HandleLock)break;
			Remote_trans_para.fornt_wheel_mode=eSpeedMode;
			Remote_trans_para.back_wheel_mode=eSpeedMode;
			Remote_trans_para.fornt_support_speed=FORNT_SPEEDDEF_UP;
			Remote_trans_para.back_support_speed=BACK_SPEEDDEF_UP;			
			break;
		case KEY_LAMP_ADD_UP:
			Remote_trans_para.fornt_support_speed=MOVE_SPEEDDEF_ZERO;	
			Remote_trans_para.back_support_speed=MOVE_SPEEDDEF_ZERO;
			break;				
		
		case KEY_LAMP_SUB_DOWN:	//前后支撑
			if(Remote_setting_para.HandleLock)break;
			Remote_trans_para.fornt_wheel_mode=eSpeedMode;
			Remote_trans_para.back_wheel_mode=eSpeedMode;
			Remote_trans_para.fornt_support_speed=FORNT_SPEEDDEF_DOWN;
			Remote_trans_para.back_support_speed=BACK_SPEEDDEF_DOWN;				
			break;
		case KEY_LAMP_SUB_UP:
			Remote_trans_para.fornt_support_speed=MOVE_SPEEDDEF_ZERO;	
			Remote_trans_para.back_support_speed=MOVE_SPEEDDEF_ZERO;	

			break;		
#endif
		
	}
	#if !APP_BT_TEST
	if(Remote_setting_para.OperationMode==eAutoUP || Remote_setting_para.OperationMode==eAutoDOWN )
	{
		delay++;
		if(delay>=40)//40*50 ms	
		{
			if(Remote_setting_para.OperationMode==eAutoUP)
			{			
				setdisplay_ui(ePAGE_UPSTAIR);//goto upstais
			}
			if(Remote_setting_para.OperationMode==eAutoDOWN)
			{				
				setdisplay_ui(ePAGE_DOWNSTAIR);//goto downstais				
			}				
		}
	}
	else
		delay = 0;
	#endif
	
}
#if 0
void display_upstairs_ui(void)
{
	uint8_t ret=0;
	int8_t 	temp;
	static 	int16_t body_angle_temp=0;
	static 	int32_t track_hall_cnt_temp=0;	
	static 	uint16_t back_ultrasonic_y_temp=0;			
	static 	uint8_t cnt=0;
	static 	uint8_t delay=0;
	static 	uint8_t sel=0;
	static 	uint8_t Up_state=0;	
	uint8_t keyState;
	uint8_t buff[20];
	
	keyState = bsp_GetKey();
	// if(keyState==KEY_POWER_LONG)
	if(keyState==KEY_POWER_DOWN)
	{
		Remote_setting_para.PowerStatus = ePowerOff;
		Remote_para_default();	
		lcd_PowerOff();
		setdisplay_ui(ePAGE_PWROFF);
	}
	switch(Control_para.Up_stairs_state)
	{
		case eUP_STAIRS_START:
			ret	= display_yes_no((uint8_t*)" Are you sure to go auto upstairs?","Yes","No",keyState);
			Remote_setting_para.CoordX = Coord_Base;
			Remote_setting_para.CoordY = Coord_Base;	
			if(ret == YES)//YES
			{
				Remote_setting_para.ControlMode=eUpstairs;
				Control_para.Up_stairs_state = eUP_STAIRS_READY;
				display_message((uint8_t *)" Ready to go upstairs!");
			}
			else if(ret == NO)//NO
			{
				Control_para.Up_stairs_state = eUP_STAIRS_OUT;		
				display_message((uint8_t *)" Up stairs out!");
			}
			break;
		case eUP_STAIRS_READY:		
		
			Remote_setting_para.SpeedGear = eGearOne;
			Remote_setting_para.CoordX = Coord_Base;
			Remote_setting_para.CoordY = Coord_Base;		
			Remote_trans_para.back_wheel_mode = eElongationMode;
			Remote_trans_para.fornt_wheel_mode = eElongationMode;
			Remote_trans_para.seat_wheel_mode = eAngleMode;
			Remote_trans_para.seat_angle=ANGLE_DEFUALT;
			Remote_trans_para.fornt_support_elongation = 0;
			Remote_trans_para.back_support_elongation = 0;
			sprintf((char*)buff,"FE:%4d ",Remote_receive_para.fornt_support_elongation);
			display_string_8x16(0,32,0,buff);		
			sprintf((char*)buff,"BE:%4d ",Remote_receive_para.back_support_elongation);
			display_string_8x16(70,32,0,buff);	
			sprintf((char*)buff,"SA:%4d ",Remote_receive_para.seat_angle);
			display_string_8x16(0,48,0,buff);		
			sprintf((char*)buff,"BX:%4d ",Remote_receive_para.back_ultrasonic_x);
			display_string_8x16(70,48,0,buff);		
			if(abs(Remote_receive_para.seat_angle -ANGLE_DEFUALT)<=ANGLE_SEAT_1 && Remote_receive_para.fornt_support_elongation<=ELONGATION_0 && Remote_receive_para.back_support_elongation<=ELONGATION_0 )
			{
				cnt++;
				if(cnt>SURE_TIMES)
				{
					if(Remote_receive_para.back_ultrasonic_x >= BACK_ULT_TOP_X_1M || Remote_receive_para.back_ultrasonic_x ==0)
					{
						Control_para.Up_stairs_state = eUP_STAIRS_FINISH;
						display_message((uint8_t *)" too far or wrong direction");					
					}
					else
					{
						Control_para.Up_stairs_state = eUP_STAIRS_BACK;	
						display_message((uint8_t *)" Going upstairs!");
						display_exit_pause();						
					}
					cnt = 0;					
					break;
				}
			}
			else
				cnt=0;
			break;
			
		case eUP_STAIRS_BACK:				

			sprintf((char*)buff,"BX:%4d ",Remote_receive_para.back_ultrasonic_x);
			display_string_8x16(0,32,0,buff);
			sprintf((char*)buff,"BY:%4d ",Remote_receive_para.back_ultrasonic_y);
			display_string_8x16(70,32,0,buff);
			
			Remote_setting_para.ControlMode = eTracks;		
			
			if(Remote_receive_para.back_ultrasonic_x >= BACK_ULT_TOP_X ||Remote_receive_para.back_ultrasonic_x ==0) 
			{
				cnt++;
				if(cnt>SURE_TIMES)
				{	
					Remote_setting_para.CoordY = Coord_Base;//停止
					display_message((uint8_t *)" Detected top of stairs!");
					Control_para.Up_stairs_state = eUP_STAIRS_TOP_UP;//检测到梯顶平面
					track_hall_cnt_temp = Remote_receive_para.track_hall_cnt;	
					display_exit_pause();
					cnt = 0;
					break;
				}
			}
			else
				cnt=0;
			#if !DEBUG_MODE
			if(abs(Remote_receive_para.body_angle)>=BODY_SAFE_ANGLE )//超过安全角度
			{ 
				Remote_setting_para.ControlMode = eUpstairs;
				Remote_setting_para.CoordY = Coord_Base;//停止
				Control_para.Up_stairs_state = eUP_STAIRS_GO_BACK;//返回平面
				display_message((uint8_t *)" Over safe angle! go back!");
				break;
			}				
			switch(keyState)
			{
				case KEY_MODE_DOWN:
					Control_para.Driver_pause++;
					Control_para.Driver_pause = Control_para.Driver_pause&0x01;
					break;
				case KEY_COMF_DOWN:				
					if(Control_para.Driver_pause==0)Control_para.Driver_pause =1;
					Up_state = Control_para.Up_stairs_state;
					Control_para.Up_stairs_state = eUP_STAIRS_EXTI;
					
					break;
			}	
			if(Control_para.Driver_pause)
				Remote_setting_para.CoordY = Coord_Base;		
			else
				Remote_setting_para.CoordY = Coord_DOWN;//后退上楼						
			#endif
			break;	
			
		case eUP_STAIRS_TOP_UP:
			sprintf((char*)buff,"HL:%4d ",abs(Remote_receive_para.track_hall_cnt-track_hall_cnt_temp));
			display_string_8x16(70,32,0,buff);				

			Remote_setting_para.ControlMode = eTracks;
			if(abs(Remote_receive_para.track_hall_cnt-track_hall_cnt_temp)>=DISTANCE_HALL_400MM)//后退一段距离
			{					
				cnt++;
				if(cnt>SURE_TIMES)
				{
					Remote_setting_para.ControlMode = eUpstairs;
					Remote_setting_para.CoordY = Coord_Base;//停止上楼	
					display_message((uint8_t *)" Detected top stairs!");
					Control_para.Up_stairs_state = eUP_STAIRS_TOP_STAIRS;
					cnt = 0;
					break;
				}					
			}	
			#if !DEBUG_MODE
			switch(keyState)
			{
				case KEY_MODE_DOWN:
					Control_para.Driver_pause++;
					Control_para.Driver_pause = Control_para.Driver_pause&0x01;
					break;
				case KEY_COMF_DOWN:				
					if(Control_para.Driver_pause==0)Control_para.Driver_pause =1;
					Up_state = Control_para.Up_stairs_state;
					Control_para.Up_stairs_state = eUP_STAIRS_EXTI;
					break;
			}	
			if(Control_para.Driver_pause)
				Remote_setting_para.CoordY = Coord_Base;		
			else
				Remote_setting_para.CoordY = Coord_DOWN;//后退上楼						
			#endif			
			break;			
			
		case eUP_STAIRS_TOP_STAIRS:
			
			Remote_setting_para.ControlMode = eTracks;
			sprintf((char*)buff,"BY:%4d ",Remote_receive_para.back_ultrasonic_y);
			display_string_8x16(70,32,0,buff);				
					
			if(Remote_receive_para.back_ultrasonic_y < DISTANCE_GAP4)
			{
				cnt++;
				if(cnt>SURE_TIMES)
				{
					cnt=0;
					Remote_setting_para.ControlMode = eUpstairs;
					Remote_setting_para.CoordY = Coord_Base;//停止
					Control_para.Up_stairs_state = eUP_STAIRS_TOP_ACK;
					clear_screen();
					break;
				}					
			}
			else
			{
				cnt=0;
			}
			#if !DEBUG_MODE
			switch(keyState)
			{
				case KEY_MODE_DOWN:
					Control_para.Driver_pause++;
					Control_para.Driver_pause = Control_para.Driver_pause&0x01;
					break;
				case KEY_COMF_DOWN:				
					if(Control_para.Driver_pause==0)Control_para.Driver_pause =1;
					Up_state = Control_para.Up_stairs_state;
					Control_para.Up_stairs_state = eUP_STAIRS_EXTI;
					break;
			}	
			if(Control_para.Driver_pause)
				Remote_setting_para.CoordY = Coord_Base;		
			else
				Remote_setting_para.CoordY = Coord_DOWN;//后退上楼						
			#endif
			break;	
		case eUP_STAIRS_TOP_ADJ:
			Remote_setting_para.ControlMode = eTracks;
			ret = display_yes_no((uint8_t*)" Detected top of stairs adj finish?","Yes","No",keyState);
			if(ret == YES)//YES
			{
				Control_para.Up_stairs_state = eUP_STAIRS_TOP_ACK;
				Remote_setting_para.ControlMode = eUpstairs;
				clear_screen();	
			}		
		break;
		case eUP_STAIRS_TOP_ACK:
			ret = display_yes_no((uint8_t*)" Are you sure Detected top of stairs?","Yes","No",keyState);
			if(ret == YES)//YES
			{
				Control_para.Up_stairs_state = eUP_STAIRS_BACK_SUPPORT_DOWN;
				display_message((uint8_t *)"Back Support down!");
				body_angle_temp = Remote_receive_para.body_angle;
				
			}
			else if(ret == NO)//NO
			{
//				Control_para.Up_stairs_state = eUP_STAIRS_TOP_ADJ;
//				clear_screen();
			}
			break;					
			
		case eUP_STAIRS_BACK_SUPPORT_DOWN:	
			sprintf((char*)buff,"BA:%4d ",Remote_receive_para.body_angle);
			display_string_8x16(0,48,0,buff);				
		
			Remote_setting_para.ControlMode = eUpstairs; //摇杆失效
			Remote_trans_para.back_wheel_mode = eAngleMode;
		#if DRIVE_TYPE_NEW
			Remote_trans_para.body_angle = body_angle_temp-ANGLE_BODY_2;
			if(Remote_receive_para.body_angle <= body_angle_temp-ANGLE_BODY_1)//撑起2度确认支到平面		
		#else
			Remote_trans_para.body_angle = body_angle_temp+ANGLE_BODY_2P5;
			if(Remote_receive_para.body_angle >= body_angle_temp+ANGLE_BODY_2)//撑起2度确认支到平面
		#endif	
			{
				cnt++;
				if(cnt>SURE_TIMES)
				{
					display_message((uint8_t *)" top of stairs back!");
					Control_para.Up_stairs_state = eUP_STAIRS_TOP_BACK;
					track_hall_cnt_temp = Remote_receive_para.track_hall_cnt;		
					#if DRIVE_TYPE_NEW
					Remote_trans_para.back_wheel_mode = eSpeedMode;
					#endif
					cnt = 0;
					break;
				}
			}
			else
				cnt=0;			
			break;	
					
		case eUP_STAIRS_TOP_BACK:
			sprintf((char*)buff,"HL:%4d ",abs(Remote_receive_para.track_hall_cnt-track_hall_cnt_temp));
			display_string_8x16(70,32,0,buff);				

			Remote_setting_para.ControlMode = eTracks;
			if(abs(Remote_receive_para.track_hall_cnt-track_hall_cnt_temp)>=DISTANCE_HALL_900MM)//后退一段距离
			{					
				cnt++;
				if(cnt>SURE_TIMES)
				{
					Remote_setting_para.ControlMode = eUpstairs;
					Remote_setting_para.CoordY = Coord_Base;//停止上楼	
					display_message((uint8_t *)" Land top of stairs!");
					Control_para.Up_stairs_state = eUP_STAIRS_TOP_LAND_ACK;
					cnt = 0;
					break;
				}					
			}	
			#if !DEBUG_MODE
			switch(keyState)
			{
				case KEY_MODE_DOWN:
					Control_para.Driver_pause++;
					Control_para.Driver_pause = Control_para.Driver_pause&0x01;
					break;
				case KEY_COMF_DOWN:				
					if(Control_para.Driver_pause==0)Control_para.Driver_pause =1;
					Up_state = Control_para.Up_stairs_state;
					Control_para.Up_stairs_state = eUP_STAIRS_EXTI;
					break;
			}	
			if(Control_para.Driver_pause)
				Remote_setting_para.CoordY = Coord_Base;		
			else
				Remote_setting_para.CoordY = Coord_DOWN;//后退上楼						
			#endif			
			break;
		case eUP_STAIRS_TOP_LAND_ADJ:		
			Remote_setting_para.ControlMode = eTracks;
			ret = display_yes_no((uint8_t*)" Land top of stairs adj Finish?","Yes","No",keyState);
			if(ret == YES)//YES
			{
				Remote_setting_para.ControlMode = eUpstairs;
				display_message((uint8_t *)" Land top of stairs!");
				Control_para.Up_stairs_state = eUP_STAIRS_TOP_LAND_ACK;
			}			
				break;
	
		case eUP_STAIRS_TOP_LAND_ACK:
			ret = display_yes_no((uint8_t*)" Are you sure Land top of stairs?","Yes","No",keyState);
			if(ret == YES)//YES
			{
				Control_para.Up_stairs_state = eUP_STAIRS_TOP_LAND;
				display_message((uint8_t *)" back support up!");  
				Remote_trans_para.back_support_elongation = Remote_receive_para.back_support_elongation;
			}
			else if(ret == NO)//NO
			{
//				clear_screen();
//				Control_para.Up_stairs_state = eUP_STAIRS_TOP_LAND_ADJ;					
			}
			break;				
			
		case eUP_STAIRS_TOP_LAND:
			sprintf((char*)buff,"BE:%4d ",Remote_receive_para.back_support_elongation);
			display_string_8x16(70,32,0,buff);			

			Remote_setting_para.ControlMode = eUpstairs;	
			Remote_trans_para.back_wheel_mode = eElongationMode;
				Remote_trans_para.back_support_elongation = 0;
			if(Remote_receive_para.back_support_elongation<=ELONGATION_0)
			{
				cnt++;
				if(cnt>SURE_TIMES)
				{
					display_message((uint8_t *)" Up stairs finish!");
					Control_para.Up_stairs_state = eUP_STAIRS_FINISH;
					cnt = 0;
					break;
				}
			}
			else
				cnt=0;	
			break;
				
		case eUP_STAIRS_EXTI:
			
			ret = display_yes_no((uint8_t *)" Warning!Exti maybe dangerous!exti now?","Yes","No",keyState);
			if(ret == YES)//YES
			{
				Control_para.Up_stairs_state = eUP_STAIRS_FINISH;
				display_message((uint8_t *)"Up stairs finish!");
			}
			else if(ret == NO)//NO
			{
				if(Control_para.Driver_pause==1)Control_para.Driver_pause=0;
				Control_para.Up_stairs_state = Up_state;	
				display_message((uint8_t *)" Going upstairs!");
				display_exit_pause();
			}
			break;
		case eUP_STAIRS_GO_BACK:
			Remote_setting_para.ControlMode = eTracks;
			Remote_setting_para.CoordY = Coord_UP;//	前进下楼
			if(Remote_receive_para.body_angle <=ANGLE_BODY_1 &&  Remote_receive_para.body_angle>=ANGLE_BODY_N1)
			{
				cnt++;
				if(cnt>SURE_TIMES)
				{
					display_message((uint8_t *)" Up stairs finish!");
					Remote_setting_para.CoordY = Coord_Base;
					Remote_setting_para.ControlMode = eUpstairs;
					Control_para.Up_stairs_state = eUP_STAIRS_FINISH;//检测到梯顶平面
					cnt = 0;
					break;
				}				
			}
			else
				cnt=0;				
			break;
		case eUP_STAIRS_FINISH:		
			cnt++;
			if(cnt>SPEED_DELAY)
			{
				cnt=0;
				Remote_setting_para.ControlMode = eTracks;
				Remote_setting_para.OperationMode = eManual;
				Control_para.Up_stairs_state = eUP_STAIRS_START;
				setdisplay_ui(ePAGE_MAIN);//退出返回主页			
			}
			break;	
		case	eUP_STAIRS_OUT:
			Control_para.Up_stairs_state = eUP_STAIRS_START;
			Remote_setting_para.OperationMode = eManual;
			setdisplay_ui(ePAGE_MAIN);//退出返回主页			
			break;			
	}	

}


void display_downstairs_ui(void)
{

	uint8_t ret=0;

	static 	int16_t body_angle_temp=0;
	static 	int32_t track_hall_cnt_temp=0;				
	static 	uint8_t cnt=0;	
	static 	uint8_t delay=0;
	uint8_t keyState;
	uint8_t buff[20];

	keyState = bsp_GetKey();
	// if(keyState==KEY_POWER_LONG)
	if(keyState==KEY_POWER_DOWN)
	{
		Remote_setting_para.PowerStatus = ePowerOff;
		Remote_para_default();	
		lcd_PowerOff();
		setdisplay_ui(ePAGE_PWROFF);
	}	
	switch(Control_para.Down_stairs_state)
	{
		case eDOWN_STAIRS_START:
			ret = display_yes_no((uint8_t*)" Are you sure to go auto downstairs?","Yes","No",keyState);
					Remote_setting_para.CoordX = Coord_Base;
			Remote_setting_para.CoordY = Coord_Base;	
			if(ret == YES)//YES
			{
				Remote_setting_para.ControlMode=eDownstairs;
				Control_para.Down_stairs_state = eDOWN_STAIRS_READY;
				display_message((uint8_t *)" Ready to go downstairs!");
			}
			else if(ret == NO)//NO
			{
				Control_para.Down_stairs_state = eDOWN_STAIRS_OUT;		
				display_message((uint8_t *)" Down stairs out!");
			}
			break;
		case eDOWN_STAIRS_READY:
			
			Remote_setting_para.SpeedGear = eGearOne;
			Remote_setting_para.CoordY = Coord_Base;		
			Remote_trans_para.back_wheel_mode = eElongationMode;
			Remote_trans_para.fornt_wheel_mode = eElongationMode;
			Remote_trans_para.seat_wheel_mode = eAngleMode;
			Remote_trans_para.seat_angle=ANGLE_DEFUALT;
			Remote_trans_para.fornt_support_elongation = 0;
			Remote_trans_para.back_support_elongation = 0;
			sprintf((char*)buff,"FL:%4d ",Remote_receive_para.fornt_support_elongation);
			display_string_8x16(0,32,0,buff);		
			sprintf((char*)buff,"BL:%4d ",Remote_receive_para.back_support_elongation);
			display_string_8x16(70,32,0,buff);	
			sprintf((char*)buff,"SA:%5d ",Remote_receive_para.seat_angle);
			display_string_8x16(70,48,0,buff);		
			if(abs(Remote_receive_para.seat_angle -ANGLE_DEFUALT)<=ANGLE_SEAT_1 && Remote_receive_para.fornt_support_elongation<=ELONGATION_0 && Remote_receive_para.back_support_elongation<=ELONGATION_0 )
			{ 
				cnt++;
				if(cnt>SURE_TIMES)
				{
					Control_para.Down_stairs_state = eDOWN_STAIRS_FORNT;//前进下楼
					display_message((uint8_t *)" Going downstairs!");
					display_exit_pause();
					Remote_setting_para.ControlMode = eTracks;
					cnt = 0;
					break;
				}
			}
			else
				cnt=0;	
			break;
			
		case eDOWN_STAIRS_FORNT:
			
			Remote_setting_para.ControlMode = eTracks;

			sprintf((char*)buff,"FY:%4d ",Remote_receive_para.fornt_ultrasonic_y);
			display_string_8x16(70,32,0,buff);			
			if(Remote_receive_para.fornt_ultrasonic_y>=BACK_ULT_MIN_Y)
			{
				cnt++;
				if(cnt>=SURE_TIMES)
				{
					cnt = 0;
					Remote_setting_para.ControlMode = eDownstairs;
					Remote_setting_para.CoordY = Coord_Base;	//停止				
					Control_para.Down_stairs_state = eDOWN_STAIRS_FORNT_ACK;//确认到楼梯边缘
					display_message((uint8_t *)" Detected top of stairs!");
					break;
				}
			}
			else
				cnt=0;
			#if !DEBUG_MODE
			switch(keyState)
			{
				case KEY_MODE_DOWN:
					Control_para.Driver_pause++;
					Control_para.Driver_pause = Control_para.Driver_pause&0x01;
					break;
				case KEY_COMF_DOWN:				
					if(Control_para.Driver_pause==0)Control_para.Driver_pause =1;
					Control_para.Down_stairs_state = eDOWN_STAIRS_FORNT_EXTI;
					break;
				
			}	
			if(Control_para.Driver_pause)
				Remote_setting_para.CoordY = Coord_Base;		
			else
				Remote_setting_para.CoordY = Coord_UP;//			
			#endif
			break;
		case eDOWN_STAIRS_FORNT_EXTI:		
			Remote_setting_para.CoordY = Coord_Base;	//停止
			ret = display_yes_no((uint8_t *)" Warning!Exti maybe dangerous!exti now?","Yes","No",keyState);
			if(ret == YES)//YES
			{
				Control_para.Down_stairs_state = eDOWN_STAIRS_FINISH;
				display_message((uint8_t *)"Exti downstairs!");
			}
			else if(ret == NO)//NO
			{
				if(Control_para.Driver_pause==1)Control_para.Driver_pause=0;
				Control_para.Down_stairs_state = eDOWN_STAIRS_FORNT;	
				display_message((uint8_t *)" Going downstairs!");
				display_exit_pause();
			}
			break;			
		case eDOWN_STAIRS_FORNT_ACK:	

			ret = display_yes_no((uint8_t*)" Are you sure Detected top of stairs?","Yes","No",keyState);
			if(ret == YES)//YES
			{
#if DEBUG_BACK_DOWN	 
					Remote_setting_para.ControlMode = eDownstairs;
					Remote_setting_para.CoordY = Coord_Base;

					Control_para.Down_stairs_state = eDOWN_STAIRS_BACK_SUPPORT_DOWN;	
					display_message((uint8_t *)" back supprot down !");				
#else				
				Control_para.Down_stairs_state = eDOWN_STAIRS_FORNT_GO;
				track_hall_cnt_temp = Remote_receive_para.track_hall_cnt;
				display_message((uint8_t *)" top of stairs go on !");
#endif	
			}
			else if(ret == NO)//NO
			{
				Control_para.Down_stairs_state = eDOWN_STAIRS_FORNT;		
				display_message((uint8_t *)" Going downstairs!");
			}			
			break;
#if !DEBUG_BACK_DOWN	 			
		case eDOWN_STAIRS_FORNT_GO:		
			Remote_setting_para.ControlMode = eTracks;
			sprintf((char*)buff,"Y:%4d",abs(Remote_receive_para.track_hall_cnt-track_hall_cnt_temp));
			display_string_8x16(60,32,0,buff);		
			if(abs(Remote_receive_para.track_hall_cnt-track_hall_cnt_temp)>DISTANCE_HALL_150MM)
			{
				cnt++;
				if(cnt>SURE_TIMES)
				{
					Remote_setting_para.ControlMode = eDownstairs;
					Remote_setting_para.CoordY = Coord_Base;
					Control_para.Down_stairs_state = eDOWN_STAIRS_FORNT_SUPPORT_DOWN;	
					display_message((uint8_t *)" fornt supprot down!");		
					body_angle_temp = Remote_receive_para.body_angle;
					Remote_trans_para.fornt_support_elongation = Remote_receive_para.fornt_support_elongation;
					
					break;
				}
			}
			else
				cnt = 0;
		#if !DEBUG_MODE
			switch(keyState)
			{
				case KEY_MODE_DOWN:
					Control_para.Driver_pause++;
					Control_para.Driver_pause = Control_para.Driver_pause&0x01;
					break;
				case KEY_COMF_DOWN:				
					if(Control_para.Driver_pause==0)Control_para.Driver_pause =1;
					Control_para.Down_stairs_state = eDOWN_STAIRS_FORNT_EXTI;
					break;
				
			}	
			if(Control_para.Driver_pause)
				Remote_setting_para.CoordY = Coord_Base;		
			else
				Remote_setting_para.CoordY = Coord_UP;//			
			#endif
			break;
#endif
#if DEBUG_BACK_DOWN	 		
		case eDOWN_STAIRS_BACK_SUPPORT_DOWN:	
			Remote_setting_para.ControlMode = eDownstairs; 	
			Remote_trans_para.back_wheel_mode = eElongationMode;	
			Remote_trans_para.back_support_elongation = 3000;		

			sprintf((char*)buff,"BA:%4d ",Remote_receive_para.body_angle);
			display_string_8x16(70,32,0,buff);
			sprintf((char*)buff,"BL:%4d ",Remote_receive_para.back_support_elongation);
			display_string_8x16(70,48,0,buff);		
#if DRIVE_TYPE_NEW
			if(abs(Remote_receive_para.body_angle )>=ANGLE_BODY_40 || Remote_receive_para.back_support_elongation>=2900)	
#else		
			if(abs(Remote_receive_para.body_angle -ANGLE_BODY_30)<=ANGLE_BODY_P5 || Remote_receive_para.back_support_elongation>=3000)
#endif
			{
				cnt++;
				if(cnt>SURE_TIMES) 
				{					
					Control_para.Down_stairs_state = eDOWN_STAIRS_GO_DOWN_ACK;
					display_message((uint8_t *)" going down!");
					track_hall_cnt_temp = Remote_receive_para.track_hall_cnt;
					Remote_trans_para.body_angle = Remote_receive_para.body_angle;
					Remote_trans_para.back_wheel_mode  = eAngleMode;
					cnt=0;
					break;
				}
			}
			else
				cnt=0;
			break;
		case eDOWN_STAIRS_GO_DOWN_ACK:
			
			ret = display_yes_no((uint8_t*)" Are you sure going down stairs?","Yes","No",keyState);
			if(ret == YES)//YES
			{
				Control_para.Down_stairs_state = eDOWN_STAIRS_GO_DOWN;
				Remote_setting_para.ControlMode = eTracks;
				track_hall_cnt_temp = Remote_receive_para.track_hall_cnt;
				display_message((uint8_t *)" going down!");
			}
			else if(ret == NO)//NO
			{
				Control_para.Down_stairs_state = eDOWN_STAIRS_BACK_SUPPORT_DOWN;		
				display_message((uint8_t *)" back supprot down !");
			}			
			break;	
			
		case eDOWN_STAIRS_GO_DOWN:
			sprintf((char*)buff,"HL:%4d ",abs(Remote_receive_para.track_hall_cnt-track_hall_cnt_temp));
			display_string_8x16(70,32,0,buff);		
			
		if(abs(Remote_receive_para.track_hall_cnt-track_hall_cnt_temp)>= DISTANCE_HALL_300MM)//
		{
			cnt++;
			if(cnt>SURE_TIMES)
			{
				display_message((uint8_t *)" back supprot up!");
				Remote_trans_para.back_support_elongation = Remote_receive_para.back_support_elongation;
				Control_para.Down_stairs_state = eDOWN_STAIRS_BACK_SUPPORT_UP;
				Remote_setting_para.ControlMode = eDownstairs;
				break;				
			}
		}
		else
			cnt=0;
		#if !DEBUG_MODE
		switch(keyState)
		{
			case KEY_MODE_DOWN:
				Control_para.Driver_pause++;
				Control_para.Driver_pause = Control_para.Driver_pause&0x01;
				break;
			case KEY_COMF_DOWN:				
				if(Control_para.Driver_pause==0)Control_para.Driver_pause =1;
				Control_para.Down_stairs_state = eDOWN_STAIRS_FORNT_EXTI;
				break;
			
		}	
		if(Control_para.Driver_pause)
			Remote_setting_para.CoordY = Coord_Base;		
		else
			Remote_setting_para.CoordY = Coord_UP;//			
		#endif
		break;
		case eDOWN_STAIRS_BACK_SUPPORT_UP:	
			Remote_trans_para.back_wheel_mode = eElongationMode;
			Remote_trans_para.back_support_elongation = 0;		
			sprintf((char*)buff,"BE:%4d ",Remote_receive_para.back_support_elongation);
			display_string_8x16(70,32,0,buff);	
		if(Remote_receive_para.back_support_elongation<=ELONGATION_0)
			{
				cnt++;
				if(cnt>SURE_TIMES)
				{
					Remote_setting_para.CoordY = Coord_Base;	
					display_message((uint8_t *)" down stairs finish!");
					Control_para.Down_stairs_state = eDOWN_STAIRS_FINISH;
					Remote_setting_para.ControlMode = eTracks;
					cnt = 0;
					break;
				}
			}
			else
				cnt=0;			
			break;
	#else			
		case eDOWN_STAIRS_FORNT_SUPPORT_DOWN:		
			#if DEBUG_MODE 
			Remote_setting_para.ControlMode = eSupportFornt;
			Remote_trans_para.fornt_wheel_mode = eSpeedMode;
			sprintf((char*)buff,"A:%4d",body_angle_temp - Remote_receive_para.body_angle);
			display_string_8x16(60,32,0,buff);			
		
			
			#else
			Remote_setting_para.ControlMode = eDownstairs;
			if(Remote_trans_para.fornt_support_elongation < Support_elongationMAX)
				Remote_trans_para.fornt_support_elongation +=10;
			else			
				Remote_trans_para.fornt_support_elongation = Support_elongationMAX;
			#endif
			#if DRIVE_TYPE_NEW
			if(Remote_receive_para.body_angle >= (body_angle_temp+ANGLE_BODY_1))//撑起1度确认到楼梯
			#else
			if(Remote_receive_para.body_angle <= (body_angle_temp-ANGLE_BODY_1))//撑起1度确认到楼梯
			#endif
			{
				cnt++;
				if(cnt>SURE_TIMES)
				{
					display_message((uint8_t *)" top of stairs fornt!");
					Control_para.Down_stairs_state = eDOWN_STAIRS_TOP_FORNT;
					body_angle_temp = Remote_receive_para.body_angle;
					track_hall_cnt_temp = Remote_receive_para.track_hall_cnt;
					Remote_setting_para.ControlMode = eTracks;
					display_exit_pause();	
					cnt = 0;
				}
			} 
			else
				cnt=0;			
			break;
			
			
		case eDOWN_STAIRS_TOP_FORNT:
	sprintf((char*)buff,"X:%4d",abs(Remote_receive_para.track_hall_cnt-track_hall_cnt_temp));
	display_string_8x16(0,32,0,buff);			
	sprintf((char*)buff,"Y:%4d",Remote_receive_para.body_angle - body_angle_temp);
	display_string_8x16(60,32,0,buff);				
			
	#if DRIVE_TYPE_NEW
	if(Remote_receive_para.body_angle <= (body_angle_temp-ANGLE_BODY_2)  || (abs(Remote_receive_para.track_hall_cnt-track_hall_cnt_temp)>= DISTANCE_HALL_300MM))//到平衡点，下降2度
		
	#else	
			if(Remote_receive_para.body_angle >= (body_angle_temp+ANGLE_BODY_2)  || (abs(Remote_receive_para.track_hall_cnt-track_hall_cnt_temp)>= DISTANCE_HALL_300MM))//到平衡点，下降2度
		#endif
			{
				cnt++;
				if(cnt>SURE_TIMES)
				{
					Remote_setting_para.ControlMode = eUpstairs;
					Remote_setting_para.CoordY = Coord_Base;					
					Control_para.Down_stairs_state = eDOWN_STAIRS_FORNT_SUPPORT_UP_ACK;
					clear_screen();
					cnt = 0;
					break;
				}
			}
			else
				cnt=0;
			#if !DEBUG_MODE
			if(Remote_receive_para.body_angle >=BODY_SAFE_ANGLE)
			{
				Remote_setting_para.CoordY = Coord_Base;
				Control_para.Down_stairs_state = eDOWN_STAIRS_GO_BACK_FORNT_SUPPORT_DOWN;
				body_angle_temp = Remote_receive_para.body_angle ;
				Remote_trans_para.fornt_support_elongation = Remote_receive_para.fornt_support_elongation;
				display_message((uint8_t *)" Over safe angle! go back!");
				break;
			}			
			switch(keyState)
			{
				case KEY_MODE_DOWN:
					Control_para.Driver_pause++;
					Control_para.Driver_pause = Control_para.Driver_pause&0x01;
					break;
				case KEY_COMF_DOWN:				
					if(Control_para.Driver_pause==0)Control_para.Driver_pause =1;
					Control_para.Down_stairs_state = eDOWN_STAIRS_TOP_FORNT_EXTI;
					break;
			}	
			if(Control_para.Driver_pause)
				Remote_setting_para.CoordY = Coord_Base;		
			else
				Remote_setting_para.CoordY = Coord_UP;//						
						
			#endif
			break;
		case eDOWN_STAIRS_FORNT_SUPPORT_UP_ACK:
			ret = display_yes_no((uint8_t*)" Are you sure fornt support up?","Yes","No",keyState);
			if(ret == YES)//YES
			{
				Control_para.Down_stairs_state = eDOWN_STAIRS_FORNT_SUPPORT_UP;
				display_message((uint8_t *)" fornt support up!");
			}
			else if(ret == NO)//NO
			{
				Control_para.Down_stairs_state = eDOWN_STAIRS_TOP_FORNT;		
				display_message((uint8_t *)" top of stairs fornt!");
				body_angle_temp = Remote_receive_para.body_angle;
			}			
			break;
			
		case eDOWN_STAIRS_TOP_FORNT_EXTI:			
			ret = display_yes_no((uint8_t *)" Warning!Exti maybe dangerous!exti now?","Yes","No",keyState);
			if(ret == YES)//YES
			{
				Control_para.Down_stairs_state = eDOWN_STAIRS_FINISH;
				display_message((uint8_t *)"Exti downstairs!");
			}
			else if(ret == NO)//NO
			{
				if(Control_para.Driver_pause==1)Control_para.Driver_pause=0;
				Control_para.Down_stairs_state = eDOWN_STAIRS_TOP_FORNT;	
				display_message((uint8_t *)" top of stairs fornt!");
				display_exit_pause();
			}		break;						
		case eDOWN_STAIRS_FORNT_SUPPORT_UP:
			#if DEBUG_MODE
			Remote_setting_para.ControlMode = eSupportFornt;
			Remote_trans_para.fornt_wheel_mode = eSpeedMode;
	sprintf((char*)buff,"Y:%5d",Remote_receive_para.fornt_support_elongation);
	display_string_8x16(60,32,0,buff);			
	
						
			#else
			Remote_setting_para.ControlMode = eDownstairs;
			Remote_trans_para.fornt_wheel_mode = eElongationMode;
			if(Remote_trans_para.fornt_support_elongation >ELONGATION_0)
				Remote_trans_para.fornt_support_elongation -=10;
			else			
				Remote_trans_para.fornt_support_elongation = 0;
			
			if(abs(Remote_receive_para.body_angle) >=BODY_SAFE_ANGLE)
			{
				Control_para.Down_stairs_state = eDOWN_STAIRS_GO_BACK_FORNT_SUPPORT_DOWN;
				display_message((uint8_t *)" Over safe angle! go back!");
				body_angle_temp = Remote_receive_para.body_angle ;
				Remote_trans_para.fornt_support_elongation = Remote_receive_para.fornt_support_elongation;
				Remote_setting_para.ControlMode = eTracks;
				break;
			}		
			#endif
			if(Remote_receive_para.fornt_support_elongation<=ELONGATION_0)
			{
				Control_para.Down_stairs_state = eDOWN_STAIRS_FINISH;// eDOWN_STAIRS_FORNT_DOWN;
				Remote_setting_para.ControlMode = eDownstairs;
				display_message((uint8_t *)" down stairs finish!");
				break;
			}			
			break;
			
	#endif		
//		case eDOWN_STAIRS_FORNT_DOWN_EXTI:			
//			ret = display_yes_no((uint8_t *)" Warning!Exti maybe dangerous!exti now?","Yes","No",keyState);
//			if(ret == YES)//YES
//			{
//				Control_para.Down_stairs_state = eDOWN_STAIRS_FINISH;
//				display_message((uint8_t *)"Exti downstairs!");
//			}
//			else if(ret == NO)//NO
//			{
//				if(Control_para.Driver_pause==1)Control_para.Driver_pause=0;
//				Control_para.Down_stairs_state = eDOWN_STAIRS_FORNT_DOWN;
//				display_message((uint8_t *)" Downing stairs!");
//				display_exit_pause();
//			}
//			break;			

		case eDOWN_STAIRS_GO_BACK_FORNT_SUPPORT_DOWN:
			Remote_setting_para.ControlMode = eDownstairs;
			Remote_trans_para.fornt_support_elongation += 10;
		#if DRIVE_TYPE_NEW
				if(Remote_receive_para.body_angle >= body_angle_temp+ANGLE_BODY_2)//撑起2度	
		#else
			if(Remote_receive_para.body_angle <= body_angle_temp-ANGLE_BODY_2)//撑起2度
		#endif
			{
				cnt++;
				if(cnt>SURE_TIMES)
				{
					Control_para.Down_stairs_state = eDOWN_STAIRS_TRACKS_GO_BACK;
					track_hall_cnt_temp = Remote_receive_para.track_hall_cnt;
					Remote_setting_para.ControlMode = eTracks;
					display_message((uint8_t *)" track go back!");
					cnt = 0;
					break;
				}
			}
			else
				cnt=0;			
			break;		
		case eDOWN_STAIRS_TRACKS_GO_BACK:
			Remote_setting_para.CoordY = Coord_DOWN;
			if(abs(Remote_receive_para.track_hall_cnt - track_hall_cnt_temp)>=DISTANCE_HALL_200MM)
			{
				Remote_setting_para.CoordY = Coord_Base;
				Control_para.Down_stairs_state = eDOWN_STAIRS_TRACKS_GO_BACK_ACK;
			}
			break;
			
		case eDOWN_STAIRS_TRACKS_GO_BACK_ACK:	
			ret = display_yes_no((uint8_t*)" Are you sure go back top of stairs?","Yes","No",keyState);
			if(ret == YES)//YES
			{
				Control_para.Down_stairs_state = eDOWN_STAIRS_FINISH;
				display_message((uint8_t *)"over safe go back finish!");
			}
			else if(ret == NO)//NO
			{
					Control_para.Down_stairs_state = eDOWN_STAIRS_TRACKS_GO_BACK;
					track_hall_cnt_temp = Remote_receive_para.track_hall_cnt;
					display_message((uint8_t *)" track go back!");
			}			
			break;		
		case eDOWN_STAIRS_FINISH:			
			cnt++;
			if(cnt>SPEED_DELAY)
			{
				cnt=0;
				Remote_setting_para.CoordY = Coord_Base;	
				Remote_setting_para.ControlMode = eTracks;
				Remote_setting_para.OperationMode = eManual;
				Control_para.Down_stairs_state = eDOWN_STAIRS_START;
				setdisplay_ui(ePAGE_MAIN);//退出返回主页			
			}
			break;	
		case	eDOWN_STAIRS_OUT:
			Control_para.Down_stairs_state = eDOWN_STAIRS_START;			
			Remote_setting_para.OperationMode = eManual;
			setdisplay_ui(ePAGE_MAIN);//退出返回主页
			break;
			
	}	
}
#endif

void display_pwroff_ui(void)
{
	uint8_t keyState;
	keyState = bsp_GetKey();
	if(keyState==KEY_POWER_DOWN)
	{		
		Remote_para_init();
		Remote_buff_init();
		mem_read();				
		lcd_PowerOn();		
		setdisplay_ui(ePAGE_MAIN);
		Remote_setting_para.PowerStatus = ePowerOn;	
		//开机发送信号量 用于订阅消息发送
		MasterMsgData_t message;
		api_on_message_received(message);
	}		
}
typedef enum 
{
	SET_LampSwitch,
	SET_VoiceSwitch,
	SET_AlarmSwitch,
	SET_VoiceLevel,
	SET_CruiseCtrlSwitch,
	SET_MUSICPLAY,
	SET_SPEED,
	SET_main,		
	SET_main_1,
	SET_Save,
	SET_Finish,

}setting_e;


void display_setting_ui(void)
{
	static uint8_t idx=SET_main,idx_temp,speedLongPressFlag = 0;
	static uint8_t menu_sel=0,chang_sel=0,cnt=0;
	uint8_t keyState;
	uint8_t buffer[40] = {0};
//	static uint8_t ret=0xff;
	
	keyState = bsp_GetKey();
	// if(keyState==KEY_POWER_LONG)
	if(keyState==KEY_POWER_DOWN)
	{
		Remote_setting_para.PowerStatus = ePowerOff;
		Remote_para_default();	
		lcd_PowerOff();
		setdisplay_ui(ePAGE_PWROFF);
	}	
	display_select_enter_exit();	
	switch(idx)
	{
		case SET_main:
			if(menu_sel<4)
			{
				display_string_8x16(0,0,menu_sel==0,(uint8_t*)"LampSwitch");				
				display_string_8x16(0,16,menu_sel==1,(uint8_t*)"VoiceSwitch");
				display_string_8x16(0,32,menu_sel==2,(uint8_t*)"AlarmSwitch");
				display_string_8x16(0,48,menu_sel==3,(uint8_t*)"VoiceLevel");
			}
			else
			{
				idx = SET_main_1;
				clear_screen();		
				break;
				
			}
			if(keyState==KEY_SPEED_SUB_DOWN)
			{
				if(menu_sel<4)menu_sel++;
			}
			if(keyState==KEY_SPEED_ADD_DOWN )
			{
				if(menu_sel)menu_sel--;
				else menu_sel = SET_main - 1;
			}			
			if(keyState==KEY_MENU_ESC_DOWN)//退出
			{
				display_main_reflash();
				setdisplay_ui(ePAGE_MAIN);
				idx = SET_main;
				menu_sel = 0;
			}		
			else if(keyState==KEY_MENU_ENTER_DOWN)
			{
				idx = menu_sel;
				if(idx==SET_LampSwitch)
				 chang_sel = 	Remote_setting_para.LampSwitch ;
				else if(idx==SET_VoiceSwitch)	
				 chang_sel = 	Remote_setting_para.VoiceSwitch ;
				else if(idx==SET_AlarmSwitch)	
				 chang_sel = 	Remote_setting_para.AlarmSwitch ;
				else if(idx==SET_VoiceLevel)	
					chang_sel = Remote_setting_para.VoiceLevel;
				else if(idx==SET_CruiseCtrlSwitch)	
					chang_sel = Remote_setting_para.CruiseCtrlSwitch ;									
				else if(idx==SET_MUSICPLAY)	
					chang_sel = Remote_setting_para.CurrentMusic ;									
				clear_screen();
			}
			break;
		case SET_main_1:
			display_string_8x16(0,0,menu_sel==4,(uint8_t*)"CruiseCtrlSwitch");
			display_string_8x16(0,16,menu_sel==5,(uint8_t*)"MusicSelect");
			display_string_8x16(0,32,menu_sel==6,(uint8_t*)"SpeedChange");

			if(keyState==KEY_SPEED_ADD_DOWN )
			{
				if(menu_sel)menu_sel--;
			}else if(keyState==KEY_SPEED_SUB_DOWN)
			{
				menu_sel++;
				if(menu_sel>6) menu_sel = 0;
			}
			else if(keyState==KEY_MENU_ESC_DOWN)//退出
			{
				display_main_reflash();
				setdisplay_ui(ePAGE_MAIN);
				menu_sel = 0;
				break;
			}		
			else if(keyState==KEY_MENU_ENTER_DOWN)
			{
				idx = menu_sel;
				if (menu_sel == SET_VoiceLevel){
					chang_sel = Remote_setting_para.VoiceLevel;
				}
				clear_screen();
			}						
			if(menu_sel<4)
			{
				idx = SET_main;
				clear_screen();
				break;
			}
			break;
		case SET_LampSwitch:		
		case SET_VoiceSwitch:
		case SET_AlarmSwitch:	
		// case SET_VoiceLevel:	
		case SET_CruiseCtrlSwitch:
		// case SET_MUSICPLAY:
			if(idx==SET_LampSwitch) 
				 display_sel_3("   LampSwitch","OFF","ON",NULL,chang_sel);
			else if(idx==SET_VoiceSwitch)	
				 display_sel_3("   VoiceSwitch","OFF","ON",NULL,chang_sel);
			else if(idx==SET_AlarmSwitch)	
				 display_sel_3("   AlarmSwitch","OFF","ON",NULL,chang_sel);
			// else if(idx==SET_VoiceLevel)				
				//  display_sel_3("   VoiceLevel","low","mid","high",chang_sel-1);
			else if(idx==SET_CruiseCtrlSwitch)	
			 display_sel_3("CruiseCtrlSwitch","OFF","ON",NULL,chang_sel);
			// else if(idx==SET_MUSICPLAY)	
			//  display_sel_3("MusicSelect","1","2","3",chang_sel-1);
			#if 0
			if(idx==SET_VoiceLevel || idx == SET_MUSICPLAY)	
			{
				if(keyState==KEY_SPEED_SUB_DOWN)
				{
					chang_sel++;
					if(chang_sel>3)chang_sel=1;
					
				}
				if(keyState==KEY_SPEED_ADD_DOWN )
				{					
					chang_sel--;	
					if(chang_sel<1)chang_sel=3;
				}
			}
			else
			#endif
			{
				if(keyState==KEY_SPEED_SUB_DOWN)
				{
					chang_sel++;
					if(chang_sel>1)chang_sel=0;
					
				}
				if(keyState==KEY_SPEED_ADD_DOWN )
				{					
					chang_sel--;	
					if(chang_sel>1)chang_sel=1;
				}			
			}

			if(keyState==KEY_MENU_ESC_DOWN)//退出
			{
				idx = SET_main;
				clear_screen();
			}		
			else if(keyState==KEY_MENU_ENTER_DOWN)
			{
				idx_temp = idx;
				idx=SET_Save;				
			}		
			break;
		case SET_MUSICPLAY:
				center_display_string(0,0,(uint8_t*)"MusicSelect");				
				sprintf(buffer," %d ",Remote_setting_para.CurrentMusic);
				center_display_string(24,1,(uint8_t*)buffer);				
				if(keyState == KEY_SPEED_SUB_DOWN || keyState == KEY_SPEED_SUB_LONG || (speedLongPressFlag&0x01)) {
					if(keyState == KEY_SPEED_SUB_LONG || keyState == KEY_SPEED_SUB_UP) 
						speedLongPressFlag ^= 0x01;

					if(Remote_setting_para.CurrentMusic==0)Remote_setting_para.CurrentMusic=30;
					else Remote_setting_para.CurrentMusic--;	
				}
				if(keyState==KEY_SPEED_ADD_DOWN || keyState == KEY_SPEED_ADD_LONG || (speedLongPressFlag&0x02)) {					
					if(keyState == KEY_SPEED_ADD_LONG || keyState == KEY_SPEED_ADD_UP)
						speedLongPressFlag ^= 0x02;

					Remote_setting_para.CurrentMusic++;
					if(Remote_setting_para.CurrentMusic>30)Remote_setting_para.CurrentMusic=0;
				}
				if(keyState==KEY_MENU_ESC_DOWN)//退出
				{
					idx = SET_main;
					clear_screen();
				}		
			break;
		case SET_VoiceLevel:	
				center_display_string(0,0,(uint8_t*)"VoiceLevel");				
				sprintf(buffer," %d ",chang_sel);
				center_display_string(24,1,(uint8_t*)buffer);				
				if(keyState == KEY_SPEED_SUB_DOWN || keyState == KEY_SPEED_SUB_LONG || (speedLongPressFlag&0x01)) {
					if(keyState == KEY_SPEED_SUB_LONG || keyState == KEY_SPEED_SUB_UP) 
						speedLongPressFlag ^= 0x01;

					if(chang_sel<1)chang_sel=30;
					else chang_sel--;	
					
				}
				if(keyState==KEY_SPEED_ADD_DOWN || keyState == KEY_SPEED_ADD_LONG || (speedLongPressFlag&0x02)) {					
					if(keyState == KEY_SPEED_ADD_LONG || keyState == KEY_SPEED_ADD_UP) 
						speedLongPressFlag ^= 0x02;

					chang_sel++;
					if(chang_sel>30)chang_sel=0;

				}
				if(keyState==KEY_MENU_ESC_DOWN)//退出
				{
					if( chang_sel != Remote_setting_para.VoiceLevel){
						Remote_setting_para.VoiceLevel = chang_sel;
						play_set_volume(chang_sel);
					}
					idx = SET_main;
					clear_screen();
				}		
				else if(keyState==KEY_MENU_ENTER_DOWN)
				{
					idx_temp = idx;
					idx=SET_Save;				
				}
			break;
		case SET_SPEED:

			center_display_string(0,0,(uint8_t*)"SpeedGaer");				
			sprintf(buffer," %d ",Remote_setting_para.SpeedGear);
			center_display_string(16,0,(uint8_t*)buffer);				

			display_string_8x16(2*8,32,0,(uint8_t*)"Speed");

			sprintf(&buffer[10]," %d ",walking_speed[Remote_setting_para.SpeedGear-1]);
			display_string_8x16(8*8,32,1,(uint8_t*) &buffer[10]);
			display_string_5x8(8*8,42,0,(uint8_t*) &buffer[10]);

			if(keyState == KEY_SPEED_SUB_DOWN || keyState == KEY_SPEED_SUB_LONG || (speedLongPressFlag&0x01)) {
				if(keyState == KEY_SPEED_SUB_LONG || keyState == KEY_SPEED_SUB_UP) 
					speedLongPressFlag ^= 0x01;
				walking_speed[Remote_setting_para.SpeedGear-1]--;
			}
			if(keyState==KEY_SPEED_ADD_DOWN || keyState == KEY_SPEED_ADD_LONG || (speedLongPressFlag&0x02)) {					
				if(keyState == KEY_SPEED_ADD_LONG || keyState == KEY_SPEED_ADD_UP) 
					speedLongPressFlag ^= 0x02;
				walking_speed[Remote_setting_para.SpeedGear-1]++;
			}
			if(keyState==KEY_MENU_ESC_DOWN)//退出
			{
				idx = SET_main;
				clear_screen();
			}		
			// else if(keyState==KEY_LAMP_DOWN)
			// {
			// 	idx_temp = idx;
			// 	idx=SET_Save;				
			// }
			break;


		case SET_Save:
			display_string_8x16(16,16,1,(uint8_t*)"Save Set?");
	
			if(keyState==KEY_MENU_ESC_DOWN)//退出
			{
				idx = idx_temp;
				display_string_8x16(16,16,0,(uint8_t*)"         ");
			}		
			else if(keyState==KEY_MENU_ESC_DOWN)
			{
				if(idx_temp==SET_LampSwitch)
					Remote_setting_para.LampSwitch = chang_sel;
				else if(idx_temp==SET_VoiceSwitch)	
					Remote_setting_para.VoiceSwitch = chang_sel;
				else if(idx_temp==SET_AlarmSwitch)	
					Remote_setting_para.AlarmSwitch = chang_sel;
				else if(idx_temp==SET_VoiceLevel)	{
					Remote_setting_para.VoiceLevel = chang_sel;
					play_set_volume(Remote_setting_para.VoiceLevel);
				}
				else if(idx_temp==SET_CruiseCtrlSwitch)	
					Remote_setting_para.CruiseCtrlSwitch = chang_sel;	
					mem_save();
				idx=SET_Finish;				
				clear_screen();
			}			
			break;
		case SET_Finish:
			display_string_8x16(0,16,1,(uint8_t*)"Save Set Finish!");
			cnt++;
			if(cnt>20)
			{
				idx = SET_main;
				clear_screen();		
				cnt=0;
			}
			break;			
	}	
	
}


/*设置显示界面*/
void setdisplay_ui(ui_e	ui)
{	
	clear_screen();
	page_num = ui;
	if(page_num == ePAGE_MAIN)
		display_main_reflash();		
}

uint8_t getdisplay_ui(void)
{	
	return page_num ;
}


void app_display(void)
{
	switch(page_num)
	{
		case ePAGE_MAIN:
			display_main_ui();
			break;
		case ePAGE_SETTING:
			display_setting_ui();	
			break;
		#if 0
		case ePAGE_UPSTAIR:
			display_upstairs_ui();
			break;		
		case ePAGE_DOWNSTAIR:
			display_downstairs_ui();
			break;
		#endif
		case ePAGE_PWROFF:
			display_pwroff_ui();
			break;
		default:
			break;
	}
	lcd_refreshGram();
}
#endif

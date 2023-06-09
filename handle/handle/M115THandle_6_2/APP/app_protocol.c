#include "app_protocol.h"
#include "ext.h"
#include "math.h"
#include "string.h"



 TcomData		Remote_trans_buff;
 TcomData		Remote_receive_buff;
 

 TcomData		Phone_trans_buff;
 TcomData		Phone_receive_buff; 


// 5档速度
int16_t walking_speed[5] = {
	500,1300,2000,2800,3500
};

void Remote_buff_init(void)
{

	memset(&Remote_trans_buff,0,sizeof(Remote_trans_buff));
	memset(&Remote_receive_buff,0,sizeof(Remote_receive_buff));
	
}




/**
  * @brief  计算校验和
  * @param  无
  * @retval 无
  */
uint8_t ProtocolCheckSum(uint8_t *pbuf, uint16_t len)
{
	static uint16_t sum, i;
  sum = 0;
	for (i=0; i<len; i++) sum += pbuf[i];
	return (uint8_t)(sum&0xff);
}

//手柄到驱动器
#if !USING_CAN
void ProtocolTransbuff(TCOMMUN* p)
{
	p->Data[0] = Remote_trans_para.track_speed_L &0xff;
	p->Data[1] = (Remote_trans_para.track_speed_L>>8) &0xff;
	p->Data[2] = Remote_trans_para.track_speed_R &0xff;
	p->Data[3] = (Remote_trans_para.track_speed_R>>8) &0xff;	
	
	p->Data[4] = Remote_trans_para.walking_speed_L &0xff;
	p->Data[5] = (Remote_trans_para.walking_speed_L>>8) &0xff;
	p->Data[6] = Remote_trans_para.walking_speed_R &0xff;
	p->Data[7] = (Remote_trans_para.walking_speed_R>>8) &0xff;	

	
	p->Data[8] = Remote_trans_para.fornt_wheel_mode | (Remote_trans_para.back_wheel_mode<<2) | (Remote_trans_para.seat_wheel_mode<<4);		
	switch(Remote_trans_para.fornt_wheel_mode)
	{		
		case eSpeedMode:
		p->Data[9] = Remote_trans_para.fornt_support_speed &0xff;
		p->Data[10] = (Remote_trans_para.fornt_support_speed>>8) &0xff;
		break;	
		case eElongationMode:
		p->Data[9] = Remote_trans_para.fornt_support_elongation &0xff;
		p->Data[10] = (Remote_trans_para.fornt_support_elongation>>8) &0xff;
		break;	
	}
	switch(Remote_trans_para.back_wheel_mode)
	{		
		case eSpeedMode:
		p->Data[11] = Remote_trans_para.back_support_speed &0xff;
		p->Data[12] = (Remote_trans_para.back_support_speed>>8) &0xff;
		break;	
		case eElongationMode:
		p->Data[11] = Remote_trans_para.back_support_elongation &0xff;
		p->Data[12] = (Remote_trans_para.back_support_elongation>>8) &0xff;
		break;	
		case eAngleMode:
		p->Data[11] = Remote_trans_para.body_angle &0xff;
		p->Data[12] = (Remote_trans_para.body_angle>>8) &0xff;
		break;		
	}	
	switch(Remote_trans_para.seat_wheel_mode)
	{		
		case eSpeedMode:
		p->Data[13] = Remote_trans_para.seat_speed &0xff;
		p->Data[14] = (Remote_trans_para.seat_speed>>8) &0xff;
		break;	
		case eElongationMode:
		p->Data[13] = Remote_trans_para.seat_elongation &0xff;
		p->Data[14] = (Remote_trans_para.seat_elongation>>8) &0xff;
		break;	
		case eAngleMode:
		p->Data[13] = Remote_trans_para.seat_angle &0xff;
		p->Data[14] = (Remote_trans_para.seat_angle>>8) &0xff;
		break;		
	}		
	p->DataLen = 15;
	p->Cmd = CMD_SetCtrl;
	p->Count++;
}

#else


#define ADD_WRITE_REG(para,regName){\
	u8 count = 0;\
	do {\
		(txbuffer)[txbufferIndex++] = (((para) >> (8*count))& 0xff);\
	} while (++count < sizeof((para)));\
	combine_wirte->combine_data[combine_wirte->combineCount].combine.address = \
	tranRegInfoDir[(regName)].addr;\
	combine_wirte->combine_data[combine_wirte->combineCount].combine.size = \
	tranRegInfoDir[(regName)].size;\
	combine_wirte->combine_data[combine_wirte->combineCount].data = \
	&txbuffer[txbufferIndex-tranRegInfoDir[(regName)].size];\
	++combine_wirte->combineCount;\
	}


//手柄->CAN->驱动器	每10ms发送一次
void CANProtocolTransbuff(combine_write_t* combine_wirte,u8 *txbuffer){
	u8 txbufferIndex = 0;
	static u8 pushTimeCount = 0;
	static u16 foldTimeCount = 0;
	static u8 heartTimeCount = 0;
	combine_wirte->combineCount = 0;
	//左轮速度
	if(Remote_receive_para.walking_speed_L != Remote_trans_para.walking_speed_L){
		ADD_WRITE_REG(Remote_trans_para.walking_speed_L,BLDC_A_SPEED_SET);
	}
	//右轮速度
	if(Remote_receive_para.walking_speed_R != Remote_trans_para.walking_speed_R){
		ADD_WRITE_REG(Remote_trans_para.walking_speed_R,BLDC_U_SPEED_SET);
	}
	//折叠状态
	if(Remote_receive_para.folding_state != Remote_trans_para.folding_state){
		++foldTimeCount;
		if(foldTimeCount == 1){
			ADD_WRITE_REG((u8)0x01,BRUSH_A_ENABLE);
			ADD_WRITE_REG((u8)0x07,BRUSH_A_MODE);
			ADD_WRITE_REG((int16_t)300,BRUSH_A_SPEED_SET);

			ADD_WRITE_REG((u8)0x01,BRUSH_U_ENABLE);
			ADD_WRITE_REG((u8)0x07,BRUSH_U_MODE);
			ADD_WRITE_REG((int16_t)-150,BRUSH_U_SPEED_SET);
		}else if(foldTimeCount > 600 && //6秒后在进行速度判断
		Remote_receive_para.push_rod_speed == 0 &&
		Remote_receive_para.seat_rod_speed == 0){
			Remote_receive_para.folding_state = Remote_trans_para.folding_state;
			foldTimeCount = 0;
		}else if(foldTimeCount > 3000){//30秒后还未停止
			ADD_WRITE_REG((u8)0x00,BRUSH_A_ENABLE);
			ADD_WRITE_REG((u8)0x00,BRUSH_U_ENABLE);
			Remote_receive_para.folding_state = Remote_trans_para.folding_state;
			foldTimeCount = 0;
		}
	}else if(Remote_receive_para.push_rod_speed != Remote_trans_para.push_rod_speed &&
			++pushTimeCount > 5){
	//推杆速度 50ms发送一次
			pushTimeCount = 0;
			if(Remote_trans_para.push_rod_speed == 0){
				ADD_WRITE_REG((u8)0x06,BRUSH_A_MODE);
				ADD_WRITE_REG((u8)0x00,BRUSH_U_ENABLE);
			}
			else{
				ADD_WRITE_REG((u8)0x01,BRUSH_A_ENABLE);
				ADD_WRITE_REG((u8)0x02,BRUSH_A_MODE);

				ADD_WRITE_REG((u8)0x01,BRUSH_U_ENABLE);
				ADD_WRITE_REG((u8)0x06,BRUSH_U_MODE);
			}
			ADD_WRITE_REG(Remote_trans_para.push_rod_speed,BRUSH_A_SPEED_SET);
	}
	if(++heartTimeCount>40){//400ms心跳
		heartTimeCount = 0;
		ADD_WRITE_REG((u8)0x01,HEART_BIT);
	}

}
#endif

#define		SET_COORANGLE_10X					Remote_setting_para.CoordAngle
#define 	NEW_COORD_X  							(Remote_setting_para.CoordX-Coord_Base)
#define 	NEW_COORD_Y  							(Remote_setting_para.CoordY-Coord_Base)
#define		PI												(3.1415)
#define		ANGLE_VAL 								(1800/PI)	//放大十倍
#define		ARCTAN(x,y)								atan2(y,x)
//#define 	M_RAT											20/100.0

void app_cltprocess(void)
{
	// static uint8_t back_wheel_mode=0,fornt_wheel_mode=0,seat_wheel_mode=0;
	// static uint8_t cnt1=0,cnt2=0;	
	// int16_t support_elongation;
	// uint8_t speedt;
	float a,b,c,d;
	float M_RAT; 
	// float temp; 
	float A_temp; 

	/*------------------蓝牙控制状态--------------*/
	if(Remote_setting_para.HandleLock)
	{		
		if(Remote_setting_para.RemoteBTConnect == eBlubtooth && Remote_setting_para.RemoteBTCmd==CMD_SetCtrl)
		{
			Remote_setting_para.CoordX = Phone_receive_para.CoordX ;
			Remote_setting_para.CoordY = Phone_receive_para.CoordY ;
			Remote_setting_para.M115CtrlMode= Phone_receive_para.M115CtrlMode;
			
			if((Phone_receive_para.SpeedLevel>=ePGearOne)&&(Phone_receive_para.SpeedLevel<=ePGearFive))
				Remote_setting_para.SpeedGear = Phone_receive_para.SpeedLevel;
					
			
			if((Remote_setting_para.VoiceLevel != Phone_receive_para.VoiceLevel)||
			(Remote_setting_para.LampSwitch != Phone_receive_para.LampSwitch)||
			(Remote_setting_para.VoiceSwitch != Phone_receive_para.VoiceSwitch)||
			(Remote_setting_para.AlarmSwitch != Phone_receive_para.AlarmSwitch)||
			(Remote_setting_para.CruiseCtrlSwitch != Phone_receive_para.CruiseCtrlSwitch))
			{
				if((Phone_receive_para.VoiceLevel>=ePVoiceLevel_1)&&(Phone_receive_para.VoiceLevel<=ePVoiceLevel_3))
					Remote_setting_para.VoiceLevel = Phone_receive_para.VoiceLevel;
				Remote_setting_para.LampSwitch = Phone_receive_para.LampSwitch&0x01;
				Remote_setting_para.VoiceSwitch = Phone_receive_para.VoiceSwitch&0x01;
				Remote_setting_para.AlarmSwitch = Phone_receive_para.AlarmSwitch&0x01;
				Remote_setting_para.CruiseCtrlSwitch = Phone_receive_para.CruiseCtrlSwitch&0x01;
							
				mem_save();

			}			
			if(Phone_receive_para.Power)
			{
				Remote_setting_para.PowerStatus = ePowerOff;			
			}			
		}
		else
		{
			Remote_setting_para.CoordX = Coord_Base ;
			Remote_setting_para.CoordY = Coord_Base ;			
		}
		
	}	
	else
	{
		
		if(Remote_setting_para.CoordXYErrorFlag == 1){
			Remote_setting_para.CoordX = Coord_Base ;
			Remote_setting_para.CoordY = Coord_Base ;
			if(joyetick_adc.valX == Coord_Base && joyetick_adc.valY == Coord_Base)
				Remote_setting_para.CoordXYErrorFlag = 0;
		} else{
			Remote_setting_para.CoordX = joyetick_adc.valX ;
			Remote_setting_para.CoordY = joyetick_adc.valY ;
		}
	}	


#if DEBUG_OUT
	printf("%c%c%c%c%c%c",
	0x03,0xfc,
	Remote_setting_para.CoordX,Remote_setting_para.CoordY,
	0xfc,0x03);
#endif
	
  /*--------------- 通讯信号 ---------------*/

#if 0
	if(Remote_setting_para.RemoteCount>Frame_OverTime)
	{
		Remote_setting_para.RemoteCount=0;
		Remote_setting_para.RemoteConnect = eOffline;
		Remote_setting_para.Battery = 0;
	}
	if(Remote_setting_para.RemoteConnect != eOffline)
	{
			Remote_setting_para.RemoteCount++;
	}
	
	if(Remote_setting_para.RemoteBTCount>Frame_OverTime)
	{
		Remote_setting_para.RemoteBTCount=0;
		Remote_setting_para.RemoteBTConnect = eOffline;
	}
	if(Remote_setting_para.RemoteBTConnect != eOffline)
	{
			Remote_setting_para.RemoteBTCount++;
	}	
#endif
	

  /*--------------- 平地行走 ---------------*/
	if(Remote_setting_para.ControlMode==eWalking )
	{
	/*------------------角度矢量坐标--------------*/
	SET_COORANGLE_10X =	atan2(NEW_COORD_Y,NEW_COORD_X);
	Remote_setting_para.CoordSqrt= sqrt((int32_t)NEW_COORD_X*NEW_COORD_X + (int32_t)NEW_COORD_Y*NEW_COORD_Y);
		//无极调速，加入半径计算
		if(Remote_setting_para.CoordSqrt > COORD_SQRT_MAX) Remote_setting_para.CoordSqrt = COORD_SQRT_MAX;
		Remote_setting_para.CoordSpeed = walking_speed[Remote_setting_para.SpeedGear-1] * Remote_setting_para.CoordSqrt/COORD_SQRT_MAX;

		if(Remote_setting_para.CoordX==Coord_Base &&Remote_setting_para.CoordY==Coord_Base)
		{
			Remote_trans_para.walking_speed_L = 0; 
			Remote_trans_para.walking_speed_R = 0;				
		}
		else
		{
			// M_RAT = 0.32;
			M_RAT = 0.37;
			if(Remote_setting_para.SpeedGear>1)
			{
				A_temp = 0.65f;						
				a = Remote_setting_para.SpeedGear*175;
				b = cosf((float)SET_COORANGLE_10X)*Remote_setting_para.CoordSpeed/(Remote_setting_para.SpeedGear/((Remote_setting_para.SpeedGear-1)/5.0f));
				c = Remote_setting_para.CoordSpeed *cosf((float)(SET_COORANGLE_10X-M_RAT*PI))*A_temp;
				d = Remote_setting_para.CoordSpeed *sinf((float)(SET_COORANGLE_10X-1/2.0f*PI+M_RAT*PI))*A_temp;
	
			}
			else
			{		
				a = 300;
				b = cosf((float)SET_COORANGLE_10X)*Remote_setting_para.CoordSpeed*0.01;
				c = Remote_setting_para.CoordSpeed *cosf((float)(SET_COORANGLE_10X-M_RAT*PI));
				d = Remote_setting_para.CoordSpeed *sinf((float)(SET_COORANGLE_10X-1/2.0f*PI+M_RAT*PI));
	
			}	
				#if 0//手柄做加减速处理
				Remote_setting_para.wheel_L_Speed = c - b;
				Remote_setting_para.wheel_R_Speed = d + b;

				if(Remote_setting_para.wheel_L_Speed > Remote_trans_para.walking_speed_L)
					Remote_trans_para.walking_speed_L += Remote_setting_para.wheel_ADD_Speed;
				if(Remote_setting_para.wheel_L_Speed < Remote_trans_para.walking_speed_L)
					Remote_trans_para.walking_speed_L -= Remote_setting_para.wheel_SUB_Speed;


				#else
				// Remote_trans_para.walking_speed_L = c -  b + a;
				// Remote_trans_para.walking_speed_R = d +  b + a;				
				Remote_trans_para.walking_speed_L = c -  b;
				Remote_trans_para.walking_speed_R = d +  b;				

				int16_t subSpeed;
				//减小后退
				if(Remote_trans_para.walking_speed_L < 0 && Remote_trans_para.walking_speed_R < 0){
					A_temp = 0.5f;
					subSpeed = 
					Remote_trans_para.walking_speed_L > Remote_trans_para.walking_speed_R ? 
					Remote_trans_para.walking_speed_L : Remote_trans_para.walking_speed_R ;

					Remote_trans_para.walking_speed_L -= subSpeed*A_temp;
					Remote_trans_para.walking_speed_R -= subSpeed*A_temp;
				// 增大前进
				}else if(Remote_trans_para.walking_speed_L > 0 && Remote_trans_para.walking_speed_R > 0){
					A_temp = 0.1f;
					subSpeed = 
					Remote_trans_para.walking_speed_L < Remote_trans_para.walking_speed_R ? 
					Remote_trans_para.walking_speed_L : Remote_trans_para.walking_speed_R ;

					Remote_trans_para.walking_speed_L += subSpeed*A_temp;
					Remote_trans_para.walking_speed_R += subSpeed*A_temp;

				}
				#endif
		}
		// if(Remote_receive_para.push_rod_state & 1 << 7 | Remote_receive_para.push_rod_state & 1 << 8){
				// play_file_voice(1,3);
		// }


	}
}





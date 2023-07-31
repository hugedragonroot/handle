#include "app_protocol.h"
#include "ext.h"
#include "math.h"
#include "string.h"



 TcomData		Remote_trans_buff;
 TcomData		Remote_receive_buff;
 

 TcomData		Phone_trans_buff;
 TcomData		Phone_receive_buff; 


#if USING_XY_TO_SPEED
// 5档速度
int16_t walking_speed[5] = {
	500,1300,2000,2880,3500
};
#endif

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
	if(combine_wirte->combineCount >= COMBINE_COUNT_MAX){\
		protocolSend();\
		txbufferIndex = 0;\
		combine_wirte->combineCount = 0;\
	}\
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
	// static u16 foldTimeCount = 0;
	static u8 timeCount = 0;
	combine_wirte->combineCount = 0;
	++timeCount;
	#if USING_XY_TO_SPEED
	//左轮速度
	ADD_WRITE_REG(Remote_trans_para.walking_speed_L,BLDC_A_SPEED_SET);
	//右轮速度
	ADD_WRITE_REG(Remote_trans_para.walking_speed_R,BLDC_U_SPEED_SET);
	//摇杆角度
	ADD_WRITE_REG(Remote_setting_para.CoordIntAngle,HANDLE_ANGLE);
	#endif

	#if 1
	//折叠状态
	if(Remote_receive_para.folding_state != Remote_trans_para.folding_state
	&& timeCount%20 == 0){//折叠状态 200ms发送一次
		++Remote_setting_para.folding_time;
			
		if(Remote_setting_para.folding_time > 15 && //3秒后在进行速度判断
		Remote_receive_para.push_rod_speed == 0 &&
		Remote_receive_para.seat_rod_speed == 0){
			Remote_receive_para.folding_state = Remote_trans_para.folding_state;
			Remote_setting_para.folding_time = 0;
			ADD_WRITE_REG((u8)0x06,BRUSH_A_MODE);
			ADD_WRITE_REG((u8)0x00,BRUSH_U_ENABLE);
			ADD_WRITE_REG((u16)0x0000,BRUSH_A_SPEED_SET);
		}else if(Remote_setting_para.folding_time > 150){//30秒后还未停止
			ADD_WRITE_REG((u8)0x00,BRUSH_A_ENABLE);
			ADD_WRITE_REG((u8)0x00,BRUSH_U_ENABLE);
			Remote_receive_para.folding_state = Remote_trans_para.folding_state;
			Remote_setting_para.folding_time = 0;
		}else{
			if(Remote_receive_para.folding_state == ePExpand){//折叠
				ADD_WRITE_REG((u8)0x01,BRUSH_A_ENABLE);
				ADD_WRITE_REG((u8)0x07,BRUSH_A_MODE);
				ADD_WRITE_REG((int16_t)300,BRUSH_A_SPEED_SET);

				ADD_WRITE_REG((u8)0x01,BRUSH_U_ENABLE);
				ADD_WRITE_REG((u8)0x07,BRUSH_U_MODE);
				ADD_WRITE_REG((int16_t)-120,BRUSH_U_SPEED_SET);
			}else{//展开
				ADD_WRITE_REG((u8)0x01,BRUSH_A_ENABLE);
				ADD_WRITE_REG((u8)0x08,BRUSH_A_MODE);

				ADD_WRITE_REG((u8)0x01,BRUSH_U_ENABLE);
				ADD_WRITE_REG((u8)0x08,BRUSH_U_MODE);
			}
		}
	}else if(timeCount%20 == 0){
	//推杆速度 200ms发送一次
			if(Remote_trans_para.push_rod_speed == 0){
				ADD_WRITE_REG((u8)0x06,BRUSH_A_MODE);
				ADD_WRITE_REG((u8)0x00,BRUSH_U_ENABLE);
			}else{
				ADD_WRITE_REG((u8)0x01,BRUSH_A_ENABLE);
				ADD_WRITE_REG((u8)0x02,BRUSH_A_MODE);

				ADD_WRITE_REG((u8)0x01,BRUSH_U_ENABLE);
				ADD_WRITE_REG((u8)0x06,BRUSH_U_MODE);
			}
			ADD_WRITE_REG(Remote_trans_para.push_rod_speed,BRUSH_A_SPEED_SET);
	}

	//X,Y坐标
	ADD_WRITE_REG((uint8_t)(Remote_setting_para.CoordX),HANDLE_X);
	ADD_WRITE_REG((uint8_t)(Remote_setting_para.CoordY),HANDLE_Y);
	//速度挡位
	ADD_WRITE_REG(Remote_setting_para.SpeedGear,HANDLE_GEAR);

	if(timeCount%40 == 0){//400ms心跳
		ADD_WRITE_REG((u8)0x01,HEART_BIT);
		timeCount = 0;
	}
	#endif



}
#endif

#define		SET_COORANGLE_10X					Remote_setting_para.CoordAngle
#define 	NEW_COORD_X  							(Remote_setting_para.CoordX-Coord_Base)
#define 	NEW_COORD_Y  							(Remote_setting_para.CoordY-Coord_Base)
#define		PI												(3.1415f)
#define		ANGLE_VAL 								(1800/PI)	//放大十倍
#define		ARCTAN(x,y)								atan2(y,x)
//#define 	M_RAT											20/100.0

void app_cltprocess(void)
{
	// static uint8_t back_wheel_mode=0,fornt_wheel_mode=0,seat_wheel_mode=0;
	// static uint8_t cnt1=0,cnt2=0;	
	// int16_t support_elongation;
	// uint8_t speedt;

	#if USING_XY_TO_SPEED
	float a,b,c,d;
	float M_RAT; 
	// float temp; 
	float A_temp; 
	static uint8_t time = 0; 
	#endif

	/*------------------蓝牙控制状态--------------*/
	if(Remote_setting_para.HandleLock == eLock)
	{		
		#if 0
		if(0&&Remote_setting_para.RemoteBTConnect == eBlubtooth && Remote_setting_para.RemoteBTCmd==CMD_SetCtrl)
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
		#endif
		Remote_setting_para.CoordX = Coord_Base ;
		Remote_setting_para.CoordY = Coord_Base ;			
		Remote_trans_para.push_rod_speed = PUSH_ORD_SPEED_ZERO;
		
	}else if(Remote_setting_para.RemoteBTConnect == eOffline){
		#if ERROR_CONTROL_FLAG
		if(Remote_trans_para.errorFlag.all[0] != 0){
			Remote_setting_para.CoordX = Coord_Base ;
			Remote_setting_para.CoordY = Coord_Base ;
			if(Remote_trans_para.errorFlag.bit.joyXyError){
				if(joyetick_adc.valX == Coord_Base && joyetick_adc.valY == Coord_Base){
					Remote_trans_para.errorFlag.bit.joyXyError = 0;
				}
			}
		}else
		#endif
		{
			Remote_setting_para.CoordX = joyetick_adc.valX ;
			Remote_setting_para.CoordY = joyetick_adc.valY ;
		}
	}	

	if((Remote_receive_para.m115_attitude.pitch_angle > 
			(ANGLE_PITCH_MAX + ANGLE_PITCH_UP_WARNING*Remote_setting_para.angleAlarmPrecent/100)
		&& (1||joyetick_adc.valY > Coord_Base))//前进 
	||(Remote_receive_para.m115_attitude.pitch_angle < 
		(-ANGLE_PITCH_MAX + ANGLE_PITCH_DOWN_WARNING*Remote_setting_para.angleAlarmPrecent/100)
		&& (1||joyetick_adc.valY < Coord_Base))){//后退

			Remote_trans_para.errorFlag.bit.angleError = 1;


	}else if((Remote_receive_para.m115_attitude.roll_angle > 
			(ANGLE_ROLL_MAX*(100+Remote_setting_para.angleAlarmPrecent)/100)
		&& (1||joyetick_adc.valX < Coord_Base))//左转
	||(Remote_receive_para.m115_attitude.roll_angle < 
			(-ANGLE_ROLL_MAX*(100+Remote_setting_para.angleAlarmPrecent)/100)
		&& (1||joyetick_adc.valX > Coord_Base))){//右转

			Remote_trans_para.errorFlag.bit.angleError = 1;

			if(Remote_setting_para.AlarmSwitch){
				u8 num = MUSIC_SLOPE_DANGER;
				LoopQueue_Write(&music_FIFO,&num,1);
			}
	}else if(Remote_receive_para.m115_attitude.pitch_angle <= 
		(ANGLE_PITCH_MAX + ANGLE_PITCH_UP_WARNING*Remote_setting_para.angleAlarmPrecent/100)
	&& (Remote_receive_para.m115_attitude.pitch_angle >= 
		(-ANGLE_PITCH_MAX + ANGLE_PITCH_DOWN_WARNING*Remote_setting_para.angleAlarmPrecent/100))
	&& (Remote_receive_para.m115_attitude.roll_angle <= 
		(ANGLE_ROLL_MAX*(100+Remote_setting_para.angleAlarmPrecent)/100)
		|| joyetick_adc.valX > Coord_Base)
	&& (Remote_receive_para.m115_attitude.roll_angle >= 
		(-ANGLE_ROLL_MAX*(100+Remote_setting_para.angleAlarmPrecent)/100)
		|| joyetick_adc.valX < Coord_Base)
		){

			Remote_trans_para.errorFlag.bit.angleError = 0;
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
	
	#if USING_XY_TO_SPEED
  /*--------------- 平地行走 ---------------*/
	if(Remote_setting_para.ControlMode==eWalking )
	{
	/*------------------角度矢量坐标--------------*/
	SET_COORANGLE_10X =	atan2(NEW_COORD_Y,NEW_COORD_X);
	//Remote_setting_para.CoordIntAngle = SET_COORANGLE_10X/3.14f*180;
	Remote_setting_para.CoordSqrt= sqrt((int32_t)NEW_COORD_X*NEW_COORD_X + (int32_t)NEW_COORD_Y*NEW_COORD_Y);
		//无极调速，加入半径计算
		if(Remote_setting_para.CoordSqrt > COORD_SQRT_MAX) Remote_setting_para.CoordSqrt = COORD_SQRT_MAX;
		Remote_setting_para.CoordSpeed = walking_speed[Remote_setting_para.SpeedGear-1] * Remote_setting_para.CoordSqrt/COORD_SQRT_MAX;

		if((Remote_setting_para.CoordX==Coord_Base &&Remote_setting_para.CoordY==Coord_Base)/* || time*/)
		{
			// if(time > 0)--time;
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
				b = cosf((float)SET_COORANGLE_10X)*Remote_setting_para.CoordSpeed*0.01f;
				c = Remote_setting_para.CoordSpeed *cosf((float)(SET_COORANGLE_10X-M_RAT*PI));
				d = Remote_setting_para.CoordSpeed *sinf((float)(SET_COORANGLE_10X-1/2.0f*PI+M_RAT*PI));
	
			}	
			// Remote_trans_para.walking_speed_L = c -  b + a;
			// Remote_trans_para.walking_speed_R = d +  b + a;				
			Remote_trans_para.walking_speed_L = c -  b;
			Remote_trans_para.walking_speed_R = d +  b;				

			int16_t subSpeed;
			if(Remote_trans_para.walking_speed_L < 0 && Remote_trans_para.walking_speed_R < 0){
				if(0&&Remote_receive_para.pitch_angle < -ANGLE_PITCH_MAX){
					Remote_trans_para.walking_speed_L = 0;
					Remote_trans_para.walking_speed_R = 0;
					time = 100;
				}else{ //减小后退
					A_temp = 0.5f;
					subSpeed = 
					Remote_trans_para.walking_speed_L > Remote_trans_para.walking_speed_R ? 
					Remote_trans_para.walking_speed_L : Remote_trans_para.walking_speed_R ;

					Remote_trans_para.walking_speed_L -= subSpeed*A_temp;
					Remote_trans_para.walking_speed_R -= subSpeed*A_temp;
				}
			}else if(Remote_trans_para.walking_speed_L > 0 && Remote_trans_para.walking_speed_R > 0){
				if(0&&Remote_receive_para.pitch_angle > ANGLE_PITCH_MAX){
					Remote_trans_para.walking_speed_L = 0;
					Remote_trans_para.walking_speed_R = 0;
					time = 100;
				}else{ // 增大前进
					A_temp = 0.1f;
					subSpeed = 
					Remote_trans_para.walking_speed_L < Remote_trans_para.walking_speed_R ? 
					Remote_trans_para.walking_speed_L : Remote_trans_para.walking_speed_R ;

					Remote_trans_para.walking_speed_L += subSpeed*A_temp;
					Remote_trans_para.walking_speed_R += subSpeed*A_temp;
				}
			}else if(Remote_trans_para.walking_speed_L > 0 && Remote_trans_para.walking_speed_R < 0){//右转
				if(0&&Remote_receive_para.roll_angle < -ANGLE_ROLL_MAX){
					Remote_trans_para.walking_speed_L = 0;
					Remote_trans_para.walking_speed_R = 0;
					time = 100;
				}

			}else if(Remote_trans_para.walking_speed_L < 0 && Remote_trans_para.walking_speed_R > 0){//左转
				if(0&&Remote_receive_para.roll_angle > ANGLE_ROLL_MAX){
					Remote_trans_para.walking_speed_L = 0;
					Remote_trans_para.walking_speed_R = 0;
					time = 100;
				}
			}
		}
	}
	#endif
}





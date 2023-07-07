#include <stdbool.h>
#include "data_pack_task.h"
/*FreeRtos includes*/
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "key_ext.h"
#include "wirelink_task.h"
#include "app_remote_para.h"
#include "main.h"
#include "joystick_ext.h"

#define BT_XY_LEN		10
 
uint16_t BT_buff_x[BT_XY_LEN]={0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80};
uint16_t BT_buff_y[BT_XY_LEN]={0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80};



#if !USING_CAN
/*wiredata解析*/
//接收数据解析
static void DataPackAnalyze(TCOMMUN *p,uint8_t type)
{
	if(type==0)//wire
	{
		Remote_setting_para.RemoteCount = 0;
		Remote_setting_para.RemoteConnect = eWire;
		switch(p->Cmd)
		{
			case CMD_Handshake:         /* 0x01 握手协议 */
				Remote_receive_para.Drivermodel = p->Data[0];
				Remote_receive_para.Driversoftware = p->Data[1];
				Remote_receive_para.Driversoftwarepre = p->Data[2];
				break;
				
			case CMD_SetCtrl:           /* 0x06 控制命令 */

			Remote_receive_para.track_speed_L = p->Data[0]|(p->Data[1]<<8);
			Remote_receive_para.track_speed_R = p->Data[2]|(p->Data[3]<<8);
			Remote_receive_para.walking_speed_L = p->Data[4]|(p->Data[5]<<8);
			Remote_receive_para.walking_speed_R = p->Data[6]|(p->Data[7]<<8);
			Remote_receive_para.fornt_wheel_mode = p->Data[8]&0x03;
			Remote_receive_para.back_wheel_mode = (p->Data[8]>>2)&0x03;
			Remote_receive_para.seat_wheel_mode = (p->Data[8]>>4)&0x03;

			Remote_receive_para.fornt_support_elongation = p->Data[9]|(p->Data[10]<<8);
			Remote_receive_para.back_support_elongation = p->Data[11]|(p->Data[12]<<8);

			Remote_receive_para.seat_angle = p->Data[13]|(p->Data[14]<<8);
			Remote_receive_para.body_angle = p->Data[15]|(p->Data[16]<<8);
			Remote_receive_para.track_hall_cnt = p->Data[17]|(p->Data[18]<<8)|(p->Data[19]<<16)|(p->Data[20]<<24);
			Remote_receive_para.fornt_ultrasonic_y = p->Data[21]|(p->Data[22]<<8);
			Remote_receive_para.back_ultrasonic_x = p->Data[23]|(p->Data[24]<<8);
			Remote_receive_para.back_ultrasonic_y = p->Data[25]|(p->Data[26]<<8);
			Remote_receive_para.Battery = p->Data[27];
			Remote_setting_para.Battery = Remote_receive_para.Battery;
			
			break;
		}
	}
	else
	{
		Remote_setting_para.RemoteBTCount = 0;
		Remote_setting_para.RemoteBTConnect = eBlubtooth;	
		Remote_setting_para.RemoteBTCmd	= p->Cmd;	
		switch(p->Cmd)
		{
			case CMD_Handshake:         /* 0x01 握手协议 */
				p->Data[0] = Remote_receive_para.Drivermodel & 0xff;
				p->Data[1] = Remote_receive_para.Driversoftware & 0xff;
				p->Data[2] = Remote_receive_para.Driversoftwarepre & 0xff;	
				p->Data[3] = SOFTWARE_VER0 ;
				p->Data[4] = SOFTWARE_VER1;
				p->Data[5] = SOFTWARE_VER2;
				p->DataLen = 6;
				break;
				
			case CMD_SetCtrl:           /* 0x06 控制命令 */		

 				Phone_receive_para.CoordX 				= p->Data[0]&0xff;
				Phone_receive_para.CoordY 				= p->Data[1]&0xff;
				Phone_receive_para.M130CtrlMode 	= p->Data[2]&0x01;			
				Phone_receive_para.M115CtrlMode 	= p->Data[3]&0x03;
				Phone_receive_para.SeatCtrl				= p->Data[4]&0x07;
				Phone_receive_para.ForntwheelCtrl = p->Data[5]&0x03;
				Phone_receive_para.BackwheelCtrl 	= p->Data[6]&0x03;
				Phone_receive_para.SpeedLevel 		= p->Data[7]&0x07;	
				Phone_receive_para.VoiceLevel 		= p->Data[8]&0x03;	
				Phone_receive_para.Power 					= p->Data[9]&0x01;		
				Phone_receive_para.LampSwitch			= p->Data[10]&0x01;
				Phone_receive_para.VoiceSwitch		= p->Data[11]&0x01;
				Phone_receive_para.AlarmSwitch		= p->Data[12]&0x01;
				Phone_receive_para.CruiseCtrlSwitch 		= p->Data[13]&0x01;		
//				printf("\r\n\r\n");
//				printf("摇杆X坐标   : %d\r\n",Phone_receive_para.CoordX);
//				printf("摇杆Y坐标   : %d\r\n",Phone_receive_para.CoordY);
//				printf("模式选择    : %d\r\n",Phone_receive_para.M130CtrlMode);
//				printf("手自动模式  : %d\r\n",Phone_receive_para.M115CtrlMode);
//				printf("座椅调节    : %d\r\n",Phone_receive_para.SeatCtrl);
//				printf("前轮调节    : %d\r\n",Phone_receive_para.ForntwheelCtrl);
//				printf("后轮调节    : %d\r\n",Phone_receive_para.BackwheelCtrl);
//				printf("速度档      : %d\r\n",Phone_receive_para.SpeedLevel);
//				printf("语言音量    : %d\r\n",Phone_receive_para.VoiceLevel);
//				printf("关机指令    : %d\r\n",Phone_receive_para.Power);
//				printf("灯光开关    : %d\r\n",Phone_receive_para.LampSwitch);
//				printf("语言提示开关: %d\r\n",Phone_receive_para.VoiceSwitch);
//				printf("语言报警开关: %d\r\n",Phone_receive_para.AlarmSwitch);
//				printf("定速巡航开关: %d\r\n\r\n",Phone_receive_para.CruiseCtrlSwitch);
//				printf("/********************************************************/\r\n\r\n");

				case CMD_GetCtrl:		
					
				#if APP_BT_TEST
					Remote_receive_para.seat_angle =30;
					Remote_receive_para.body_angle = 30;
					Remote_setting_para.Battery = 0;
					Remote_setting_para.speed_kmh = 125;
					Remote_setting_para.totalkm = 0x5678; 
				
				#endif

				p->Data[0] = (Remote_setting_para.ControlMode==eWalking)?ePwalking: ePTracks;
				p->Data[1] = Remote_setting_para.M115CtrlMode;
				p->Data[2] = Remote_setting_para.SpeedGear ;
				p->Data[3] = Remote_receive_para.seat_angle ;
				p->Data[4] = Remote_receive_para.body_angle ;
				p->Data[5] = Remote_setting_para.VoiceLevel ;	
				p->Data[6] = Remote_setting_para.LampSwitch ;	
				p->Data[7] = Remote_setting_para.VoiceSwitch ;	
				p->Data[8] = Remote_setting_para.AlarmSwitch ;
				p->Data[9] = Remote_setting_para.HandleLock ;				
				p->Data[10] = Remote_setting_para.CruiseCtrlSwitch ;				
				p->Data[11] = Remote_setting_para.Battery*10 &0xff;
				p->Data[12] = 0;															//电池状态
				p->Data[13] = Remote_setting_para.speed_kmh;	//速度
				p->Data[14] = Remote_setting_para.Model;			//设备类型
				p->Data[15] = Remote_setting_para.totalkm>>24 &0xff;	//总里程
				p->Data[16] = Remote_setting_para.totalkm>>16 &0xff;
				p->Data[17] = Remote_setting_para.totalkm>>8 &0xff;
				p->Data[18] = Remote_setting_para.totalkm &0xff;
				p->Data[19] = 0;
				p->Data[20] = 0;
				p->Data[21] = 0;
				p->DataLen = 22;						
			default:
				break;
		}			
	}
}
void wireProcessTask(void* param)
{
	TCOMMUN p;
	
	while(1)
	{
		wirelinkReceivePacketBlocking(&p);
		DataPackAnalyze(&p,0);
	}
}

void BTProcessTask(void* param)
{
	TCOMMUN p;
	while(1)
	{
		BTlinkReceivePacketBlocking(&p);
		DataPackAnalyze(&p,1);
		BTlinkSendPacket(&p);
	}
}

#else

void CANCheckValue(void){
	if(Remote_setting_para.AlarmSwitch){
		u8 num = MUSIC_SLOPE_DANGER;
		if(abs(Remote_receive_para.roll_angle) > ANGLE_SLOPE_MAX || abs(Remote_receive_para.pitch_angle) > ANGLE_SLOPE_MAX){
			LoopQueue_Write(&music_FIFO,&num,1);
		}

		for(u8 i = 0;i<4;i++){
			if(Remote_receive_para.ultrasonic[i]!=0 && Remote_receive_para.ultrasonic[i]<ULTRASONIC_DISTANCE_MAX ){
				LoopQueue_Write(&music_FIFO,&num,1);
				break;
			}
		}
	}

    if(Remote_receive_para.BatteryVol < BATTERY_MIN_VOL) Remote_setting_para.Battery = 0;
    else if(Remote_receive_para.BatteryVol >= BATTERY_MAX_VOL) Remote_setting_para.Battery = 99;
    else Remote_setting_para.Battery = (Remote_receive_para.BatteryVol-BATTERY_MIN_VOL)*100/(BATTERY_MAX_VOL-BATTERY_MIN_VOL);


}


typedef struct {
	void *addr;
	uint8_t size;
}Recv_Reg_Info;

Recv_Reg_Info RecvRegInfos[16];

void CANPackAnalyze(MasterMsgData_t *recvMessage){

	switch (recvMessage->cmd)
	{
	case FUNC_SUBSCRIBE:
		if(recvMessage->msg_id == 'S'){
			u8 index = 0,regIndex = 0;
			while (index < recvMessage->Length){
				if(RecvRegInfos[regIndex].addr == NULL)return;
				memcpy(RecvRegInfos[regIndex].addr,&recvMessage->Data[index],RecvRegInfos[regIndex].size);
				index+=RecvRegInfos[regIndex++].size;
			}
		}else if(recvMessage->msg_id == 's'){
			u8 index = 0,regIndex = 8;
			while (index < recvMessage->Length){
				if(RecvRegInfos[regIndex].addr == NULL)return;
				memcpy(RecvRegInfos[regIndex].addr,&recvMessage->Data[index],RecvRegInfos[regIndex].size);
				index+=RecvRegInfos[regIndex++].size;
			}
			CANCheckValue();
		}
		else{
		}
		Remote_setting_para.RemoteCount = 0;
		Remote_setting_para.RemoteConnect = eWire;
		break;
	case FUNC_WRITE:
		break;
	
	default:
		break;
	}


}


#define ADD_SUB_REG(para,regName){\
	RecvRegInfos[regIndex].addr = (&para);\
	RecvRegInfos[regIndex].size = tranRegInfoDir[(regName)].size;\
	combine_sub.combine_read.combine[combine_sub.combine_read.combineCount].address =\
	tranRegInfoDir[(regName)].addr;\
	combine_sub.combine_read.combine[combine_sub.combine_read.combineCount].size = \
	tranRegInfoDir[(regName)].size;\
	++combine_sub.combine_read.combineCount;\
	++regIndex;\
	}

void CANSubInit(void){

	PDU_t req_PDU;
	MsgData_t request_msg_data;
	combine_subscribe_t combine_sub;
	u8 regIndex = 0;

#if 1
	memset(&combine_sub,0,sizeof(combine_sub));
	combine_sub.timing = 200;

	ADD_SUB_REG(Remote_receive_para.walking_speed_L,BLDC_A_SPEED_REAL);
	
	ADD_SUB_REG(Remote_receive_para.walking_speed_R,BLDC_U_SPEED_REAL);

	ADD_SUB_REG(Remote_receive_para.push_rod_speed,BRUSH_A_SPEED_REAL);

	ADD_SUB_REG(Remote_receive_para.seat_rod_speed,BRUSH_U_SPEED_REAL);

	combine_subscribe_to_PDU('S',&combine_sub,&req_PDU);
	PDU_to_msg_data(NODE_SELF, &req_PDU, &request_msg_data);
	send_message_data(&request_msg_data);

	delay_ms(20);
#endif

	memset(&combine_sub,0,sizeof(combine_sub));
	combine_sub.timing = 1000;
	regIndex = 8;

	ADD_SUB_REG(Remote_receive_para.BatteryVol,DEVICE_VOLT);

	ADD_SUB_REG(Remote_receive_para.pitch_angle,PITCH_ANGLE);

	ADD_SUB_REG(Remote_receive_para.roll_angle,ROLL_ANGLE);

	ADD_SUB_REG(Remote_receive_para.ultrasonic[0],ULTRASONIC);

	combine_subscribe_to_PDU('s',&combine_sub,&req_PDU);
	PDU_to_msg_data(NODE_SELF, &req_PDU, &request_msg_data);
	send_message_data(&request_msg_data);
	
}


static uint8_t DataPackAnalyze(TCOMMUN *p,uint8_t type)
{
	uint8_t ret=0;
	// static uint8_t cnt=0;
	uint8_t tempx;
	uint8_t tempy;		
	
	// int X,Y;
	if(type==0)//wire
	{
		Remote_setting_para.RemoteCount = 0;
		Remote_setting_para.RemoteConnect = eWire;
		switch(p->Cmd)
		{
			
		}
	}
	else
	{
		Remote_setting_para.RemoteBTCount = 0;
		Remote_setting_para.RemoteBTConnect = eBlubtooth;	
		// Remote_setting_para.HandleLock = 1;
		Remote_setting_para.RemoteBTCmd	= p->Cmd;	
		switch(p->Cmd)
		{
			case CMD_Handshake:         /* 0x01 握手协议 */
				p->Data[0] = Remote_receive_para.Drivermodel & 0xff;
				p->Data[1] = Remote_receive_para.Driversoftware & 0xff;
				p->Data[2] = Remote_receive_para.Driversoftwarepre & 0xff;	
				p->Data[3] = SOFTWARE_VER0 ;
				p->Data[4] = SOFTWARE_VER1;
				p->Data[5] = SOFTWARE_VER2;
				p->DataLen = 6;
			
//				printf("%c%c%c%c%c%c",p->Data[0],p->Data[1],p->Data[2],p->Data[3],p->Data[4],p->Data[5]);
				ret = 1;
				break;
				
			case CMD_SetCtrl:           /* 0x06 控制命令 */		
				if((p->DataLen==0x11) && (p->Data[2]<2))
				{
					tempx 				= p->Data[0]&0xff;
					tempy 				= p->Data[1]&0xff; 
					tempx = deadband(tempx,Coord_Base,Coord_Dead);
					tempy = deadband(tempy,Coord_Base,Coord_Dead);

					// tempx = GildeAverageValueFilter_MAG(tempx,BT_buff_x,BT_XY_LEN); 
					// tempy = GildeAverageValueFilter_MAG(tempy,BT_buff_y,BT_XY_LEN);  				
					Phone_receive_para.CoordX = tempx;
					Phone_receive_para.CoordY = tempy;

					// Phone_receive_para.CoordX 				= p->Data[0]&0xff;
					// Phone_receive_para.CoordY 				= p->Data[1]&0xff;
						
					Phone_receive_para.M130CtrlMode 	= p->Data[2]&0x01;			
					Phone_receive_para.M115CtrlMode 	= p->Data[3]&0x03;
					Phone_receive_para.SeatCtrl				= p->Data[4]&0x07;
					Phone_receive_para.ForntwheelCtrl = p->Data[5]&0x03;
					Phone_receive_para.BackwheelCtrl 	= p->Data[6]&0x03;
					Phone_receive_para.SpeedLevel 		= p->Data[7]&0x07;	
					Phone_receive_para.VoiceLevel 		= p->Data[8]&0x03;	
					Phone_receive_para.Power 					= p->Data[9]&0x01;		
					Phone_receive_para.LampSwitch			= p->Data[10]&0x01;
					Phone_receive_para.VoiceSwitch		= p->Data[11]&0x01;
					Phone_receive_para.AlarmSwitch		= p->Data[12]&0x01;
					Phone_receive_para.CruiseCtrlSwitch 		= p->Data[13]&0x01;		
				}
				
				if(!Remote_setting_para.HandleLock )
				{
					
					Remote_setting_para.CoordX = Phone_receive_para.CoordX ;
					Remote_setting_para.CoordY = Phone_receive_para.CoordY ;
					switch(Phone_receive_para.SeatCtrl)
					{
	
						case ePSeatUp:
//							Remote_trans_para.push_rod_speed=PUSH_ORD_SPEED_ADD;	
							Remote_trans_para.push_rod_speed=PUSH_ORD_SPEED_SUB;			
							break;
						case ePSeatDown:
							Remote_trans_para.push_rod_speed=PUSH_ORD_SPEED_ADD;	
//							Remote_trans_para.push_rod_speed=PUSH_ORD_SPEED_SUB;			
							break;
						default:
							Remote_trans_para.push_rod_speed=PUSH_ORD_SPEED_ZERO;					
							break;					
					}
					#if 0
					switch(Phone_receive_para.ForntwheelCtrl)
					{
						case ePAngleUp:
//							set_wheel_mode(eFornt_wheel,eSpeedMode,FORNT_SPEEDDEF_UP);		
							break;
						case ePAngleDown:
//							set_wheel_mode(eFornt_wheel,eSpeedMode,FORNT_SPEEDDEF_DOWN);					
							break;
						default:
//							set_wheel_mode(eFornt_wheel,eSpeedMode,MOVE_SPEEDDEF_ZERO);					
							break;					
					}			
					switch(Phone_receive_para.BackwheelCtrl)
					{
						case ePAngleUp:
//							set_wheel_mode(eBack_wheel,eSpeedMode,BACK_SPEEDDEF_UP);		
							break;
						case ePAngleDown:
//							set_wheel_mode(eBack_wheel,eSpeedMode,BACK_SPEEDDEF_DOWN);					
							break;
						default:
//							set_wheel_mode(eBack_wheel,eSpeedMode,MOVE_SPEEDDEF_ZERO);						
							break;					
					}			
					#endif 
							
					if(1){
						if((Remote_setting_para.ControlMode != 1)||
						(Remote_setting_para.M115CtrlMode != Phone_receive_para.M115CtrlMode)||
						(Remote_setting_para.SpeedGear != Phone_receive_para.SpeedLevel)||
						(Remote_setting_para.VoiceLevel != Phone_receive_para.VoiceLevel)||
						(Remote_setting_para.LampSwitch != Phone_receive_para.LampSwitch)||
						(Remote_setting_para.VoiceSwitch != Phone_receive_para.VoiceSwitch)||
						(Remote_setting_para.AlarmSwitch != Phone_receive_para.AlarmSwitch)||
						(Remote_setting_para.CruiseCtrlSwitch != Phone_receive_para.CruiseCtrlSwitch))
						{
							// cnt = 5;
//							if(Remote_setting_para.ControlMode != mode)
//							{
//								Remote_setting_para.ControlMode = M115;//(Phone_receive_para.M130CtrlMode==ePwalking)?eWalking:eTracks;
//							}
							
							
//							if(Remote_setting_para.M115CtrlMode != Phone_receive_para.M115CtrlMode){
//									Remote_setting_para.M115CtrlMode= Phone_receive_para.M115CtrlMode;
//								if(Remote_receive_para.push_rod_speed == 0 && Remote_receive_para.seat_rod_speed == 0)
//										Remote_trans_para.folding_state = !Remote_receive_para.folding_state;
//							}
							
							if(Remote_setting_para.M115CtrlMode != Phone_receive_para.M115CtrlMode){
//									Remote_setting_para.M115CtrlMode= Phone_receive_para.M115CtrlMode;
								if(Remote_receive_para.push_rod_speed == 0 && Remote_receive_para.seat_rod_speed == 0)
										Remote_trans_para.folding_state = !Remote_receive_para.folding_state;
							}
								

								
							
							if(Remote_setting_para.SpeedGear != Phone_receive_para.SpeedLevel)
							{
								if((Phone_receive_para.SpeedLevel>=ePGearOne)&&(Phone_receive_para.SpeedLevel<=ePGearFive))
									Remote_setting_para.SpeedGear = Phone_receive_para.SpeedLevel;		
							}				
							if(Remote_setting_para.VoiceLevel != Phone_receive_para.VoiceLevel)
							{
								if((Phone_receive_para.VoiceLevel>=ePVoiceLevel_1)&&(Phone_receive_para.VoiceLevel<=ePVoiceLevel_3))
									Remote_setting_para.VoiceLevel = Phone_receive_para.VoiceLevel;
							}
							if(Remote_setting_para.LampSwitch != Phone_receive_para.LampSwitch)							
								Remote_setting_para.LampSwitch = Phone_receive_para.LampSwitch&0x01;							
							if(Remote_setting_para.VoiceSwitch != Phone_receive_para.VoiceSwitch)
								Remote_setting_para.VoiceSwitch = Phone_receive_para.VoiceSwitch&0x01;
							if(Remote_setting_para.AlarmSwitch != Phone_receive_para.AlarmSwitch)
								Remote_setting_para.AlarmSwitch = Phone_receive_para.AlarmSwitch&0x01;
							if(Remote_setting_para.CruiseCtrlSwitch != Phone_receive_para.CruiseCtrlSwitch)
								Remote_setting_para.CruiseCtrlSwitch = Phone_receive_para.CruiseCtrlSwitch&0x01;									
							// mem_save();

						}		

					}

				}
			case CMD_GetCtrl:		
					
				#if APP_BT_TEST
					Remote_receive_para.seat_angle =30;
					Remote_receive_para.body_angle = 30;
				#endif
				Remote_setting_para.totalkm = 0x5678; 

				p->Data[0] = 1;//(Remote_setting_para.ControlMode==eWalking)?ePwalking: ePTracks;
				p->Data[1] = Remote_setting_para.M115CtrlMode;
				p->Data[2] = Remote_setting_para.SpeedGear ;
				// p->Data[3] = Remote_receive_para.seat_angle ;
				p->Data[3] = 0 ;
				// p->Data[4] = Remote_receive_para.body_angle ;
				p->Data[4] = 0 ;
				p->Data[5] = Remote_setting_para.VoiceLevel ;	
				p->Data[6] = Remote_setting_para.LampSwitch ;	
				p->Data[7] = Remote_setting_para.VoiceSwitch ;	
				p->Data[8] = Remote_setting_para.AlarmSwitch ;
				// p->Data[9] = Remote_setting_para.HandleLock ;				
				p->Data[9] = 1 ;				
				p->Data[10] = Remote_setting_para.CruiseCtrlSwitch ;				
				p->Data[11] = Remote_setting_para.Battery&0xff;
				p->Data[12] = 0;															//电池状态
				p->Data[13] = Remote_setting_para.speed_kmh;	//速度
				p->Data[14] = Remote_setting_para.Model;			//设备类型
				p->Data[15] = Remote_setting_para.totalkm>>24 &0xff;	//总里程
				p->Data[16] = Remote_setting_para.totalkm>>16 &0xff;
				p->Data[17] = Remote_setting_para.totalkm>>8 &0xff;
				p->Data[18] = Remote_setting_para.totalkm &0xff;
				p->Data[19] = 0;
				p->Data[20] = 0;
				p->Data[21] = 0;
				p->DataLen = 22;		
//				for(i=0;i<22;i++)
//				{
//					printf("%c",p->Data[i]);
//				}	
				ret = 1;
				break;
			default:
				break;
		}	
	}	
	return ret;	
}

#if USING_RTOS
void BTProcessTask(void* param)
{
	TCOMMUN p;
	while(1)
	{
		BTlinkReceivePacketBlocking(&p);//蓝牙数据包接收
		if(DataPackAnalyze(&p,1))					//蓝牙数据解析
			BTlinkSendPacket(&p);						//蓝牙回复
	}
}
#else
void BTNoneOsProcessTask(TCOMMUN *p){
	if(DataPackAnalyze(p,1)){
		BTlinkSendPacket(p);						//蓝牙回复
	}	
}

#endif


#if USING_RTOS
void CANProcessTask(void* param){

	u8 powerOffFlag = 1;
	MasterMsgData_t recvMessage;
	memset(&recvMessage,0,sizeof(recvMessage));

	while (1)
	{

		if(Remote_setting_para.PowerStatus == ePowerOn && powerOffFlag){
			CANSubInit();
			powerOffFlag = 0;
		}else if(Remote_setting_para.PowerStatus == ePowerOff && !powerOffFlag){
			powerOffFlag = 1;
    }
		CANlinkReceivePacketBlocking(&recvMessage);
		CANPackAnalyze(&recvMessage);
  }
	
	
}
#endif

#endif

#include <stdbool.h>
#include "data_pack_task.h"
/*FreeRtos includes*/
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "key_ext.h"
#include "app_protocol.h"
#include "wirelink_task.h"
#include "app_remote_para.h"
#include "main.h"

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

typedef struct {
	void *addr;
	uint8_t size;
}RecvRegInfo;

void CANPackAnalyze(MasterMsgData_t *recvMessage,RecvRegInfo *recvRegInfo){

	switch (recvMessage->cmd)
	{
	case FUNC_SUBSCRIBE:
		if(recvMessage->msg_id == 0){
			#if 1
			u8 index = 0,regIndex = 0;
			while (index < recvMessage->Length)
			{
				memcpy(recvRegInfo[regIndex].addr,&recvMessage->Data[index],recvRegInfo[regIndex].size);
				index+=recvRegInfo[regIndex++].size;
			}
			#else
			Remote_receive_para.walking_speed_L = recvMessage->Data[0] | (recvMessage->Data[1] << 8);
			Remote_receive_para.walking_speed_R = recvMessage->Data[2] | (recvMessage->Data[3] << 8);
			#endif
		}
		break;
	case FUNC_WRITE:
		break;
	
	default:
		break;
	}
}


#define ADD_SUB_REG(para,regName){\
	recvRegInfo[combine_sub.combine_read.combineCount].addr = &(para);\
	recvRegInfo[combine_sub.combine_read.combineCount].size = sizeof(para);\
	combine_sub.combine_read.combine[combine_sub.combine_read.combineCount].address =\
	tranRegInfoDir[(regName)].addr;\
	combine_sub.combine_read.combine[combine_sub.combine_read.combineCount].size = \
	tranRegInfoDir[(regName)].size;\
	++combine_sub.combine_read.combineCount;\
	}

void CANSubInit(RecvRegInfo *recvRegInfo){

	PDU_t req_PDU;
	MsgData_t request_msg_data;
	combine_subscribe_t combine_sub;
	memset(&combine_sub,0,sizeof(combine_sub));
	combine_sub.timing = 200;

	ADD_SUB_REG(Remote_receive_para.walking_speed_L,BLDC_A_SPEED_REAL);
	
	ADD_SUB_REG(Remote_receive_para.walking_speed_R,BLDC_U_SPEED_REAL);

	ADD_SUB_REG(Remote_receive_para.push_rod_speed,BRUSH_A_SPEED_REAL);

	ADD_SUB_REG(Remote_receive_para.seat_rod_speed,BRUSH_U_SPEED_REAL);

	ADD_SUB_REG(Remote_receive_para.push_rod_state,BRUSH_A_STATUS);


	combine_subscribe_to_PDU(0,&combine_sub,&req_PDU);
	PDU_to_msg_data(NODE_SELF, &req_PDU, &request_msg_data);
	send_message_data(&request_msg_data);

	// vTaskDelay(10);
	// memset(&combine_sub,0,sizeof(combine_sub));
	// combine_sub.timing = 1000;

	// combine_subscribe_to_PDU(1,&combine_sub,&req_PDU);
	// PDU_to_msg_data(NODE_SELF, &req_PDU, &request_msg_data);
	// send_message_data(&request_msg_data);

}

void CANProcessTask(void* param){

	u8 powerOffFlag = 1;
	MasterMsgData_t recvMessage;
	RecvRegInfo RecvRegInfo[8];
	memset(&recvMessage,0,sizeof(recvMessage));
	memset(RecvRegInfo,0,sizeof(RecvRegInfo[0]));

	while (1)
	{

		if(Remote_setting_para.PowerStatus == ePowerOn && powerOffFlag){
			CANSubInit(RecvRegInfo);
			powerOffFlag = 0;
		}else if(Remote_setting_para.PowerStatus == ePowerOff && !powerOffFlag){
			powerOffFlag = 1;
    }
		CANlinkReceivePacketBlocking(&recvMessage);
		CANPackAnalyze(&recvMessage,RecvRegInfo);
  }
	
	
}
#endif

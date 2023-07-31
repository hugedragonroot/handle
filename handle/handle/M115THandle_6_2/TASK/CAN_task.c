#include <string.h>
#include "wirelink_task.h"
/*FreeRtos includes*/
#include "FreeRTOS.h"
#include "task.h"
#include "key_ext.h"
#include "uart_ext.h"
#include "app_protocol.h"
#include "queue.h"
#include "debug.h"


#include "../PROTOCOL/protocol_427/protocol.h"

// static TCOMMUN RxPack;

//static combine_write_t combine_wirte;
#if USING_RTOS 
static bool isInit;
static xQueueHandle  txQueue;
static xQueueHandle  rxQueue;
#else
// MasterMsgData_t recvMessage;
#endif


#if USING_RTOS
/*CAN初始化*/
void CAN_Init(void){
    if(isInit) return;
	txQueue = xQueueCreate(8, sizeof(combine_write_t));
	// txQueue = xQueueCreate(16, sizeof(combine_write_t));
	ASSERT(txQueue);
	rxQueue = xQueueCreate(8, sizeof(MasterMsgData_t));
	// rxQueue = xQueueCreate(16, sizeof(MasterMsgData_t));
	ASSERT(rxQueue);
	isInit = true;



}


/*CAN连接发送Packet*/
bool CANlinkSendPacket(const combine_write_t *p)
{
	ASSERT(p);
	if(p->combineCount == 0) return false;
	return xQueueSend(txQueue, p, 0);
}





void CANTxTask(void* param)
{
	combine_write_t p;
	PDU_t req_PDU;
	MsgData_t request_msg_data;

	#if 0
	CANSubInit(&req_PDU);
	PDU_to_msg_data(NODE_SELF, &req_PDU, &request_msg_data);
	send_message_data(&request_msg_data);
	#endif
	
	while(1)
	{
		xQueueReceive(txQueue, &p, portMAX_DELAY);

		combine_write_to_PDU('W',&p,&req_PDU);
		PDU_to_msg_data(NODE_SELF, &req_PDU, &request_msg_data);
		send_message_data(&request_msg_data);

	}
}

bool CANlinkReceivePacketBlocking(MasterMsgData_t *p)
{
	ASSERT(p);
	return xQueueReceive(rxQueue, p, portMAX_DELAY);
}


void CANRxTask(void* param)
{
	// uint8_t ch;
	// uint8_t state;
	// uint8_t i=0;
	// uint8_t sumchk=0;

	while(1)
	{
		if (CANGetDataWithTimeOut())
		{	//入队操作放到 api_on_message_received 里
			//xQueueSend(rxQueue, &recvMessage, 0);
		}
	}
}
#endif

static combine_write_t CANTxCombineWrite;
static PDU_t req_PDU;
static MsgData_t request_msg_data;
static u8 CANTxBuffer[40];

#define SUBTIME_S 200
#define SUBTIME_s 1000

void CANCheckValue_S(void){
	u8 num = MUSIC_SLOPE_DANGER;
	static u8 pitchAngleCount = 0,ulrasonicCount = 0;

	Remote_setting_para.angleAlarmPrecent = 
	(Remote_receive_para.m115_attitude.pcbAngle - 
	Remote_receive_para.m115_attitude.pitch_angle + 1700)*100/(1700+1700);


	if((Remote_receive_para.m115_attitude.pitch_angle > 
	ANGLE_PITCH_UP_WARNING*(100+Remote_setting_para.angleAlarmPrecent)/100)){
		if(++pitchAngleCount == 2){
			Remote_setting_para.SpeedGear = eGearOne;
			pitchAngleCount = 0;
		}
		if(Remote_setting_para.AlarmSwitch){
			LoopQueue_Write(&music_FIFO,&num,1);
		}
	}else if((Remote_receive_para.m115_attitude.pitch_angle < 
	ANGLE_PITCH_DOWN_WARNING*(100+Remote_setting_para.angleAlarmPrecent)/100)){
		if(++pitchAngleCount == 3){
			Remote_setting_para.SpeedGear = eGearOne;
			pitchAngleCount = 0;
		}
		if(Remote_setting_para.AlarmSwitch){
			LoopQueue_Write(&music_FIFO,&num,1);
		}
	}else{
		pitchAngleCount = 0;
	}

	if((abs(Remote_receive_para.m115_attitude.roll_angle) > 
	ANGLE_ROLL_MAX)){
		// Remote_setting_para.SpeedGear = eGearOne;
		if(Remote_setting_para.AlarmSwitch){
			// LoopQueue_Write(&music_FIFO,&num,1);
		}
	}

	num = MUSIC_ULRASONIC_ATTEN;
	for(u8 i = 0;i<3;i++){
		if(Remote_receive_para.m115_attitude.ultrasonic[i] != 0 
		&& Remote_receive_para.m115_attitude.ultrasonic[i] < ULTRASONIC_DISTANCE_MAX){

			if(Remote_setting_para.AlarmSwitch
			&&(Remote_setting_para.CoordY != Coord_Base || Remote_setting_para.CoordX != Coord_Base)
			&&(++ulrasonicCount)*SUBTIME_S > Remote_receive_para.m115_attitude.ultrasonic[i]){
				LoopQueue_Write(&music_FIFO,&num,1);
				ulrasonicCount = 0;
				break;
			}
		} 
	}
}

void CANCheckValue_s(void){
	

    if(Remote_receive_para.BatteryVol < BATTERY_MIN_VOL) Remote_setting_para.Battery = 0;
    else if(Remote_receive_para.BatteryVol >= BATTERY_MAX_VOL) Remote_setting_para.Battery = 100;
    else Remote_setting_para.Battery = (Remote_receive_para.BatteryVol-BATTERY_MIN_VOL)*100/(BATTERY_MAX_VOL-BATTERY_MIN_VOL);


#if 1
	Remote_trans_para.errorFlag.all[0] &= 0xff>>(8-HANDLE_ERROR_NUM);

	Remote_trans_para.errorFlag.all[0] ^= Remote_receive_para.errorFlag.all[0]<<HANDLE_ERROR_NUM;
#else
	Remote_receive_para.errorFlag.all[0] <<= HANDLE_ERROR_NUM;
	Remote_trans_para.errorFlag.bit.leftWheelError = Remote_receive_para.errorFlag.bit.leftWheelError;
	Remote_trans_para.errorFlag.bit.rightWheelError = Remote_receive_para.errorFlag.bit.rightWheelError;
	Remote_trans_para.errorFlag.bit.pushRodError = Remote_receive_para.errorFlag.bit.pushRodError;
	Remote_trans_para.errorFlag.bit.seatRodError = Remote_receive_para.errorFlag.bit.seatRodError;
	Remote_trans_para.errorFlag.bit.breakError = Remote_receive_para.errorFlag.bit.breakError;
#endif


}


typedef struct {
	void *addr;
	uint8_t size;
}Recv_Reg_Info;

Recv_Reg_Info RecvRegInfos[16];

void CANPackAnalyze(MasterMsgData_t *recvMessage){
	static u8 subflag = 0;
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
			CANCheckValue_S();
		}else if(recvMessage->msg_id == 's'){
			u8 index = 0,regIndex = 8;
			while (index < recvMessage->Length){
				if(RecvRegInfos[regIndex].addr == NULL)return;
				memcpy(RecvRegInfos[regIndex].addr,&recvMessage->Data[index],RecvRegInfos[regIndex].size);
				index+=RecvRegInfos[regIndex++].size;
			}
			CANCheckValue_s();
		}
		else{
		}
		subflag = 1;
		break;
	case FUNC_WRITE:
		if(recvMessage->msg_id == 'I'){
			Remote_setting_para.RemoteConnect = eWire;
		}else if(recvMessage->msg_id == 'W'){
			if(subflag){
				Remote_setting_para.RemoteCount = 0;
				Remote_trans_para.errorFlag.bit.CANDisconnectError = 0;
				subflag = 0;
			}
		}
		break;
	
	default:
		break;
	}


}
#define ADD_SUB_REG(para,regName){\
	RecvRegInfos[regIndex].addr = (&para);\
	RecvRegInfos[regIndex].size = sizeof(para);\
	combine_sub.combine_read.combine[combine_sub.combine_read.combineCount].address =\
	tranRegInfoDir[(regName)].addr;\
	combine_sub.combine_read.combine[combine_sub.combine_read.combineCount].size = \
	sizeof(para);\
	++combine_sub.combine_read.combineCount;\
	++regIndex;\
	}

void CANSubInit(void){

	// PDU_t req_PDU;
	// MsgData_t request_msg_data;
	combine_subscribe_t combine_sub;
	u8 regIndex = 0;

#if 1
	memset(&combine_sub,0,sizeof(combine_sub));
	combine_sub.timing = SUBTIME_S;

	ADD_SUB_REG(Remote_receive_para.m115_attitude,PCB_ANGLE);
	// sizeof(Remote_receive_para.m115_attitude);

	combine_subscribe_to_PDU('S',&combine_sub,&req_PDU);
	PDU_to_msg_data(NODE_SELF, &req_PDU, &request_msg_data);
	send_message_data(&request_msg_data);

	// delay_ms(20);
#endif

	memset(&combine_sub,0,sizeof(combine_sub));
	combine_sub.timing = SUBTIME_s;
	regIndex = 8;

	ADD_SUB_REG(Remote_receive_para.walking_speed_L,BLDC_A_SPEED_REAL);
	
	ADD_SUB_REG(Remote_receive_para.walking_speed_R,BLDC_U_SPEED_REAL);

	ADD_SUB_REG(Remote_receive_para.push_rod_speed,BRUSH_A_SPEED_REAL);

	ADD_SUB_REG(Remote_receive_para.seat_rod_speed,BRUSH_U_SPEED_REAL);

	ADD_SUB_REG(Remote_receive_para.BatteryVol,DEVICE_VOLT);

	ADD_SUB_REG(Remote_receive_para.errorFlag.all[0],APP_ERROR_REG);


	combine_subscribe_to_PDU('s',&combine_sub,&req_PDU);
	PDU_to_msg_data(NODE_SELF, &req_PDU, &request_msg_data);
	send_message_data(&request_msg_data);
	
}




void protocolSend(void){
	if(CANTxCombineWrite.combineCount > 0){
		combine_write_to_PDU('W',&CANTxCombineWrite,&req_PDU);
		PDU_to_msg_data(NODE_SELF, &req_PDU, &request_msg_data);
		send_message_data(&request_msg_data);
	}
}

void CANNoneOsTxTask(void){
	if(0||Remote_setting_para.RemoteConnect == eWire){
		CANProtocolTransbuff(&CANTxCombineWrite,CANTxBuffer);
		protocolSend();
	}
	if(1 && ++Remote_setting_para.RemoteCount>70){
		Remote_setting_para.RemoteConnect = eOffline;
		Remote_setting_para.RemoteCount = 0;
		Remote_trans_para.errorFlag.bit.CANDisconnectError = 1;
		paraInit();
		CANSubInit();
	}
}

void CANNoneOsRxTask(void){
	can_receive();
}

void api_on_message_received(MasterMsgData_t message)
{

	#if USING_RTOS 
	xQueueSend(rxQueue, &message, 0);
	#else
	// memcpy(&recvMessage,&message,sizeof(MasterMsgData_t));
	CANPackAnalyze(&message);
	#endif
}

void api_port_send(uint16_t len, uint8_t * data) 
{
	can_serial_write(data,len);
	// can_transmit();		
}

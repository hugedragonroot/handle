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
void api_on_message_received(MasterMsgData_t message)
{

	#if USING_RTOS 
	xQueueSend(rxQueue, &message, 0);
	#else
	// memcpy(&recvMessage,&message,sizeof(MasterMsgData_t));
	CANPackAnalyze(&message);
	#endif
}

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
	if(++Remote_setting_para.RemoteCount == 70){
		Remote_setting_para.RemoteConnect = eOffline;
		Remote_setting_para.RemoteCount = 0;
		CANSubInit();
	}
}

void CANNoneOsRxTask(void){
	can_receive();
}

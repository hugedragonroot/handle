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

static bool isInit;
// static TCOMMUN RxPack;

//static combine_write_t combine_wirte;
static xQueueHandle  txQueue;
static xQueueHandle  rxQueue;


// MasterMsgData_t recvMessage;
void api_on_message_received(MasterMsgData_t message)
{
	// memcpy(&recvMessage,&message,sizeof(MasterMsgData_t));

	xQueueSend(rxQueue, &message, 0);
}


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

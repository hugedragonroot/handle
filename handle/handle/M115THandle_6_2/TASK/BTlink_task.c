#include <stdbool.h>
#include <string.h>
#include "BTlink_task.h"
/*FreeRtos includes*/

#if 0
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#endif

#include "key_ext.h"
#include "uart_ext.h"
#include "app_protocol.h"
#include "debug.h"

static TCOMMUN RxPack;

#if USING_RTOS
static bool isInit;
static xQueueHandle  txQueue;
static xQueueHandle  rxQueue;

/*usb连接初始化*/
void BTlink_Init(void)
{
	if (isInit) return;
	txQueue = xQueueCreate(16, sizeof(TCOMMUN));
	ASSERT(txQueue);
	rxQueue = xQueueCreate(16, sizeof(TCOMMUN));
	ASSERT(rxQueue);
	isInit = true;
}
bool BTlinkReceivePacketBlocking(TCOMMUN *p)
{
	ASSERT(p);
	return xQueueReceive(rxQueue, p, portMAX_DELAY);
}
#endif

#if USING_RTOS
void BTRxTask(void* param)
{
	uint8_t ch;
	uint8_t state;
	uint8_t i=0;
	uint8_t sumchk=0;
	while(1)
	{
		if (BTGetDataWithTimout(&ch)&&(Remote_setting_para.PowerStatus == ePowerOn))
		{			
			switch(state)
			{	
				case 0:
					state = (ch==FrameHeadUpH)?1:0;
					sumchk =ch;					
					break;
				case 1:									
					state = (ch==FrameHeadUpL)?2:0;					
					sumchk +=ch;				
					break;					
				case 2:
					RxPack.DataLen = ch;
					sumchk +=ch;				
					state++;
				break;		
				case 3:
					RxPack.Count = ch;
					sumchk +=ch;				
					state++;
				break;	
				case 4:
					RxPack.Cmd = ch;
					sumchk +=ch;				
					state++;
					i=0;
				break;
				case 5:
					RxPack.Data[i]=ch;
					sumchk +=ch;				
					i++;
					if(i==RxPack.DataLen)
					{
						state++;
						i=0;
					}
				break;
				case 6:
					if(sumchk==ch)
					{
						xQueueSend(rxQueue, &RxPack, 0);
					}
					state=0;
					sumchk = 0;					
				break;
				default:
					break;			
			}					
		}
		else	/*超时处理*/
		{
			state=0;
		}		
	}	
}
#else



void BTNoneOSTask(void){
	uint8_t ch = 0;
	uint8_t state = 0;
	uint8_t i=0;
	uint8_t sumchk=0;
#if 1
	while(BTGetDataWithTimout(&ch)) {			
		switch(state)
		{	
			case 0:
				state = (ch==FrameHeadUpH)?1:0;
				sumchk =ch;					
				break;
			case 1:									
				state = (ch==FrameHeadUpL)?2:0;					
				sumchk +=ch;				
				break;					
			case 2:
				RxPack.DataLen = ch;
				sumchk +=ch;				
				state++;
			break;		
			case 3:
				RxPack.Count = ch;
				sumchk +=ch;				
				state++;
			break;	
			case 4:
				RxPack.Cmd = ch;
				sumchk +=ch;				
				state++;
				i=0;
			break;
			case 5:

				RxPack.Data[i]=ch;
				sumchk +=ch;				
				i++;
				if(i==RxPack.DataLen)
				{
					state++;
					i=0;
				}
			break;
			case 6:
				if(sumchk==ch)
				{
					// xQueueSend(rxQueue, &RxPack, 0);
					BTNoneOsProcessTask(&RxPack);
				}
				state=0;
				sumchk = 0;					
			break;
			default:
				break;			
		}					
		if(state == 0) return;
	}

	#else
	if(BTGetPack(&RxPack)&&(Remote_setting_para.PowerStatus == ePowerOn)){
		BTNoneOsProcessTask(&RxPack);
	}
	#endif
}


#endif


/*BT连接发送Packet*/

#if USING_RTOS
bool BTlinkSendPacket(const TCOMMUN *p)
{
	ASSERT(p);
	return xQueueSend(txQueue, p, 0);
}
void BTTxTask(void* param)
{
	TCOMMUN p;
	uint8_t sendBuffer[64];
	uint8_t cksum;
	uint8_t dataLen;	
	uint8_t i;
	
	while(1)
	{
		xQueueReceive(txQueue, &p, portMAX_DELAY);
		sendBuffer[0] = FrameHeadUpH;
		sendBuffer[1] = FrameHeadUpL;
		sendBuffer[2] = p.DataLen;
		sendBuffer[3] = p.Count;
		sendBuffer[4] = p.Cmd;
		memcpy(&sendBuffer[5], p.Data, p.DataLen);
		cksum = 0;
		for ( i=0; i<p.DataLen+5; i++)
		{
			cksum += sendBuffer[i];
		}
		dataLen = p.DataLen+6;
		sendBuffer[dataLen - 1] = cksum;
		uart_bt_send(sendBuffer, dataLen);		
	}	
}
#else


uint8_t btSendBuffer[64];
bool BTlinkSendPacket(const TCOMMUN *p){
	uint8_t cksum;
	uint8_t dataLen;	

	btSendBuffer[0] = FrameHeadUpH;
	btSendBuffer[1] = FrameHeadUpL;
	btSendBuffer[2] = p->DataLen;
	btSendBuffer[3] = p->Count;
	btSendBuffer[4] = p->Cmd;
	memcpy(&btSendBuffer[5], p->Data, p->DataLen);
	cksum = 0;
	for (uint8_t i = 0; i<p->DataLen+5; i++)
	{
		cksum += btSendBuffer[i];
	}
	dataLen = p->DataLen+6;
	btSendBuffer[dataLen - 1] = cksum;

	uart_wire_send(btSendBuffer, dataLen);
	return uart_bt_send(btSendBuffer, dataLen);
}


#endif


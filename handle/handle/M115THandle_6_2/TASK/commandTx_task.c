#include <stdbool.h>
#include "commandTx_task.h"
/*FreeRtos includes*/
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "app_protocol.h"
#include "wirelink_task.h"
#include "app_remote_para.h"
#include "uart_ext.h"
#include "can_ext.h"
#include "../PROTOCOL/protocol_427/protocol.h"

#if 0
// uint8_t buffer_test[3] ={0x00,0x00,0x01};

//读版本号
const uint8_t buffer_test[] ={
0x5a,0x5a,0x01,0x0a,0x03,0x00,0x0e,0x00,
0x00,0x05,0x00,0x01,0x00,0x00,0x04,0x00,
0x31,0x8f
};

// MasterMsgData_t sendMessage;
MasterMsgData_t recvMessage;
void api_on_message_received(MasterMsgData_t message)
{
	memcpy(&recvMessage,&message,sizeof(MasterMsgData_t));
}

int16_t leftSpeed = 500;
int16_t rightSpeed = 1500;
u8 readFlag = 1;
void test_protocol(void)
{
	static u8 msgID = 0;
	PDU_t req_PDU;
	MsgData_t request_msg_data;
	u8 sendBuffer[4];
	//读
	#if 1
	
	combine_read_t combine_read;
	combine_read.combineCount = 1;	//读以下1个组合
	combine_read.combine[0].address = 0x0000;
	combine_read.combine[0].size = 4;
	combine_read_to_PDU(0, &combine_read, &req_PDU);
	PDU_to_msg_data(NODE_SELF, &req_PDU, &request_msg_data);
	send_message_data(&request_msg_data);
	#endif


	//写
	#if 1
	if(!msgID){
		u8 write_buff[4]= {0x11,0x22,0x33,0x44};
		combine_write_t combine_wirte;
		combine_wirte.combineCount = 1;
		combine_wirte.combine_data[0].combine.address = 0x0000;
		combine_wirte.combine_data[0].combine.size = 4;
		combine_wirte.combine_data[0].data =write_buff;

		combine_write_to_PDU(msgID++,&combine_wirte,&req_PDU);
		PDU_to_msg_data(NODE_SELF, &req_PDU, &request_msg_data);
		send_message_data(&request_msg_data);
	}
	#endif

	//订阅
	#if 1
	if(!msgID){
		combine_subscribe_t combine_sub;
		combine_sub.timing = 100;
		combine_sub.combine_read.combineCount = 1;
		combine_sub.combine_read.combine[0].address = 0x0000;
		combine_sub.combine_read.combine[0].size = 4;

		combine_subscribe_to_PDU(msgID++,&combine_sub,&req_PDU);
		PDU_to_msg_data(NODE_SELF, &req_PDU, &request_msg_data);
		send_message_data(&request_msg_data);
	}
	#endif

  if(!readFlag){
	#if 1//写电机速度
	sendBuffer[0] = (leftSpeed) & 0xff;
	sendBuffer[1] = (leftSpeed>>8) & 0xff;
	sendBuffer[2] = (rightSpeed) & 0xff;
	sendBuffer[3] = (rightSpeed>>8) & 0xff;


	combine_write_t combine_wirte;
	combine_wirte.combineCount = 2;
	// combine_wirte.combineCount = 1;
	combine_wirte.combine_data[0].combine.address = 0x000C;//左轮速度
	combine_wirte.combine_data[0].combine.size = 2;
	combine_wirte.combine_data[0].data = &sendBuffer[0];

	combine_wirte.combine_data[1].combine.address = 0x0036;//右轮速度
	combine_wirte.combine_data[1].combine.size = 2;
	combine_wirte.combine_data[1].data = &sendBuffer[2];

	combine_write_to_PDU(0,&combine_wirte,&req_PDU);
	PDU_to_msg_data(NODE_SELF, &req_PDU, &request_msg_data);
	send_message_data(&request_msg_data);
	#endif
	}
  else{
    #if 1//读电机速度
	combine_read_t combine_read;
	combine_read.combineCount = 2;	//读以下2个组合
	combine_read.combine[0].address = 0x000C;
	combine_read.combine[0].size = 2;
	combine_read.combine[1].address = 0x0036;
	combine_read.combine[1].size = 2;

	combine_read_to_PDU(3, &combine_read, &req_PDU);
	PDU_to_msg_data(NODE_SELF, &req_PDU, &request_msg_data);
	send_message_data(&request_msg_data);
	#endif
}

	//接收
	u8 rx_buff[32];
	can_receive();
	memcpy(rx_buff,recvMessage.Data,recvMessage.Length);
	//memset(&recvMessage,0x00,sizeof(recvMessage));
}
#endif


// #define USING_TASK_DEBUG 1
// #if USING_TASK_DEBUG
// u8 debugTimeCount = 0;
// char sendBuffer[128];
// #endif

#if USING_RTOS
void commandTxTask(void* param)
{
	#if !USING_CAN
	TCOMMUN p;
	#else
	combine_write_t p;
	u8 CANTxBuffer[128];

	#endif

	while(1)
	{
		// vTaskDelay(100);
		vTaskDelay(10);

		fwdgt_feed();

		if(Remote_setting_para.PowerStatus == ePowerOn )
		{
			app_cltprocess();
			#if !USING_CAN
			ProtocolTransbuff(&p);
			wirelinkSendPacket(&p);
			#else
			CANProtocolTransbuff(&p,CANTxBuffer);
			CANlinkSendPacket(&p);

			#endif

		}	

		#if USING_TASK_DEBUG
		if(++debugTimeCount > 250){
			debugTimeCount = 0;
			memset(sendBuffer,0,sizeof(sendBuffer));
			sendBuffer[0] = '\n';
			vTaskList((char *)&sendBuffer[1]);
			uart_wire_send((uint8_t *)sendBuffer,strlen((char *)sendBuffer));
		}
		#endif



		// can_serial_write(buffer_test,sizeof(buffer_test));
		// can_transmit();		
		// can_receive();
		// test_protocol();
	
	}
}
#endif

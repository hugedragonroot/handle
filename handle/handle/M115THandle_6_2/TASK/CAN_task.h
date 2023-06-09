#ifndef __CAN_TASK_H
#define __CAN_TASK_H
#include <stdbool.h>
#include "app_protocol.h"

#define USING_CAN 1


void CAN_Init(void);
bool CANlinkSendPacket(const combine_write_t *p);
void CANTxTask(void* param);

bool CANlinkReceivePacketBlocking(MasterMsgData_t *p);
void CANRxTask(void* param);


#endif

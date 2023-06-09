#ifndef __BTLINK_TASK_H
#define __BTLINK_TASK_H
#include <stdbool.h>
#include "app_protocol.h"

void BTlink_Init(void);
bool BTlinkReceivePacketBlocking(TCOMMUN *p);
void BTRxTask(void* param);

bool BTlinkSendPacket(const TCOMMUN *p);
void BTTxTask(void* param);
#endif


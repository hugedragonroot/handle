#ifndef __WIRELINK_TASK_H
#define __WIRELINK_TASK_H
#include <stdbool.h>
#include "app_protocol.h"

void wire_Init(void);
bool wirelinkSendPacket(const TCOMMUN *p);
bool wirelinkReceivePacketBlocking(TCOMMUN *p);
void wireRxTask(void* param);
void wireTxTask(void* param);
#endif


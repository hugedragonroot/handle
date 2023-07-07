#ifndef __DATA_PACK_TASK_H
#define __DATA_PACK_TASK_H

#include "protocol.h"
#include "app_protocol.h"

void wireProcessTask(void* param);	
void BTProcessTask(void* param);	
void CANProcessTask(void* param);


void CANSubInit(void);
void CANPackAnalyze(MasterMsgData_t *recvMessage);


void BTNoneOsProcessTask(TCOMMUN *p);

#endif


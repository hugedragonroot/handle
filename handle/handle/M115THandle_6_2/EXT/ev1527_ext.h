#ifndef __EV1527_EXT_H
#define __EV1527_EXT_H

#include "gd32f30x.h"
#include "String.h"

void ev1527_init(void);
void ev1527_task(void);
void ev1527_scan(void);
void ev1527_key_scan(void);

void ev1527_set_userID(uint8_t *setUserID[],uint8_t userNum);
#endif

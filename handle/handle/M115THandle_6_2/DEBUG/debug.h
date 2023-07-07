#ifndef __DEBUG_H
#define __DEBUG_H

#include "main.h"

#define ASSERT(e)  if (e) ; \
        else assertFail( #e, __FILE__, __LINE__ )
void debug_test(void);
void assertFail(char *exp, char *file, int line);

void debugTask(void* param);
void debugUartPrint(void);
#endif

#ifndef __BT_EXT_H
#define __BT_EXT_H

#include "stdint.h"

/*ÐÅÏ¢Êä³ö*/
#define BT_DEBUG_ON        0
#define BT_DEBUG_FUNC_ON    0

#define BT_INFO(fmt,arg...)           printf("<<-BT-INFO->> "fmt"\r\n",##arg)
#define BT_ERROR(fmt,arg...)          printf("<<-BT-ERROR->> "fmt"\r\n",##arg)
#define BT_DEBUG(fmt,arg...)          do{\
                                          if(BT_DEBUG_ON)\
                                          printf("<<-BT-DEBUG->> [%d]"fmt"\r\n",__LINE__, ##arg);\
                                          }while(0)

#define BT_DEBUG_FUNC()               do{\
                                         if(BT_DEBUG_FUNC_ON)\
                                         printf("<<-BT-FUNC->> Func:%s@Line:%d\r\n",__func__,__LINE__);\
                                       }while(0)

	
void BLT_Usart_SendString(uint8_t *str);
void bt_init(void);
int BTGetDataWithTimout(uint8_t *c);

#endif

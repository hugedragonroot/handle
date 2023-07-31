#ifndef __BSP_WWDG_H
#define	__BSP_WWDG_H


#include "stm32f10x.h"

/* ���ڼ�����ֵ,��ΧΪ:0x40~0x7f,һ�����ó����0X7F */
#define WWDG_CNT     0X7F
#define WWDG_Feed    WWDG_SetCounter( WWDG_CNT );


void WWDG_Config(uint8_t tr, uint8_t wr, uint32_t prv);

#endif /* __BSP_WWDG_H */

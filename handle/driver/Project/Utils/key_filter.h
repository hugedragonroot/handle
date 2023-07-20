/**********************************************************/
//@Autor:Raw
//@Version:0.0.1
//@End Time:2022/02/06
/**********************************************************/
#ifndef __KEY_FILTER_H__
#define __KEY_FILTER_H__
#include "common.h"
#define keyNum	5

typedef enum
{
	key_top = 0,
	key_zero = 1,
	key_bottom = 2,
	key_bz1 = 3,
	key_bz2 = 4,
}key_num;
typedef struct
{
  uint32_t holdTime;
  uint32_t timePoint;
  uint32_t triggerVariable;
  uint8_t lastVarialbeState;
  uint8_t result;
} str_hold_filter;
extern str_hold_filter Spacing[keyNum];
void Key_Array_Init(void);
uint8_t hold_filter(str_hold_filter *filter_p, uint32_t variable,uint32_t ms_count);
#endif

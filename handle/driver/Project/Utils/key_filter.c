#include "key_filter.h"

str_hold_filter Spacing[keyNum];
/*结构体初值*/
void Key_Array_Init(void)
{
  str_hold_filter *key_p;
  key_p = &Spacing[0];
  key_p->holdTime = 3;
  key_p->timePoint = 0;
  key_p->triggerVariable = 0;
  key_p->result = 0;
	
	key_p = &Spacing[1];
  key_p->holdTime = 3;
  key_p->timePoint = 0;
  key_p->triggerVariable = 0;
  key_p->result = 0;
	
	key_p = &Spacing[2];
  key_p->holdTime = 3;
  key_p->timePoint = 0;
  key_p->triggerVariable = 0;
  key_p->result = 0;
  

}

/*保持型滤波*/
uint8_t hold_filter(str_hold_filter *filter_p, uint32_t variable,uint32_t ms_count)
{
  uint8_t varialbeState = 0;
  uint32_t passTime, systemTimeTemp;
  systemTimeTemp = ms_count;
  passTime = systemTimeTemp - filter_p->timePoint;

  if (filter_p->triggerVariable == variable)
  {
    varialbeState = 1;
  }

  if (varialbeState == filter_p->lastVarialbeState)
  {
    if (filter_p->lastVarialbeState)
    {
      if (passTime > filter_p->holdTime && filter_p->result == 1)
      {
        filter_p->result = 0;
      }
    }
    else
    {
      if (passTime > filter_p->holdTime && filter_p->result == 0)
      {
        filter_p->result = 1;
      }
    }
  }
  else
  {
    if (passTime >= filter_p->holdTime)
    {
      filter_p->timePoint = systemTimeTemp;
    }
    else
    {
      filter_p->timePoint = systemTimeTemp - (filter_p->holdTime - passTime);
    }
  }

  filter_p->lastVarialbeState = varialbeState;

  return filter_p->result;
}












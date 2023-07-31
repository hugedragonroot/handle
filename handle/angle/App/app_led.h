#ifndef __APP_LED_H
#define __APP_LED_H

#include <stdint.h>
#include <stdbool.h>


#define LEDNUMBER     3

#define LED_ONE       0
#define LED_TWO       1
#define LED_THREE     2

/* led状态指示 */
typedef enum
{
	atOFF   = 0,
	atON    = 1,
	ONtoOFF = 2,
	OFFtoON = 3,
} LED_type;

typedef struct ledOperation
{
  bool      comfirmFlag;
  uint8_t   typeFlag;     /* 0.off  1.on  2.on>>off  3.off>>on */
  uint16_t  count;
  uint16_t  onTime;
  uint16_t  offTime;
  bool      enableFlag;   /* 使能标志 */

  void (*ledOn)(void);
  void (*ledOff)(void);
  void (*ledToggle)(void);
} LEDOPERATION;


void app_LedInit(void);
void app_LedSetMode(uint8_t ledport, uint16_t ledon, uint16_t ledoff, bool isConfirm);
void app_LedSetEnable(uint8_t ledport, bool able);
void app_LedReset(uint8_t ledport);
void app_LedUpdataState(void);

#endif  /* __APP_LED_H */

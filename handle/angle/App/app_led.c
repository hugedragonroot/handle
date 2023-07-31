#include "app_led.h"
#include ".\gpio\bsp_gpio.h"
#include <string.h>


static struct ledOperation led[LEDNUMBER];


/**
  * @brief  LED_ONE
  * @param  None
  * @retval None
  */
static void LED_ONE_ON(void)
{
  LED1_ON;
}
static void LED_ONE_OFF(void)
{
  LED1_OFF;
}
static void LED_ONE_TOGGLE(void)
{
  LED1_TOGGLE;
}

/**
  * @brief  LED_TWO
  * @param  None
  * @retval None
  */
static void LED_TWO_ON(void)
{
  LED2_ON;
}
static void LED_TWO_OFF(void)
{
  LED2_OFF;
}
static void LED_TWO_TOGGLE(void)
{
  LED2_TOGGLE;
}

/**
  * @brief  LED_THREE
  * @param  None
  * @retval None
  */
static void LED_THREE_ON(void)
{
  LED3_ON;
}
static void LED_THREE_OFF(void)
{
  LED3_OFF;
}
static void LED_THREE_TOGGLE(void)
{
  LED3_TOGGLE;
}

/**
  * @brief  LED控制初始化
  * @param  None
  * @retval None
  */
void app_LedInit(void)
{
  memset(led, 0, sizeof(struct ledOperation) * LEDNUMBER);
  led[LED_ONE].ledOn       = LED_ONE_ON;
  led[LED_ONE].ledOff      = LED_ONE_OFF;
  led[LED_ONE].ledToggle   = LED_ONE_TOGGLE;
  led[LED_ONE].enableFlag  = true;
  
  led[LED_TWO].ledOn       = LED_TWO_ON;
  led[LED_TWO].ledOff      = LED_TWO_OFF;
  led[LED_TWO].ledToggle   = LED_TWO_TOGGLE;
  led[LED_TWO].enableFlag  = true;
  
  led[LED_THREE].ledOn       = LED_THREE_ON;
  led[LED_THREE].ledOff      = LED_THREE_OFF;
  led[LED_THREE].ledToggle   = LED_THREE_TOGGLE;
  led[LED_THREE].enableFlag  = true;
}

/**
  * @brief  设置使能
  * @param  None
  * @retval None
  */
void app_LedSetEnable(uint8_t ledport, bool able)
{
  led[ledport].enableFlag = able;
}

/* ************** led模式设置 **************
* 输入参数：
*           ledport     led编号
*           ledon.      led点亮时间s    (50的倍数) 为0时常灭
*           ledoff.     led灭时间s         (50的倍数) 为0时常亮
*           isConfirm.  led快闪3次提示
*               false:不提示   true:提示
* **************************************/
void app_LedSetMode(uint8_t ledport, uint16_t ledon, uint16_t ledoff, bool isConfirm)
{
  if(led[ledport].onTime == (ledon / 50) && 
     led[ledport].offTime == (ledoff / 50) &&
     led[ledport].comfirmFlag == isConfirm) {
    return;
  }

  led[ledport].count = 0;
  led[ledport].ledOff();
  led[ledport].onTime = ledon / 50;
  led[ledport].offTime = ledoff / 50;
  led[ledport].comfirmFlag = isConfirm;

  if(led[ledport].onTime == 0) {
    led[ledport].typeFlag = atOFF;
  } else if(led[ledport].offTime == 0) {
    led[ledport].typeFlag = atON;
  } else {
    led[ledport].typeFlag = ONtoOFF;
    led[ledport].ledOn();
  }
}

/* ************** led提示 **************
* led快闪3次提示
* **************************************/
void app_LedReset(uint8_t ledport) 
{
  led[ledport].count = 0;
  led[ledport].ledOff();
  led[ledport].comfirmFlag = true;
}

/* -----------------------
* 		LED updata state
*		freq: 50ms
* -----------------------*/
void app_LedUpdataState(void)
{
	uint8_t led_count = 0;
  
	for(; led_count < LEDNUMBER; led_count++) {
    if(!led[led_count].enableFlag) continue;
    led[led_count].count ++;
    
    if(led[led_count].comfirmFlag == true) {/* 快闪处理 */
      led[led_count].ledToggle();
      if(led[led_count].count >= 10) {
        led[led_count].comfirmFlag = false;
        led[led_count].count = 0;
        if(led[led_count].onTime == 0) {
          led[led_count].typeFlag = atOFF;
        } else if(led[led_count].offTime == 0) {
          led[led_count].typeFlag = atON;
        } else {
          led[led_count].typeFlag = ONtoOFF;
          led[led_count].ledOn();
        }
      }
    } else {
      if(led[led_count].typeFlag == ONtoOFF) {
        if(led[led_count].count >= led[led_count].onTime) {
          led[led_count].count = 0;
          led[led_count].typeFlag = OFFtoON;
          led[led_count].ledOff();
        }
      } else if(led[led_count].typeFlag == OFFtoON) {
        if(led[led_count].count >= led[led_count].offTime) {
          led[led_count].count = 0;
          led[led_count].typeFlag = ONtoOFF;
          led[led_count].ledOn();
        }
      } else if(led[led_count].typeFlag == atON) {   //always on
        led[led_count].ledOn();
      } else {   //always off
        led[led_count].ledOff();
      }
    }
  }
}

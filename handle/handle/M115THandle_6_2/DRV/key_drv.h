#ifndef __KEY_DRV_H
#define __KEY_DRV_H
#include "main.h"

#define USING_TPAD 0

#if USING_TPAD

#define TPAD_NUM 4
#define TPAD_GATE_VAL 50

#define KEY_SPEED_ADD_PORT  GPIOC
#define KEY_SPEED_ADD_PIN   GPIO_PIN_8

#define KEY_SPEED_SUB_PORT  GPIOA
#define KEY_SPEED_SUB_PIN   GPIO_PIN_8

#define KEY_POWER_PORT  GPIOC
#define KEY_POWER_PIN   GPIO_PIN_9

#define KEY_PUSH_ROB_UP_PORT    GPIOC
#define KEY_PUSH_ROB_UP_PIN     GPIO_PIN_7

#define KEY_PUSH_ROB_DOWN_PORT  GPIOB
#define KEY_PUSH_ROB_DOWN_PIN   GPIO_PIN_14

#define KEY_HORN_PORT   GPIOC
#define KEY_HORN_PIN    GPIO_PIN_6

#else

#if USING_6KEY

#define KEY_SPEED_ADD_PORT  GPIOC
#define KEY_SPEED_ADD_PIN   GPIO_PIN_8

#define KEY_SPEED_SUB_PORT  GPIOA
#define KEY_SPEED_SUB_PIN   GPIO_PIN_8

#define KEY_POWER_PORT  GPIOC
#define KEY_POWER_PIN   GPIO_PIN_9

#define KEY_PUSH_ROB_ADD_PORT    GPIOC
#define KEY_PUSH_ROB_ADD_PIN     GPIO_PIN_7

#define KEY_PUSH_ROB_SUB_PORT  GPIOB
#define KEY_PUSH_ROB_SUB_PIN   GPIO_PIN_14

#define KEY_HORN_PORT   GPIOC
#define KEY_HORN_PIN    GPIO_PIN_6

#else
#define KEY_ROW1_GPIO_PORT 		GPIOB 

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#if USING_LED_POINT_DISPLAY
#define KEY_ROW1_PIN 					GPIO_PIN_14
#else
#define KEY_ROW1_PIN 					GPIO_PIN_15
#endif

#define KEY_ROW2_GPIO_PORT 		GPIOC 
#define KEY_ROW2_PIN 					GPIO_PIN_6
#define KEY_ROW3_GPIO_PORT 		GPIOC 
#define KEY_ROW3_PIN 					GPIO_PIN_7

#define KEY_COL1_GPIO_PORT 		GPIOC 
#define KEY_COL1_PIN 					GPIO_PIN_8
#define KEY_COL2_GPIO_PORT 		GPIOC 
#define KEY_COL2_PIN 					GPIO_PIN_9
#define KEY_COL3_GPIO_PORT 		GPIOA
#define KEY_COL3_PIN 					GPIO_PIN_8
#endif

#endif

void key_initIO(void);
#endif

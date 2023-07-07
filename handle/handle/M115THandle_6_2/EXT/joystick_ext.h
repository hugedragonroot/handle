#ifndef __JOYSTICK_EXT_H
#define __JOYSTICK_EXT_H

#include <stdint.h>
#include "main.h"
/************************* ҡ�˿��� *************************/
/*    ����ϵ
					^ Y 
					| 255 
					|
					|
					|
					|
					|
					|
0					| 127				255
---------------------------------------> X
					| 
					|
					|
					|
					|
					|
					|
					| 0		

//ʵ��ֵ 20 ~ 230 ��
*/

#define Coord_MAX             255     //�������ֵ
#define Coord_MIN             0       //������Сֵ

#define Coord_Base            127     //�������
#define Coord_UP	    				230     //ҡ��ǰ��		//0xe6
#define Coord_DOWN	    			26     	//ҡ�˺���	//0x1A

#define Coord_Dead           15

/* ң��ͨ�� */
typedef struct joystick_adc
{
	uint16_t valX;
	uint16_t valY;
	
}JOYSTICK;

extern JOYSTICK joyetick_adc;

void ADC_EXT_Init(void);

void scan_joyxy(void);
void joystick_init(void);

uint16_t GildeAverageValueFilter_MAG(uint16_t NewValue,uint16_t *Data,uint8_t len);
uint16_t deadband(uint16_t value, const uint16_t midval, const uint16_t threshold);

#endif

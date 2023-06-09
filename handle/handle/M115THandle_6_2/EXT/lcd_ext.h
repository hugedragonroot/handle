#ifndef __LCD_EXT_H
#define __LCD_EXT_H

#include "stdint.h"

void lcd_init(void);
void lcd_PowerOff(void);
void lcd_PowerOn(void);
void clear_screen(void);
void display_test(void);
void display_graphic(uint8_t column,uint8_t page,uint8_t w,uint8_t h,const uint8_t *dp);
void display_string_5x8_1(uint8_t column,uint8_t page,uint8_t *text);
void display_string_8x16(uint8_t column,uint8_t page,uint8_t reverse,uint8_t *text);
void clear_graphic(uint8_t column,uint8_t page,uint8_t w,uint8_t h);
void display_string_5x8(uint8_t column,uint8_t page,uint8_t reverse,uint8_t *text);
void lcd_refreshGram(void);

void center_display_string(uint8_t page,uint8_t reverse,uint8_t *text);
#endif

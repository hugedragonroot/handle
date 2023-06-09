#ifndef __LCD_DRV_H
#define __LCD_DRV_H

#include "gd32f30x.h"
#include "main.h"

#define LCD_CS_GPIO_PORT 				GPIOB 
#define LCD_CS_PIN							GPIO_PIN_12

#define LCD_RST_GPIO_PORT				GPIOB 
#define LCD_RST_PIN							GPIO_PIN_2

#define LCD_RS_GPIO_PORT 				GPIOB 
#define LCD_RS_PIN							GPIO_PIN_1

#define LCD_CLK_GPIO_PORT 			GPIOB 
#define LCD_CLK_PIN							GPIO_PIN_14

#define LCD_DAT_GPIO_PORT 			GPIOB 
#define LCD_DAT_PIN							GPIO_PIN_13

//#define LCD_ROM_IN_GPIO_PORT		GPIOA 
//#define LCD_ROM_IN_PIN					GPIO_PIN_6

//#define LCD_ROM_OUT_GPIO_PORT		GPIOA 
//#define LCD_ROM_OUT_PIN					GPIO_PIN_7

//#define LCD_ROM_CLK_GPIO_PORT		GPIOC 
//#define LCD_ROM_CLK_PIN					GPIO_PIN_4

//#define LCD_ROM_CS_GPIO_PORT 		GPIOC 
//#define LCD_ROM_CS_PIN					GPIO_PIN_5

#define LCD_LCON_GPIO_PORT 			GPIOB
#define LCD_LCON_PIN						GPIO_PIN_0


#define LCD_CLK_OFF() 		gpio_bit_write(LCD_CLK_GPIO_PORT,LCD_CLK_PIN,RESET)//SDA
#define LCD_CLK_ON() 			gpio_bit_write(LCD_CLK_GPIO_PORT,LCD_CLK_PIN,SET)

#define LCD_DAT_OFF() 		gpio_bit_write(LCD_DAT_GPIO_PORT,LCD_DAT_PIN,RESET)//SCL
#define LCD_DAT_ON() 			gpio_bit_write(LCD_DAT_GPIO_PORT,LCD_DAT_PIN,SET)

#define LCD_RS_OFF()  		gpio_bit_write(LCD_RS_GPIO_PORT,LCD_RS_PIN,RESET)//RES
#define LCD_RS_ON()  			gpio_bit_write(LCD_RS_GPIO_PORT,LCD_RS_PIN,SET)

#define LCD_RST_OFF()  		gpio_bit_write(LCD_RST_GPIO_PORT,LCD_RST_PIN,RESET)//DC
#define LCD_RST_ON()  		gpio_bit_write(LCD_RST_GPIO_PORT,LCD_RST_PIN,SET)

#define LCD_CS_OFF()  		gpio_bit_write(LCD_CS_GPIO_PORT,LCD_CS_PIN,RESET)//CS1
#define LCD_CS_ON()  			gpio_bit_write(LCD_CS_GPIO_PORT,LCD_CS_PIN,SET)


#define LCD_ROM_OUT_OFF()	gpio_bit_write(LCD_ROM_OUT_GPIO_PORT,LCD_ROM_OUT_PIN,RESET)//
#define LCD_ROM_OUT_ON()	gpio_bit_write(LCD_ROM_OUT_GPIO_PORT,LCD_ROM_OUT_PIN,SET)

#define LCD_ROM_CLK_OFF()	gpio_bit_write(LCD_ROM_CLK_GPIO_PORT,LCD_ROM_CLK_PIN,RESET)//
#define LCD_ROM_CLK_ON()	gpio_bit_write(LCD_ROM_CLK_GPIO_PORT,LCD_ROM_CLK_PIN,SET)

#define LCD_ROM_CS_OFF()	gpio_bit_write(LCD_ROM_CS_GPIO_PORT,LCD_ROM_CS_PIN,RESET)//
#define LCD_ROM_CS_ON()		gpio_bit_write(LCD_ROM_CS_GPIO_PORT,LCD_ROM_CS_PIN,SET)



#define	LCON_ON()					gpio_bit_write(LCD_LCON_GPIO_PORT,LCD_LCON_PIN,RESET)
#define	LCON_OFF()				gpio_bit_write(LCD_LCON_GPIO_PORT,LCD_LCON_PIN,SET)

void lcd_initIO(void);
void lcd_initIO_PwrOff(void);
#endif


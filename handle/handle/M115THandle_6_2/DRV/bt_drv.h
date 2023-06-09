#ifndef __BT_DRV_H
#define __BT_DRV_H

#define BT_KEY_GPIO_PORT 					GPIOC 
#define BT_KEY_PIN								GPIO_PIN_2

#define BT_LINK_STATE_GPIO_PORT		GPIOC 
#define BT_LINK_STATE_PIN					GPIO_PIN_3


#define BT_KEY_ON() 		gpio_bit_write(BT_KEY_GPIO_PORT,BT_KEY_PIN,SET)
#define BT_KEY_OFF() 		gpio_bit_write(BT_KEY_GPIO_PORT,BT_KEY_PIN,RESET)

void bt_initIO(void);
#endif

#include "led_ext.h"
#include "main.h"


/*FreeRtos includes*/
#if USING_RTOS
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#endif

#define LED_WIDTH   12
#define LED_HIGH    6
#define SPACE_BIT   2 //第一个74HC595最后2位未启用




//图像输出数组
#if !USING_LED_DMA_SEND
volatile uint16_t ledGrapArray[LED_HIGH] = {
    2049,
    1026,
    516,
    264,
    144,
    96
};
#else

#define DMA_DATA_LEN 3

volatile uint8_t ledGrapArray[LED_HIGH][DMA_DATA_LEN] = {
    //列       //行 
    #if 0
    {0x10,0x80,0x7F},
    {0x20,0x40,0xBF},
    {0x40,0x20,0xDF},
    {0x80,0x10,0xEF},
    {0x00,0x09,0xF7},
    {0x00,0x06,0xFB},
    #elif 0
    {0xff,0xff,0x7F},
    {0xff,0xff,0xBF},
    {0xff,0xff,0xDF},
    {0xff,0xff,0xEF},
    {0xff,0xff,0xF7},
    {0xff,0xff,0xFB},
    #elif 0
    {0x00,0x00,0x00},
    {0x00,0x00,0x00},
    {0x00,0x00,0x00},
    {0x00,0x00,0x00},
    {0x00,0x00,0x00},
    {0x00,0x00,0x00},
    #elif 0
    {0x00,0x00,0xff},
    {0x00,0x00,0xff},
    {0x00,0x00,0xff},
    {0x00,0x00,0xff},
    {0x00,0x00,0xff},
    {0x00,0x00,0xff},
    #else
    {0x00,0x00,0x7F},
    {0x00,0x00,0xBF},
    {0x00,0x00,0xDF},
    {0x00,0x00,0xEF},
    {0x00,0x00,0xF7},
    {0x00,0x00,0xFB},
    #endif
};


//插入图像
static void ledInsertImage(int8_t x,int8_t y,u8 width,u8 high,const u8 *ledImageArray){
    #if 0
    for (u8 h = 0; h < high && y+h < LED_HIGH; h++){
        if(8>x){
            ledGrapArray[y+h][1] |= (ledImageArray[h] >> x) & 0xff;
            ledGrapArray[y+h][0] |= (ledImageArray[h] << (8-x)) & 0xf0;
        } else{
            ledGrapArray[y+h][0] |= (ledImageArray[h] >> (x-8)) & 0xf0;
        }

    }
    #else

    if(width < 1) return;

    for (u8 h = 0; h < high && y+h < LED_HIGH; h++){
        if(y+h < 0) continue;

        if(x<8 && x>=0){
            ledGrapArray[y+h][1] |= (ledImageArray[h*((width-1)/8+1)] >> x) & 0xff;
            ledGrapArray[y+h][0] |= (ledImageArray[h*((width-1)/8+1)] << (8-x)) & 0xf0;
            if(width+x >= 8){
                ledGrapArray[y+h][0] |= (ledImageArray[h*((width-1)/8+1)+1] >> x) & 0xf0;
            }
        }else if(x<0){
            int8_t index = -x/8,offset = x%8; 
            if(index < width/8)
                ledGrapArray[y+h][1] |= (ledImageArray[h*((width-1)/8+1)+index] << -offset) & 0xff;
            if(index+1 < width/8)
                ledGrapArray[y+h][1] |= (ledImageArray[h*((width-1)/8+1)+index+1] >> (8+offset)) & 0xff;
            if(index+1 < width/8)
                ledGrapArray[y+h][0] |= (ledImageArray[h*((width-1)/8+1)+index+1] << -offset) & 0xf0;
            if(index+2 < width/8)
                ledGrapArray[y+h][0] |= (ledImageArray[h*((width-1)/8+1)+index+2] >> (8+offset)) & 0xf0;

        }else if(x >= 8){
            ledGrapArray[y+h][0] |= (ledImageArray[h*((width-1)/8+1)] >> (x-8)) & 0xf0;
        }

    }
    #endif
}

#endif

const u8 led5x6NumGrap[][LED_HIGH]={
    {0x70,0x88,0x88,0x88,0x88,0x70},/*"0",0*/
    {0x10,0x70,0x10,0x10,0x10,0x10},/*"1",1*/
    {0x70,0x88,0x08,0x10,0x60,0xF8},/*"2",2*/
    {0xF0,0x08,0x30,0x08,0x88,0x70},/*"3",3*/
    // {0x18,0x28,0x48,0x88,0xF8,0x08},/*"4",4*/
    {0x10,0x30,0x50,0x90,0xF8,0x10},/*"4",4*/
    // {0x78,0x80,0xF0,0x08,0x88,0x70},/*"5",5*/
    {0xF8,0x80,0xF0,0x08,0x88,0x70},/*"5",5*/
    {0x78,0x80,0xF0,0x88,0x88,0x70},/*"6",6*/
    {0xF8,0x10,0x20,0x20,0x40,0x40},/*"7",7*/
    {0x70,0x88,0x70,0x88,0x88,0x70},/*"8",8*/
    {0x70,0x88,0x88,0x78,0x08,0xF0} /*"9",9*/
};


const u8 led5x6BTGrap[]={0x30,0xA8,0x70,0x70,0xA8,0x30};

const uint8_t LogoGrap[][LED_HIGH]={
		{0x38,0x6C,0xC6,0xC6,0x6C,0x38},/*"O",O*/
		{0x7E,0x18,0x18,0x18,0x18,0x18},/*"T",T*/
		// {0x7E,0x22,0x10,0x08,0x44,0x7E},/*"s",s*/
		{0x70,0x88,0x40,0x30,0x88,0x78},/*"s",s*/
		{0xC3,0x64,0x38,0x18,0x2C,0xC7},/*"x",x*/		
};
const uint8_t ErrorGrap[][LED_HIGH]={
		{0x00,0x1c,0x72,0x13,0x72,0x1c},	// 电源打开期间连接充电器，
		{0x00,0x00,0x00,0x20,0x20,0x20},	// 或链接充电器期间打开设备
		{0x3F,0x68,0x44,0x42,0x61,0x3f},	// 电池充电
		{0xc0,0x40,0x40,0x40,0x40,0xc0},	// 水平低
		{0xf8,0x21,0x22,0x20,0x21,0x22},	// 电池温度
		{0x20,0x10,0x08,0x20,0x10,0x08},	// 显著下降
		{0xfA,0x21,0x20,0x22,0x21,0x20},	// 电池温度
		{0x08,0x10,0x20,0x08,0x10,0x20},	// 显著上升
		{0xf8,0x80,0xf8,0x80,0x80,0xf8},	// E
};




//清除图像
#if USING_LED_DMA_SEND
static void ledCleanImage(){
    for (u8 h = 0; h < LED_HIGH; h++)
    {
        memset((void *)ledGrapArray[h],0x00,sizeof(ledGrapArray[0][0])*2);
    }
    
}

static void ledSpeedGearGrap(){//速度挡位显示
    ledCleanImage();
    // uint8_t col[6] = {0x80,0x80,0x80,0x80,0x80,0x80};
    // ledInsertImage(1,0,1,6,col);
    ledInsertImage(4,0,5,6,led5x6NumGrap[Remote_setting_para.SpeedGear]);
    // ledInsertImage(11,0,1,6,col);
}

static void ledBattaryGrap(){
    // static u8 Battery = 255;
    // if(Battery != Remote_setting_para.Battery){
        ledCleanImage();
        ledInsertImage(1,0,5,6,led5x6NumGrap[Remote_setting_para.Battery/10]);
        ledInsertImage(7,0,5,6,led5x6NumGrap[Remote_setting_para.Battery%10]);
        // Battery = Remote_setting_para.Battery;
    // }
}


static void ledBTGrap(void){
    ledCleanImage();
    // uint8_t col[6] = {0x80,0x80,0x80,0x80,0x80,0x80};
    // ledInsertImage(1,0,1,6,col);
    ledInsertImage(4,0,5,6,led5x6BTGrap);
    // ledInsertImage(11,0,1,6,col);

}

//	错误代码显示
//	error:1-7
//	1：
//  2：
//	3：
//	4：电源打开期间连接充电器，或连接充电器期间打开设备
//	5：电池充电水平低
//	6：电池温度显著下降
//	7：电池温度上升至0度以上
static void ledErrorGrap(uint8_t error){
		ledCleanImage();
		if((error>=1) && (error<=3)){
			ledInsertImage(1,0,5,6,ErrorGrap[8]);
			ledInsertImage(7,0,5,6,led5x6NumGrap[error]);
		}else if ((error>=4) && (error<=7)){
			ledInsertImage(1,0,5,6,ErrorGrap[(error-4)*2]);
			ledInsertImage(7,0,5,6,ErrorGrap[(error-4)*2+1]);
		}
}
// 开场画面显示
static void ledLogoGrap(void){

    for(uint8_t temp = 32; temp>0; temp--){
      	ledCleanImage();
         if((temp ==32)||(temp == 24)||(temp == 16)||(temp== 8)) ledInsertImage(8,0,5,6,LogoGrap[(temp/8)-1]);
         else ledInsertImage(temp%8,0,5,6,LogoGrap[(temp/8)]);
         delay_ms(70);
    }

}

u8 testOverWidthGray[] = {
    #if 0
    0x80,0x18,0x01,//0x80,
    0x40,0x24,0x02,//0x40,
    0x20,0x42,0x04,//0x20,
    0x10,0x81,0x08,//0x10,
    0x09,0x00,0x90,//0x09,
    0x06,0x00,0x60,//0x06
    #elif 0
    0x00,0x00,0x00,
    0x00,0x00,0x00,
    0x00,0x00,0x00,
    0x00,0x00,0x00,
    0x00,0x00,0x00,
    0x00,0x00,0x00,
    #elif 0
    0x7f,0x1f,0xff,
    0x7f,0x1f,0xff,
    0x7f,0x1f,0xff,
    0x7f,0x1f,0xff,
    0x7f,0x1f,0xff,
    0x7f,0x1f,0xff,
    #else
    0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff
    #endif
};

void ledLeftMoveLoopShow(int8_t y,u8 width,u8 hight,const u8 *ledImageArray){
// void ledLoopShow(void){

    static int16_t x = 0;
    ledCleanImage();
    #if 0

    ledInsertImage(x,0,sizeof(testOverWidthGray)/LED_HIGH*8,LED_HIGH,testOverWidthGray);

    if(sizeof(testOverWidthGray)/LED_HIGH*8 + x <= LED_WIDTH)
        ledInsertImage(sizeof(testOverWidthGray)/LED_HIGH*8 + x 
        ,0,sizeof(testOverWidthGray)/LED_HIGH*8,LED_HIGH,testOverWidthGray);

    if(--x <= -(sizeof(testOverWidthGray)/LED_HIGH*8)) x = 0;

    #else

    ledInsertImage(x,0,width,hight,ledImageArray);

    if(width + x <= LED_WIDTH)
        ledInsertImage(width + x,0,width,hight,ledImageArray);

    // if((--x) <= -width) x = 0;

    #endif
}

#else
static void ledCleanImage(){
    memset((void *)ledGrapArray,0x00,sizeof(ledGrapArray));
}
#endif




#if !USING_LED_DMA_SEND
//插入图像
void ledInsertImage(u8 x,u8 y,u8 width,u8 high,const u8 *ledImageArray){
    for (u8 h = 0; h < high; h++){
        ledGrapArray[y+h] |= ((ledImageArray[h] & (0xFFF>>width)) << (LED_WIDTH - x - width));
    }
    
}

//刷新图像
static void ledRefreshImage(){

    for(uint8_t h = 0; h < LED_HIGH ; h++){//逐行扫描
        // if(!(ledGrapArray[h] & 0x0FFF)) continue;

        LED_SCK_0();
        LED_RCK_0();
        for (uint8_t w = 0; w < LED_WIDTH; w++)//第h行扫描
        {
            delay_us(1);
            if(ledGrapArray[h]>>w & 0x01){
                LED_DATA_1();
            }else{
                LED_DATA_0();
            }
            delay_us(1);

            LED_SCK_1();
            LED_SCK_0();
        }
        for (uint8_t i = 0; i < SPACE_BIT; i++){

            delay_us(1);
            LED_DATA_1();
            delay_us(1);

            LED_SCK_1();
            LED_SCK_0();
        }
        for (uint8_t j = 0; j < LED_HIGH; j++){
            delay_us(1);

            if(j == h&&ledGrapArray[h]) LED_DATA_0();//上1左0才能导通,同一时刻只能导通1行
            else LED_DATA_1();
            delay_us(1);

            LED_SCK_1();
            LED_SCK_0();
        }
        LED_RCK_1();//更新第h行输出,每次输出20位
        LED_RCK_0();

    }
}

//8x8 测试
void led8x8Test(void){

    u8 x = 5;
    u8 y = 0;
    u16 timeCount = 0;
    u8 num = 0;


    while (1)
    {

        ledCleanImage();
        ledInsertImage(x,y,5,6,led5x6NumGrap[num]);

        LED_SCK_0();
        LED_RCK_0();

        for(u8 h = 0;h<LED_HIGH;h++){
            //行
            for (u8 i = 0; i < 8; i++) {

                delay_us(1);
                if(i == h && ledGrapArray[h])
                    LED_DATA_0();
                else
                    LED_DATA_1();
                delay_us(1);

                LED_SCK_1();
                LED_SCK_0();
            }

            //列
            for (u8 j = 0; j < 8; j++) {

                delay_us(1);
                if(ledGrapArray[h]>>j & 0x01)
                // if(i==8-1)
                    LED_DATA_1();
                else
                    LED_DATA_0();
                delay_us(1);

                LED_SCK_1();
                LED_SCK_0();
            }
            
            LED_RCK_1();
            LED_RCK_0();

        }
        if(++timeCount == 2535){
            timeCount = 0;
            if(++num == 10) num = 0;
        }
    }
    
}

#else

void led_ext_init(void){

    rcu_periph_clock_enable(RCU_SPI1);
    rcu_periph_clock_enable(RCU_DMA0);

    dma_parameter_struct dma_init_struct;
    dma_deinit(DMA0,DMA_CH4);
    dma_init_struct.periph_addr = (uint32_t) &SPI_DATA(SPI1);
    dma_init_struct.memory_addr = (uint32_t) ledGrapArray[0];//!!!!!!!!!!!
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.periph_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_HIGH;
    dma_init_struct.number = DMA_DATA_LEN;
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init(DMA0,DMA_CH4,&dma_init_struct);
    dma_circulation_disable(DMA0,DMA_CH4);
    dma_memory_to_memory_disable(DMA0,DMA_CH4);

    spi_i2s_deinit(SPI1);

    spi_parameter_struct spi_init_struct;
    spi_init_struct.trans_mode = SPI_TRANSMODE_BDTRANSMIT;
    spi_init_struct.device_mode = SPI_MASTER;
    spi_init_struct.frame_size = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
    spi_init_struct.nss = SPI_NSS_HARD;
    spi_init_struct.prescale = SPI_PSC_2;
    spi_init_struct.endian = SPI_ENDIAN_LSB;

    spi_init(SPI1,&spi_init_struct);
    spi_nssp_mode_enable(SPI1);
    spi_nss_output_enable(SPI1);
    spi_enable(SPI1);

    dma_channel_enable(DMA0,DMA_CH4);
    spi_dma_enable(SPI1,SPI_DMA_TRANSMIT);
}

void ledSpiDmaRefreshRow(void){
    if(Remote_setting_para.PowerStatus == ePowerOff) return;
    static u8 h = 0;
    if(dma_flag_get(DMA0,DMA_CH4,DMA_FLAG_FTF) != RESET){
        dma_channel_disable(DMA0,DMA_CH4);
        dma_memory_address_config(DMA0,DMA_CH4,(uint32_t)ledGrapArray[h]);
        dma_transfer_number_config(DMA0,DMA_CH4,DMA_DATA_LEN);
        dma_channel_enable(DMA0,DMA_CH4);
        if(++h >= LED_HIGH) h = 0;
    }
}

void ledSpiDmaRefresh(void){

    for(u8 h = 0;h < LED_HIGH; h++)
    {
        while (dma_flag_get(DMA0,DMA_CH4,DMA_FLAG_FTF) == RESET){
            delay_ms(1);
        }
        dma_channel_disable(DMA0,DMA_CH4);
        dma_memory_address_config(DMA0,DMA_CH4,(uint32_t)ledGrapArray[h]);
        dma_transfer_number_config(DMA0,DMA_CH4,DMA_DATA_LEN);
        dma_channel_enable(DMA0,DMA_CH4);
        delay_ms(1);
    }
}

#endif

void ledDisplayON(void){
    Remote_para_default();	
    powerON();//下板电源开
    LED_POWER_0();
    Remote_setting_para.PowerStatus = ePowerOn;
    ledLogoGrap();
    // BTEnterStandby();
    paraInit();
}
void ledDisplayOFF(void){
    ledCleanImage();
    paraInit();
    delay_ms(10);
    powerOFF();//下板电源关
    LED_POWER_1();
    BTdisconnect();
    
    Remote_para_default();	
}


#define LED_SHOW_SPEED_TIME 30

void ledShow(void){
    static uint8_t timeCount = LED_SHOW_SPEED_TIME, hornFlag = 0;
    // uint8_t keyState = bsp_GetKey();	
    static uint8_t keyState = KEY_POWER_DOWN;	

    if(keyState == KEY_POWER_DOWN && Remote_setting_para.PowerStatus == ePowerOff){
        ledDisplayON();
        keyState = KEY_NONE;
    }else if(Remote_setting_para.PowerStatus == ePowerOn && Remote_setting_para.ErrorFlag != ERROR_NONE){
        for (u8 i = 0; i < 8; i++) {
            if(Remote_setting_para.ErrorFlag & 1<<i) ledErrorGrap(i);
        }
	}else if(Remote_setting_para.HandleLock && Remote_setting_para.PowerStatus == ePowerOn){
        ledLeftMoveLoopShow(0,sizeof(testOverWidthGray)/LED_HIGH*8,LED_HIGH,testOverWidthGray);
    }else if(Remote_setting_para.RemoteBTConnect == eBlubtooth && Remote_setting_para.PowerStatus == ePowerOn){
        ledBTGrap();
        if(keyState == KEY_POWER_DOWN){
            ledDisplayOFF();
        }
    }else if(Remote_setting_para.PowerStatus == ePowerOn && Remote_setting_para.RemoteBTConnect == eOffline){
        switch(keyState) {
            case KEY_POWER_DOWN:    //关机
                ledDisplayOFF();
                break;
            case KEY_SPEED_ADD_DOWN://加速档	
                if(Remote_setting_para.SpeedGear < eGearFive)
                    Remote_setting_para.SpeedGear++;							
                timeCount = LED_SHOW_SPEED_TIME;
                break;
            case KEY_SPEED_SUB_DOWN://减速档
                if(Remote_setting_para.SpeedGear > eGearOne)
                    Remote_setting_para.SpeedGear--;			
                timeCount = LED_SHOW_SPEED_TIME;
                break;
            case KEY_HORN_DOWN://喇叭
                // LoopQueue_Write(&music_FIFO,&Remote_setting_para.CurrentMusic,1);
                hornFlag = 1;
                break;
            case KEY_HORN_UP://喇叭
                hornFlag = 0;
                break;
            case KEY_PUSH_ORD_ADD_DOWN://重定义为推杆升
                Remote_trans_para.push_rod_speed=PUSH_ORD_SPEED_ADD;			
                break;		
            case KEY_PUSH_ORD_ADD_UP:
            case KEY_PUSH_ORD_SUB_UP:
                Remote_trans_para.push_rod_speed=PUSH_ORD_SPEED_ZERO;		
                break;
            case KEY_PUSH_ORD_SUB_DOWN:	//重定义为推杆降
                Remote_trans_para.push_rod_speed=PUSH_ORD_SPEED_SUB;
                break;		
            //组合按键
            case KEY_PUSH_ORD_ADD_SUB_LONG:
                if(Remote_receive_para.push_rod_speed == 0 && Remote_receive_para.seat_rod_speed == 0)
                    Remote_trans_para.folding_state = !Remote_receive_para.folding_state;
            break;
            default:
            break;
        }

        if(hornFlag) LoopQueue_Write(&music_FIFO,&hornFlag,1);

        if(Remote_trans_para.folding_state != Remote_receive_para.folding_state){
            if(Remote_trans_para.folding_state == ePfold){

            }else{

            }
        }else if(timeCount > 0){
            ledSpeedGearGrap();
            --timeCount;
        }else{
            ledBattaryGrap();
        }

    }else{
    }

    #if USING_LED_DMA_SEND
    // ledSpiDmaRefresh();
    #else
    ledRefreshImage();
    #endif
}

#include "led_ext.h"
#include "main.h"


/*FreeRtos includes*/
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

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

volatile uint8_t ledGrapArray[LED_HIGH+1][DMA_DATA_LEN] = {
    //列       //行 
    {0x10,0x80,0x7F},
    {0x20,0x40,0xBF},
    {0x40,0x20,0xDF},
    {0x80,0x10,0xEF},
    {0x00,0x09,0xF7},
    {0x00,0x06,0xFB},
    {0x00,0x00,0xFF},
};

//插入图像
static void ledInsertImage(u8 x,u8 y,u8 width,u8 high,const u8 *ledImageArray){
    for (u8 h = 0; h < high && y+h < LED_HIGH; h++){
        if(8>x){
            ledGrapArray[y+h][1] |= (ledImageArray[h] >> x) & 0xff;
            ledGrapArray[y+h][0] |= (ledImageArray[h] << (8-x)) & 0xf0;
        } else{
            ledGrapArray[y+h][0] |= (ledImageArray[h] >> (x-8)) & 0xf0;
        }

    }
}

#endif

const u8 led5x6NumGrap[][LED_HIGH]={
    {0x70,0x88,0x88,0x88,0x88,0x70},/*"0",0*/
    {0x10,0x70,0x10,0x10,0x10,0x10},/*"1",1*/
    {0x70,0x88,0x08,0x10,0x60,0xF8},/*"2",2*/
    {0xF0,0x08,0x30,0x08,0x88,0x70},/*"3",3*/
    // {0x18,0x28,0x48,0x88,0xF8,0x08},/*"4",4*/
    {0x10,0x30,0x50,0x90,0xF8,0x10},/*"4",4*/
    {0x78,0x80,0xF0,0x08,0x88,0x70},/*"5",5*/
    {0x78,0x80,0xF0,0x88,0x88,0x70},/*"6",6*/
    {0xF8,0x10,0x20,0x20,0x40,0x40},/*"7",7*/
    {0x70,0x88,0x70,0x88,0x88,0x70},/*"8",8*/
    {0x70,0x88,0x88,0x78,0x08,0xF0} /*"9",9*/
};

//清除图像
static void ledCleanImage(){
    for (u8 h = 0; h < LED_HIGH; h++)
    {
        // ledGrapArray[h][0] = 0x00;
        // ledGrapArray[h][1] = 0x00;
        // ledGrapArray[h][2] = ~(1 << (7-h));
        memset(ledGrapArray[h],0x00,sizeof(ledGrapArray[0][0])*2);
    }
    
}


static void ledSpeedGearGrap(){//速度挡位显示
    ledCleanImage();
    uint8_t col[6] = {0x80,0x80,0x80,0x80,0x80,0x80};
    ledInsertImage(1,0,1,6,col);
    ledInsertImage(4,0,5,6,led5x6NumGrap[Remote_setting_para.SpeedGear]);
    ledInsertImage(11,0,1,6,col);
}

static void ledBattaryGrap(){
    ledCleanImage();
    ledInsertImage(1,0,5,6,led5x6NumGrap[Remote_setting_para.Battery/10]);
    ledInsertImage(7,0,5,6,led5x6NumGrap[Remote_setting_para.Battery%10]);
}

#if !USING_LED_DMA_SEND
//插入图像
void ledInsertImage(u8 x,u8 y,u8 width,u8 high,const u16 *ledImageArray){
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
            // delay_us(1);
            if(ledGrapArray[h]>>w & 0x01){
                LED_DATA_1();
            }else{
                LED_DATA_0();
            }
            // delay_us(1);

            LED_SCK_1();
            LED_SCK_0();
        }
        for (uint8_t i = 0; i < SPACE_BIT; i++){

            // delay_us(1);
            LED_DATA_1();
            // delay_us(1);

            LED_SCK_1();
            LED_SCK_0();
        }
        for (uint8_t j = 0; j < LED_HIGH; j++){
            // delay_us(1);

            if(j == h&&ledGrapArray[h]) LED_DATA_0();//上1左0才能导通,同一时刻只能导通1行
            else LED_DATA_1();
            // delay_us(1);

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

void ledSpiDmaRefreshRow(u8 h){
    if(dma_flag_get(DMA0,DMA_CH4,DMA_FLAG_FTF) != RESET){
        dma_memory_address_config(DMA0,DMA_CH4,(uint32_t)ledGrapArray[h]);
        dma_channel_disable(DMA0,DMA_CH4);
        dma_transfer_number_config(DMA0,DMA_CH4,DMA_DATA_LEN);
        dma_channel_enable(DMA0,DMA_CH4);
    }
}

void ledSpiDmaRefresh(void){

    for(u8 h = 0;h < LED_HIGH+1; h++)
    {
        while (dma_flag_get(DMA0,DMA_CH4,DMA_FLAG_FTF) == RESET){
            delay_ms(1);
        }
        dma_memory_address_config(DMA0,DMA_CH4,(uint32_t)ledGrapArray[h]);
        dma_channel_disable(DMA0,DMA_CH4);
        dma_transfer_number_config(DMA0,DMA_CH4,DMA_DATA_LEN);
        dma_channel_enable(DMA0,DMA_CH4);
        delay_ms(1);
    }
}

#endif

#define LED_SHOW_SPEED_TIME 500

void ledShow(void){
    static uint16_t timeCount = 0;
    uint8_t keyState = bsp_GetKey();	
    if(Remote_setting_para.PowerStatus == ePowerOff)
	{
		Remote_para_default();	
        ledCleanImage();
        if(keyState == KEY_POWER_DOWN){
            Remote_setting_para.PowerStatus = ePowerOn;
        }
	}else{
        switch(keyState)
        {
            case KEY_POWER_DOWN:    //关机
                Remote_setting_para.PowerStatus = ePowerOff;
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
                // play_file_voice(1,Remote_setting_para.CurrentMusic);
                break;
            case KEY_PUSH_ORD_ADD_DOWN://重定义为推杆升
                Remote_trans_para.push_rod_speed=PUSH_ORD_SPEED_SUB;			
                break;		
            case KEY_PUSH_ORD_ADD_UP:
            case KEY_PUSH_ORD_SUB_UP:
                Remote_trans_para.push_rod_speed=PUSH_ORD_SPEED_ZERO;		
                break;
            case KEY_PUSH_ORD_SUB_DOWN:	//重定义为推杆降
                Remote_trans_para.push_rod_speed=PUSH_ORD_SPEED_ADD;//实际正速度座椅往下
                break;		
            //组合按键
            case KEY_PUSH_ORD_ADD_SUB_LONG:
                if(Remote_receive_para.push_rod_speed == 0 && Remote_receive_para.seat_rod_speed == 0)
                    Remote_trans_para.folding_state = !Remote_receive_para.folding_state;
            break;
            default:
            break;
        }
        if(timeCount > 0){
            ledSpeedGearGrap();
            --timeCount;
        }
        else
            ledBattaryGrap();
    }
    #if USING_LED_DMA_SEND
    // ledSpiDmaRefresh();
    #else
    ledRefreshImage();
    #endif
}
#include "led_ext.h"
#include "main.h"

#if USING_LED_POINT_DISPLAY

/*FreeRtos includes*/
#if USING_RTOS
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#endif




#if 0
24*6
{0x8B,0xBE,0x60,
0x52,0x08,0x90,
0x21,0x08,0x90,
0x50,0x88,0x90,
0x8B,0x88,0x60,
0x00,0x00,0x00},/*"xsto",xsto*/

3*6
{0x00,0xE0,0x80,
0xE0,0x80,0xE0},/*"E",E*/
{0x00,0x40,0x40,
0x40,0x40,0x40},/*"1",1*/
{0x00,0xE0,0x20
,0xE0,0x80,0xE0},/*"2",2*/
{0x00,0xE0,0x20,
0xE0,0x20,0xE0},/*"3",3*/
{0x00,0xA0,0xA0,
0xE0,0x20,0x20},/*"4",4*/
{0x00,0xE0,0x80,
0xE0,0x20,0xE0},/*"5",5*/
{0x00,0xE0,0x80,
0xE0,0xA0,0xE0},/*"6",6*/
{0x00,0xE0,0x20,
0x20,0x20,0x20},/*"7",7*/
{0x00,0xE0,0xA0,
0xE0,0xA0,0xE0},/*"8",8*/
{0x00,0xE0,0xA0,
0xE0,0x20,0xE0},/*"9",9*/

12*6
{0x09,0x00,0x16,
0x80,0x3F,0xC0,
0x09,0x00,0x10,
0x80,0x00,0x00},/*"蓝牙", 蓝牙*/

12*6
{0x06,0x00,0x09,
0x00,0x09,0x00,
0x0F,0x00,0x0F,
0x00,0x0F,0x00},/*"锁",锁*/

12*6
{0x00,0xC0,0x01,
0x20,0x01,0x20,
0x0F,0x00,0x0F,
0x00,0x0F,0x00},/*"解锁",解锁*/


12*6
{0x06,0x00,0x06,
0x00,0x06,0x00,
0x16,0x80,0x0F,
0x00,0x06,0x00},/*"下降",下降*/


12*6
{0x06,0x00,0x0F
,0x00,0x16,0x80,
0x06,0x00,0x06,
0x00,0x06,0x00},/*"上升",上升*/


12*6
{0x09,0x00,0x09,
0x00,0x09,0x00,
0x09,0x00,0x06,
0x00,0x00,0x00},/*"U展开",U展开*/


12*6
{0x0F,0x00,0x08,
0x00,0x0E,0x00,
0x08,0x00,0x08,
0x00,0x00,0x00},/*"F折叠",F折叠*/

12*6
{0x02,0x00,0x06,
0x00,0x02,0x00,
0x02,0x00,0x02,
0x00,0x00,0x00},/*"1",1*/





12 *6
{0x06,0x00,0x09,
0x00,0x02,0x00,
0x04,0x00,0x0F,
0x00,0x00,0x00},/*"2",2*/

12*6
{0x0F,0x00,0x01,
0x00,0x07,0x00,
0x01,0x00,0x0F,
0x00,0x00,0x00},/*"3",3*/

12*6
{0x02,0x00,0x06,
0x00,0x0A,0x00,
0x0F,0x00,0x02,
0x00,0x00,0x00},/*"4",4*/

12*6
{0x0F,0x00,0x08,
0x00,0x0F,0x00,
0x01,0x00,0x0E,
0x00,0x00,0x00},/*"5",5*/





#endif


#define LED_WIDTH   12
#define LED_HIGH    6
#define SPACE_BIT   2 //第一个74HC595最后2位未启用

const u8 ledData1_1X6[] = { 0x80, 0x80, 0x80, 0x80, 0x80, 0x80 };

#if 0
const uint8_t LogoGrap[][LED_HIGH]={
		{0x38,0x6C,0xC6,0xC6,0x6C,0x38},/*"O",O*/
		{0x7E,0x18,0x18,0x18,0x18,0x18},/*"T",T*/
		// {0x7E,0x22,0x10,0x08,0x44,0x7E},/*"s",s*/
		{0x70,0x88,0x40,0x30,0x88,0x78},/*"s",s*/
		{0xC3,0x64,0x38,0x18,0x2C,0xC7},/*"x",x*/		
};
#else
const uint8_t LogoGrap[]={
    0x8B,0xBE,0x60,
    0x52,0x08,0x90,
    0x21,0x08,0x90,
    0x50,0x88,0x90,
    0x8B,0x88,0x60,
    0x00,0x00,0x00
};
#endif

const u8 led3x6EGrapArray[] = {
0x00,0xE0,0x80, 0xE0,0x80,0xE0,/*"E",E*/
};

const u8 led3x6NumGrapArray[][LED_HIGH] = {
    {0x00,0xE0,0xA0, 0xA0,0xA0,0xE0},/*"0",0*/
    {0x00,0x40,0x40, 0x40,0x40,0x40},/*"1",1*/
    // {0x00,0x80,0x80, 0x80,0x80,0x80},/*"1",1*/
    {0x00,0xE0,0x20, 0xE0,0x80,0xE0},/*"2",2*/
    {0x00,0xE0,0x20, 0xE0,0x20,0xE0},/*"3",3*/
    {0x00,0xA0,0xA0, 0xE0,0x20,0x20},/*"4",4*/
    {0x00,0xE0,0x80, 0xE0,0x20,0xE0},/*"5",5*/
    {0x00,0xE0,0x80, 0xE0,0xA0,0xE0},/*"6",6*/
    {0x00,0xE0,0x20, 0x20,0x20,0x20},/*"7",7*/
    {0x00,0xE0,0xA0, 0xE0,0xA0,0xE0},/*"8",8*/
    {0x00,0xE0,0xA0, 0xE0,0x20,0xE0},/*"9",9*/
};

const u8 ledSpeedNumGrapArray[][LED_HIGH*2]={
    {0x02,0x00, 0x06,0x00, 0x02,0x00, 0x02,0x00, 0x02,0x00,0x00,0x00},/*"1",1*/
    // 12*6
    {0x06,0x00,0x09, 0x00,0x02,0x00, 0x04,0x00,0x0F, 0x00,0x00,0x00},/*"2",2*/
    // 12*6
    {0x0F,0x00,0x01, 0x00,0x07,0x00, 0x01,0x00,0x0F, 0x00,0x00,0x00},/*"3",3*/
    // 12*6
    {0x02,0x00,0x06, 0x00,0x0A,0x00, 0x0F,0x00,0x02, 0x00,0x00,0x00},/*"4",4*/
    // 12*6
    {0x0F,0x00,0x08, 0x00,0x0F,0x00, 0x01,0x00,0x0E, 0x00,0x00,0x00},/*"5",5*/
};

const u8 ledLockStateGrapArray[][LED_HIGH*2] = {
    {0x00,0xC0, 0x01,0x20, 0x01,0x20, 0x0F,0x00, 0x0F,0x00, 0x0F,0x00},/*"解锁",解锁*/
    // 12*6
    {0x06,0x00,0x09, 0x00,0x09,0x00, 0x0F,0x00,0x0F, 0x00,0x0F,0x00},/*"锁",锁*/
};

const u8 ledBTGrapArray[] = {
// 12*6
0x09,0x00,0x16, 0x80,0x3F,0xC0, 0x09,0x00,0x10, 0x80,0x00,0x00/*"蓝牙", 蓝牙*/
};

const u8 ledPushRodStateGrapArray[][LED_HIGH*2] = {
// 12*6
    {0x06,0x00,0x06, 0x00,0x06,0x00, 0x16,0x80,0x0F, 0x00,0x06,0x00},/*"下降",下降*/
    // 12*6
    {0x06,0x00,0x0F ,0x00,0x16,0x80, 0x06,0x00,0x06, 0x00,0x06,0x00},/*"上升",上升*/
};

const u8 ledFolderStateArray[][LED_HIGH*2] = {
    // 12*6
    {0x09,0x00,0x09, 0x00,0x09,0x00, 0x09,0x00,0x06, 0x00,0x00,0x00},/*"U展开",U展开*/
    // 12*6
    {0x0F,0x00,0x08, 0x00,0x0E,0x00, 0x08,0x00,0x08, 0x00,0x00,0x00},/*"F折叠",F折叠*/
};


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
    #elif 0
    {0x00,0x00,0x7F},
    {0x00,0x00,0xBF},
    {0x00,0x00,0xDF},
    {0x00,0x00,0xEF},
    {0x00,0x00,0xF7},
    {0x00,0x00,0xFB},
    #else
    {0x0F,0x00,0x7C},
    {0x0F,0x00,0xBC},
    {0x0F,0x00,0xDC},
    {0x0F,0x00,0xEC},
    {0x0F,0x00,0xF4},
    {0x0F,0x00,0xF8},
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

void ledInsertRed(int8_t x,int8_t y,u8 hight){
    if(0||(y<3 || y>5) &&(x != 0 || x != 11)) return;
    
    for (u8 i = 0; i < hight && y+i < LED_HIGH; i++) {
        if(x == 0) ledGrapArray[y+i][2] |= 0x02;
        else if(x == 11) ledGrapArray[y+i][2] |= 0x01;
        ledGrapArray[y+i][0] &= ~(1 << (6-y-i));
    }
}


#endif



//清除图像
#if USING_LED_DMA_SEND
static void ledCleanImage(){
    for (u8 h = 0; h < LED_HIGH; h++)
    {
        #if 0
        memset((void *)ledGrapArray[h],0x00,sizeof(ledGrapArray[0][0])*2);
        #else
        ledGrapArray[h][0] = 0x0f;
        ledGrapArray[h][1] = 0x00;
        ledGrapArray[h][2] = (~(1<<(7-h))) & 0xfc;
        #endif
    }
    
}
void ledCleanCol(uint8_t col){
    if(col > 12) return;

    for (u8 h = 0; h < LED_HIGH; h++){
            ledGrapArray[h][1-col/8] &= ~(1 << (7-col%8));
    }
    
}
void ledCleanRow(uint8_t row){
    memset((void *)ledGrapArray[row],0x00,sizeof(ledGrapArray[0][0])*2);
}



void ledUpDownLoopShow(int8_t dir,int8_t x,u8 width,u8 hight,const u8 *ledImageArray){
    static int8_t y = 0;
    ledCleanImage();
    if(dir > 0){
        ledInsertImage(x,y,width,hight,ledImageArray);
        #if 0
        if(y > LED_HIGH ){
            ledInsertImage(x,y-LED_HIGH-hight,width,hight,ledImageArray);
        }
        if(++y >= LED_HIGH + hight) y = 0;
        #else
        if(y > LED_HIGH/2 ){
            ledInsertImage(x,y-LED_HIGH/2-hight,width,hight,ledImageArray);
        }
        if(++y >= LED_HIGH/2 + hight) y = 0;
        #endif
    }else{
        ledInsertImage(x,y,width,hight,ledImageArray);
        #if 0
        if(y < -LED_HIGH ){
            ledInsertImage(x,y+LED_HIGH+hight,width,hight,ledImageArray);
        }
        if(--y <= -LED_HIGH - hight) y = 0;
        #else
        if(y < -LED_HIGH/2 ){
            ledInsertImage(x,y+LED_HIGH/2+hight,width,hight,ledImageArray);
        }
        if(--y <= -LED_HIGH/2 - hight) y = 0;
        #endif
    }
}


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

    if((--x) <= -width) x = 0;

    #endif
}

// 开场画面显示
void ledLogoGrap(void){
    for (int8_t x = 10; x != -20; x--) {
        ledCleanImage();

        ledInsertImage(x,0,24,LED_HIGH,LogoGrap);

        ledCleanCol(11);
        ledCleanCol(0);
        delay_ms(100);
    }
}

void ledErrorGrap(uint8_t error){
    ledCleanImage();
    // ledInsertImage(0,3,1,3,ledData1_1X6);
    // ledInsertImage(11,3,1,3,ledData1_1X6);
    ledInsertRed(0,3,3);
    ledInsertRed(11,3,3);

    ledInsertImage(2,0,3,6,led3x6EGrapArray);
    ledInsertImage(7,0,3,6,led3x6NumGrapArray[error]);
}

void ledSpeedGearGrap(){//速度挡位显示
    ledCleanImage();
    for(u8 i = 0;i<Remote_setting_para.SpeedGear;i++){
        ledInsertImage(i+1,5,1,1,ledData1_1X6);
        ledInsertImage(10-i,5,1,1,ledData1_1X6);
    }
    ledInsertImage(0,0,12,6,ledSpeedNumGrapArray[Remote_setting_para.SpeedGear-1]);
}

void ledBattaryGrap(){
    ledCleanImage();
    static u8 timeCount = 0;
    if(Remote_setting_para.Battery == 100){
        ledInsertImage(0,0,3,6,led3x6NumGrapArray[1]);
        ledInsertImage(3,0,3,6,led3x6NumGrapArray[0]);
        ledInsertImage(7,0,3,6,led3x6NumGrapArray[0]);
    }else{
        ledInsertImage(2,0,5,6,led3x6NumGrapArray[Remote_setting_para.Battery/10]);
        ledInsertImage(7,0,5,6,led3x6NumGrapArray[Remote_setting_para.Battery%10]);

        if(Remote_setting_para.Battery < 20 && ++timeCount%5 == 0 ){
            // ledInsertImage(0,5,1,1,ledData1_1X6);
            // ledInsertImage(11,5,1,1,ledData1_1X6);

            ledInsertRed(0,3,3);
            ledInsertRed(11,3,3);
        }
    }
}
void ledLockStateGrap(){
    ledCleanImage();
    ledInsertImage(0,0,12,6,ledLockStateGrapArray[Remote_setting_para.HandleLock]);
};


void ledBTGrap(void){
    ledCleanImage();
    // ledInsertImage(4,0,5,6,led5x6BTGrap);
    static u8 timeCount = 0;
    if(timeCount < 5){
        ledInsertImage(0,0,12,6,ledBTGrapArray);
    }
    if(++timeCount > 10) timeCount = 0;
}
void ledPushRodStateGrap(void){
    ledCleanImage();
    if(Remote_trans_para.push_rod_speed > 0){
        ledUpDownLoopShow(1,0,12,6,ledPushRodStateGrapArray[0]);
    }else if(Remote_trans_para.push_rod_speed < 0){
        ledUpDownLoopShow(-1,0,12,6,ledPushRodStateGrapArray[1]);
    }
}

void ledFolderStateGrap(void){
    ledCleanImage();
    static u8 timeCount = 1;
    ledInsertImage(0,0,12,6,ledFolderStateArray[Remote_trans_para.folding_state]);

    if(Remote_trans_para.folding_state == ePExpand){
        for (u8 i = 0; i < timeCount; i++) {
            ledInsertImage(5 - i/4,5,1,1,ledData1_1X6);
            ledInsertImage(i/4 + 6,5,1,1,ledData1_1X6);
        }
    }else{
        for (u8 i = 0; i < 20 - timeCount; i++) {
            ledInsertImage(5 - i/4,5,1,1,ledData1_1X6);
            ledInsertImage(i/4 + 6,5,1,1,ledData1_1X6);
        }
    }

    if(++timeCount >= 20) timeCount = 1;

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

    // for(uint8_t h = 0;h < LED_HIGH;h++){
    //     ledGrapArray[h][2] = ~(1 << (7 - h));
    // }

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

    Remote_setting_para.PowerStatus = ePowerOn;
    // LED_POWER_0();

    // ledLogoGrap();

    // BTEnterStandby();
    // paraInit();
}

void ledDisplayLOGO(int8_t x){
// for (int8_t x = 10; x != -20; x--) {
        ledCleanImage();

        ledInsertImage(x,0,24,LED_HIGH,LogoGrap);

        ledCleanCol(11);
        ledCleanCol(0);
        // delay_ms(100);
    // }
}

void ledDisplayOFF(void){
    ledCleanImage();
    paraInit();
    delay_ms(10);
    powerOFF();//下板电源关
    // LED_POWER_1();
    
    Remote_para_default();	
}

void ledDisplayError(u8 keyVal){
    // sizeof(Remote_trans_para.errorFlag);
    for (int8_t i = 7; i >= 0; i--) {
        if(Remote_trans_para.errorFlag.all[0] & 1<<i) ledErrorGrap(i);
    }
    if(keyVal == KEY_POWER_DOWN) ledDisplayOFF();
}

void ledDisplayLock(u8 keyVal){
    ledLockStateGrap();
    switch (keyVal){
        case KEY_POWER_DOWN:    //关机
            ledDisplayOFF();
            break;
#if 0
        // case KEY_PUSH_ORD_ADD_DOWN://重定义为推杆升
        //     Remote_trans_para.push_rod_speed=PUSH_ORD_SPEED_ADD;			
        //     break;		
        // case KEY_PUSH_ORD_ADD_UP:
        // case KEY_PUSH_ORD_SUB_UP:
        //     Remote_trans_para.push_rod_speed=PUSH_ORD_SPEED_ZERO;		
        //     break;
        // case KEY_PUSH_ORD_SUB_DOWN:	//重定义为推杆降
        //     Remote_trans_para.push_rod_speed=PUSH_ORD_SPEED_SUB;
        //     break;
        // case KEY_SPEED_ADD_SUB_LONG:
        //     Remote_setting_para.HandleLock = eUnLock;
        //     ledLockStateGrap();
        //     delay_ms(500);
#endif
    default:
        break;
    }
}

void ledDisplayBT(u8 keyVal){
    ledBTGrap();
    switch (keyVal){
        case KEY_POWER_DOWN:    //关机
            ledDisplayOFF();
            break;
        // case KEY_SPEED_ADD_SUB_LONG:
            // Remote_setting_para.HandleLock = eUnLock;
    default:
        break;
    }
}

#define LED_SHOW_SPEED_TIME 30

void ledDisplayMain(u8 keyVal){
    static uint8_t timeCount = LED_SHOW_SPEED_TIME;

    if(Remote_setting_para.SpeedPre != Remote_setting_para.SpeedGear){
        timeCount = LED_SHOW_SPEED_TIME;
        Remote_setting_para.SpeedPre = Remote_setting_para.SpeedGear;
    }

    if(Remote_trans_para.folding_state != Remote_receive_para.folding_state){
        ledFolderStateGrap();
    }else if(Remote_trans_para.push_rod_speed != PUSH_ORD_SPEED_ZERO){
        ledPushRodStateGrap();
    }else if (timeCount > 0){
        ledSpeedGearGrap();
        --timeCount;
    }else{
        ledBattaryGrap();
    }


    switch(keyVal) {
        case KEY_POWER_DOWN:    //关机
            ledDisplayOFF();
            timeCount = LED_SHOW_SPEED_TIME;
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
            keyVal = MUSIC_HORN;
            LoopQueue_Write(&music_FIFO,&keyVal,1);
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
        // case KEY_SPEED_ADD_SUB_LONG:
            // Remote_setting_para.HandleLock = eLock;
        default:
        break;
    }

}

#if 0

void ledShow(void){
    uint8_t keyState = bsp_GetKey();	
    // static uint8_t keyState = KEY_POWER_DOWN;	

    if(keyState == KEY_POWER_DOWN && Remote_setting_para.PowerStatus == ePowerOff){
        ledDisplayON();
    }else if(Remote_setting_para.PowerStatus == ePowerOn && Remote_setting_para.ErrorFlag.all[0] != 0){
        ledDisplayError(keyState);
	}else if(Remote_setting_para.HandleLock == eLock && Remote_setting_para.PowerStatus == ePowerOn){
        ledDisplayLock(keyState);
    }else if(Remote_setting_para.RemoteBTConnect == eBlubtooth && Remote_setting_para.PowerStatus == ePowerOn){
        ledDisplayBT(keyState);
    }else if(Remote_setting_para.PowerStatus == ePowerOn && Remote_setting_para.RemoteBTConnect == eOffline){
        ledDisplayMain(keyState);
    }else{
    }
}
#endif

#endif

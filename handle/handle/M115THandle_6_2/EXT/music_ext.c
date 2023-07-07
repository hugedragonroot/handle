#include "music_ext.h"
//#include "delay.h"
#include "main.h"

/********************************************************************************************
 - 功能描述：求和校验
 - 隶属模块：
 - 参数说明：
 - 返回说明：
 - 注：      和校验的思路如下
             发送的指令，去掉起始和结束。将中间的6个字节进行累加，最后取反码
             接收端就将接收到的一帧数据，去掉起始和结束。将中间的数据累加，再加上接收到的校验
             字节。刚好为0.这样就代表接收到的数据完全正确。
********************************************************************************************/

/**********************************************************************************************
									串口编程需要适当延时的注意点

1、芯片上电之后，需要大概 1S-1.5S 时间进行初花的相关操作，初始化完毕之后，会有初始化的相关数据
发送出来。用户也可以直接不理会这些数据
2、当指定设备播放之后，需要延时 200ms 的时间，再发送指定曲目等等相关指令。
3、因为芯片自带文件系统，正常情况下，在曲目不大于 1000 首的话，响应速度是低于 50ms 的
曲目超过 3000 首之后，文件系统的切换速度会变慢一点，响应速度在 100ms --- 1S 之间不等
4、芯片内部对串口的处理是 10MS 处理一次，所以连续的指令发送时，必须要间隔 20MS 的延时。否则前
面的指令将会被覆盖而得不到执行
5、如果指定文件夹文件名播放[0x0F、0x14]延时必须大于 40ms，因为芯片锁定文件是需要时间的。
只要涉及到文件夹文件名查找的相关指令，40MS 的延时是必不可少的。如果芯片当前正在查找文件，串
口的数据过来太频繁，会导致芯片的工作不正常
***********************************************************************************************/
static uint16_t play_checksum( uint8_t *Str, uint8_t len)
{
    uint16_t xorsum = 0;
    uint8_t i;

    for(i=0; i<len; i++)
    {
        xorsum  = xorsum + Str[i];
    }
	xorsum     = 0 -xorsum;
	return xorsum;
}

uint8_t play_buf[10]={PLAY_HEAD,PLAY_ADDR,PLAY_LEN,0x00,PLAY_FB_N,0x00,0x00,0x00,0x00,PLAY_TAIL};
uint16_t play_sum=0;

//设置存储设备为SPI FLASH
void play_set_flash(void)
{
	uint16_t sum;
	play_buf[3] = PLAY_CMD_DEVICE;
	play_buf[5] = 0x00;
	play_buf[6] = 0x04;
	sum = play_checksum(play_buf+1,6);
	play_buf[7] = (uint8_t)(sum >>8);
	play_buf[8] = (uint8_t)(sum & 0x00ff);
	uart_music_send( play_buf,sizeof(play_buf));
	delay_ms(20);//必须要间隔 20MS延时
}
//设置音量 0-30
void play_set_volume(uint8_t volume)
{
	uint16_t sum;
	play_buf[3] = PLAY_CMD_VOLUME;
	play_buf[5] = 0x00;
	play_buf[6] = volume;	
	sum = play_checksum(play_buf+1,6);
	play_buf[7] = (uint8_t)(sum >>8);
	play_buf[8] = (uint8_t)(sum & 0x00ff);
	uart_music_send( play_buf,sizeof(play_buf));
	// delay_ms(20);//必须要间隔 20MS延时
}

//播放指定文件夹下的某个文件
//文件夹名字file 1-99
//曲目		num  1-255
void play_file_voice(uint8_t file,uint8_t num)
{
	uint16_t sum;
	play_buf[3] = PLAY_CMD_FILE;
	play_buf[5] = file;		//文件夹
	play_buf[6] = num;		//第几曲
	sum = play_checksum(play_buf+1,6);
	play_buf[7] = (uint8_t)(sum >>8);
	play_buf[8] = (uint8_t)(sum & 0x00ff);
	uart_music_send( play_buf,sizeof(play_buf));
	// delay_ms(20);//必须要间隔 20MS延时
}

//循环顺序播放指定文件夹的文件
//文件夹名字file 1-99
void play_file_loop(uint8_t file)
{
	uint16_t sum;
	play_buf[3] = PLAY_CMD_FILE_LOOP;
	play_buf[5] = 0x00;
	play_buf[6] = file;		//文件夹
	sum = play_checksum(play_buf+1,6);
	play_buf[7] = (uint8_t)(sum >>8);
	play_buf[8] = (uint8_t)(sum & 0x00ff);
	uart_music_send( play_buf,sizeof(play_buf));
	delay_ms(20);//必须要间隔 20MS延时

}




void play_test(void)
{
	delay_ms(2000);
	play_set_flash();	//上电2000ms后才发送指令
	delay_ms(2000);		//设置存储设备后要延时200ms
	play_set_volume(1);
  delay_ms(2000);
	play_file_voice(1,1);
  delay_ms(2000);
	//play_file_loop(1);	
	while(1)
	{
		delay_ms(50);
	}
}

uint8_t musicNums[8];
TLoopBuf music_FIFO;

void music_init(void)
{
	delay_ms(1000);
	// play_set_flash();	//上电2000ms后才发送指令
	// delay_ms(200);		//设置存储设备后要延时200ms

	play_set_volume(ePVoiceLevel_1*10);	
	
	LoopQueue_Init(&music_FIFO,musicNums,sizeof(musicNums));
	// play_test();
}


void music_play_task(void){
	u8 num;
	static u8 voiceLevel = ePVoiceLevel_1;
	if(!READ_MUSIC_BUSY() && LoopQueue_ReadRelease(&music_FIFO,&num,1)){
		play_file_voice(1,num);
	}else if(voiceLevel != Remote_setting_para.VoiceLevel){
		voiceLevel = Remote_setting_para.VoiceLevel;
		play_set_volume(voiceLevel*10);
	}
}

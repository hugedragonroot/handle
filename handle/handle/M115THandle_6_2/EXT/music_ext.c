#include "music_ext.h"
//#include "delay.h"
#include "main.h"

/********************************************************************************************
 - �������������У��
 - ����ģ�飺
 - ����˵����
 - ����˵����
 - ע��      ��У���˼·����
             ���͵�ָ�ȥ����ʼ�ͽ��������м��6���ֽڽ����ۼӣ����ȡ����
             ���ն˾ͽ����յ���һ֡���ݣ�ȥ����ʼ�ͽ��������м�������ۼӣ��ټ��Ͻ��յ���У��
             �ֽڡ��պ�Ϊ0.�����ʹ�����յ���������ȫ��ȷ��
********************************************************************************************/

/**********************************************************************************************
									���ڱ����Ҫ�ʵ���ʱ��ע���

1��оƬ�ϵ�֮����Ҫ��� 1S-1.5S ʱ����г�������ز�������ʼ�����֮�󣬻��г�ʼ�����������
���ͳ������û�Ҳ����ֱ�Ӳ������Щ����
2����ָ���豸����֮����Ҫ��ʱ 200ms ��ʱ�䣬�ٷ���ָ����Ŀ�ȵ����ָ�
3����ΪоƬ�Դ��ļ�ϵͳ����������£�����Ŀ������ 1000 �׵Ļ�����Ӧ�ٶ��ǵ��� 50ms ��
��Ŀ���� 3000 ��֮���ļ�ϵͳ���л��ٶȻ����һ�㣬��Ӧ�ٶ��� 100ms --- 1S ֮�䲻��
4��оƬ�ڲ��Դ��ڵĴ����� 10MS ����һ�Σ�����������ָ���ʱ������Ҫ��� 20MS ����ʱ������ǰ
���ָ��ᱻ���Ƕ��ò���ִ��
5�����ָ���ļ����ļ�������[0x0F��0x14]��ʱ������� 40ms����ΪоƬ�����ļ�����Ҫʱ��ġ�
ֻҪ�漰���ļ����ļ������ҵ����ָ�40MS ����ʱ�Ǳز����ٵġ����оƬ��ǰ���ڲ����ļ�����
�ڵ����ݹ���̫Ƶ�����ᵼ��оƬ�Ĺ���������
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

//���ô洢�豸ΪSPI FLASH
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
	delay_ms(20);//����Ҫ��� 20MS��ʱ
}
//�������� 0-30
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
	// delay_ms(20);//����Ҫ��� 20MS��ʱ
}

//����ָ���ļ����µ�ĳ���ļ�
//�ļ�������file 1-99
//��Ŀ		num  1-255
void play_file_voice(uint8_t file,uint8_t num)
{
	uint16_t sum;
	play_buf[3] = PLAY_CMD_FILE;
	play_buf[5] = file;		//�ļ���
	play_buf[6] = num;		//�ڼ���
	sum = play_checksum(play_buf+1,6);
	play_buf[7] = (uint8_t)(sum >>8);
	play_buf[8] = (uint8_t)(sum & 0x00ff);
	uart_music_send( play_buf,sizeof(play_buf));
	// delay_ms(20);//����Ҫ��� 20MS��ʱ
}

//ѭ��˳�򲥷�ָ���ļ��е��ļ�
//�ļ�������file 1-99
void play_file_loop(uint8_t file)
{
	uint16_t sum;
	play_buf[3] = PLAY_CMD_FILE_LOOP;
	play_buf[5] = 0x00;
	play_buf[6] = file;		//�ļ���
	sum = play_checksum(play_buf+1,6);
	play_buf[7] = (uint8_t)(sum >>8);
	play_buf[8] = (uint8_t)(sum & 0x00ff);
	uart_music_send( play_buf,sizeof(play_buf));
	delay_ms(20);//����Ҫ��� 20MS��ʱ

}




void play_test(void)
{
	delay_ms(2000);
	play_set_flash();	//�ϵ�2000ms��ŷ���ָ��
	delay_ms(2000);		//���ô洢�豸��Ҫ��ʱ200ms
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
	// play_set_flash();	//�ϵ�2000ms��ŷ���ָ��
	// delay_ms(200);		//���ô洢�豸��Ҫ��ʱ200ms

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

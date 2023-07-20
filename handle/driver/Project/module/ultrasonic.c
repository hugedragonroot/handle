/**********************************************************/
//@Autor:Raw
//@Version:0.0.1
//@End Time:
//@Function:
/**********************************************************/
#include "ultrasonic.h"
#include "usart.h"
ultrasonic_pro_t ultrasonic;
#define max_len 13

static void ultrasonic_parse(uint8_t input)
{
	if(ultrasonic.mb_statue==RECE_DATA&&ultrasonic.len_in>=max_len)  goto RESET_ultrasonic;//����״̬�ҳ��ȴ��ڸù�����Ԥ�����ȣ�����ʧ�ܣ����¸�λ
	if((input==0x55)&&(ultrasonic.mb_statue==RECE_IDLE)) goto START_ultrasonic;//�豸��ַ���뿪ʼ����
	ultrasonic.buffer[ultrasonic.len_in++]=input;//��ֵ

	ultrasonic.sum +=input;
	if((ultrasonic.mb_statue==RECE_DATA)&&(ultrasonic.len_in==max_len))
	{
		ultrasonic.sum = ultrasonic.sum -input;
		uint8_t temp_sum = (ultrasonic.sum&0x00ff);
		if(ultrasonic.buffer[ultrasonic.len_in-1]==temp_sum)
		{
			goto FINISH_ultrasonic;//��ɽ���
		}
	}
	return;
START_ultrasonic:
	ultrasonic.len_in=0;
	ultrasonic.sum +=input;
	ultrasonic.buffer[ultrasonic.len_in++]=input;
	ultrasonic.mb_statue=RECE_DATA;
	return;
FINISH_ultrasonic:
	ultrasonic.mb_statue=RECE_IDLE;
	ultrasonic.sum = 0;
	app_ultrasonic.length_first = ultrasonic.buffer[4]<<8|ultrasonic.buffer[5];
	app_ultrasonic.length_second = ultrasonic.buffer[6]<<8|ultrasonic.buffer[7];
	app_ultrasonic.length_third = ultrasonic.buffer[8]<<8|ultrasonic.buffer[9];
	app_ultrasonic.length_four = ultrasonic.buffer[10]<<8|ultrasonic.buffer[11];
//	memcpy(&app_ultrasonic.length_first,&ultrasonic.buffer[4],8);
	return;
RESET_ultrasonic:
	if(input==0x55) goto START_ultrasonic;
	ultrasonic.len_in=0;
	ultrasonic.sum = 0;;
	ultrasonic.mb_statue=RECE_IDLE;
	return;
}	

void modbus_recve(void)
{
	serial3_update(serial3);//����dma����
	if(ultrasonic.deal_statue==DEAL_IDLE)//ֻ�еȴ���Э���ʽ���вŽ������
	while(serial_rx_queue_empty(serial3)!=0)
	{
		ultrasonic_parse(serial_receive(serial3));//�����ж�һ֡�Ƿ����
	}
}



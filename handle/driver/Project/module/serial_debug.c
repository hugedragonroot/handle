#include "serial_debug.h"
#include "usart.h"
#include "can.h"
#include "adc.h"
#include "adc_callback.h"
#include "tim.h"
#include "PMSM_Driver.h"
#include "HALL_Encoder.h"
#include "Brush_Driver.h"
#include "weak_handle.h"
#include "xsto_api_ii.h"
DebugData_t DebugData;
uint8_t get_decode_data[]=
{	
	0x23,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00
};
/****************************************************************************
 *
 * 函数名：         uart1_WaveformData
 * 输入：           data 为要发送的波形数据 在上位机中要设置相同的数据类型 waresize为数据大小
 * 输出：           无
 * 返回值：         无
 * 描述：           通信协议的格式为：[CMD] [~CMD][…数据…][~ CMD] [CMD]
                   虚拟示波器的 CMD 为 3，即一次数据传输的通信协议为：[0x03] [0xFC][…数据…][0xFC]
                   [0x03]
 ***************************************************************************/
void uart_WaveformData (/*uint8_t* data,uint16_t wavesize*/void)
{
//	//数据头
//	uart_SendByte(WAVE_UART,0x03);
//	uart_SendByte(WAVE_UART,0xfc);
//	
//	uart_SendBuf(data,wavesize);
//	//数据尾
//	uart_SendByte(WAVE_UART,0xfc);
//	uart_SendByte(WAVE_UART,0x03);
	uint8_t i;
  
	get_decode_data[0] = 0x23;
	memcpy(get_decode_data+1,&DebugData,sizeof(DebugData));
	
	get_decode_data[65]=0;
	for(i=1;i<65;i++)
	{	  				    
		get_decode_data[65] += get_decode_data[i];
	}	
//	Uart0_SendData(get_decode_data,66);
	serial0_send(get_decode_data,66);
}

uint8_t values=0;
extern API_Config_t  API_Config;
void serial_loop(void)
{
	if(++values>=3)
	{
		values = 0;
		DebugData.data1 = mpu_filter.ptich - Brush[Brush_A].MPU_Basic;
		DebugData.data2 = message_online_flage;
		DebugData.data3 = Brush[Brush_A].real_speed;
		DebugData.data4 = PMSM[PMSM_A].SpeedNew;
		DebugData.data5 = PMSM[PMSM_A].SpeedNow;
		DebugData.data6 = hall_encoder[PMSM_A].PLL_realspeed;
		DebugData.data7 = PMSM[PMSM_U].SpeedNew;
		DebugData.data8 = PMSM[PMSM_U].SpeedNow;
		DebugData.data9 =	hall_encoder[PMSM_U].PLL_realspeed;
		DebugData.data10 = hall_encoder[PMSM_A].PLL_realspeed;
		DebugData.data11 = PMSM[PMSM_A].SpeedSet;
		DebugData.data12 =  PMSM[PMSM_A].acc_status;
		DebugData.data13 = hall_encoder[PMSM_U].PLL_realspeed;
		DebugData.data14 =  PMSM[PMSM_U].SpeedSet;
		DebugData.data15 = API_Config.Status;
		DebugData.data16 = PMSM[PMSM_A].SpeedFIL;
		
//		GPIO_WritePin(GPIOE,GPIO_PIN2,values%2);
		
//		Uart0_SendData(buffers,2);
//		usart_send(buffers,2);
		uart_WaveformData(); // 6ms发送完成
	}
}

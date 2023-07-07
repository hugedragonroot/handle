#include "gps_ext.h"
#include "main.h"
#include "string.h"

#if USING_RTOS
/*FreeRtos includes*/
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
/* USER CODE END 0 */
static xQueueHandle gps_uartData;		/*串口数据接收队列*/
#else

static uint8_t GPS_recv_buff[64];
static uint8_t GPS_data_buff[256];
TLoopBuf loop_gps_uart;
#endif

static uint8_t dtu_rxcmdbuf[DTU_RX_CMD_BUF_SIZE]; /*处理DTU相关数据缓存*/
gps_data_st gps_data;

void gps_init(void)
{
    #if USING_RTOS
	gps_uartData = xQueueCreate(256, sizeof(uint8_t));
    #else
    LoopQueue_Init(&loop_gps_uart,GPS_data_buff,sizeof(GPS_data_buff));
    //Tx DMA
    const u8 array[] ="DMA test" ;
    dma_channel_disable(DMA1,DMA_CH4);
    dma_transfer_number_config(DMA1,DMA_CH4,sizeof(array));
    dma_memory_address_config(DMA1,DMA_CH4,(uint32_t)&array);
    dma_channel_enable(DMA1,DMA_CH4);
    //Rx DMA
    dma_channel_disable(DMA1,DMA_CH2);
    dma_transfer_number_config(DMA1,DMA_CH2,sizeof(GPS_recv_buff));
    dma_memory_address_config(DMA1,DMA_CH2,(uint32_t)GPS_recv_buff);
    dma_channel_enable(DMA1,DMA_CH2);
    #endif
	
}

void UART3_IRQHandler(void)
{
    #if USING_RTOS
	uint8_t data;
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	
    if(RESET != usart_interrupt_flag_get(UART3, USART_INT_FLAG_RBNE)){
        /* read one byte from the receive data register */
		data = 	(uint8_t)usart_data_receive(UART3);
		xQueueSendFromISR(gps_uartData, &data, &xHigherPriorityTaskWoken);
    }       
    #else

    if(RESET != usart_interrupt_flag_get(UART3, USART_INT_FLAG_IDLE)){
        uint8_t gps_recv_len = sizeof(GPS_recv_buff) - dma_transfer_number_get(DMA1,DMA_CH2);

		dma_channel_disable(DMA1,DMA_CH2);

		LoopQueue_Write(&loop_gps_uart,GPS_recv_buff,gps_recv_len);

		dma_transfer_number_config(DMA1,DMA_CH2,sizeof(GPS_recv_buff));

		dma_channel_enable(DMA1,DMA_CH2);

		usart_data_receive(UART3);
		usart_interrupt_flag_clear(UART3,USART_INT_FLAG_IDLE);
    }

    #endif

}
int Gps_GetDataWithTimout(uint8_t *c)
{
    #if USING_RTOS
	if (xQueueReceive(gps_uartData, c, 100) == pdTRUE)
	{
		return 1;
	}
	*c = 0;
	return 0;
    #else
    return LoopQueue_ReadRelease(&loop_gps_uart,c,1);

    #endif
}

void send_data_to_dtu(uint8_t *data, uint32_t size)
{
    uart_gps_send(data, size);
}


/**
 * @brief       发送命令到DTU并进行数据校验
 * 
 * @param       cmd     :   需要发送的AT指令
 * @param       ask     :   需要校验的应答数据
 * @param       timeout :   AT指令校验超时时间，单位：100ms
 * 
 * @return      1  :   校验ask数据成功
 *              0  :   DTU返回OK
 *             -1  :   DTU返回ERROR
 *             -2  :   发送AT指令校验超时
 */
static int send_cmd_to_dtu(char *cmd, char *ask, uint32_t timeout)
{
		uint8_t ch;
    uint32_t rx_len = 0;

    /*初始化缓存数据*/
    memset(dtu_rxcmdbuf, 0, DTU_RX_CMD_BUF_SIZE);


    /*发送AT指令到DTU*/
    send_data_to_dtu((uint8_t *)cmd, strlen(cmd));

    /*等待DTU应答AT指令结果*/
    while (1)
    {
        if (strstr((char *)dtu_rxcmdbuf, ask) != NULL)
        {
            return 1;
        }
//        else if (strstr((char *)dtu_rxcmdbuf, "OK") != NULL)
//        {
//            return 0;
//        }
        else if (strstr((char *)dtu_rxcmdbuf, "ERROR\r\n") != NULL)
        {
            return -1;
        }

        if (Gps_GetDataWithTimout(&ch))
        {
					dtu_rxcmdbuf[rx_len++] = ch;//从串口缓存中读一个字节

            if (rx_len >= DTU_RX_CMD_BUF_SIZE) /*接收应答数据超长，返回ERROR*/
            {
                return -1;
            }
        }
        else
        {
            timeout--;

            if (timeout == 0)
            {
                return -2;
            }
            #if USING_RTOS

            vTaskDelay(100);
            #endif
        }
    }
}


static const _dtu_atcmd_st dtu_aliyun_param_info[] = { 
//AT+QMTCFG="recv/mode",0,0,1	
//AT+QMTCFG="aliauth",0,"if31LbK8Cqi","M130Ttest","88f59404404747beea850e4684161cb8"
//AT+QMTOPEN=0,"iot-as-mqtt.cn-shanghai.aliyuncs.com",1883
//AT+QMTCONN=0,"M130Ttest"
//AT+QMTSUB=0,1,"/sys/if31LbK8Cqi/M130Ttest/thing/event/property/post",0 //订阅主题
//AT+QMTPUBEX=0,0,0,0,"/sys/if31LbK8Cqi/M130Ttest/thing/event/property/post",30	//发布消息
	
    {5, "AT+QMTCFG=\"recv/mode\",0,0,1\r\n",																	"OK\r\n"},
    {5,	"AT+QMTCFG=\"aliauth\",0,"PRODUCTKEY","DEVICENAME","DEVICESECRET"\r\n","OK\r\n"},
//    {5, "AT+QMTOPEN=?\r\n",																										"OK\r\n"},
		{5, "AT+QMTOPEN=0,\"iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883\r\n",			"OK\r\n"},
//		{5, "AT+QMTOPEN?\r\n",																										"OK\r\n"},
//		{5, "AT+QMTOPEN=?\r\n",	"+QMTOPEN: 0,\"iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883\r\n"},
//		{5, "AT+QMTCONN=?\r\n",																										"OK\r\n"},
		{5, "AT+QMTCONN=0,\""DEVICENAME"\"\r\n",																	"OK\r\n"},
//		{5, "AT+QMTSUB=?\r\n",																										"OK\r\n"},
		{5, "AT+QMTSUB=0,1,\"/sys/"PRODUCTKEY"/"DEVICENAME"/thing/event/property/post\",0\r\n",	"OK\r\n"},
//		{5, "AT+QMTPUBEX=?\r\n",																									"OK\r\n"},
		{5, "AT+QMTPUBEX=0,0,0,0,\"/sys/"PRODUCTKEY"/"DEVICENAME"/thing/event/property/post\",17\r\n",	">"},
		{5, "{params:{Data:5}}",	"OK\r\n"},		
		{5, "AT+QMTDISC=0\r\n",																										"OK\r\n"},

	
};

//static const _dtu_atcmd_st dtu_onenet_param_info[] = {

//    /*1.选择工作模式为：OneNET透传模式*/
//    {5, "AT+WORK\r\n",      "AT+WORK=\"ONENET\"\r\n"},

//    /*2.配置OneNET透传模式的工作参数*/
//    {5, "AT+ONEDI\r\n",     "AT+ONEDI=\"12345\"\r\n"},
//    {5, "AT+ONEPI\r\n",     "AT+ONEPI=\"1234567890\"\r\n"},
//    {5, "AT+ONEAI\r\n",     "AT+ONEAI=\"12345678901234567890\"\r\n"},
//    {5, "AT+ONEIP\r\n",     "AT+ONEIP=\"mqtt.hecloud.com\",\"6002\"\r\n"},
//    {5, "AT+ONECON\r\n",    "AT+ONECON=\"1\",\"0\",\"0\",\"1\",\"300\"\r\n"},

//    /*3.其他参数功能配置*/
//};


/**
 * @brief       配置DTU工作参数
 * 
 * @param       work_param      :   工作模式相关AT指令参数
 * @param       num             :   需要配置的AT指令参数数量
 * 
 * @return      0  :    所有参数配置成功
 *              n  :    第几个参数配置失败：1-n
 */
static int dtu_config_work_param(_dtu_atcmd_st *work_param, uint8_t num)
{
    int i;
    int res = 0;

    for (i = 0; i < num; i++)
    {
        res = send_cmd_to_dtu((work_param + i)->set_cmd,
                              (work_param + i)->read_cmd + strlen((work_param + i)->read_cmd) - 1,
                              work_param[i].timeout);
				if (res < 0)
				{
						return i+1;
				}
    }

    return 0;
}

/** 
 * @brief       初始化DTU的工作状态
 * 
 * @param       work_mode   :   DTU工作模式


 *  @arg        DTU_WORKMODE_ALIYUN,      阿里云透传模式
 *  @arg        DTU_WORKMODE_ONENET,      OneNET透传模式
 *  @arg        DTU_WORKMODE_GPS,      		GPS模式

 * 
 * @return      0   :   初始化成功
 *             -1   :   进入配状态失败
 *             -2   :   DTU工作参数配置失败
 *             -3   ：  DTU进入透传状态失败
 */
int dtu_config_init(_dtu_work_mode_eu work_mode)
{
    int res;
	 char* p;
	uint8_t i;


    /*2.配置DTU的工作参数*/
    switch (work_mode)
    {
        case DTU_WORKMODE_ALIYUN:
        {
            res = dtu_config_work_param((_dtu_atcmd_st *)&dtu_aliyun_param_info, sizeof(dtu_aliyun_param_info) / sizeof(_dtu_atcmd_st));
            break;
        }
        case DTU_WORKMODE_ONENET:
        {
//            res = dtu_config_work_param((_dtu_atcmd_st *)&dtu_onenet_param_info, sizeof(dtu_onenet_param_info) / sizeof(_dtu_atcmd_st));
            break;
        }
        case DTU_WORKMODE_GPS:
        {
					res = send_cmd_to_dtu("AT+QGPS=1\r\n","OK\r\n",5);
					if(res==1)
					{
						res = send_cmd_to_dtu("AT+QGPSLOC=0\r\n","OK\r\n",5);
						if(res==1)
						{
							if (strstr((char *)dtu_rxcmdbuf, "+QGPSLOC") != NULL)
							{
								//纬度
								for(i=0;i<9;i++)
								{
									gps_data.gps.lbs_latitude[1+i]=dtu_rxcmdbuf[20+i];
								}
								if(dtu_rxcmdbuf[29]=='S')gps_data.gps.lbs_latitude[0]='-';
								else gps_data.gps.lbs_latitude[0]='+';
								//经度
								for(i=0;i<10;i++)
								{
									gps_data.gps.lbs_longitude[1+i]=dtu_rxcmdbuf[31+i];
								}
								if(dtu_rxcmdbuf[40]=='W')gps_data.gps.lbs_longitude[0]='-';
								else gps_data.gps.lbs_longitude[0]='+';								
							}							
						}
					}
					res = send_cmd_to_dtu("AT+QGPS=0\r\n","OK\r\n",5);

            break;
        }
        default:
        {
            break;
        }
    }

    if( res != 0 )
    {
        return -2;
    }

//    /*3.DTU进入透传状态*/
//    res = dtu_enter_transfermode();
//    if( res != 0 )
//    {
//        return -3;
//    }

    return 0;
}

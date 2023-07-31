#include "gps_ext.h"
#include "main.h"
#include "string.h"

#if 0

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
#else

#if 0
const char *GNSS_MQTT_CMD[] ={
    "AT+QGPS=1\r\n",//打开GNSS
    "AT+QGPSLOC=0\r\n",//获取定位信息
    "AT+QMTCFG=\"recv/mode\",0,0,1\r\n",//配置服务器数据接受模式 
    "AT+QMTOPEN=0,\"8.134.39.55\",1883\r\n",//MQTT 客户端打开网络
    "AT+QMTCONN=0,\"M130T\"\r\n",//连接MQTT
    "AT+QMTSUB=0,1,\"/sys/pub/M130T/XSTOM130T000004/thing/GPS_Data/property/post\",0\r\n",//订阅

};

const char *GNSS_MQTT_RES[][8] = {
    {"OK\r\n", "504",NULL},
    {"OK\r\n", "505","516",NULL},

    {"OK\r\n",NULL},
};
#endif 


// static xQueueHandle gps_uartData;		/*串口数据接收队列*/
static char dtu_rxcmdbuf[DTU_RX_CMD_BUF_SIZE]; /*处理DTU相关数据缓存*/
static char dtu_txcmdbuf[DTU_TX_CMD_BUF_SIZE]; /*处理DTU相关数据缓存*/
static uint8_t dtu_rx_len;
gps_data_st gps_data;


void gps_init_default(void);

char dev_id_string[16]="XSTOM130T000004";//"1C12202004000001";
//char lbs_longitude_string[16]="+11331.1151";
//char lbs_latitude_string[16]="+2309.2613";
char lbs_longitude_string[16]="+11331.0900";
char lbs_latitude_string[16]="+2309.3714";
char res_3_string[16]="test";
char dev_version_string[16]="1.1.2";

// uint8_t gps_recv_buff[64];//接收缓存
// uint8_t gps_data_buff[128];//队列处理缓存


// TLoopBuf loop_gps_uart;

void gps_init(void)
{	
//	gps_uartData = xQueueCreate(256, sizeof(uint8_t));
	
	gps_init_default();

    // LoopQueue_Init(&loop_gps_uart,gps_data_buff,sizeof(gps_data_buff));

	dma_channel_disable(DMA1,DMA_CH2);
	dma_transfer_number_config(DMA1,DMA_CH2,sizeof(dtu_rxcmdbuf));
	dma_memory_address_config(DMA1,DMA_CH2,(uint32_t)dtu_rxcmdbuf);
	dma_channel_enable(DMA1,DMA_CH2);
	
    // delay_ms(100);
	const u8 test[] = "AT+QGPSEND\r\n";
	//Tx_DMA
	dma_channel_disable(DMA1,DMA_CH4);
	dma_transfer_number_config(DMA1,DMA_CH4,sizeof(test));
	dma_memory_address_config(DMA1,DMA_CH4,(uint32_t)test);
	dma_channel_enable(DMA1,DMA_CH4);
	
}
//AT+QGPSLOC=0
//+QGPSLOC: 090836.00,2309.2613N,11331.1151E,1.27,75.6,3,0.000,0.267,0.144,290323,04
//OK

void gps_init_default(void)
{
	gps_data.dev_id=dev_id_string;
	gps_data.gps.lbs_vail_flag = 1;
	gps_data.gps.lbs_type = 1;
	gps_data.gps.lbs_longitude = lbs_longitude_string;
	gps_data.gps.lbs_latitude = lbs_latitude_string;	
	gps_data.dev_type = DRIVER_MODEL;			
	gps_data.dev_M130Tmode = 1;	
	gps_data.dev_M115mode = 1;	
	gps_data.dev_lock = 1;	
	gps_data.dev_lampsw = 1;	
	gps_data.dev_voicesw = 0;	
	gps_data.dev_alarmsw = 1;	
	gps_data.dev_cruisesw = 0;	
	gps_data.dev_speed =2;	
	gps_data.dev_seat_angle = +120;		
	gps_data.dev_body_angle = -120;	
	gps_data.dev_vol = 1;	
	gps_data.dev_battery = 100;	
	gps_data.dev_total_mil = 100000;	
	gps_data.res_1 = 10;	
	gps_data.res_2 = 100000;	
	gps_data.res_3 = res_3_string;		 
	sprintf(dev_version_string,"%d.%d.%d",SOFTWARE_VER0,SOFTWARE_VER1,SOFTWARE_VER2);
	gps_data.dev_version = dev_version_string;	
}


void gps_data_update(void)
{			
	gps_data.dev_M130Tmode = 1;/*(Remote_setting_para.ControlMode==eWalking)?ePwalking: ePTracks;	*/
	gps_data.dev_M115mode = Remote_setting_para.M115CtrlMode;	
	gps_data.dev_lock = Remote_setting_para.HandleLock ;	
	gps_data.dev_lampsw =  Remote_setting_para.LampSwitch ;	
	gps_data.dev_voicesw = Remote_setting_para.VoiceSwitch;	
	gps_data.dev_alarmsw = Remote_setting_para.AlarmSwitch ;	
	gps_data.dev_cruisesw = Remote_setting_para.CruiseCtrlSwitch;	
	gps_data.dev_speed = Remote_setting_para.SpeedGear ;	
	gps_data.dev_seat_angle =0; /*Remote_receive_para.seat_angle ;	*/	
	gps_data.dev_body_angle =0;/*Remote_receive_para.body_angle;	*/
	gps_data.dev_vol = Remote_setting_para.VoiceLevel;	
	gps_data.dev_battery =0; /*Remote_receive_para.Battery;	*/
	gps_data.dev_total_mil = Remote_setting_para.totalkm;	
}


void UART3_IRQHandler(void)
{
	#if USING_RTOS
	uint8_t data;
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	
	if(RESET != usart_interrupt_flag_get(UART3, USART_INT_FLAG_RBNE)){
			/* read one byte from the receive data register */
		data = 	(uint8_t)usart_data_receive(UART3);
//		xQueueSendFromISR(gps_uartData, &data, &xHigherPriorityTaskWoken);
		LoopQueue_Write(&loop_gps_uart,&data,1);
	} 
	#else 
	if(RESET != usart_interrupt_flag_get(UART3, USART_INT_FLAG_IDLE)){
		dtu_rx_len = sizeof(dtu_rxcmdbuf) - dma_transfer_number_get(DMA1,DMA_CH2);

		dma_channel_disable(DMA1,DMA_CH2);

		// LoopQueue_Write(&loop_gps_uart,gps_recv_buff,gps_recv_len);

		dma_transfer_number_config(DMA1,DMA_CH2,sizeof(dtu_rxcmdbuf));

		dma_channel_enable(DMA1,DMA_CH2);

		usart_data_receive(UART3);
		usart_interrupt_flag_clear(UART3,USART_INT_FLAG_IDLE);
	
	}
	#endif
}
//int Gps_GetDataWithTimout(uint8_t *c)
//{
//	if (xQueueReceive(gps_uartData, c, 1000) == pdTRUE)
//	{
//		return 1;
//	}
//	*c = 0;
//	return 0;
		// return LoopQueue_ReadRelease(&loop_gps_uart,c,1);
//}

void send_data_to_dtu(uint8_t *data, uint32_t size)
{
    uart_gps_send(data, size);
}


// static int send_cmd_to_dtu(char *cmd, char *ask, uint32_t timeout)
static GPSDtuStatus send_cmd_to_dtu(char *cmd, char *ask, uint32_t timeout)
{
    #if 1
    static u8 timeCount = 0;

    /*发送AT指令到DTU*/
    if(timeCount == 0) {
        send_data_to_dtu((uint8_t *)cmd, strlen(cmd));
        // uart_wire_send((uint8_t *)cmd, strlen(cmd));
    }
    /*等待DTU应答AT指令结果*/
    else if(timeCount && dtu_rx_len){
        // uart_wire_send((uint8_t *)dtu_rxcmdbuf,strlen(dtu_rxcmdbuf));
        
        dtu_rx_len = 0;
        timeCount = 0;
        if(strstr(dtu_rxcmdbuf,ask) != NULL)
            return DTU_SUCCEED;
        else 
            return DTU_FAILE;
    }
    if(++timeCount > timeout)timeCount = 0;

    return DTU_FAILE;


    #else
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
					// #if USING_DEBUG
					// uart_wire_send((uint8_t *)dtu_rxcmdbuf,sizeof(dtu_rxcmdbuf));
					// #endif
            return 1;
        }
        else if (strstr((char *)dtu_rxcmdbuf, "ERROR") != NULL)
        {
					// #if USING_DEBUG
					// uart_wire_send((uint8_t *)dtu_rxcmdbuf,sizeof(dtu_rxcmdbuf));
					// #endif
            return -1;
        }

        if (Gps_GetDataWithTimout(&ch))
        {
					dtu_rxcmdbuf[rx_len++] = ch;//从串口缓存中读一个字节

					if (rx_len >= DTU_RX_CMD_BUF_SIZE) /*接收应答数据超长，返回ERROR*/
					{
					// 	#if USING_DEBUG
					// uart_wire_send((uint8_t *)dtu_rxcmdbuf,sizeof(dtu_rxcmdbuf));
					// #endif
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

            delay_ms(100);
        }
    }
    #endif
		
}



//413
const char GPS_data[] ={ 
"{\"Dev_ID\":\"%s\",\
\"LBS_Valid_Flag\":%d,\
\"LBS_Type\":%d,\
\"Longiude\":\"%s\",\
\"Latitube\":\"%s\",\
\"Dev_Type\":%d,\
\"Dev_M130TMode\":%d,\
\"Dev_M115Mode\":%d,\
\"Dev_Lock\":%d,\
\"Dev_LampSW\":%d,\
\"Dev_VoiceSW\":%d,\
\"Dev_AlarmSW\":%d,\
\"Dev_CruiseSW\":%d,\
\"Dev_Speed\":%d,\
\"Dev_Seat_Angle\":\"%d\",\
\"Dev_Body_Angle\":\"%d\",\
\"Dev_Vol\":%d,\
\"Dev_Battery\":\"%d\",\
\"Dev_Total_Mil\":%d,\
\"Dev_Res_1\":%d,\
\"Dev_Res_2\":%d,\
\"Dev_Res_3\":\"%s\",\
\"Dev_Version\":\"%s\"}"};

void  dtu_gps_data_pack(void)
{
	sprintf((char*)dtu_txcmdbuf,GPS_data,
	gps_data.dev_id,
	gps_data.gps.lbs_vail_flag ,
	gps_data.gps.lbs_type ,
	gps_data.gps.lbs_longitude ,
	gps_data.gps.lbs_latitude ,
	gps_data.dev_type ,		
	gps_data.dev_M130Tmode ,
	gps_data.dev_M115mode ,	
	gps_data.dev_lock ,
	gps_data.dev_lampsw,
	gps_data.dev_voicesw,	
	gps_data.dev_alarmsw,
	gps_data.dev_cruisesw,	
	gps_data.dev_speed ,
	gps_data.dev_seat_angle,
	gps_data.dev_body_angle ,
	gps_data.dev_vol ,
	gps_data.dev_battery ,
	gps_data.dev_total_mil ,
	gps_data.res_1 ,	
	gps_data.res_2 ,	
	gps_data.res_3 ,		 
	gps_data.dev_version 	
	);
	
}


int8_t  dtu_work_xstoyun(void)
{
    #if 1
    static u8 status = 0;
	static uint8_t buf[100];
    switch (status) {
    case 0:
        // 配置服务器数据接受模式 AT+QMTCFG="recv/mode",<client_idx>[,<msg_recv_mode>[,<msg_len_enable>]]
        // <client_idx> 整型。MQTT 客户端标识符。范围：0~5。
        // <msg_recv_mode> 整型。配置 MQTT 消息接收模式。0 从服务器接收的 MQTT 消息以 URC 的形式上报 1 从服务器接收的 MQTT 消息不以 URC 的形式上报
        // <msg_len_enable> 整型。配置 URC 中是否包含从服务器接收的 MQTT 消息长度。 0 不包含 1 包含
        if(DTU_SUCCEED == send_cmd_to_dtu("AT+QMTCFG=\"recv/mode\",0,0,1\r\n","OK\r\n",5)){
            ++status;
        }
        break;
    case 1:
        // 该命令用于为 MQTT 客户端打开网络AT+QMTOPEN=<client_idx>,<host_name>,<port>
        // <host_name> 字符串类型。服务器地址，可以是 IP 地址或者域名。最大长度：100 字节。
        // <port> 整型。服务器端口。范围：1~65535。
		if(DTU_SUCCEED == send_cmd_to_dtu("AT+QMTOPEN=0,\"8.134.39.55\",1883\r\n","\r\n+QMTOPEN:",5)){
            ++status;
        }
        break;
    case 2:
        // 客户端请求连接 MQTT 服务器时使用该命令。当客户端创建和服务器之间的 TCP/IP Socket 连接时，必须使用 CONNECT 流创建一个协议层会话。
        // AT+QMTCONN=<client_idx>,<clientid>[,<username>,<password>]
		if(DTU_SUCCEED == send_cmd_to_dtu("AT+QMTCONN=0,\"M130T\"\r\n","\r\n+QMTCONN:",5)){
            ++status;
        }
        break;
    case 3:
        // 该命令用于订阅一个或多个主题。客户端发送 SUBSCRIBE 消息向服务器订阅一个或多个主题，当订阅主题发布消息时，服务器会把这些消息作为 PUBLISH 消息传输到客户端。
        // AT+QMTSUB=<client_idx>,<msgid>,<topic1>,<qos1>[,<topic2>,<qos2>…]
        // <msgid> 整型。数据包标识符。范围：1~65535。
        // <topic> 字符串类型。客户端订阅或者退订的主题。
        // <qos> 整型。客户端发布消息的 QoS 等级。 0 最多发送一次
        // 1 至少发送一次
        // 2 只发送一次
		sprintf((char*)buf,"AT+QMTSUB=0,1,\"/sys/pub/M130T/%s/thing/GPS_Data/property/post\",0\r\n",dev_id_string);
		if(DTU_SUCCEED == send_cmd_to_dtu((char*)buf,	"\r\n+QMTSUB:",5)){
            ++status;
            dtu_gps_data_pack();
        }
		
        break;
    case 4:
        // 客户端通过该命令可以发布定长消息到服务器，再由服务器分配到感兴趣的订阅者。每一个 PUBLISH 消息都关联一个主题名称，
        // 若一个客户端订阅了一个或者多个主题，当订阅主题发布消息时，服务器会把这些消息作为 PUBLISH 消息传输到客户端。
        // AT+QMTPUBEX=<client_idx>,<msgid>,<qos>,<retain>,<topic>,<length>
        // <retain><retain> 整型。消息发送到当前订阅者后，服务器是否保存该消息。 0 不保存 1 保存
        sprintf((char*)buf,"AT+QMTPUBEX=0,0,0,0,\"/sys/pub/M130T/%s/thing/GPS_Data/property/post\",%d\r\n",dev_id_string,strlen((char*)dtu_txcmdbuf));
        if(DTU_SUCCEED == send_cmd_to_dtu((char*)buf, ">",5)){
            ++status;
        }	
        break;
    case 5:
        // 该命令用于从存储缓存中读取消息，服务器上报消息时都会存储到这个缓存中。
        // AT+QMTRECV=<client_idx>[,<recv_id>]
        if(DTU_SUCCEED == send_cmd_to_dtu((char*)dtu_txcmdbuf,"\r\n+QMTRECV:",5)){
            memset(dtu_txcmdbuf,0,strlen((char*)dtu_txcmdbuf));
            ++status;
        }

        break;
    case 6:
        // 当客户端请求断开与 MQTT 服务器的连接时使用该命令。客户端发送 DISCONNECT 消息到服务器表示即将与服务器断开 TCP/IP 连接。
        // res返回-1 断开连接失败 0 断开连接成功
		if(DTU_SUCCEED == send_cmd_to_dtu("AT+QMTDISC=0\r\n",	"\r\n+QMTDISC:",5)){
            ++status;
            // status = 0;
        }	
        break;
    case 7:
        status = 0;
        return DTU_SUCCEED;
        //break;

    default:
        break;
    }
    return DTU_FAILE;


    #else
	uint8_t i;
	uint8_t buf[100];
	int8_t res = 0;
	//gps_init_default();
	// 断开连接
	res = send_cmd_to_dtu("AT+QMTDISC=0\r\n",	"\r\n+QMTDISC:",5);
	// 配置服务器数据接受模式 AT+QMTCFG="recv/mode",<client_idx>[,<msg_recv_mode>[,<msg_len_enable>]]
	// <client_idx> 整型。MQTT 客户端标识符。范围：0~5。
	// <msg_recv_mode> 整型。配置 MQTT 消息接收模式。0 从服务器接收的 MQTT 消息以 URC 的形式上报 1 从服务器接收的 MQTT 消息不以 URC 的形式上报
	// <msg_len_enable> 整型。配置 URC 中是否包含从服务器接收的 MQTT 消息长度。 0 不包含 1 包含
	res = 0;
	res = send_cmd_to_dtu("AT+QMTCFG=\"recv/mode\",0,0,1\r\n","OK\r\n",5);
	if(res)
	// 该命令用于为 MQTT 客户端打开网络AT+QMTOPEN=<client_idx>,<host_name>,<port>
	// <host_name> 字符串类型。服务器地址，可以是 IP 地址或者域名。最大长度：100 字节。
	// <port> 整型。服务器端口。范围：1~65535。
		res = send_cmd_to_dtu("AT+QMTOPEN=0,\"8.134.39.55\",1883\r\n","\r\n+QMTOPEN:",5);
	if(res)
	// 客户端请求连接 MQTT 服务器时使用该命令。当客户端创建和服务器之间的 TCP/IP Socket 连接时，必须使用 CONNECT 流创建一个协议层会话。
	// AT+QMTCONN=<client_idx>,<clientid>[,<username>,<password>]
		res = send_cmd_to_dtu("AT+QMTCONN=0,\"M130T\"\r\n","\r\n+QMTCONN:",5);
	if(res)
	{
	// 该命令用于订阅一个或多个主题。客户端发送 SUBSCRIBE 消息向服务器订阅一个或多个主题，当订阅主题发布消息时，服务器会把这些消息作为 PUBLISH 消息传输到客户端。
	// AT+QMTSUB=<client_idx>,<msgid>,<topic1>,<qos1>[,<topic2>,<qos2>…]
	// <msgid> 整型。数据包标识符。范围：1~65535。
	// <topic> 字符串类型。客户端订阅或者退订的主题。
	// <qos> 整型。客户端发布消息的 QoS 等级。 0 最多发送一次
	// 1 至少发送一次
	// 2 只发送一次
		sprintf((char*)buf,"AT+QMTSUB=0,1,\"/sys/pub/M130T/%s/thing/GPS_Data/property/post\",0\r\n",dev_id_string);
		res = send_cmd_to_dtu((char*)buf,	"\r\n+QMTSUB:",5);	
	}	
	dtu_gps_data_pack();
	if(res)
	{
	// 客户端通过该命令可以发布定长消息到服务器，再由服务器分配到感兴趣的订阅者。每一个 PUBLISH 消息都关联一个主题名称，
	// 若一个客户端订阅了一个或者多个主题，当订阅主题发布消息时，服务器会把这些消息作为 PUBLISH 消息传输到客户端。
	// AT+QMTPUBEX=<client_idx>,<msgid>,<qos>,<retain>,<topic>,<length>
	// <retain><retain> 整型。消息发送到当前订阅者后，服务器是否保存该消息。 0 不保存 1 保存
		sprintf((char*)buf,"AT+QMTPUBEX=0,0,0,0,\"/sys/pub/M130T/%s/thing/GPS_Data/property/post\",%d\r\n",dev_id_string,strlen((char*)dtu_txcmdbuf));
		res = send_cmd_to_dtu((char*)buf, ">",5);	
	}
	if(res)		
	// 该命令用于从存储缓存中读取消息，服务器上报消息时都会存储到这个缓存中。
	// AT+QMTRECV=<client_idx>[,<recv_id>]
		res = send_cmd_to_dtu((char*)dtu_txcmdbuf,"\r\n+QMTRECV:",5);	
	memset(dtu_txcmdbuf,0,strlen((char*)dtu_txcmdbuf));
	if(res)	
	// 当客户端请求断开与 MQTT 服务器的连接时使用该命令。客户端发送 DISCONNECT 消息到服务器表示即将与服务器断开 TCP/IP 连接。
	// res返回-1 断开连接失败 0 断开连接成功
		res = send_cmd_to_dtu("AT+QMTDISC=0\r\n",	"\r\n+QMTDISC:",5);	
	else
		send_cmd_to_dtu("AT+QMTDISC=0\r\n",	"\r\n+QMTDISC:",5);	
	return res;
    #endif
}


int8_t  dtu_work_gps(void)
{
	//uint8_t i;
	GPSDtuStatus res = DTU_FAILE;
	static uint8_t Flag = 1;
    if (Flag == 1){
        if (DTU_SUCCEED == send_cmd_to_dtu("AT+QGPS=1\r\n","OK\r\n",5)){
            Flag = 0;
        }else if (DTU_SUCCEED == send_cmd_to_dtu("AT+QGPS=1\r\n","504",5)){
            Flag = 0;
        }
	}
    else if(Flag == 0) {
    
		res = send_cmd_to_dtu("AT+QGPSLOC=0\r\n","OK\r\n",5);
		if(res==DTU_SUCCEED)
		{
			if (strstr((char *)dtu_rxcmdbuf, "+QGPSLOC") != NULL) {
				//纬度
				gps_data.gps.lbs_latitude[1]=dtu_rxcmdbuf[20];
				gps_data.gps.lbs_latitude[2]=dtu_rxcmdbuf[21];
				gps_data.gps.lbs_latitude[3]=dtu_rxcmdbuf[22];
				gps_data.gps.lbs_latitude[4]=dtu_rxcmdbuf[23];
				gps_data.gps.lbs_latitude[5]=dtu_rxcmdbuf[24];
				gps_data.gps.lbs_latitude[6]=dtu_rxcmdbuf[25];
				gps_data.gps.lbs_latitude[7]=dtu_rxcmdbuf[26];
				gps_data.gps.lbs_latitude[8]=dtu_rxcmdbuf[27];
				gps_data.gps.lbs_latitude[9]=dtu_rxcmdbuf[28];
				if(dtu_rxcmdbuf[31]=='S')gps_data.gps.lbs_latitude[0]='-';
				else gps_data.gps.lbs_latitude[0]='+';
				//经度
				gps_data.gps.lbs_longitude[1]=dtu_rxcmdbuf[31];
				gps_data.gps.lbs_longitude[2]=dtu_rxcmdbuf[32];
				gps_data.gps.lbs_longitude[3]=dtu_rxcmdbuf[33];
				gps_data.gps.lbs_longitude[4]=dtu_rxcmdbuf[34];
				gps_data.gps.lbs_longitude[5]=dtu_rxcmdbuf[35];
				gps_data.gps.lbs_longitude[6]=dtu_rxcmdbuf[36];
				gps_data.gps.lbs_longitude[7]=dtu_rxcmdbuf[37];
				gps_data.gps.lbs_longitude[8]=dtu_rxcmdbuf[38];
				gps_data.gps.lbs_longitude[9]=dtu_rxcmdbuf[39];
				gps_data.gps.lbs_longitude[10]=dtu_rxcmdbuf[40];
				if(dtu_rxcmdbuf[45]=='W')gps_data.gps.lbs_longitude[0]='-';
				else gps_data.gps.lbs_longitude[0]='+';								
			}							
		}
	}
//	res = send_cmd_to_dtu("AT+QGPSEND\r\n","OK\r\n",5);
	return res;
}
#if 0
static int dtu_config_work_param(_dtu_atcmd_st *work_param, uint8_t num)
{
    int i;
    int res = 0;

    for (i = 0; i < num; i++)
    {
        res = send_cmd_to_dtu((work_param + i)->set_cmd,
                              (work_param + i)->read_cmd ,
                              work_param[i].timeout);
				delay_ms(100);
    }

    return 0;
}
#endif

uint8_t dtu_work_mode(_dtu_work_mode_eu work_mode)
{
	u8 res;
	// char* p;
	// uint8_t i;

	switch (work_mode)
	{
		case DTU_WORKMODE_XSTOYUN:
			res = dtu_work_xstoyun();
		break;      
		case DTU_WORKMODE_GPS:       
			res = dtu_work_gps();	
		break; 
	
		default:       
		break;        
	}
	return res;
}




#endif

#include "can_ext.h"
#include "string.h"


/*FreeRtos includes*/
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

#include "../PROTOCOL/protocol_427/protocol.h"

static SemaphoreHandle_t CANRecvFlag;//CAN中断接收信号量

uint8_t Car_buff[CAN_BUFFER_SIZE];
Can_fifo_buffer_t CANRx_Buffer;
uint8_t CAN_Rx_buffer[CAN_BUFFER_SIZE];

Can_fifo_buffer_t CANTx_Buffer;
uint8_t CAN_Tx_buffer[CAN_BUFFER_SIZE];

can_parameter_struct	can_parameter;
can_filter_parameter_struct can_filter;
void CAN_Para_Init(void);
static void StdCanSetID(uint8_t filterNumber, uint32_t id1,uint32_t id2);


void can_myinit(void)
{
	/* initialize CAN register */
  can_deinit(CAN0);
    /* enable CAN clock */
  rcu_periph_clock_enable(RCU_CAN0);
  rcu_periph_clock_enable(RCU_GPIOA);//使能时钟
	
		
	CAN_Para_Init();
	
//	rcu_periph_clock_enable(RCU_AF);
	
  gpio_init(GPIOA, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ,GPIO_PIN_11);
  gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_12);//IO复用为CAN功能
  
    /* configure CAN0 NVIC */
  nvic_irq_enable(CAN0_RX1_IRQn,3,0); //中断配置

  can_struct_para_init(CAN_INIT_STRUCT, &can_parameter);
  can_struct_para_init(CAN_FILTER_STRUCT, &can_filter);//初始化参数
  /* baudrate 500Kbps */
	can_parameter.time_triggered = DISABLE;
  can_parameter.auto_bus_off_recovery = ENABLE;
  can_parameter.auto_wake_up = DISABLE;
  can_parameter.auto_retrans = ENABLE;
  can_parameter.rec_fifo_overwrite = DISABLE;
//   can_parameter.trans_fifo_order = DISABLE;
  can_parameter.trans_fifo_order = ENABLE;
  can_parameter.working_mode = CAN_NORMAL_MODE;
  can_parameter.resync_jump_width = CAN_BT_SJW_1TQ;
	can_parameter.time_segment_1 = CAN_BT_BS1_15TQ;
	can_parameter.time_segment_2 = CAN_BT_BS2_1TQ;
	can_parameter.prescaler = 7;//baudrate=60M/((bs1+bs2+1)*7)=
    /* initialize CAN */
  can_init(CAN0, &can_parameter);
	

	StdCanSetID(0,CAN_RECEIVE_ID,0);
	 /* enable can receive FIFO0 not empty interrupt */
  can_interrupt_enable(CAN0, CAN_INT_RFNE1|CAN_INT_TME);

	CANRecvFlag = xSemaphoreCreateBinary();

	paraInit();

}


void CAN_Para_Init(void)
{
	
	CANTx_Buffer.data = CAN_Tx_buffer;
	CANTx_Buffer.size = CAN_BUFFER_SIZE;
	CANTx_Buffer.read_index = 0;
	CANTx_Buffer.write_index = 0;
	
	CANRx_Buffer.data = CAN_Rx_buffer;
	CANRx_Buffer.size = CAN_BUFFER_SIZE;
	CANRx_Buffer.read_index = 0;
	CANRx_Buffer.write_index = 0;
}
//can_trasnmit_message_struct transmit_message;
//can_receive_message_struct receive_message;

static void StdCanSetID(uint8_t filterNumber, uint32_t id1,uint32_t id2)//标准帧普通过滤函数filterNumber：通道   一个通道最多支持2个id号，一共13个通道，最多只支持过滤26个id号
{
	can_filter_parameter_struct can_filter;
	can_filter.filter_number=0;
  can_filter.filter_mode = CAN_FILTERMODE_MASK;
	can_filter.filter_bits = CAN_FILTERBITS_32BIT;
	id1 %= (1 << 11);
	id2 %= (1 << 11);
	can_filter.filter_list_high = id1<<5;
	can_filter.filter_list_low = CAN_FF_STANDARD|CAN_FT_DATA;
	can_filter.filter_mask_high = id2<<5;
	can_filter.filter_mask_low = CAN_FF_STANDARD|CAN_FT_DATA;
	can_filter.filter_fifo_number = CAN_FIFO1;
	can_filter.filter_enable = ENABLE;
	can_filter_init(&can_filter);
}


//*****************************************************************************
//原  型：uint8_t CanSend(u32 id, u8 *data, u8 size)
//功  能:  
//输  入：
//输  出：
//***************************************************************************** 
uint8_t CanSend(uint32_t id, uint8_t *data, uint8_t size)
{
	can_trasnmit_message_struct transmit_message;
	uint16_t i = 0;
	uint8_t  Mailbox = CAN_NOMAILBOX;
	if(id <= 0x7FF)
	{
		transmit_message.tx_sfid = id;
		transmit_message.tx_ff = CAN_FF_STANDARD;
	}
	else
	{
		transmit_message.tx_efid = id;
		transmit_message.tx_ff = CAN_FF_EXTENDED;
	}
	if(size > 8)
		size = 8;
	transmit_message.tx_ft = CAN_FT_DATA;
	transmit_message.tx_dlen =size;
	memcpy(transmit_message.tx_data, data, size);
	while( Mailbox == CAN_NOMAILBOX &&  i < 0xFFF  )
	{
			Mailbox = can_message_transmit(CAN0,&transmit_message);
			i++;
	}
	
	if(i >= 0xFFF)return 0;
	return 1;
}


void gd32_can_error(void)
{
	if(	can_flag_get(CAN0, CAN_FLAG_MTE2) != RESET ||
		can_flag_get(CAN0, CAN_FLAG_MTE0) != RESET ||
		can_flag_get(CAN0, CAN_FLAG_MTE1) != RESET || 
		can_flag_get(CAN0, CAN_FLAG_PERR) != RESET ||
		can_flag_get(CAN0, CAN_FLAG_WERR) != RESET)
	{
		can_flag_clear(CAN0, CAN_FLAG_MTE0);
		can_flag_clear(CAN0, CAN_FLAG_MTE1);
		can_flag_clear(CAN0, CAN_FLAG_MTE2);
		can_flag_clear(CAN0, CAN_FLAG_PERR);
		can_flag_clear(CAN0, CAN_FLAG_WERR);
		can_wakeup(CAN0);
		can_myinit();
	}
}


/*****************************************************************************
*原  型：uint16_t can_serial_available(void)
*功  能: 从缓存区读取数据
*输  入：
*输  出：
******************************************************************************/
uint16_t can_serial_available(void)
{
    uint16_t len = 0;
	  uint16_t CANRx_Buffer_read_index = CANRx_Buffer.read_index;
	  uint16_t CANRx_Buffer_write_index = CANRx_Buffer.write_index;
	
    if (CANRx_Buffer_read_index > CANRx_Buffer_write_index)
    {
        len = CANRx_Buffer.size + CANRx_Buffer_write_index - CANRx_Buffer_read_index;
    }
    else if (CANRx_Buffer_read_index  < CANRx_Buffer_write_index)
    {
        len = CANRx_Buffer_write_index - CANRx_Buffer_read_index;
    }
    return len;
}
uint8_t can_serial_read_char(void)
{
    uint8_t ch = 0;
    ch = CANRx_Buffer.data[CANRx_Buffer.read_index];
    CANRx_Buffer.read_index = (CANRx_Buffer.read_index + 1) % CANRx_Buffer.size;
    return ch;
}
/*****************************************************************************
*原  型：uint16_t can_serial_read(uint8_t *buffer, uint16_t length)
*功  能: 从缓存区读取数据
*输  入：
*输  出：
******************************************************************************/
uint16_t can_serial_read(uint8_t *buffer, uint16_t length)
{
    uint16_t i = 0;
	
    for (i = 0; i < length; i++)
    {
        buffer[i] = CANRx_Buffer.data[CANRx_Buffer.read_index];
        CANRx_Buffer.read_index = (CANRx_Buffer.read_index + 1) % CANRx_Buffer.size;
    }
    return i;
}
/*****************************************************************************
*原  型：uint16_t can_tx_buf(uint8_t *buffer, uint16_t length, Can_fifo_buffer_t* Tx_Buffer)
*功  能: 把待发送的数据写入缓冲区
*输  入：
*输  出：
******************************************************************************/
uint16_t can_tx_buf(uint8_t *buffer, uint16_t length, Can_fifo_buffer_t* Tx_Buffer)
{
    uint16_t i = 0;
	
    for (i = 0; i < length; i++)
    {
        Tx_Buffer->data[Tx_Buffer->write_index] = buffer[i];
        Tx_Buffer->write_index = (Tx_Buffer->write_index + 1) % Tx_Buffer->size;
    }
    return i;
}
/*****************************************************************************
*原  型：uint16_t can_serial_write(uint8_t *buffer, uint16_t length)
*功  能: 把待发送的数据写入缓冲区
*输  入：
*输  出：
******************************************************************************/
uint16_t can_serial_write(uint8_t *buffer, uint16_t length)
{
    uint16_t i = 0;
	  can_tx_buf(buffer, length, &CANTx_Buffer);
    return i;
}

/*****************************************************************************
*原  型：uint16_t can_tx_available(Can_fifo_buffer_t * Tx_Buffer)
*功  能: 检测待发送数据的长度
*输  入：
*输  出：
******************************************************************************/
uint16_t can_tx_available(Can_fifo_buffer_t * Tx_Buffer)
{
    uint16_t len = 0;
    if (Tx_Buffer->read_index > Tx_Buffer->write_index)
    {
        len = Tx_Buffer->size + Tx_Buffer->write_index - Tx_Buffer->read_index;
    }
    else if (Tx_Buffer->read_index  < Tx_Buffer->write_index)
    {
        len = Tx_Buffer->write_index - Tx_Buffer->read_index;
    }
    return len;
}


/*****************************************************************************
*原  型：void can_transmit(void)
*功  能: can总线数据发出
*输  入：
*输  出：
******************************************************************************/
uint16_t tx_len1 = 0;
uint8_t TxMsg[8];
void can_transmit(void)
{  
	tx_len1 =  can_tx_available(&CANTx_Buffer);
	while(tx_len1 > 0)
	{
		
		uint16_t temp;
		if(CAN_SEND_ID != 0)
		{
			if(tx_len1 > 8)
			{
				temp = CANTx_Buffer.size - CANTx_Buffer.read_index;
				if(temp < 8)
				{
					memcpy(TxMsg,&(CANTx_Buffer.data[CANTx_Buffer.read_index]), temp);
					memcpy(&TxMsg[temp],&(CANTx_Buffer.data[0]) ,8-temp);
				}
				else
				{
					memcpy(TxMsg, &(CANTx_Buffer.data[CANTx_Buffer.read_index]), 8);
				}
				CanSend(CAN_SEND_ID, TxMsg, 8);
				tx_len1 -= 8;
				CANTx_Buffer.read_index = (CANTx_Buffer.read_index + 8)%CANTx_Buffer.size;
			}
			else
			{
				temp = CANTx_Buffer.size - CANTx_Buffer.read_index;
				if(temp < tx_len1)
				{
					memcpy(TxMsg,&(CANTx_Buffer.data[CANTx_Buffer.read_index]), temp);
					memcpy(&TxMsg[temp],&(CANTx_Buffer.data[0]) ,tx_len1-temp);
				}
				else
				{
					memcpy(TxMsg,&(CANTx_Buffer.data[CANTx_Buffer.read_index]), tx_len1);
				}
				CanSend(CAN_SEND_ID, TxMsg, tx_len1);
				CANTx_Buffer.read_index = (CANTx_Buffer.read_index + tx_len1)%CANTx_Buffer.size;
        tx_len1 = 0;
			}
		}
	}
	
}



void CAN0_RX1_IRQHandler(void)
{
	uint8_t i = 0;

	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

	can_receive_message_struct receive_message;
	can_message_receive(CAN0, CAN_FIFO1, &receive_message);

	if(receive_message.rx_ff == CAN_FF_STANDARD) 
	{
    if(receive_message.rx_sfid == CAN_RECEIVE_ID)
		{
      for(i=0;i<receive_message.rx_dlen;i++)
		  {
				CANRx_Buffer.data[CANRx_Buffer.write_index] = receive_message.rx_data[i];	
				CANRx_Buffer.write_index = (CANRx_Buffer.write_index + 1) % CANRx_Buffer.size;
		  }	
		  xSemaphoreGiveFromISR(CANRecvFlag,&xHigherPriorityTaskWoken);
		}		

	}
}

int CANGetDataWithTimeOut(){
	if(xSemaphoreTake(CANRecvFlag,1000) == pdTRUE){
		can_receive();
		return 1;
	}
	return 0;
}



void can_receive(void)
{	
	uint16_t len = 0;

	len = can_serial_available();
	if(len > 0 && len < CAN_BUFFER_SIZE)
	{
		can_serial_read(Car_buff,len);
		/*!!!!!!!!!!!!!!*/
		api_port_received(len,Car_buff);
	}
}

void api_port_send(uint16_t len, uint8_t * data) 
{
	can_serial_write(data,len);
	can_transmit();		
}

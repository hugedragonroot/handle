#include "protocol.h"
#include "crc.h"
#include "main.h"

	
/**
 *@brief:      	protocol_received
 *@details:    	放在 api_on_message_received 接收函数中处理
 *@param[in]   	API_Message_t *message
				uint8_t is_device =1当前消息是对本设备进行数据寄存器操作
 *@param[out]  	void
 *@retval:     	void
 */

void protocol_received(API_Message_t *message, uint8_t is_device,MasterMsgData_t *req_Msg)
{
	if(is_device == 1)
	{
		//数据寄存器处理
		MsgData_t respond_msg_data;
		protocol_respond(&message->msg_data, &respond_msg_data);
		//回复
		API_Message_t msg;
		message_init(&msg, respond_msg_data.TYPE,respond_msg_data.Length,respond_msg_data.cmd, respond_msg_data.Data);
		api_message_send(&msg);
	}
	else if(is_device == 0)
	{
		req_Msg->TYPE = message->msg_data.TYPE;
		req_Msg->cmd= message->msg_data.cmd;
		req_Msg->msg_id = message->msg_data.Data[0];
		memcpy(&req_Msg->Length,&message->msg_data.Data[1],2);
		req_Msg->Data = &message->msg_data.Data[3];		
	}
}

/**
 *@brief:      	message_init
 *@details:    	API_Message_t 消息结构体填充
 *@param[in]   	uint32_t From_Port, uint32_t To_Port, uint32_t To, uint8_t Request_Id, uint8_t Respond_Id, 
				uint8_t cmd, uint16_t Length, uint8_t * Data
 *@param[out]  	API_Message_t *message
 *@retval:     	void
 */
void message_init(API_Message_t *message, uint16_t TYPE, uint16_t Length,uint8_t cmd, uint8_t * Data)
{
	message->msg_data.cmd =  cmd;
	message->msg_data.TYPE = TYPE;
	message->msg_data.Length = Length;
	message->msg_data.Data = Data;
}

/**
 *@brief:      	send_message_data
 *@details:    	填充MsgData_t *respond_msg_data到API_Message_t msg发送
 *@param[in]   	MsgData_t *respond_msg_data
 *@param[out]  	void
 *@retval:     	void
 */
void send_message_data(MsgData_t *respond_msg_data)
{
	API_Message_t msg;
	uint16_t TYPE = respond_msg_data->TYPE;
	message_init(&msg, NODE_SELF,respond_msg_data->Length,respond_msg_data->cmd, respond_msg_data->Data);
	api_message_send(&msg);
}

/**
 *@brief:      	list_mem
 *@details:    	内存使用情况打印
 *@param[in]   	void
 *@param[out]  	void
 *@retval:     	void
 */
void list_mem(void)
{
	rt_uint32_t total, used, max_used;
	xa_mem_info(&total, &used, &max_used);
	xa_printf("--------------------------------------------\r\n");
    xa_printf("total memory: %d\r\n", total);//总大小
    xa_printf("used memory : %d\r\n", used);//已使用大小
    xa_printf("maximum allocated memory: %d\r\n", max_used);//使用过的最大大小
	xa_printf("--------------------------------------------\r\n");
}





//  地址     	 类型       	 变量名                 描述           	 操作方式
//  ------	 --------	 --------------------	 ------------	 		----
//  0x0000	 uint32_t	 Firmware_version     	 固件版本号        	 		R    
//  0x0004	 uint32_t	 Hardware_version     	 硬件版本号        	 		R    
//  0x0008	 uint8_t  	 BLDC_A_ENABLE        	 左轮毂电机使能     		 R/W  
//  0x0009	 uint8_t  	 BLDC_A_MODE          	 左轮毂电机模式      		 R/W  
//  0x000A	 uint16_t	 BLDC_A_STATUS        	 左轮毂电机状态      	 	R    
//  0x000C	 int16_t  	 BLDC_A_SPEED_SET     	 左轮毂电机速度设置    		 R/W  
//  0x000E	 int16_t  	 BLDC_A_SPEED_REAL    	 左轮毂电机速度真实    		 R    
//  0x0010	 float    	 BLDC_A_CURRENT_REAL  	 左轮毂电机电流真实    		 R    
//  0x0014	 float    	 BLDC_A_CURRENT_OVER  	 左轮毂电机过流值     		 R/W  
//  0x0018	 float    	 BLDC_A_ANGEL_REAL    	 左轮毂电机角度值     		 R    
//  0x001C	 float    	 BLDC_A_TEMP          	 左轮毂电机温度值     		 R    
//	0x0020   int16_t	 HANDLE_ANGLE			 手柄摇杆角度
//	0x0022	 uint8_t	 HANDLE_GEAR			 手柄速度挡位
//	0x0023	 uint8_t	 HANDLE_X			 	 手柄X坐标
//	0x0024	 uint8_t	 HANDLE_Y			 	 手柄Y坐标
//  ....   	          	                      	              		      
//  0x0032	 uint8_t  	 BLDC_U_ENABLE        	 右轮毂电机使能      		 R/W  
//  0x0033	 uint8_t  	 BLDC_U_MODE          	 右轮毂电机模式      		 R/W  
//  0x0034	 uint16_t	 BLDC_U_STATUS        	 右轮毂电机状态      	 	R    
//  0x0036	 int16_t  	 BLDC_U_SPEED_SET     	 右轮毂电机速度设置    		 R/W  
//  0x0038	 int16_t  	 BLDC_U_SPEED_REAL    	 右轮毂电机速度真实    		 R    
//  0x003a	 float    	 BLDC_U_CURRENT_REAL  	 右轮毂电机电流真实    		 R    
//  0x003e	 float    	 BLDC_U_CURRENT_OVER  	 右轮毂电机过流值     		 R/W  
//  0x0042	 float    	 BLDC_U_ANGEL_REAL    	 右轮毂电机角度值     		 R    
//  0x0046	 float    	 BLDC_U_TEMP          	 右轮毂电机温度值     		 R    
//  ....   	          	                      	              		      
//  0x0064	 uint8_t  	 BRUSH_A_ENABLE       	 升降推杆使能       		 R/W  
//  0x0065	 uint8_t  	 BRUSH_A_MODE         	 升降推杆模式       		 R/W  
//  0x0066	 uint16_t	 BRUSH_A_STATUS       	 升降推杆状态       		 R    
//  0x0068	 int16_t  	 BRUSH_A_ANGEL_SET    	 升降推杆角度设定值   	 	 R/W  
//  0x006a	 int16_t  	 BRUSH_A_ANGEL_REAL   	 升降推杆角度真实值   	 	 R    
//  0x006c	 int16_t  	 BRUSH_A_SPEED_SET    	 升降推杆速度设定值   	 	 R/W  
//  0x006e	 int16_t  	 BRUSH_A_SPEED_REAL   	 升降推杆速度真实值   	 	 R    
//  0x0070	 int16_t  	 BRUSH_A_CURRENT_REAL	 升降推杆电流真实值    		 R    
//  0X0072	 uint16_t	 BRUSH_A_CURRENT_OVER	 升降推杆过流值      		 R/W  
//  .....  	          	                      	            	  	      
//  0x0096	 uint8_t  	 BRUSH_U_ENABLE       	 座椅推杆使能      	 	 	R/W  
//  0x0097	 uint8_t  	 BRUSH_U_MODE         	 座椅升降推杆模式    	 	 R/W  
//  0x0098	 uint16_t	 BRUSH_U_STATUS       	 座椅升降推杆状态     		 R    
//  0x009A	 int16_t  	 BRUSH_U_MPU_SET      	 座椅升降推杆陀螺仪设定值	 R/W  
//  0x009C	 int16_t  	 BRUSH_U_MPU_REAL     	 座椅升降推杆陀螺仪真实值	 R    
//  0x009E	 int16_t  	 BRUSH_U_ANGLE_SET    	 座椅升降推杆角度设定值  	 R/W  
//  0x00A0	 int16_t  	 BRUSH_U_ANGLE_REAL   	 座椅升降推杆角度真实值  	 R    
//  0x00A2	 int16_t  	 BRUSH_U_SPEED_SET    	 座椅升降推杆速度真实值  	 R/W  
//  0X00A4	 int16_t  	 BRUSH_U_SPEED_REAL   	 座椅升降推杆速度真实值  	 R    
//  0X00A6	 int16_t  	 BRUSH_U_CURRENT_REAL	 座椅升降推杆电流真实值  	 R    
//  0X00A8	 uint16_t	 BRUSH_U_CURRENT_OVER	 座椅升降推杆过流值    	 	R/W  
//  .....  	          	                      	              	      
//  0X00C8	 float    	 DEVICE_TEMP          	 驱动器温度        	 		R    
//  0X00CC	 float    	 DEVICE_VOLT          	 驱动器母线电压      	 	R    

//  0X00D0	 uint8_t    HEART_BIT          	  心跳     	 	R/w    

//  0X00D1	 uint16_t   ULTRASONIC_FIRST         	第一超声波     	 	R    
//  0X00D3	 uint16_t   ULTRASONIC_SECOND          第二超声波     	 	R    
//  0X00D5	 uint16_t   ULTRASONIC_THIRD          	第三超声波     	 	R    
//  0X00D7	 uint16_t   ULTRASONIC_FOUR          	第四超声波   	 	R    

//  0X00D9	 int16_t    PITCH_ANGLE         	  俯仰角     	 	R    
//  0X00DB	 int16_t    ROLL_ANGLE          	  横滚角     	 	R    
//	0X00DD	 uint8_t   APP_ERROR_REG          	  返回错误寄存器     	 	



#define ADDREG(addr,size) {\
	(addr),\
	sizeof(size)\
}


const RegInfo tranRegInfoDir[] = {
	ADDREG(0x0000,uint32_t ),//Firmware_version     	 固件版本号        
	ADDREG(0x0004,uint32_t ),//Hardware_version     	 硬件版本号        
	ADDREG(0x0008,uint8_t  ),// BLDC_A_ENABLE        	 左轮毂电机使能     
	ADDREG(0x0009,uint8_t  ),// BLDC_A_MODE          	 左轮毂电机模式     
	ADDREG(0x000A,uint16_t ),//BLDC_A_STATUS        	 左轮毂电机状态      
	ADDREG(0x000C,int16_t  ),// BLDC_A_SPEED_SET     	 左轮毂电机速度设置   
	ADDREG(0x000E,int16_t  ),// BLDC_A_SPEED_REAL    	 左轮毂电机速度真实   
	ADDREG(0x0010,float    ),// BLDC_A_CURRENT_REAL  	 左轮毂电机电流真实   
	ADDREG(0x0014,float    ),// BLDC_A_CURRENT_OVER  	 左轮毂电机过流值    
	ADDREG(0x0018,float    ),// BLDC_A_ANGEL_REAL    	 左轮毂电机角度值    
	ADDREG(0x001C,float    ),// BLDC_A_TEMP          	 左轮毂电机温度值    
	ADDREG(0x0020,int16_t  ),//ANDLE_ANGLE			 	 手柄摇杆角度
	ADDREG(0x0022,uint8_t  ),//HANDLE_GEAR			 	 手柄速度挡位
	ADDREG(0x0023,uint8_t  ),//HANDLE_X			 	 	 手柄X坐标
	ADDREG(0x0024,uint8_t  ),//HANDLE_Y			 	 	 手柄Y坐标
	// ADDREG(....  ,         ),
	ADDREG(0x0032,uint8_t  ),// BLDC_U_ENABLE        	 右轮毂电机使能     
	ADDREG(0x0033,uint8_t  ),// BLDC_U_MODE          	 右轮毂电机模式     
	ADDREG(0x0034,uint16_t ),//BLDC_U_STATUS        	 右轮毂电机状态      
	ADDREG(0x0036,int16_t  ),// BLDC_U_SPEED_SET     	 右轮毂电机速度设置   
	ADDREG(0x0038,int16_t  ),// BLDC_U_SPEED_REAL    	 右轮毂电机速度真实   
	ADDREG(0x003a,float    ),// BLDC_U_CURRENT_REAL  	 右轮毂电机电流真实   
	ADDREG(0x003e,float    ),// BLDC_U_CURRENT_OVER  	 右轮毂电机过流值    
	ADDREG(0x0042,float    ),// BLDC_U_ANGEL_REAL    	 右轮毂电机角度值    
	ADDREG(0x0046,float    ),// BLDC_U_TEMP          	 右轮毂电机温度值    
	// ADDREG(....  ,        //	                      	             ),
	ADDREG(0x0064,uint8_t  ),// BRUSH_A_ENABLE       	 升降推杆使能      
	ADDREG(0x0065,uint8_t  ),// BRUSH_A_MODE         	 升降推杆模式      
	ADDREG(0x0066,uint16_t ),//BRUSH_A_STATUS       	 升降推杆状态       
	ADDREG(0x0068,int16_t  ),// BRUSH_A_ANGEL_SET    	 升降推杆角度设定值   
	ADDREG(0x006a,int16_t  ),// BRUSH_A_ANGEL_REAL   	 升降推杆角度真实值   
	ADDREG(0x006c,int16_t  ),// BRUSH_A_SPEED_SET    	 升降推杆速度设定值   
	ADDREG(0x006e,int16_t  ),// BRUSH_A_SPEED_REAL   	 升降推杆速度真实值   
	ADDREG(0x0070,int16_t  ),// BRUSH_A_CURRENT_REAL	 升降推杆电流真实值    
	ADDREG(0X0072,uint16_t ),//BRUSH_A_CURRENT_OVER	 	 升降推杆过流值      	
	// ADDREG(..... ,        //	                      	             ),
	ADDREG(0x0096,uint8_t  ),// BRUSH_U_ENABLE       	 座椅推杆使能      
	ADDREG(0x0097,uint8_t  ),// BRUSH_U_MODE         	 座椅升降推杆模式    
	ADDREG(0x0098,uint16_t ),//BRUSH_U_STATUS       	 座椅升降推杆状态     
	ADDREG(0x009A,int16_t  ),// BRUSH_U_MPU_SET      	 座椅升降推杆陀螺仪设定值
	ADDREG(0x009C,int16_t  ),// BRUSH_U_MPU_REAL     	 座椅升降推杆陀螺仪真实值
	ADDREG(0x009E,int16_t  ),// BRUSH_U_ANGLE_SET    	 座椅升降推杆角度设定值 
	ADDREG(0x00A0,int16_t  ),// BRUSH_U_ANGLE_REAL   	 座椅升降推杆角度真实值 
	ADDREG(0x00A2,int16_t  ),// BRUSH_U_SPEED_SET    	 座椅升降推杆速度真实值 
	ADDREG(0X00A4,int16_t  ),// BRUSH_U_SPEED_REAL   	 座椅升降推杆速度真实值 
	ADDREG(0X00A6,int16_t  ),// BRUSH_U_CURRENT_REAL	 座椅升降推杆电流真实值  
	ADDREG(0X00A8,uint16_t ),//BRUSH_U_CURRENT_OVER	 座椅升降推杆过流值    	
	// ADDREG(..... ,        //	                      	             ),
	ADDREG(0X00C8,float    ),// DEVICE_TEMP          	 驱动器温度       
	ADDREG(0X00CC,float    ),// DEVICE_VOLT          	 驱动器母线电压     
	//新增                     
	ADDREG(0X00D0,uint8_t  ),// HEART_BIT          	  心跳     	 	R/w
#if 1                       	
	ADDREG(0X00D1,uint16_t  ), // PCB_ANGLE         	//修改为板载角度                       ,
	ADDREG(0X00D3,uint16_t  ), // ULTRASONIC_SECOND          第二超声波
	ADDREG(0X00D5,uint16_t  ), // ULTRASONIC_THIRD          	第三超声波
	ADDREG(0X00D7,uint16_t  ), // ULTRASONIC_FOUR          	第四超声波 
#else                     
	ADDREG(0X00D1,Remote_receive_para.ultrasonic),
#endif
	ADDREG(0X00D9,int16_t   ),//PITCH_ANGLE         	  俯仰角     	 	
	ADDREG(0X00DB,int16_t   ),//ROLL_ANGLE          	  横滚角     	 	

	ADDREG(0X00DD,uint8_t   ),//APP_ERROR_REG          	  返回错误寄存器     	 	

};

#define ADD_WRITE_REG(para,regName){\
	uint8_t count = 0;\
	do {\
		(txbuffer)[txbufferIndex++] = (((para) >> (8*count))& 0xff);\
	} while (++count < sizeof((para)));\
	combine_wirte.combine_data[combine_wirte.combineCount].combine.address = \
	tranRegInfoDir[(regName)].addr;\
	combine_wirte.combine_data[combine_wirte.combineCount].combine.size = \
	tranRegInfoDir[(regName)].size;\
	combine_wirte.combine_data[combine_wirte.combineCount].data = \
	&txbuffer[txbufferIndex-tranRegInfoDir[(regName)].size];\
	++combine_wirte.combineCount;\
	}

//发送协议初始化
void paraInit(){
	combine_write_t combine_wirte;
	PDU_t req_PDU;
	MsgData_t request_msg_data;
	uint8_t txbuffer[32];
	uint8_t txbufferIndex = 0;
	combine_wirte.combineCount = 0;

	ADD_WRITE_REG((uint8_t)0x01,HEART_BIT);
	#if USING_XY_TO_SPEED
	ADD_WRITE_REG((int16_t)0x0000,BLDC_A_SPEED_SET);
	ADD_WRITE_REG((int16_t)0x0000,BLDC_U_SPEED_SET);
	#else
	ADD_WRITE_REG((uint8_t)Coord_Base,HANDLE_X);
	ADD_WRITE_REG((uint8_t)Coord_Base,HANDLE_Y);
	#endif
	ADD_WRITE_REG((uint8_t)0x01,BRUSH_A_ENABLE);
	ADD_WRITE_REG((uint8_t)0x06,BRUSH_A_MODE);
	ADD_WRITE_REG((int16_t)0x0000,BRUSH_A_SPEED_SET);
	ADD_WRITE_REG((uint8_t)0x00,BRUSH_U_ENABLE);
	// ADD_WRITE_REG((uint8_t)0x06,BRUSH_U_MODE);


	combine_write_to_PDU('I',&combine_wirte,&req_PDU);
	PDU_to_msg_data(NODE_SELF, &req_PDU, &request_msg_data);
	send_message_data(&request_msg_data);
	
}


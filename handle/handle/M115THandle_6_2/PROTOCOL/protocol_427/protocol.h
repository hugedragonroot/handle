#ifndef __PROTOCOL_H_
#define __PROTOCOL_H_
#include "xsto_api_ii.h"
#include "protocol_api.h"
#include "alloc.h"
#include "klist.h"
#include "protocol_register_management.h"


//内存使用打印
#define xa_printf printf
#ifndef xa_printf
#define xa_printf(...)
#endif

/**
 *@brief:      	node_on_router
 *@details:    	判断 node 是否是router上的
 *@param[in]   	uint32_t node     
 *@param[out]  	void
 *@retval:     	uint8_t 1=是router上，0=不是
 */
uint8_t node_on_router(uint32_t node);

/**
 *@brief:      	node_on_router
 *@details:    	放在 api_on_message_received 接收函数中处理
 *@param[in]   	API_Message_t *message
				uint8_t is_device =1当前消息是对本设备进行数据寄存器操作
 *@param[out]  	void
 *@retval:     	void
 */
void protocol_received(API_Message_t *message, uint8_t is_device,MasterMsgData_t *req_Msg);

/**
 *@brief:      	message_init
 *@details:    	API_Message_t 消息结构体填充
 *@param[in]   	uint32_t From_Port, uint32_t To_Port, uint32_t To, uint8_t Request_Id, uint8_t Respond_Id, 
				uint8_t cmd, uint16_t Length, uint8_t * Data
 *@param[out]  	API_Message_t *message
 *@retval:     	void
 */
void message_init(API_Message_t *message, uint16_t TYPE, uint16_t Length,uint8_t cmd, uint8_t * Data);
/**
 *@brief:      	send_message_data
 *@details:    	填充MsgData_t *respond_msg_data到API_Message_t msg发送
 *@param[in]   	MsgData_t *respond_msg_data
 *@param[out]  	void
 *@retval:     	void
 */
void send_message_data(MsgData_t *respond_msg_data);

/**
 *@brief:      	list_mem
 *@details:    	内存使用情况打印
 *@param[in]   	void
 *@param[out]  	void
 *@retval:     	void
 */
void list_mem(void);
void map_init(void);
void protocol_init(void);

void paraInit(void);

typedef struct {
	uint16_t addr;
	uint8_t size;
}RegInfo;

extern const RegInfo tranRegInfoDir[];


typedef enum {
	Firmware_version    ,// 固件版本号        		R   
	Hardware_version    ,// 硬件版本号        		R   
	BLDC_A_ENABLE       ,// 左轮毂电机使能     	  	R/W  
	BLDC_A_MODE         ,// 左轮毂电机模式       	R/W 
	BLDC_A_STATUS       ,// 左轮毂电机状态     		R    
	BLDC_A_SPEED_SET    ,// 左轮毂电机速度设置     	R/W 
	BLDC_A_SPEED_REAL   ,// 左轮毂电机速度真实     	R   
	BLDC_A_CURRENT_REAL ,// 左轮毂电机电流真实     	R   
	BLDC_A_CURRENT_OVER ,// 左轮毂电机过流值      	R/W 
	BLDC_A_ANGEL_REAL   ,// 左轮毂电机角度值      	R   
	BLDC_A_TEMP         ,// 左轮毂电机温度值      	R   
						// ,            // 	             		    
	BLDC_U_ENABLE       ,// 右轮毂电机使能       	 R/W 
	BLDC_U_MODE         ,// 右轮毂电机模式       	 R/W 
	BLDC_U_STATUS       ,// 右轮毂电机状态      	R    
	BLDC_U_SPEED_SET    ,// 右轮毂电机速度设置     	 R/W 
	BLDC_U_SPEED_REAL   ,// 右轮毂电机速度真实     	 R   
	BLDC_U_CURRENT_REAL ,// 右轮毂电机电流真实     	 R   
	BLDC_U_CURRENT_OVER ,// 右轮毂电机过流值      	 R/W 
	BLDC_U_ANGEL_REAL   ,// 右轮毂电机角度值      	 R   
	BLDC_U_TEMP         ,// 右轮毂电机温度值      	 R   
						// ,            // 	             		    
	BRUSH_A_ENABLE      ,// 升降推杆使能        	 R/W 
	BRUSH_A_MODE        ,// 升降推杆模式        	 R/W 
	BRUSH_A_STATUS      ,// 升降推杆状态       	 	 R    
	BRUSH_A_ANGEL_SET   ,// 升降推杆角度设定值   	  R/W 
	BRUSH_A_ANGEL_REAL  ,// 升降推杆角度真实值   	  R   
	BRUSH_A_SPEED_SET   ,// 升降推杆速度设定值   	  R/W 
	BRUSH_A_SPEED_REAL  ,// 升降推杆速度真实值   	  R   
	BRUSH_A_CURRENT_REAL,// 升降推杆电流真实值    	 R    
	BRUSH_A_CURRENT_OVER,// 降推杆过流值      		R/W  
						// ,            // 	            	 	    
	BRUSH_U_ENABLE      ,// 座椅推杆使能      	  	R/W
	BRUSH_U_MODE        ,// 座椅升降推杆模式    	 R/W 
	BRUSH_U_STATUS      ,// 座椅升降推杆状态     	 R    
	BRUSH_U_MPU_SET     ,// 座椅升降推杆陀螺仪设定值	 R/W  
	BRUSH_U_MPU_REAL    ,// 座椅升降推杆陀螺仪真实值	 R    
	BRUSH_U_ANGLE_SET   ,// 座椅升降推杆角度设定值    R/W  
	BRUSH_U_ANGLE_REAL  ,// 座椅升降推杆角度真实值    R    
	BRUSH_U_SPEED_SET   ,// 座椅升降推杆速度真实值    R/W  
	BRUSH_U_SPEED_REAL  ,// 座椅升降推杆速度真实值    R    
	BRUSH_U_CURRENT_REAL,// 座椅升降推杆电流真实值  	R    
	BRUSH_U_CURRENT_OVER,// 椅升降推杆过流值    	 R/W  
						// ,            // 	             	     
	DEVICE_TEMP         ,// 驱动器温度         		R  
	DEVICE_VOLT         ,// 驱动器母线电压        	R   

	HEART_BIT

}REG_NAME;

#endif


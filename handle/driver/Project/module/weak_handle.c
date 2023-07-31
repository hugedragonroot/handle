/**********************************************************/
//@Autor:Raw
//@Version:0.0.1
//@End Time:2023/4/28
//@Function:weak_handle.c
/**********************************************************/
#include "weak_handle.h"
#include "can.h"
#include "protocol.h"
#include "xsto_api_ii.h"
#include "PMSM_Driver.h"
#include "HALL_Encoder.h"
#include "Brush_Driver.h"
#include "adc_callback.h"
#include "usart.h"
#include "Status_Check.h"
//#include "QMI8658C.h"
#include "icm42688.h"
bool send_flage = false;
API_Config_t  API_Config;
API_Message_t API_Message;
uint8_t msg_buff[128]={0};
bool message_online_flage = false;

static uint32_t firmwareV=SOFTWAREVERSION;
static uint32_t hardwreV=HARDWAREVERSION;

void test_protocol(void)
{
	PDU_t req_PDU;
	MsgData_t request_msg_data;
	combine_write_t combine_write;
	float buf = 1000;
	float buf2 = 2000;
	combine_write.combineCount = 2;	//写以下两个组合
	combine_write.combine_data[0].combine.address = 0x001a;
	combine_write.combine_data[0].combine.size = 4;
	combine_write.combine_data[0].data = (uint8_t*)&buf;
	combine_write.combine_data[1].combine.address = 0x0016;
	combine_write.combine_data[1].combine.size = 4;
	combine_write.combine_data[1].data = (uint8_t*)&buf2;
	combine_write_to_PDU(0, &combine_write, &req_PDU);
	PDU_to_msg_data(0x0a01, &req_PDU, &request_msg_data);
	
	
//	req_PDU.funC = FUNC_SUBSCRIBE;
//	req_PDU.msgID = 0;
//	req_PDU.length = 3+1*4;
//	//uint16_t timing 
//	req_PDU.data[0] = 0x64;
//	req_PDU.data[1] = 0x00;
//	//uint8_t combineCount
//	req_PDU.data[2] = 0x01;
//	//uint16_t address;	uint16_t size;
//	req_PDU.data[3] = 0x00;
//	req_PDU.data[4] = 0x00;
//	req_PDU.data[5] = 0x04;
//	req_PDU.data[6] = 0x00;
//	
//	PDU_to_msg_data(0x0a01, &req_PDU, &request_msg_data);
	if(send_flage == true)
	{
		send_flage = false;
		send_message_data(&request_msg_data);
	}
}

int16_t xpitch_temp = 0;
int16_t xroll_temp = 0;
void map_init(void)
{
	register_map(0x0000, (uint8_t *)&firmwareV, 4);
	register_map(0x0004, (uint8_t *)&hardwreV, 4);
	
	register_map(0x0008, (uint8_t *)&PMSM[PMSM_A].MotorPreState, 1);
	register_map(0x0009, (uint8_t *)&PMSM[PMSM_A].ControlMode, 1);
	register_map(0x000A, (uint8_t *)&PMSM[PMSM_A].PMSM_Status, 2);
	register_map(0x000C, (uint8_t *)&PMSM[PMSM_A].SpeedNew, 2);
	register_map(0x000E, (uint8_t *)&hall_encoder[PMSM_A].return_speed, 2);
	register_map(0x0010, (uint8_t *)&PMSM[PMSM_A].foc.Id_Iq.I2, 4);
	register_map(0x0014, (uint8_t *)&PMSM[PMSM_A].Current_over, 4);
	register_map(0x0018, (uint8_t *)&hall_encoder[PMSM_A].AngleReal, 4);
	register_map(0x001C, (uint8_t *)&PMSM[PMSM_A].Mostemp, 4);
	register_map(0x0020, (uint8_t *)&APP_PMSM.angle, 2);
	register_map(0x0022, (uint8_t *)&APP_PMSM.Gear, 1);
	register_map(0x0023, (uint8_t *)&APP_PMSM.axis_x, 1);
	register_map(0x0024, (uint8_t *)&APP_PMSM.axis_y, 1);
	
	register_map(0x0032, (uint8_t *)&PMSM[PMSM_U].MotorPreState, 1);
	register_map(0x0033, (uint8_t *)&PMSM[PMSM_U].ControlMode, 1);
	register_map(0x0034, (uint8_t *)&PMSM[PMSM_U].PMSM_Status, 2);
	register_map(0x0036, (uint8_t *)&PMSM[PMSM_U].SpeedNew, 2);
	register_map(0x0038, (uint8_t *)&hall_encoder[PMSM_U].return_speed, 2);
	register_map(0x003A, (uint8_t *)&PMSM[PMSM_U].foc.Id_Iq.I2, 4);
	register_map(0x003E, (uint8_t *)&PMSM[PMSM_U].Current_over, 4);
	register_map(0x0042, (uint8_t *)&hall_encoder[PMSM_U].AngleReal, 4);
	register_map(0x0046, (uint8_t *)&PMSM[PMSM_U].Mostemp, 4);
	
	register_map(0x0064, (uint8_t *)&Brush[Brush_A].brush_Cmd_Pre, 1);
	register_map(0x0065, (uint8_t *)&Brush[Brush_A].Control_mode, 1);
	register_map(0x0066, (uint8_t *)&Brush[Brush_A].brush_state, 2);
	register_map(0x0068, (uint8_t *)&Brush[Brush_A].AngelNew, 2);
	register_map(0x006a, (uint8_t *)&Brush[Brush_A].AngelReal, 2);
	register_map(0x006c, (uint8_t *)&Brush[Brush_A].SpeedlNew, 2);
	register_map(0x006e, (uint8_t *)&Brush[Brush_A].return_speed, 2);
	register_map(0x0070, (uint8_t *)&Brush[Brush_A].return_current, 2);
	register_map(0x0072, (uint8_t *)&Brush[Brush_A].Total_Current, 2);

	register_map(0x0096, (uint8_t *)&Brush[Brush_U].brush_Cmd_Pre, 1);
	register_map(0x0097, (uint8_t *)&Brush[Brush_U].Control_mode, 1);
	register_map(0x0098, (uint8_t *)&Brush[Brush_U].brush_state, 2);	
	register_map(0x009a, (uint8_t *)&Brush[Brush_U].MPU_New, 2);
	register_map(0x009C, (uint8_t *)&Brush[Brush_U].return_mpu, 2);
	register_map(0x009E, (uint8_t *)&Brush[Brush_U].AngelNew, 2);
	register_map(0x00A0, (uint8_t *)&Brush[Brush_U].AngelReal, 2);
	register_map(0x00A2, (uint8_t *)&Brush[Brush_U].SpeedlNew, 2);
	register_map(0x00A4, (uint8_t *)&Brush[Brush_U].return_speed, 2);
	register_map(0x00A6, (uint8_t *)&Brush[Brush_U].return_current, 2);
	register_map(0x00A8, (uint8_t *)&Brush[Brush_U].Total_Current, 2);
	
	
	register_map(0x00C8, (uint8_t *)&ADC_Data.mos_temp,4);
	register_map(0x00CC, (uint8_t *)&ADC_Data.Bus_Volt,4);
	register_map(0x00D0, (uint8_t *)&message_online_flage,1);
	register_map(0x00D1, (uint8_t *)&imu_mpu.local_pitch,2);
	register_map(0x00D3, (uint8_t *)&app_ultrasonic.length_second,2);
	register_map(0x00D5, (uint8_t *)&app_ultrasonic.length_third,2);
	register_map(0x00D7, (uint8_t *)&app_ultrasonic.length_four,2);

//	register_map(0x00D9, (uint8_t *)&imu_mpu.pitch_return ,2);
//	register_map(0x00DB, (uint8_t *)&imu_mpu.roll_return,2);
	register_map(0x00D9, (uint8_t *)&xpitch_temp ,2);
	register_map(0x00DB, (uint8_t *)&xroll_temp,2);
	register_map(0x00DD, (uint8_t *)&APP_PMSM.APP_ERROR,1);
	register_map(0x00DE, (uint8_t *)&imu_mpu.abs_angel,2);
//	register_map(0x00DD, (uint8_t *)&APP_PMSM.APP_ERROR,1);
//

	
	
	

//	register_map(0x0098, (uint8_t *)&Brush[Brush_U].brush_state, 2);	
//	register_map(0x009a, (uint8_t *)&Brush[Brush_U].MPU_New, 4);
//	register_map(0x009E, (uint8_t *)&Brush[Brush_U].MPU_REAL, 4);
	
	
	
	

}
/**
 *@brief:      	protocol_init
 *@details:    	协议初始化
 *@param[in]   	void
 *@param[out]  	void
 *@retval:     	void
 */
void protocol_init(void)
{
	
	map_init();
	xa_mem_init();
	API_Config.ModuleID[0] = 0x01;
	API_Config.ModuleID[1] = 0x02;
	API_Config.ModuleID[2] = 0x03;
	
	API_Config.Type = NODE_SELF;
	API_Config.Status = MODULE_STATUS_OFFLINE;
	API_Config.HardwareVersion = HARDWAREVERSION;
	API_Config.SoftwareVersion = SOFTWAREVERSION;
	API_Config.ModuleTimeout = 1000;
	
	api_init(&API_Config);
	API_Message.msg_data.Data = msg_buff;
}




void can_receive(void)
{	
	uint16_t len = 0;

	len = can_serial_available();
	if(len > 0 && len < CAN_BUFFER_SIZE)
	{
		can_serial_read(Car_buff,len);
		api_port_received(len, Car_buff);
	}
}

void api_port_send(uint16_t len, uint8_t * data) 
{
	can_serial_write(data,len);
}
//MasterMsgData_t rxmessage;
//void api_on_message_received(MasterMsgData_t message)
//{
//	memcpy(&rxmessage,&message,sizeof(MasterMsgData_t));
//	
//}

















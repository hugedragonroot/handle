#include "icm42688.h"
#include "common.h"
#include "bsp_i2c.h"
static float accSensitivity   = 0.244f;   //加速度的最小分辨率 mg/LSB
static float gyroSensitivity  = 0.000532f;    //陀螺仪的最小分辨率

icm42688RawData_t stAccData;
icm42688RawData_t stGyroData;
tQmi_type tQmi;

uint8_t ICM42688_Write_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
  return Soft_I2C_Write(addr, reg, len, buf);
}

uint8_t icm42688_write_reg(uint8_t reg,uint8_t byte)
{
	return ICM42688_Write_Len(0x69, reg, 1, &byte);
}

uint8_t ICM42688_Read_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
	return Soft_I2C_Read(addr,reg,len,buf);
}

uint8_t icm42688_read_reg(uint8_t reg)
{
	uint8_t temp = 0;
	ICM42688_Read_Len(0x69, reg, 1, &temp);
	return temp;
}

void icm42688_read_regs(uint8_t reg, uint8_t* buf, uint16_t len)
{
	ICM42688_Read_Len(0x69, reg, len, buf);
}


float bsp_Icm42688GetAres(uint8_t Ascale)
{
    switch(Ascale)
    {
    // Possible accelerometer scales (and their register bit settings) are:
    // 2 Gs (11), 4 Gs (10), 8 Gs (01), and 16 Gs  (00).
    case AFS_2G:
        accSensitivity = 2000 / 32768.0f;
        break;
    case AFS_4G:
        accSensitivity = 4000 / 32768.0f;
        break;
    case AFS_8G:
        accSensitivity = 8000 / 32768.0f;
        break;
    case AFS_16G:
        accSensitivity = 16000 / 32768.0f;
        break;
    }

    return accSensitivity;
}

float bsp_Icm42688GetGres(uint8_t Gscale)
{
    switch(Gscale)
    {
    case GFS_15_125DPS:
        gyroSensitivity = 15.125f / 32768.0f/57.295f;
        break;
    case GFS_31_25DPS:
        gyroSensitivity = 31.25f / 32768.0f/57.295f;
        break;
    case GFS_62_5DPS:
        gyroSensitivity = 62.5f / 32768.0f/57.295f;
        break;
    case GFS_125DPS:
        gyroSensitivity = 125.0f / 32768.0f/57.295f;
        break;
    case GFS_250DPS:
        gyroSensitivity = 250.0f / 32768.0f/57.295f;
        break;
    case GFS_500DPS:
        gyroSensitivity = 500.0f / 32768.0f/57.295f;
        break;
    case GFS_1000DPS:
        gyroSensitivity = 1000.0f / 32768.0f/57.295f;
        break;
    case GFS_2000DPS:
        gyroSensitivity = 2000.0f / 32768.0f/57.295f;
        break;
    }
    return gyroSensitivity;
}


/*******************************************************************************
* 名    称： Icm42688Init
* 功    能： Icm42688 传感器初始化
* 入口参数： 无
* 出口参数： 0: 初始化成功  其他值: 初始化失败
* 创建日期： 2022-07-25
* 修    改：
* 修改日期：
* 备    注：
*******************************************************************************/
void Icm42688Init(void)
{
	
		tQmi.bais_pitch = 128.0f;
    I2C_Bus_Init();
	
	  uint8_t reg_val = 0;
    /* 读取 who am i 寄存器 */
    reg_val = icm42688_read_reg(ICM42688_WHO_AM_I);
    icm42688_write_reg(ICM42688_REG_BANK_SEL, 0); //设置bank 0区域寄存器
    icm42688_write_reg(ICM42688_REG_BANK_SEL, 0x01); //软复位传感器
    delay_1ms(100);

    if(reg_val == ICM42688_ID)
    {
        icm42688_write_reg(ICM42688_REG_BANK_SEL, 1); //设置bank 1区域寄存器
//        icm42688_write_reg(ICM42688_INTF_CONFIG4, 0x02); //设置为4线SPI通信
			
        icm42688_write_reg(ICM42688_REG_BANK_SEL, 0); //设置bank 0区域寄存器
        icm42688_write_reg(ICM42688_FIFO_CONFIG, 0x40); //Stream-to-FIFO Mode(page63)

        reg_val = icm42688_read_reg(ICM42688_INT_SOURCE0);
        icm42688_write_reg(ICM42688_INT_SOURCE0, 0x00);
        icm42688_write_reg(ICM42688_FIFO_CONFIG2, 0x00); // watermark
        icm42688_write_reg(ICM42688_FIFO_CONFIG3, 0x02); // watermark
        icm42688_write_reg(ICM42688_INT_SOURCE0, reg_val);
        icm42688_write_reg(ICM42688_FIFO_CONFIG1, 0x63); // Enable the accel and gyro to the FIFO

        icm42688_write_reg(ICM42688_REG_BANK_SEL, 0x00);
        icm42688_write_reg(ICM42688_INT_CONFIG, 0x36);

        icm42688_write_reg(ICM42688_REG_BANK_SEL, 0x00);
        reg_val = icm42688_read_reg(ICM42688_INT_SOURCE0);
        reg_val |= (1 << 2); //FIFO_THS_INT1_ENABLE
        icm42688_write_reg(ICM42688_INT_SOURCE0, reg_val);

        bsp_Icm42688GetAres(AFS_8G);
        icm42688_write_reg(ICM42688_REG_BANK_SEL, 0x00);
        reg_val = icm42688_read_reg(ICM42688_ACCEL_CONFIG0);//page74
        reg_val |= (AFS_8G << 5);   //量程 ±8g
        reg_val |= (AODR_1000Hz);     //输出速率 1000HZ
        icm42688_write_reg(ICM42688_ACCEL_CONFIG0, reg_val);

        bsp_Icm42688GetGres(GFS_1000DPS);
        icm42688_write_reg(ICM42688_REG_BANK_SEL, 0x00);
        reg_val = icm42688_read_reg(ICM42688_GYRO_CONFIG0);//page73
        reg_val |= (GFS_1000DPS << 5);   //量程 ±1000dps
        reg_val |= (GODR_1000Hz);     //输出速率 1000HZ
        icm42688_write_reg(ICM42688_GYRO_CONFIG0, reg_val);

        icm42688_write_reg(ICM42688_REG_BANK_SEL, 0x00);
        reg_val = icm42688_read_reg(ICM42688_PWR_MGMT0); //读取PWR—MGMT0当前寄存器的值(page72)
        reg_val &= ~(1 << 5);//使能温度测量
        reg_val |= ((3) << 2);//设置GYRO_MODE  0:关闭 1:待机 2:预留 3:低噪声
        reg_val |= (3);//设置ACCEL_MODE 0:关闭 1:关闭 2:低功耗 3:低噪声
        icm42688_write_reg(ICM42688_PWR_MGMT0, reg_val);
        delay_1ms(1); //操作完PWR—MGMT0寄存器后 200us内不能有任何读写寄存器的操作
			}
}

/*******************************************************************************
* 名    称： bsp_IcmGetTemperature
* 功    能： 读取Icm42688 内部传感器温度
* 入口参数： 无
* 出口参数： 无
* 创建日期： 2022-07-25
* 修    改：
* 修改日期：
* 备    注： datasheet page62
*******************************************************************************/
int8_t bsp_IcmGetTemperature(int16_t* pTemp)
{
    uint8_t buffer[2] = {0};

    icm42688_read_regs(ICM42688_TEMP_DATA1, buffer, 2);

    *pTemp = (int16_t)(((int16_t)((buffer[0] << 8) | buffer[1])) / 132.48 + 25);
    return 0;
}

/*******************************************************************************
* 名    称： bsp_IcmGetAccelerometer
* 功    能： 读取Icm42688 加速度的值
* 入口参数： 三轴加速度的值
* 出口参数： 无
* 创建日期： 2022-07-25
* 修    改：
* 修改日期：
* 备    注： datasheet page62
*******************************************************************************/
int8_t bsp_IcmGetAccelerometer(icm42688RawData_t* accData)
{
    uint8_t buffer[6] = {0};

    icm42688_read_regs(ICM42688_ACCEL_DATA_X1, buffer, 6);

    accData->x = ((uint16_t)buffer[0] << 8) | buffer[1];
    accData->y = ((uint16_t)buffer[2] << 8) | buffer[3];
    accData->z = ((uint16_t)buffer[4] << 8) | buffer[5];

    accData->x = (int16_t)(accData->x * accSensitivity);
    accData->y = (int16_t)(accData->y * accSensitivity);
    accData->z = (int16_t)(accData->z * accSensitivity);

    return 0;
}

/*******************************************************************************
* 名    称： IcmGetGyroscope
* 功    能： 读取Icm42688 陀螺仪的值
* 入口参数： 三轴陀螺仪的值
* 出口参数： 无
* 创建日期： 2022-07-25
* 修    改：
* 修改日期：
* 备    注： datasheet page63
*******************************************************************************/
int8_t IcmGetGyroscope(icm42688RawData_t* GyroData)
{
    uint8_t buffer[6] = {0};

    icm42688_read_regs(ICM42688_GYRO_DATA_X1, buffer, 6);

    GyroData->x = ((uint16_t)buffer[0] << 8) | buffer[1];
    GyroData->y = ((uint16_t)buffer[2] << 8) | buffer[3];
    GyroData->z = ((uint16_t)buffer[4] << 8) | buffer[5];

    GyroData->x = (int16_t)(GyroData->x * gyroSensitivity);
    GyroData->y = (int16_t)(GyroData->y * gyroSensitivity);
    GyroData->z = (int16_t)(GyroData->z * gyroSensitivity);
    return 0;
}

/*******************************************************************************
* 名    称： IcmGetRawData
* 功    能： 读取Icm42688加速度陀螺仪数据
* 入口参数： 六轴
* 出口参数： 无
* 创建日期： 2022-07-25
* 修    改：
* 修改日期：
* 备    注： datasheet page62,63
*******************************************************************************/
//float pitch_data;
//float roll_data;
//float yaw_data;


float FirstOrderFilter_pitch(float accel_m, float gyro_m)
{
	static float Angle_one,Angle_L1;	
	Angle_L1 = Angle_one;
	Angle_one = 0.04f*accel_m + (1.0f-0.04f)*(Angle_L1 + gyro_m*0.001f);
	return Angle_one;
}


float FirstOrderFilter_yaw(float accel_m, float gyro_m)
{
	static float Angle_one,Angle_L1;	
	Angle_L1 = Angle_one;
	Angle_one = 0.04f*accel_m + (1.0f-0.04f)*(Angle_L1 + gyro_m*0.001f);
	return Angle_one;
}


float FirstOrderFilter_roll(float accel_m, float gyro_m)
{
	static float Angle_one,Angle_L1;	
	Angle_L1 = Angle_one;
	Angle_one = 0.04f*accel_m + (1.0f-0.04f)*(Angle_L1 + gyro_m*0.001f);
	return Angle_one;
}

void IcmGetRawData(icm42688RawData_t* accData, icm42688RawData_t* GyroData)
{
    uint8_t buffer[12] = {0};
		float Angle[3]={0};
    float GyroAccSpeed[3]={0};

    icm42688_read_regs(ICM42688_ACCEL_DATA_X1, buffer, 12);

    accData->x  = ((uint16_t)buffer[0] << 8)  | buffer[1];
    accData->y  = ((uint16_t)buffer[2] << 8)  | buffer[3];
    accData->z  = ((uint16_t)buffer[4] << 8)  | buffer[5];
    GyroData->x = ((uint16_t)buffer[6] << 8)  | buffer[7];
    GyroData->y = ((uint16_t)buffer[8] << 8)  | buffer[9];
    GyroData->z = ((uint16_t)buffer[10] << 8) | buffer[11];


    accData->x = (int16_t)(accData->x * accSensitivity);
    accData->y = (int16_t)(accData->y * accSensitivity);
    accData->z = (int16_t)(accData->z * accSensitivity);

    GyroData->x = (int16_t)(GyroData->x * gyroSensitivity);
    GyroData->y = (int16_t)(GyroData->y * gyroSensitivity);
    GyroData->z = (int16_t)(GyroData->z * gyroSensitivity);
		
		GyroAccSpeed[0] = GyroData->x;
		GyroAccSpeed[1] = GyroData->y;
		GyroAccSpeed[2] = GyroData->z;
		
	  Angle[0] = (atan2((float)accData->y,(float)(accData->z))*57.29578);  /* 计算倾角 */ 						//俯仰角，即上下角度
	  Angle[1] = (atan2((float)accData->x,(float)(accData->z))*57.29578);
	  Angle[2] = (atan2((float)accData->x,(float)(accData->y))*57.29578);

	  tQmi.Pitch = FirstOrderFilter_pitch(Angle[0], GyroAccSpeed[0]);
	  tQmi.Roll =  FirstOrderFilter_roll(Angle[1], GyroAccSpeed[1]);	
//	  yaw_data =   FirstOrderFilter_yaw(Angle[2], GyroAccSpeed[2]);

}


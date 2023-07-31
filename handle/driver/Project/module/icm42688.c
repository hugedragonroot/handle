#include "icm42688.h"
#include "common.h"
#include "bsp_i2c.h"
static float accSensitivity   = 0.244f;   //���ٶȵ���С�ֱ��� mg/LSB
static float gyroSensitivity  = 0.000532f;    //�����ǵ���С�ֱ���

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
* ��    �ƣ� Icm42688Init
* ��    �ܣ� Icm42688 ��������ʼ��
* ��ڲ����� ��
* ���ڲ����� 0: ��ʼ���ɹ�  ����ֵ: ��ʼ��ʧ��
* �������ڣ� 2022-07-25
* ��    �ģ�
* �޸����ڣ�
* ��    ע��
*******************************************************************************/
void Icm42688Init(void)
{
	
		tQmi.bais_pitch = 128.0f;
    I2C_Bus_Init();
	
	  uint8_t reg_val = 0;
    /* ��ȡ who am i �Ĵ��� */
    reg_val = icm42688_read_reg(ICM42688_WHO_AM_I);
    icm42688_write_reg(ICM42688_REG_BANK_SEL, 0); //����bank 0����Ĵ���
    icm42688_write_reg(ICM42688_REG_BANK_SEL, 0x01); //��λ������
    delay_1ms(100);

    if(reg_val == ICM42688_ID)
    {
        icm42688_write_reg(ICM42688_REG_BANK_SEL, 1); //����bank 1����Ĵ���
//        icm42688_write_reg(ICM42688_INTF_CONFIG4, 0x02); //����Ϊ4��SPIͨ��
			
        icm42688_write_reg(ICM42688_REG_BANK_SEL, 0); //����bank 0����Ĵ���
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
        reg_val |= (AFS_8G << 5);   //���� ��8g
        reg_val |= (AODR_1000Hz);     //������� 1000HZ
        icm42688_write_reg(ICM42688_ACCEL_CONFIG0, reg_val);

        bsp_Icm42688GetGres(GFS_1000DPS);
        icm42688_write_reg(ICM42688_REG_BANK_SEL, 0x00);
        reg_val = icm42688_read_reg(ICM42688_GYRO_CONFIG0);//page73
        reg_val |= (GFS_1000DPS << 5);   //���� ��1000dps
        reg_val |= (GODR_1000Hz);     //������� 1000HZ
        icm42688_write_reg(ICM42688_GYRO_CONFIG0, reg_val);

        icm42688_write_reg(ICM42688_REG_BANK_SEL, 0x00);
        reg_val = icm42688_read_reg(ICM42688_PWR_MGMT0); //��ȡPWR��MGMT0��ǰ�Ĵ�����ֵ(page72)
        reg_val &= ~(1 << 5);//ʹ���¶Ȳ���
        reg_val |= ((3) << 2);//����GYRO_MODE  0:�ر� 1:���� 2:Ԥ�� 3:������
        reg_val |= (3);//����ACCEL_MODE 0:�ر� 1:�ر� 2:�͹��� 3:������
        icm42688_write_reg(ICM42688_PWR_MGMT0, reg_val);
        delay_1ms(1); //������PWR��MGMT0�Ĵ����� 200us�ڲ������κζ�д�Ĵ����Ĳ���
			}
}

/*******************************************************************************
* ��    �ƣ� bsp_IcmGetTemperature
* ��    �ܣ� ��ȡIcm42688 �ڲ��������¶�
* ��ڲ����� ��
* ���ڲ����� ��
* �������ڣ� 2022-07-25
* ��    �ģ�
* �޸����ڣ�
* ��    ע�� datasheet page62
*******************************************************************************/
int8_t bsp_IcmGetTemperature(int16_t* pTemp)
{
    uint8_t buffer[2] = {0};

    icm42688_read_regs(ICM42688_TEMP_DATA1, buffer, 2);

    *pTemp = (int16_t)(((int16_t)((buffer[0] << 8) | buffer[1])) / 132.48 + 25);
    return 0;
}

/*******************************************************************************
* ��    �ƣ� bsp_IcmGetAccelerometer
* ��    �ܣ� ��ȡIcm42688 ���ٶȵ�ֵ
* ��ڲ����� ������ٶȵ�ֵ
* ���ڲ����� ��
* �������ڣ� 2022-07-25
* ��    �ģ�
* �޸����ڣ�
* ��    ע�� datasheet page62
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
* ��    �ƣ� IcmGetGyroscope
* ��    �ܣ� ��ȡIcm42688 �����ǵ�ֵ
* ��ڲ����� ���������ǵ�ֵ
* ���ڲ����� ��
* �������ڣ� 2022-07-25
* ��    �ģ�
* �޸����ڣ�
* ��    ע�� datasheet page63
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
* ��    �ƣ� IcmGetRawData
* ��    �ܣ� ��ȡIcm42688���ٶ�����������
* ��ڲ����� ����
* ���ڲ����� ��
* �������ڣ� 2022-07-25
* ��    �ģ�
* �޸����ڣ�
* ��    ע�� datasheet page62,63
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
		
	  Angle[0] = (atan2((float)accData->y,(float)(accData->z))*57.29578);  /* ������� */ 						//�����ǣ������½Ƕ�
	  Angle[1] = (atan2((float)accData->x,(float)(accData->z))*57.29578);
	  Angle[2] = (atan2((float)accData->x,(float)(accData->y))*57.29578);

	  tQmi.Pitch = FirstOrderFilter_pitch(Angle[0], GyroAccSpeed[0]);
	  tQmi.Roll =  FirstOrderFilter_roll(Angle[1], GyroAccSpeed[1]);	
//	  yaw_data =   FirstOrderFilter_yaw(Angle[2], GyroAccSpeed[2]);

}


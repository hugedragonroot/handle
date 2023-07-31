#include "QMI8658C.h"
#include "math.h"
/*************************************************************/

extern void system_driver_delay_us(uint32_t us);
float Gyro_X,Gyro_Y,Accel_X,Accel_Y;
int16_t Roll,Pitch;
tQmi_type tQmi;
void i2c_delay(uint16_t n)
{
  while(n--);
}
void delay_1ms(uint32_t count)
{
	uint32_t target;
	target = system_times + count;
	while(system_times<target);

}
void QMI_SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOB_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

void QMI_SDA_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitStruct.Pin =GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}
void I2C_Start(void)
{
	QMI_SDA_OUT();
	i2c_sda_1;
	i2c_scl_1;
	system_driver_delay_us(1);
	i2c_sda_0;	
	system_driver_delay_us(1);
	i2c_scl_0;
}	

void I2C_Stop(void)
{
	QMI_SDA_OUT();
	i2c_scl_0;
	i2c_sda_0;
	system_driver_delay_us(1);
	i2c_scl_1;
	i2c_sda_1;
	system_driver_delay_us(1);
}

uint8_t I2C_WaitAck(void)
{
	uint16_t cnt_data = 0;
	i2c_sda_1;
	QMI_SDA_IN();
	system_driver_delay_us(1);
	i2c_scl_1;
	system_driver_delay_us(1);
	while(i2c_sda_state)
	{
		cnt_data++;
		if(cnt_data>250)
		{
			I2C_Stop();
			return 1;
		}
	}
	i2c_scl_0;
	return 0;
}

void I2C_SendAck(void)
{
	
	i2c_scl_0;
	QMI_SDA_OUT();
	i2c_sda_0;
	system_driver_delay_us(1);
	i2c_scl_1;
	system_driver_delay_us(1);
	i2c_scl_0;
	system_driver_delay_us(1);
	i2c_sda_1;
}
void I2C_SendnAck(void)
{
	i2c_scl_0;
	QMI_SDA_OUT();
	i2c_sda_1;
	system_driver_delay_us(1);
	i2c_scl_1;
	system_driver_delay_us(1);
	i2c_scl_0;
}

void I2C_WriteBit(uint8_t data)
{
	uint8_t t;
	QMI_SDA_OUT();
	system_driver_delay_us(1);
	i2c_scl_0;
	system_driver_delay_us(1);
	for(t=0;t<8;t++)
	{					
		i2c_sda_write((data&0x80)>>7);
		data<<=1;
		system_driver_delay_us(1);
		i2c_scl_1;
		system_driver_delay_us(1);
		i2c_scl_0;
		system_driver_delay_us(1);
	}
}

uint8_t I2C_ReadBit(uint8_t ack)
{
	uint8_t i,q;
	QMI_SDA_IN();
	system_driver_delay_us(1);
	for(i=0;i<8;i++)
	{
		
		i2c_scl_0;
		system_driver_delay_us(1);
		q <<=1;
		i2c_scl_1;
		if(i2c_sda_state)q++;		
		system_driver_delay_us(1);		
	}
	if(!ack)	I2C_SendnAck();
	else		I2C_SendAck();
	return q;
}
/*************************************************************/
uint8_t 	I2C_WriteByte(uint8_t addr,uint8_t reg,uint8_t Byte)
{
	I2C_Start();
	I2C_WriteBit(addr<<1|0);
	if(I2C_WaitAck())	return 1;	
	I2C_WriteBit(reg);
	if(I2C_WaitAck())	return 1;
	I2C_WriteBit(Byte);
	if(I2C_WaitAck())	return 1;
	I2C_Stop();
	return 0;
}

 
uint8_t	I2C_ReadByte(uint8_t addr,uint8_t reg)
{
	uint8_t Byte;
	I2C_Start();
	I2C_WriteBit(addr<<1|0);
	I2C_WaitAck();
	I2C_WriteBit(reg);
	I2C_WaitAck();
	I2C_Start();
	I2C_WriteBit((addr<<1)|1);
	I2C_WaitAck();
	Byte=I2C_ReadBit(0);
	I2C_Stop();
	return Byte;
}


uint8_t I2C_WriteData(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
	uint8_t i;
	I2C_Start();
	I2C_WriteBit(addr<<1);
	if(I2C_WaitAck())	return 1;	
	I2C_WriteBit(reg);
	if(I2C_WaitAck())	return 1;
	for(i=0;i<len;i++)
	{
		I2C_WriteBit(buf[i]);
		if(I2C_WaitAck())
			return 1;
	}
	I2C_Stop();
	return 0;
}


uint8_t I2C_ReadData(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
	I2C_Start();
	I2C_WriteBit(addr<<1|0);
	if(I2C_WaitAck())	return 1;
	I2C_WriteBit(reg);
	if(I2C_WaitAck())	return 1;
	I2C_Start();
	I2C_WriteBit((addr<<1)|1);
	if(I2C_WaitAck())	return 1;
	while(len)
	{
		if(len==1)	*buf=I2C_ReadBit(0);
		else		*buf=I2C_ReadBit(1);
		buf++;
		len--;
	}
	I2C_Stop();
	return 0;
}

uint8_t QMI8658_init(void)
{
	tQmi.bais_pitch = 128.0f;
	uint8_t mistake,flag=0;
	mistake += I2C_WriteByte(QMI8658_SA0_H,CTL1,CTL1_INIT);
	delay_1ms(50);
	mistake += I2C_WriteByte(QMI8658_SA0_H,CTL7,CTL7_INIT);
	delay_1ms(50);
	mistake += I2C_WriteByte(QMI8658_SA0_H,CTL2,CTL2_INIT);
	delay_1ms(50);
	mistake += I2C_WriteByte(QMI8658_SA0_H,CTL3,CTL3_INIT);
	delay_1ms(50);
	mistake += I2C_WriteByte(QMI8658_SA0_H,CTL4,CTL4_INIT);
	delay_1ms(50);
	mistake += I2C_WriteByte(QMI8658_SA0_H,CTL5,CTL5_INIT);
	delay_1ms(50);
	mistake += I2C_WriteByte(QMI8658_SA0_H,CTL6,CTL6_INIT);	
	delay_1ms(50);
	mistake += I2C_WriteByte(QMI8658_SA0_H,CTL8,CTL8_INIT);	
	delay_1ms(50);	
	mistake += I2C_WriteByte(QMI8658_SA0_H,CTL9,CTL9_INIT);	
	delay_1ms(50);		
	return mistake;	
	flag=I2C_ReadByte(QMI8658_SA0_H,0x3c);
	while(flag!=0x05)
	{
		delay_1ms(50);
		flag++;
		if(flag>10)	return	flag;
	}
	
}
int16_t Accel[3];
void Read_Accele(void)
{
	uint8_t buf[6];
	I2C_ReadData(QMI8658_SA0_H,AX_L,6,buf);
	Accel[0]=(int16_t)((buf[1]<<8)|buf[0]);
	Accel[1]=(int16_t)(buf[3]<<8)|buf[2];
	Accel[2]=(int16_t)(buf[5]<<8)|buf[4];	
	
	Accel_X = (atan2((float)Accel[1],(float)(Accel[2]))*57.29578);  /* ¼ÆËãÇã½Ç */ 						//¸©Ñö½Ç£¬¼´ÉÏÏÂ½Ç¶È
	Accel_Y = (atan2((float)Accel[0],(float)(Accel[2]))*57.29578);
}

void Read_Gyro(void)
{
	int16_t Gyro[3];
	uint8_t buf[6];
	I2C_ReadData(QMI8658_SA0_H,GX_L,6,buf);
	Gyro[0]=(int16_t)(buf[0]|(uint16_t)(buf[1]<<8));
	Gyro[1]=(int16_t)(buf[2]|(uint16_t)(buf[3]<<8));
	Gyro[2]=(int16_t)(buf[4]|(uint16_t)(buf[5]<<8));	
	Gyro_X  = (float)(Gyro[0])*5/82;   /* ÍÓÂÝÒÇÁ¿³Ì×ª»»,Gyro/16.4 ~ 2000deg/s */
	Gyro_Y  = (float)(Gyro[1])*5/82;
}

float Yijielvbo(float accel_m, float gyro_m)
{
	static float Angle_one,Angle_L1;
	
	Angle_L1 = Angle_one;
	Angle_one = 0.1f*accel_m + (1.0f-0.1f)*Angle_L1;
	return Angle_one;
}

int16_t Erjielvbo(float accel_m, float gyro_m)
{
	static float x1, x2, y1, Angle_two, Angle_L2;
	
	Angle_L2 = Angle_two;
	x1 = (accel_m-Angle_L2) * (0.95*0.95);  /* Kp */
	y1 = y1 + (x1*0.01);  /* (Kp*t)/Ti */
	x2 = y1 + (2*0.95) * (accel_m-Angle_L2) + gyro_m; /* (Kp*Td)/t */
	Angle_two = Angle_L2 + x2*0.01;
	
	return Angle_two;
}

void QMI_ReadData(void)
{
//	Read_Gyro();
	Read_Accele();
	tQmi.Pitch=Yijielvbo(Accel_X,Gyro_X);	
//	tSys.AngleData = tQmi.Pitch;
}





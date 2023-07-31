/**********************************************************/
//@Autor:Raw
//@Version:0.0.1
//@End Time:2023/4/18
//@Function:Brush_Calib.c
/**********************************************************/
#include "Brush_Calib.h"
#include "bsp_flash.h"
#include "Brush_Driver.h"
#include "adc_callback.h"
typedef struct
{
	uint16_t Trip_Value;
	uint16_t OVER_Current;
	int32_t Angle_Basic;
}Brush_data_t;
Brush_data_t Brush_data[2];

#define Brush_Data_Flash_A	0x0803D000  //244k
#define Brush_Data_Flash_U	0x0803D800  //246k
void Brush_Save_Para(Brush_Num num)
{
		if(Brush[num].AngelTrip != Brush_data[num].Trip_Value
			||Brush[num].Total_Current != Brush_data[num].OVER_Current
			||Brush_data[num].Angle_Basic != (int32_t)(Brush[num].MPU_Basic*1000.0f)
		)
	{
		Brush_data[num].Trip_Value = Brush[num].AngelTrip;
		Brush_data[num].OVER_Current = Brush[num].Total_Current;
		Brush_data[num].Angle_Basic = (int32_t)(Brush[num].MPU_Basic*1000.0f);
	}
	else return;
	
	if(num == Brush_A)
	{
		fmc_erase_pages(Brush_Data_Flash_A,sizeof(Brush_data_t));
		fmc_program(Brush_Data_Flash_A,(uint8_t *)&Brush_data[num],sizeof(Brush_data_t));
	}
	else if(num == Brush_U)
	{
		fmc_erase_pages(Brush_Data_Flash_U,sizeof(Brush_data_t));
		fmc_program(Brush_Data_Flash_U,(uint8_t *)&Brush_data[num],sizeof(Brush_data_t));
	}

}


#define Brush_Current_lim 500
#define Brush_mpu_basic	15.0f
void Brush_Read_Para(Brush_Num num)
{
	if(num == Brush_A)
	{
		fmc_read_data(Brush_Data_Flash_A,(uint8_t *)&Brush_data[num],sizeof(Brush_data_t));
	}
	else if(num == Brush_U)
	{
		fmc_read_data(Brush_Data_Flash_U,(uint8_t *)&Brush_data[num],sizeof(Brush_data_t));
	}
	
	if((Brush_data[num].Trip_Value == 0xffff||Brush_data[num].Trip_Value == 0x0000))
	{
		Brush[num].brush_state.bit.FINISH_calibration_One = 0; 
	}
	else
	{
		Brush[num].AngelTrip = Brush_data[num].Trip_Value;
		Brush[num].brush_state.bit.FINISH_calibration_One = 1;
	}
	if((Brush_data[num].OVER_Current == 0xffff||Brush_data[num].OVER_Current == 0x0000))
	{
		Brush[num].Total_Current = Brush_Current_lim; 
	}
	else
	{
		Brush[num].Total_Current = Brush_data[num].OVER_Current;
	}
	
	if(((Brush_data[num].Angle_Basic&0xffffffff) == 0xffffffff||Brush_data[num].Angle_Basic == 0x00000000))
	{
		Brush[num].MPU_Basic = Brush_mpu_basic; 
	}
	else
	{
		Brush[num].MPU_Basic = (float)Brush_data[num].Angle_Basic/1000.0f;
	}


}


void Brush_Data_Check(void)
{
	Brush_ADC_OffsetSample();
	if(ADC_Data.Brush_adc[Brush_A].adc_offer<50)
	{
		Brush[Brush_A].brush_state.bit.Current_Error = 0;
	}
	else
	{
		Brush[Brush_A].brush_state.bit.Current_Error = 1;
	}
	
	if(ADC_Data.Brush_adc[Brush_U].adc_offer<50)
	{
		Brush[Brush_U].brush_state.bit.Current_Error = 0;
	}
	else
	{
		Brush[Brush_U].brush_state.bit.Current_Error = 1;
	}
	
}












#include "key_ext.h"
#include "main.h"
#include "key_drv.h"


#define KEY_NUM_ID(x)	((x+1)/3)    

static uint8_t gpio_val[HARD_KEY_NUM];


/* ����һ���꺯���򻯺������� 
	�ж�GPIO�����Ƿ���Ч����
*/
static KEY_T s_tBtn[KEY_COUNT] = {0};
static KEY_FIFO_T s_tKey;		/* ����FIFO����,�ṹ�� */

static void bsp_InitKeyVar(void);
static void bsp_DetectKey(uint8_t i);

void get_mixkey(void)
{

	#if USING_6KEY

	gpio_val[KID_SPEED_ADD] = gpio_input_bit_get(KEY_SPEED_ADD_PORT,KEY_SPEED_ADD_PIN);
	gpio_val[KID_SPEED_SUB] = gpio_input_bit_get(KEY_SPEED_SUB_PORT,KEY_SPEED_SUB_PIN);
	gpio_val[KID_POWER] = gpio_input_bit_get(KEY_POWER_PORT,KEY_POWER_PIN);
	gpio_val[KID_PUSH_ORD_ADD] = gpio_input_bit_get(KEY_PUSH_ROB_ADD_PORT,KEY_PUSH_ROB_ADD_PIN);
	gpio_val[KID_PUSH_ORD_SUB] = gpio_input_bit_get(KEY_PUSH_ROB_SUB_PORT,KEY_PUSH_ROB_SUB_PIN);
	gpio_val[KID_HORN] = gpio_input_bit_get(KEY_HORN_PORT,KEY_HORN_PIN);


	#else
	uint8_t val[HARD_KEY_NUM]={0};

	GPIO_InitIO(KEY_ROW1_GPIO_PORT,KEY_ROW1_PIN,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ);	
	GPIO_InitIO(KEY_ROW2_GPIO_PORT,KEY_ROW2_PIN,GPIO_MODE_OUT_OD,GPIO_OSPEED_50MHZ);	
	GPIO_InitIO(KEY_ROW3_GPIO_PORT,KEY_ROW3_PIN,GPIO_MODE_OUT_OD,GPIO_OSPEED_50MHZ);

	gpio_bit_reset(KEY_ROW1_GPIO_PORT,KEY_ROW1_PIN);
	gpio_bit_set(KEY_ROW2_GPIO_PORT,KEY_ROW2_PIN);
	gpio_bit_set(KEY_ROW3_GPIO_PORT,KEY_ROW2_PIN);

	val[0] = gpio_input_bit_get(KEY_COL1_GPIO_PORT,KEY_COL1_PIN);
	val[1] = gpio_input_bit_get(KEY_COL2_GPIO_PORT,KEY_COL2_PIN);
	val[2] = gpio_input_bit_get(KEY_COL3_GPIO_PORT,KEY_COL3_PIN);
	gpio_bit_set(KEY_ROW1_GPIO_PORT,KEY_ROW1_PIN);

	GPIO_InitIO(KEY_ROW1_GPIO_PORT,KEY_ROW1_PIN,GPIO_MODE_OUT_OD,GPIO_OSPEED_50MHZ);	
	GPIO_InitIO(KEY_ROW2_GPIO_PORT,KEY_ROW2_PIN,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ);	
	GPIO_InitIO(KEY_ROW3_GPIO_PORT,KEY_ROW3_PIN,GPIO_MODE_OUT_OD,GPIO_OSPEED_50MHZ);
	
	gpio_bit_reset(KEY_ROW2_GPIO_PORT,KEY_ROW2_PIN);
	gpio_bit_set(KEY_ROW1_GPIO_PORT,KEY_ROW1_PIN);
	// gpio_bit_reset(KEY_ROW2_GPIO_PORT,KEY_ROW2_PIN);
	gpio_bit_set(KEY_ROW3_GPIO_PORT,KEY_ROW3_PIN);	
	
	val[3] = gpio_input_bit_get(KEY_COL1_GPIO_PORT,KEY_COL1_PIN);
	val[4] = gpio_input_bit_get(KEY_COL2_GPIO_PORT,KEY_COL2_PIN);
	val[5] = gpio_input_bit_get(KEY_COL3_GPIO_PORT,KEY_COL3_PIN);
	gpio_bit_set(KEY_ROW2_GPIO_PORT,KEY_ROW2_PIN);	

	GPIO_InitIO(KEY_ROW1_GPIO_PORT,KEY_ROW1_PIN,GPIO_MODE_OUT_OD,GPIO_OSPEED_50MHZ);	
	GPIO_InitIO(KEY_ROW2_GPIO_PORT,KEY_ROW2_PIN,GPIO_MODE_OUT_OD,GPIO_OSPEED_50MHZ);	
	GPIO_InitIO(KEY_ROW3_GPIO_PORT,KEY_ROW3_PIN,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ);
	
	
	gpio_bit_reset(KEY_ROW3_GPIO_PORT,KEY_ROW3_PIN);
	gpio_bit_set(KEY_ROW1_GPIO_PORT,KEY_ROW1_PIN);
	gpio_bit_set(KEY_ROW2_GPIO_PORT,KEY_ROW2_PIN);
	// gpio_bit_reset(KEY_ROW3_GPIO_PORT,KEY_ROW3_PIN);
	//ִ�����ͺ�������ⰴ��������쳣
	
	val[6] = gpio_input_bit_get(KEY_COL1_GPIO_PORT,KEY_COL1_PIN);
	val[7] = gpio_input_bit_get(KEY_COL2_GPIO_PORT,KEY_COL2_PIN);
	val[8] = gpio_input_bit_get(KEY_COL3_GPIO_PORT,KEY_COL3_PIN);
	gpio_bit_set(KEY_ROW3_GPIO_PORT,KEY_ROW3_PIN);
	
	GPIO_InitIO(KEY_ROW1_GPIO_PORT,KEY_ROW1_PIN,GPIO_MODE_OUT_OD,GPIO_OSPEED_50MHZ);	
	GPIO_InitIO(KEY_ROW2_GPIO_PORT,KEY_ROW2_PIN,GPIO_MODE_OUT_OD,GPIO_OSPEED_50MHZ);	
	GPIO_InitIO(KEY_ROW3_GPIO_PORT,KEY_ROW3_PIN,GPIO_MODE_OUT_OD,GPIO_OSPEED_50MHZ);
	
	gpio_bit_set(KEY_ROW1_GPIO_PORT,KEY_ROW1_PIN);
	gpio_bit_set(KEY_ROW2_GPIO_PORT,KEY_ROW2_PIN);
	gpio_bit_set(KEY_ROW3_GPIO_PORT,KEY_ROW3_PIN);	

	gpio_val[0] =val[0];
	gpio_val[1] =val[1];
	gpio_val[2] =val[2];
	gpio_val[3] =val[3];
	gpio_val[4] =val[4];
	gpio_val[5] =val[5];
	gpio_val[6] =val[6];
	gpio_val[7] =val[7];
	gpio_val[8] =val[8];
	#endif
	              
}

/*
*********************************************************************************************************
*	�� �� ��: KeyPinVal
*	����˵��: �жϰ����Ƿ���,Ӳ����ȡ
*	��    ��: ��
*	�� �� ֵ: ����ֵ1 ��ʾ����(��ͨ����0��ʾδ���£��ͷţ�
*********************************************************************************************************
*/
static uint8_t KeyPinVal()
{	
	get_mixkey();
	return 0;
}

/*
*********************************************************************************************************
*	�� �� ��: KeyPinActive
*	����˵��: �жϰ����Ƿ���
*	��    ��: ��
*	�� �� ֵ: ����ֵ1 ��ʾ����(��ͨ����0��ʾδ���£��ͷţ�
*********************************************************************************************************
*/
static uint8_t KeyPinActive(uint8_t _id)
{	
	if(gpio_val[_id]==0)
	{
		return 1;
	}
	else
	{
		return 0;
	}	
}

/*
*********************************************************************************************************
*	�� �� ��: IsKeyDownFunc
*	����˵��: �жϰ����Ƿ��¡���������ϼ����֡������¼������������������¡�
*	��    ��: ��
*	�� �� ֵ: ����ֵ1 ��ʾ����(��ͨ����0��ʾδ���£��ͷţ�
*********************************************************************************************************
*/
static uint8_t IsKeyDownFunc(uint8_t _id)
{
	static uint8_t keys;
	
	/* ʵ�嵥�� */
	if (_id < HARD_KEY_NUM)
	{
		uint8_t i;
		uint8_t count = 0;
		uint8_t save = 255;
		
		/* �ж��м��������� */
		for (i = 0; i < HARD_KEY_NUM; i++)
		{
			if (KeyPinActive(i)) 
			{
				count++;
				save = i;
			}
		}
		if(count > 1 ) keys=1;			
		if(count ==0 ) 	keys=0;		
		if (count == 1 && save == _id && keys==0)
		{
			return 1;	/* ֻ��1��������ʱ����Ч */
		}		
		
		return 0;
	}
	
	/* ��ϼ�*/
	if (_id == HARD_KEY_NUM + 0)
	{
		if (KeyPinActive(KID_PUSH_ORD_ADD) && KeyPinActive(KID_PUSH_ORD_SUB))
			return 1;
		else 
			return 0;
	}
	
	return 0;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitKey
*	����˵��: ��ʼ������. �ú����� bsp_Init() ���á�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void key_init(void)
{
	bsp_InitKeyVar();		/* ��ʼ���������� */
}


/*
*********************************************************************************************************
*	�� �� ��: bsp_InitKeyVar
*	����˵��: ��ʼ����������
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_InitKeyVar(void)
{
	uint8_t i;

	/* �԰���FIFO��дָ������ */
	s_tKey.Read = 0;
	s_tKey.Write = 0;
	s_tKey.Read2 = 0;

	/* ��ÿ�������ṹ���Ա������һ��ȱʡֵ */
	for (i = 0; i < KEY_COUNT; i++)
	{
		s_tBtn[i].LongTime = KEY_LONG_TIME;			/* ����ʱ�� 0 ��ʾ����ⳤ�����¼� */
		s_tBtn[i].Count = KEY_FILTER_TIME / 2;		/* ����������Ϊ�˲�ʱ���һ�� */
		s_tBtn[i].State = 0;							/* ����ȱʡ״̬��0Ϊδ���� */
		s_tBtn[i].RepeatSpeed = 0;						/* �����������ٶȣ�0��ʾ��֧������ */
		s_tBtn[i].RepeatCount = 0;						/* ���������� */
	}

	/* �����Ҫ��������ĳ�������Ĳ����������ڴ˵������¸�ֵ */
		
//	/* ҡ���������ң�֧�ֳ���1����Զ����� */
//	bsp_SetKeyParam(KID_JOY_U, 100, 0);
//	bsp_SetKeyParam(KID_JOY_D, 100, 0);
//	bsp_SetKeyParam(KID_JOY_L, 100, 0);
//	bsp_SetKeyParam(KID_JOY_R, 100, 0);
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_PutKey
*	����˵��: ��1����ֵѹ�밴��FIFO��������������ģ��һ��������
*	��    ��:  _KeyCode : ��������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_PutKey(uint8_t _KeyCode)
{
	s_tKey.Buf[s_tKey.Write] = _KeyCode;

	if (++s_tKey.Write  >= KEY_FIFO_SIZE)
	{
		s_tKey.Write = 0;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_GetKey
*	����˵��: �Ӱ���FIFO��������ȡһ����ֵ��
*	��    ��: ��
*	�� �� ֵ: ��������
*********************************************************************************************************
*/
uint8_t bsp_GetKey(void)
{
	uint8_t ret;

	if (s_tKey.Read == s_tKey.Write)
	{
		return KEY_NONE;
	}
	else
	{
		ret = s_tKey.Buf[s_tKey.Read];

		if (++s_tKey.Read >= KEY_FIFO_SIZE)
		{
			s_tKey.Read = 0;
		}
		return ret;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_GetKey2
*	����˵��: �Ӱ���FIFO��������ȡһ����ֵ�������Ķ�ָ�롣
*	��    ��:  ��
*	�� �� ֵ: ��������
*********************************************************************************************************
*/
uint8_t bsp_GetKey2(void)
{
	uint8_t ret;

	if (s_tKey.Read2 == s_tKey.Write)
	{
		return KEY_NONE;
	}
	else
	{
		ret = s_tKey.Buf[s_tKey.Read2];

		if (++s_tKey.Read2 >= KEY_FIFO_SIZE)
		{
			s_tKey.Read2 = 0;
		}
		return ret;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_GetKeyState
*	����˵��: ��ȡ������״̬
*	��    ��:  _ucKeyID : ����ID����0��ʼ
*	�� �� ֵ: 1 ��ʾ���£� 0 ��ʾδ����
*********************************************************************************************************
*/
uint8_t bsp_GetKeyState(KEY_ID_E _ucKeyID)
{
	return s_tBtn[_ucKeyID].State;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_SetKeyParam
*	����˵��: ���ð�������
*	��    �Σ�_ucKeyID : ����ID����0��ʼ
*			_LongTime : �����¼�ʱ��
*			 _RepeatSpeed : �����ٶ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t  _RepeatSpeed)
{
	s_tBtn[_ucKeyID].LongTime = _LongTime;			/* ����ʱ�� 0 ��ʾ����ⳤ�����¼� */
	s_tBtn[_ucKeyID].RepeatSpeed = _RepeatSpeed;/* �����������ٶȣ�0��ʾ��֧������ */
	s_tBtn[_ucKeyID].RepeatCount = 0;						/* ���������� */
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_ClearKey
*	����˵��: ��հ���FIFO������
*	��    �Σ���
*	�� �� ֵ: ��������
*********************************************************************************************************
*/
void bsp_ClearKey(void)
{
	s_tKey.Read = s_tKey.Write;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_DetectKey
*	����˵��: ���һ��������������״̬�����뱻�����Եĵ��á�
*	��    ��: IO��id�� ��0��ʼ����
*	�� �� ֵ: ��
*********************************************************************************************************
*/

static void bsp_DetectKey(uint8_t i)
{
	KEY_T *pBtn;

	pBtn = &s_tBtn[i];
	if (IsKeyDownFunc(i))
	{
		if (pBtn->Count < KEY_FILTER_TIME)
		{
			pBtn->Count = KEY_FILTER_TIME;
		}
		else if(pBtn->Count < 2 * KEY_FILTER_TIME)
		{
			pBtn->Count++;
		}
		else
		{
			if (pBtn->State == 0)
			{
				pBtn->State = 1;

				/* ���Ͱ�ť���µ���Ϣ */
				bsp_PutKey((uint8_t)(3 * i + 1));
			}

			if (pBtn->LongTime > 0)
			{
				if (pBtn->LongCount < pBtn->LongTime)
				{
					/* ���Ͱ�ť�������µ���Ϣ */
					if (++pBtn->LongCount == pBtn->LongTime)
					{
						/* ��ֵ���밴��FIFO */
						bsp_PutKey((uint8_t)(3 * i + 3));
					}
				}
				else
				{
					if (pBtn->RepeatSpeed > 0)
					{
						if (++pBtn->RepeatCount >= pBtn->RepeatSpeed)
						{
							pBtn->RepeatCount = 0;
							/* ��������ÿ��10ms����1������ */
							bsp_PutKey((uint8_t)(3 * i + 1));
						}
					}
				}
			}
		}
	}
	else
	{
		if(pBtn->Count > KEY_FILTER_TIME)
		{
			pBtn->Count = KEY_FILTER_TIME;
		}
		else if(pBtn->Count != 0)
		{
			pBtn->Count--;
		}
		else
		{
			if (pBtn->State == 1)
			{
				pBtn->State = 0;

				/* ���Ͱ�ť�������Ϣ */
				bsp_PutKey((uint8_t)(3 * i + 2));
			}
		}

		pBtn->LongCount = 0;
		pBtn->RepeatCount = 0;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_DetectFastIO
*	����˵��: �����ٵ�����IO. 1msˢ��һ��
*	��    ��: IO��id�� ��0��ʼ����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_DetectFastIO(uint8_t i)
{
	KEY_T *pBtn;

	pBtn = &s_tBtn[i];
	if (IsKeyDownFunc(i))
	{
		if (pBtn->State == 0)
		{
			pBtn->State = 1;

			/* ���Ͱ�ť���µ���Ϣ */
			bsp_PutKey((uint8_t)(3 * i + 1));
		}

		if (pBtn->LongTime > 0)
		{
			if (pBtn->LongCount < pBtn->LongTime)
			{
				/* ���Ͱ�ť�������µ���Ϣ */
				if (++pBtn->LongCount == pBtn->LongTime)
				{
					/* ��ֵ���밴��FIFO */
					bsp_PutKey((uint8_t)(3 * i + 3));
				}
			}
			else
			{
				if (pBtn->RepeatSpeed > 0)
				{
					if (++pBtn->RepeatCount >= pBtn->RepeatSpeed)
					{
						pBtn->RepeatCount = 0;
						/* ��������ÿ��10ms����1������ */
						bsp_PutKey((uint8_t)(3 * i + 1));
					}
				}
			}
		}
	}
	else
	{
		if (pBtn->State == 1)
		{
			pBtn->State = 0;

			/* ���Ͱ�ť�������Ϣ */
			bsp_PutKey((uint8_t)(3 * i + 2));
		}

		pBtn->LongCount = 0;
		pBtn->RepeatCount = 0;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_KeyScan10ms
*	����˵��: ɨ�����а���������������systick�ж������Եĵ��ã�10msһ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
 void bsp_KeyScan10ms(void)
{
		uint8_t i;
	

	KeyPinVal();


	for (i = 0; i < KEY_COUNT; i++)
	{
		bsp_DetectKey(i);
	}
	
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_KeyScan1ms
*	����˵��: ɨ�����а���������������systick�ж������Եĵ��ã�1msһ��.
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_KeyScan1ms(void)
{
	//uint8_t i;
	
//	for (i = 0; i < HARD_KEY_NUM; i++)
//	{
//		KeyPinVal(i);
//	}		

//	for (i = 0; i < KEY_COUNT; i++)
//	{
//		bsp_DetectFastIO(i);
//	}
}

#if USING_TPAD
typedef struct 
{
	uint32_t gpio_periph;
	uint32_t GPIO_Pin;
	uint32_t timer_periph;
	uint16_t channel;
	uint32_t flag;
}TPAD_GPIO2TIMER;

TPAD_GPIO2TIMER TPAD_io_timer_link[TPAD_NUM] = {
	{KEY_HORN_PORT,KEY_HORN_PIN,TIMER2,TIMER_CH_0,TIMER_FLAG_CH0O},
	{KEY_PUSH_ROB_UP_PORT,KEY_PUSH_ROB_UP_PIN,TIMER2,TIMER_CH_1,TIMER_FLAG_CH1O},
	{KEY_SPEED_ADD_PORT,KEY_SPEED_ADD_PIN,TIMER2,TIMER_CH_2,TIMER_FLAG_CH2O},
	// {KEY_SPEED_SUB_PORT,KEY_SPEED_SUB_PIN,TIMER0,TIMER_CH_0},
	{KEY_POWER_PORT,KEY_POWER_PIN,TIMER2,TIMER_CH_3,TIMER_FLAG_CH3O},
	// {KEY_PUSH_ROB_DOWN_PORT,KEY_PUSH_ROB_DOWN_PIN,TIMER11,TIMER_CH_0},
};


void TPAD_Timer_Cap_Config(void){

	timer_parameter_struct timer_initpara;
	timer_ic_parameter_struct timer_icinitpara;

    rcu_periph_clock_enable(RCU_TIMER2);
    timer_deinit(TIMER2);

    /* TIMER2 configuration */
    timer_initpara.prescaler         = 59;// 1 Mhz
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 65535; 
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER2,&timer_initpara);	

	timer_icinitpara.icpolarity = TIMER_IC_POLARITY_RISING;
	timer_icinitpara.icselection = TIMER_IC_SELECTION_DIRECTTI;
	timer_icinitpara.icprescaler = TIMER_IC_PSC_DIV1;
	timer_icinitpara.icfilter = 0x0;

	timer_input_capture_config(TIMER2,TIMER_CH_0,&timer_icinitpara);
	timer_input_capture_config(TIMER2,TIMER_CH_1,&timer_icinitpara);
	timer_input_capture_config(TIMER2,TIMER_CH_2,&timer_icinitpara);
	timer_input_capture_config(TIMER2,TIMER_CH_3,&timer_icinitpara);

    timer_auto_reload_shadow_enable(TIMER2);
    timer_enable(TIMER2);	

}
void TPAD_Reset(TPAD_GPIO2TIMER *io2timer){
	GPIO_InitIO(io2timer->gpio_periph,io2timer->GPIO_Pin,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ);

	gpio_bit_reset(io2timer->gpio_periph,io2timer->GPIO_Pin);

	// vTaskDelay(1);
	delay_ms(1);

	timer_counter_value_config(io2timer->timer_periph,0);
	GPIO_InitIO(io2timer->gpio_periph,io2timer->GPIO_Pin,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ);
}

u32 TPAD_Get_val(TPAD_GPIO2TIMER *io2timer){

	TPAD_Reset(io2timer);
	while(timer_flag_get(io2timer->timer_periph,io2timer->flag) == RESET){
		if(timer_counter_read(io2timer->timer_periph) > 10000) break;
	}
	timer_flag_clear(io2timer->timer_periph,io2timer->flag);
	return timer_counter_read(io2timer->timer_periph);
}

u32 TPAD_init_val[TPAD_NUM];

void TPAD_Init(void){
	u32 buf[10],temp;
	TPAD_Timer_Cap_Config();
	for(u8 i = 0;i<TPAD_NUM;i++){
		for(u8 j = 0;j < 10 ;j++){
			buf[j] = TPAD_Get_val(&TPAD_io_timer_link[i]);
			delay_ms(10);
		}
		for(u8 j = 0;j < 9;j++){
			for (u8 k = j+1; k < 10; k++) {
				if(buf[j] > buf[k]){
					temp = buf[j];
					buf[j] = buf[k];
					buf[k] = temp;
				}
			}
		}
		temp = 0;
		for(u8 j = 2;j < 8;j++){
			temp += buf[j];
		}
		TPAD_init_val[i] = temp/6;
	}
}

u32 TPAD_Get_MaxVal(TPAD_GPIO2TIMER *io2timer){
	u32 temp = 0, res = 0;
	for (u8 i = 0; i < 3; i++)
	{
		temp = TPAD_Get_val(io2timer);
		if(temp > res) res = temp;
	}
	return res;
}

u8 resKeyVal = 0;

u32 TPAD_Cur_val[TPAD_NUM];

u8 TPAD_SCAN(void){
	for(u8 i = 0;i < TPAD_NUM; i++){
		TPAD_Cur_val[i] = TPAD_Get_MaxVal(&TPAD_io_timer_link[i]);
		if(TPAD_Cur_val[i] > (TPAD_init_val[i] + TPAD_GATE_VAL)){
			resKeyVal |= 1<<i;
		}
	}
	return 0;
}
#endif


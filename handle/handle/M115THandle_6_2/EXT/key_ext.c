#include "key_ext.h"
#include "main.h"
#include "key_drv.h"


#define KEY_NUM_ID(x)	((x+1)/3)    

static uint8_t gpio_val[HARD_KEY_NUM];


/* 定义一个宏函数简化后续代码 
	判断GPIO引脚是否有效按下
*/
static KEY_T s_tBtn[KEY_COUNT] = {0};
static KEY_FIFO_T s_tKey;		/* 按键FIFO变量,结构体 */

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
	//执行拉低后立即检测按键会出现异常
	
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
*	函 数 名: KeyPinVal
*	功能说明: 判断按键是否按下,硬件读取
*	形    参: 无
*	返 回 值: 返回值1 表示按下(导通），0表示未按下（释放）
*********************************************************************************************************
*/
static uint8_t KeyPinVal()
{	
	get_mixkey();
	return 0;
}

/*
*********************************************************************************************************
*	函 数 名: KeyPinActive
*	功能说明: 判断按键是否按下
*	形    参: 无
*	返 回 值: 返回值1 表示按下(导通），0表示未按下（释放）
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
*	函 数 名: IsKeyDownFunc
*	功能说明: 判断按键是否按下。单键和组合键区分。单键事件不允许有其他键按下。
*	形    参: 无
*	返 回 值: 返回值1 表示按下(导通），0表示未按下（释放）
*********************************************************************************************************
*/
static uint8_t IsKeyDownFunc(uint8_t _id)
{
	static uint8_t keys;
	
	/* 实体单键 */
	if (_id < HARD_KEY_NUM)
	{
		uint8_t i;
		uint8_t count = 0;
		uint8_t save = 255;
		
		/* 判断有几个键按下 */
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
			return 1;	/* 只有1个键按下时才有效 */
		}		
		
		return 0;
	}
	
	/* 组合键*/
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
*	函 数 名: bsp_InitKey
*	功能说明: 初始化按键. 该函数被 bsp_Init() 调用。
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void key_init(void)
{
	bsp_InitKeyVar();		/* 初始化按键变量 */
}


/*
*********************************************************************************************************
*	函 数 名: bsp_InitKeyVar
*	功能说明: 初始化按键变量
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_InitKeyVar(void)
{
	uint8_t i;

	/* 对按键FIFO读写指针清零 */
	s_tKey.Read = 0;
	s_tKey.Write = 0;
	s_tKey.Read2 = 0;

	/* 给每个按键结构体成员变量赋一组缺省值 */
	for (i = 0; i < KEY_COUNT; i++)
	{
		s_tBtn[i].LongTime = KEY_LONG_TIME;			/* 长按时间 0 表示不检测长按键事件 */
		s_tBtn[i].Count = KEY_FILTER_TIME / 2;		/* 计数器设置为滤波时间的一半 */
		s_tBtn[i].State = 0;							/* 按键缺省状态，0为未按下 */
		s_tBtn[i].RepeatSpeed = 0;						/* 按键连发的速度，0表示不支持连发 */
		s_tBtn[i].RepeatCount = 0;						/* 连发计数器 */
	}

	/* 如果需要单独更改某个按键的参数，可以在此单独重新赋值 */
		
//	/* 摇杆上下左右，支持长按1秒后，自动连发 */
//	bsp_SetKeyParam(KID_JOY_U, 100, 0);
//	bsp_SetKeyParam(KID_JOY_D, 100, 0);
//	bsp_SetKeyParam(KID_JOY_L, 100, 0);
//	bsp_SetKeyParam(KID_JOY_R, 100, 0);
}

/*
*********************************************************************************************************
*	函 数 名: bsp_PutKey
*	功能说明: 将1个键值压入按键FIFO缓冲区。可用于模拟一个按键。
*	形    参:  _KeyCode : 按键代码
*	返 回 值: 无
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
*	函 数 名: bsp_GetKey
*	功能说明: 从按键FIFO缓冲区读取一个键值。
*	形    参: 无
*	返 回 值: 按键代码
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
*	函 数 名: bsp_GetKey2
*	功能说明: 从按键FIFO缓冲区读取一个键值。独立的读指针。
*	形    参:  无
*	返 回 值: 按键代码
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
*	函 数 名: bsp_GetKeyState
*	功能说明: 读取按键的状态
*	形    参:  _ucKeyID : 按键ID，从0开始
*	返 回 值: 1 表示按下， 0 表示未按下
*********************************************************************************************************
*/
uint8_t bsp_GetKeyState(KEY_ID_E _ucKeyID)
{
	return s_tBtn[_ucKeyID].State;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_SetKeyParam
*	功能说明: 设置按键参数
*	形    参：_ucKeyID : 按键ID，从0开始
*			_LongTime : 长按事件时间
*			 _RepeatSpeed : 连发速度
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t  _RepeatSpeed)
{
	s_tBtn[_ucKeyID].LongTime = _LongTime;			/* 长按时间 0 表示不检测长按键事件 */
	s_tBtn[_ucKeyID].RepeatSpeed = _RepeatSpeed;/* 按键连发的速度，0表示不支持连发 */
	s_tBtn[_ucKeyID].RepeatCount = 0;						/* 连发计数器 */
}

/*
*********************************************************************************************************
*	函 数 名: bsp_ClearKey
*	功能说明: 清空按键FIFO缓冲区
*	形    参：无
*	返 回 值: 按键代码
*********************************************************************************************************
*/
void bsp_ClearKey(void)
{
	s_tKey.Read = s_tKey.Write;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_DetectKey
*	功能说明: 检测一个按键。非阻塞状态，必须被周期性的调用。
*	形    参: IO的id， 从0开始编码
*	返 回 值: 无
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

				/* 发送按钮按下的消息 */
				bsp_PutKey((uint8_t)(3 * i + 1));
			}

			if (pBtn->LongTime > 0)
			{
				if (pBtn->LongCount < pBtn->LongTime)
				{
					/* 发送按钮持续按下的消息 */
					if (++pBtn->LongCount == pBtn->LongTime)
					{
						/* 键值放入按键FIFO */
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
							/* 常按键后，每隔10ms发送1个按键 */
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

				/* 发送按钮弹起的消息 */
				bsp_PutKey((uint8_t)(3 * i + 2));
			}
		}

		pBtn->LongCount = 0;
		pBtn->RepeatCount = 0;
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_DetectFastIO
*	功能说明: 检测高速的输入IO. 1ms刷新一次
*	形    参: IO的id， 从0开始编码
*	返 回 值: 无
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

			/* 发送按钮按下的消息 */
			bsp_PutKey((uint8_t)(3 * i + 1));
		}

		if (pBtn->LongTime > 0)
		{
			if (pBtn->LongCount < pBtn->LongTime)
			{
				/* 发送按钮持续按下的消息 */
				if (++pBtn->LongCount == pBtn->LongTime)
				{
					/* 键值放入按键FIFO */
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
						/* 常按键后，每隔10ms发送1个按键 */
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

			/* 发送按钮弹起的消息 */
			bsp_PutKey((uint8_t)(3 * i + 2));
		}

		pBtn->LongCount = 0;
		pBtn->RepeatCount = 0;
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_KeyScan10ms
*	功能说明: 扫描所有按键。非阻塞，被systick中断周期性的调用，10ms一次
*	形    参: 无
*	返 回 值: 无
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
*	函 数 名: bsp_KeyScan1ms
*	功能说明: 扫描所有按键。非阻塞，被systick中断周期性的调用，1ms一次.
*	形    参: 无
*	返 回 值: 无
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


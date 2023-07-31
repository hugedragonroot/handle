#include "ev1527_ext.h"
#include "ev1527_drv.h"
#include "main.h"

uint8_t RF;
uint8_t decode_ok;				// 解码成功
uint8_t hh_w, ll_w;				// 高,低电平宽度
uint8_t ma_x;					// 接收到第几位编码了
uint8_t bma1, bma2, bma3, bma4; // 用于接收过程存放遥控编码，编码比较两次，这是第一次
uint8_t mma1, mma2, mma3, mma4;

uint8_t rf_ok1, rf_ok; // 解码过程中的临时接收成功标志,接收到一个完整的遥控命令后置1,通知解码程序可以解码了
uint8_t old_rc5;	   // 保存上一次查询到的电平状态
uint8_t tb_ok;		   // 接收到同步的马时置1
// uint16_t s ,s1;
// uint8_t bt_auto;     //自动设置遥控接收波特率标志
uint8_t rf_data[4];

uint8_t user[EV1527_USER_NUM][2];
uint8_t len;
uint8_t mode;
uint16_t count;
uint8_t temp = 0;
uint8_t flag = 0;

void ev1527_init(void)
{
	mode = 0;
	count = 0;
	memset(&rf_data, 0, sizeof(rf_data));
	WARMING(0);
}
void ev1527_task(void)
{
	ev1527_key_scan();
  

	if (decode_ok == 1)
	{
		LED(1);
		decode_ok = 0;
		switch (rf_data[2])
		{
		case 0x28:
		{
			break;
		}
		case 0x24:
		{
			break;
		}
		case 0x22:
		{
			Remote_setting_para.HandleLock = eUnLock;
			break;
		}
		case 0x21:
		{
			Remote_setting_para.HandleLock = eLock;
			break;
		}
		}
		rf_data[2] = 0;
	}
	else if (decode_ok == 0)
	{
		LED(0);
	}
}

void ev1527_scan(void)
{

	if (mode & 0x80)
	{ // 按钮按下
		count++;
		WARMING(1);
		if (count == 65000)
		{			  // 90us*45000=5850000=5.85ms后断开用户记录 学习位置1表示正在学习遥控
			mode = 0; // 5.58秒内没有学习到遥控，标志位置零
			count = 0;
			//mode |= 0x20;
			WARMING(0);
		}
	}
	// 接收数据的电平 PB9
	RF = EV1527();
	if (!RF)
	{
		ll_w++; // 检测到低电平 低电平时间加1,记录本
				// 次电平状态old_rc5
		old_rc5 = 0;
	}
	else // 检测到高电平
	{
		hh_w++;
		if (!old_rc5) // 检测到从低到高的跳变,已检测到一个完整(高-低)电平周期
		{			  // 判同步码    2/5 100/130
			//                if (((hh_w>=2)&&(hh_w<=5))&&((ll_w>=100)&&(ll_w<=130)))
			if (((hh_w >= 2) && (hh_w <= 5)) && ((ll_w >= 100) && (ll_w <= 130)))
			{
				tb_ok = 1;
				ma_x = 0;
				bma1 = 0;
				bma2 = 0;
				bma3 = 0;
				bma4 = 0;
			}
			else if ((tb_ok) && ((ll_w >= 8) && (ll_w <= 13))) // 8/13
			{
				ma_x++; // 已经接收到同步码,判0
				if (ma_x > 23)
				{
					if (!rf_ok1) // rf_1临时接收成功
					{			 // 将接收到的编码复制到解码寄存器中
						mma1 = bma1;
						mma2 = bma2;
						mma3 = bma3;
						mma4 = bma4;
						// 通知解码子程序可以解码了
						rf_ok1 = 1;
						tb_ok = 0;
					}
				}
			}
			else if ((tb_ok) && ((ll_w >= 2) && (ll_w <= 7))) // 2/7
			{
				switch (ma_x)
				{
				case 0:
				{
					bma1 = bma1 | 0x80;
					break;
				} // 遥控编码第1位
				case 1:
				{
					bma1 = bma1 | 0x40;
					break;
				}
				case 2:
				{
					bma1 = bma1 | 0x20;
					break;
				}
				case 3:
				{
					bma1 = bma1 | 0x10;
					break;
				}
				case 4:
				{
					bma1 = bma1 | 0x08;
					break;
				}
				case 5:
				{
					bma1 = bma1 | 0x04;
					break;
				}
				case 6:
				{
					bma1 = bma1 | 0x02;
					break;
				}
				case 7:
				{
					bma1 = bma1 | 0x01;
					break;
				}
				case 8:
				{
					bma2 = bma2 | 0x80;
					break;
				}
				case 9:
				{
					bma2 = bma2 | 0x40;
					break;
				}
				case 10:
				{
					bma2 = bma2 | 0x20;
					break;
				}
				case 11:
				{
					bma2 = bma2 | 0x10;
					break;
				}
				case 12:
				{
					bma2 = bma2 | 0x08;
					break;
				}
				case 13:
				{
					bma2 = bma2 | 0x04;
					break;
				}
				case 14:
				{
					bma2 = bma2 | 0x02;
					break;
				}
				case 15:
				{
					bma2 = bma2 | 0x01;
					break;
				}
				case 16:
				{
					bma3 = bma3 | 0x80;
					break;
				}
				case 17:
				{
					bma3 = bma3 | 0x40;
					break;
				}
				case 18:
				{
					bma3 = bma3 | 0x20;
					break;
				}
				case 19:
				{
					bma3 = bma3 | 0x10;
					break;
				}
				case 20:
				{
					bma3 = bma3 | 0x08;
					break;
				} // 按键状态第1位
				case 21:
				{
					bma3 = bma3 | 0x04;
					break;
				}
				case 22:
				{
					bma3 = bma3 | 0x02;
					break;
				}
				case 23:
				{
					bma3 = bma3 | 0x01;
					if (!rf_ok1)
					{
						mma1 = bma1;
						mma2 = bma2;
						mma3 = bma3;
						rf_ok1 = 1; // 通知解码子程序可以解码了
						tb_ok = 0;
						break;
					}
				}
				}
				ma_x++;
			}
			else
			{
				ma_x = 0;
				tb_ok = 0;
				bma1 = 0;
				bma2 = 0;
				bma3 = 0;
				hh_w = 1;
				ll_w = 0;
			} // 接收到不符合的高-低电平序列
			ll_w = 0;
			hh_w = 1;
		}
		old_rc5 = 1; // 记录本次电平状态
	}

	if ((rf_ok1)) // 判断是否接收成功
	{
		rf_ok1 = 0;
		rf_data[0] = mma1;
		rf_data[1] = mma2;
		rf_data[2] = mma3;
		#if 0
		decode_ok = 1;
		#endif

		if ((mode & 0x80) && ((mode & 0x40) == 0))
		{
			user[EV1527_USER_NUM-1][0] = mma1;
			user[EV1527_USER_NUM-1][1] = mma2;
			Remote_setting_para.ev1527_user_id[EV1527_USER_NUM-1][0] = mma1;
			Remote_setting_para.ev1527_user_id[EV1527_USER_NUM-1][1] = mma2;
			count = 0;
			decode_ok = 1;
		}else{
			if (((rf_data[0] == user[EV1527_USER_NUM-1][0]) && (rf_data[1] == user[EV1527_USER_NUM-1][1])))
			{
				decode_ok = 1;
			}
		}
	}
}
void ev1527_key_scan(void)
{
	static uint8_t key_up = 1;
	// if (MODE() == 0)
	if (VT_KEY_STATE() == 0)
	{
		key_up = 1;
		// if (MODE() == 0)
		if (VT_KEY_STATE() == 0)
		{
			if (((mode & 0x80) == 0))
			{
				mode |= 0x80;
			}
			if (((mode & 0x40) == 0) && (count >= 5000) && (count <= 50000))
			{
				mode |= 0x40;
				memset(&user, 0, sizeof(user));
				len = 0;
				mode &= 0x7f;
				count = 0;
			}
		}
	}
	else
	{
		key_up = 0;
	}
}

void ev1527_set_userID(uint8_t *setUserID[], uint8_t userNum)
{
	memcpy(user, setUserID, sizeof(user[0]) * userNum);
}

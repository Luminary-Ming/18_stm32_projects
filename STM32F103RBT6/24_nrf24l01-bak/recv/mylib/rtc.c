#include "rtc.h"//包含头文件
#include "string.h"//包含头文件

const uint8_t monthTable[12] = {31,28,31,30,31,30,31,31,30,31,30,31};//存储每个月(平年)有多少天用于转换时间
static struct time_st T;//定义了日期时间结构体类型的全局变量

static uint8_t isLeapYear(uint16_t y)//判断是否是闰年
{
	return (((!(y % 4)) && (y % 100)) || (!(y % 400)));
}

void RTC_IRQHandler(void)//异常向量表中有关RTC秒中断的中断处理函数
{
	if(RTC_GetITStatus(RTC_IT_SEC) == SET)//判断是否是RTC的秒中断触发的该中断处理函数
	{
		conv_time(&T);//转换时间并保存到全局变量T中
		RTC_ClearITPendingBit(RTC_IT_SEC);//清除RTC秒中断的中断标志位
	}
}

void NVIC_Configuration(void)//配置有关RTC的中断控制器的配置信息
{
	NVIC_InitTypeDef Nvic_Value;//定义了中断控制器固件的结构体变量
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//配置组优先级和子优先级的所占比例
	Nvic_Value.NVIC_IRQChannel = RTC_IRQn;//选择中断号
	Nvic_Value.NVIC_IRQChannelCmd = ENABLE;//使能该中断
	Nvic_Value.NVIC_IRQChannelPreemptionPriority = 2;//配置组优先级
	Nvic_Value.NVIC_IRQChannelSubPriority = 2;//配置子优先级
	NVIC_Init(&Nvic_Value);//按照上述配置初始化NVIC
}

void RTC_Configuration(void)//配置RTC的初始化(如果第一次使用RTC需要调用该函数进行配置)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP | RCC_APB1Periph_PWR, ENABLE);//通过APB1使能BKP和PWR的时钟
	PWR_BackupAccessCmd(ENABLE);//通过PWR使能后备存储区域的写访问(关闭写保护)
	BKP_DeInit();//复位BKP
	RCC_LSEConfig(RCC_LSE_ON);//开启LSE时钟源
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);//等待LSE稳定
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);//为RTC选择由LSE提供时钟源
	RCC_RTCCLKCmd(ENABLE);//使能RTC的时钟
	RTC_WaitForSynchro();//等待RTC时钟同步
	RTC_WaitForLastTask();//等待最后一次对RTC寄存器的写操作完成
	RTC_ITConfig(RTC_IT_SEC, ENABLE);//使能RTC的秒中断
	RTC_WaitForLastTask();//等待最后一次对RTC寄存器的写操作完成
	RTC_EnterConfigMode();//进入RTC的配置模式
	RTC_SetPrescaler(32767);//设置RTC的预分频
	RTC_WaitForLastTask();//等待最后一次对RTC寄存器的写操作完成
	RTC_ExitConfigMode();//退出RTC的配置模式
}

void rtc_init(struct time_st *t)//初始化RTC
{
	NVIC_Configuration();//配置中断控制器
	if(BKP_ReadBackupRegister(BKP_DR1) != 0x9527)//判断BKP的寄存器中是否存入了特定的值(判断是否是第一次操作RTC)
	{
		RTC_Configuration();//配置RTC
		RTC_WaitForLastTask();//等待最后一次对RTC寄存器的写操作完成
		RTC_EnterConfigMode();//进入RTC的配置模式
		set_time(t);//设置时间
		RTC_WaitForLastTask();//等待最后一次对RTC寄存器的写操作完成
		RTC_ExitConfigMode();//退出RTC的配置模式
		BKP_WriteBackupRegister(BKP_DR1, 0x9527);//为BKP的寄存器中写入特定的值
	}
	else
	{
		RTC_WaitForSynchro();//等待RTC时钟同步
		RTC_WaitForLastTask();//等待最后一次对RTC寄存器的写操作完成
		RTC_ITConfig(RTC_IT_SEC, ENABLE);//使能RTC的秒中断
		RTC_WaitForLastTask();//等待最后一次对RTC寄存器的写操作完成
	}
}

void set_time(struct time_st* t)//设置时间(从参数t中存储的时间到1970/1/1 00:00:00的秒数存储到计数器中)
{
	uint32_t secCount = 0;//计算秒值
	uint16_t i;//循环标量

	if(t->Y < 1970 || t->Y > 2099)//判断年份是否超出了计算范围
		return ;
	
	for(i = 1970; i < t->Y; i++)//计算t->Y之前的年份的秒数和
	{
		if(isLeapYear(i))//判断是否是闰年
			secCount += 31622400;
		else
			secCount += 31536000;
	}

	for (i = 0; i < t->M - 1; i++)//计算t->M之前的月份的秒数和
	{
		secCount += ((uint32_t)monthTable[i] * 86400);
		if (isLeapYear(t->Y) && i == 1)//如果是闰年并且是二月份多加一天的秒数
			secCount += 86400;
	}
	secCount += (uint32_t)(t->D - 1) * 86400;//计算t->D之前的天数的秒数和
	secCount += (uint32_t)(t->h) * 3600;//计算t->h之前的小时的秒数和
	secCount += (uint32_t)(t->m) * 60;//计算t->m之前的分钟的秒数和
	secCount += (uint32_t)(t->s);//计算t->s的秒数

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR|RCC_APB1Periph_BKP, ENABLE);//通过APB1使能PWR和BKP的时钟
	PWR_BackupAccessCmd(ENABLE);//通过PWR使能后备存储区域的写访问(关闭写保护)
	RTC_SetCounter(secCount);//把计算的秒数和存储到计数器中
	RTC_WaitForLastTask();//等待最后一次对RTC寄存器的写操作完成
}

void conv_time(struct time_st* t)//转换时间(把计数器中存储的秒数和计算出年月日星期时分秒)
{
	uint32_t timeCount = 0;//存储从计数器中获取的秒数和
	uint32_t Count = 0;//中间变量
	uint16_t tmp = 0;//中间变量
	
	timeCount = RTC_GetCounter();//从计数器中获取秒数和
	Count = timeCount / 86400;//计算有多少天
	t->W = (4 + Count) % 7;//计算星期
	if(Count != 0)//计算年份
	{
		tmp = 1970;
		while(Count >= 365)
		{
			if(isLeapYear(tmp))
			{
				if(Count>= 366)
				{
					Count -= 366;
				}
				else
				{
					tmp++;
					break;
				}
			}
			else
			{
					Count -= 365;
			}
			tmp++;
		}
		t->Y = tmp;
		tmp = 0;
		while(Count >= 28)//计算月份
		{
			if(isLeapYear(t->Y) && tmp == 1)
			{
				if(Count >= 29) 
						Count -= 29;
				else    
						break;
			}
			else
			{
				if(Count >= monthTable[tmp]) 
					Count -= monthTable[tmp];
				else
					break;
			}
			tmp++;
		}
		t->M = tmp + 1;
		t->D = Count + 1;
    }
    Count = timeCount % 86400;
    t->h = Count / 3600;//计算小时数
    t->m = (Count % 3600) / 60;//计算分钟数
    t->s = (Count % 3600) % 60;//计算秒数
}

void get_time(struct time_st *t)//获取时间
{
	memcpy(t, &T, sizeof(T));//把T中获取到的时间拷贝到参数t中
}














#include "rtc.h"
#include "string.h"

static struct time_st T;  // 存储每一次获取的年月日星期时分秒

// 每个月的天数数组 (平年情况)
const u8 month_days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// 判断是否是闰年
static u8 is_leap_year(u16 year)
{
	return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// 获取月份天数
static u8 get_month_days(u16 year, u8 month)
{
	if(month == 2 && is_leap_year(year))  // 如果是闰年2月
		return 29;  
	return month_days[month - 1];  // month-1: 为了对应数组下标
}

void RTC_IRQHandler(void)
{
	if(RTC_GetITStatus(RTC_IT_SEC) == SET)  // 判断是否是由 RTC 的秒中断触发
	{
		conv_time(&T);  // 把时间戳转换成时间存储到全局变量中
		RTC_ClearFlag(RTC_IT_SEC);  // 清除 RTC 秒中断的中断标志位
	}
}

// 第一次操作 RTC 的配置
static void rtc_config(struct time_st *t)
{
	// 通过 APB1 总线使能 PWR(电源接口) 和 BKP(备份寄存器) 的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	PWR_BackupAccessCmd(ENABLE);  // 通过 PWR 使能备份区域的写访问(关闭写保护)
	BKP_DeInit();  // 复位 BKP 备份寄存器, 确保干净的初始状态
	
	RCC_LSEConfig(RCC_LSE_ON);  // 使能 LSE 外部低速时钟(通过 RCC 开启 LSE)
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);  // 以死等的方式等待 LSE 稳定(因为时钟源刚开启不稳定)
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);  // 通过 RCC 为 RTC 选择由 LSE 提供时钟来源
	RCC_RTCCLKCmd(ENABLE);  // 通过 RCC 使能 RTC 的时钟
	
	RTC_WaitForSynchro();  // 等待 RTC 的时钟同步
	RTC_WaitForLastTask();  // 等待最近一次对 RTC 寄存器写操作完成
	RTC_ITConfig(RTC_IT_SEC, ENABLE);  // 使能 RTC 的秒中断
	RTC_WaitForLastTask();  // 等待最近一次对 RTC 寄存器写操作完成
	
	RTC_EnterConfigMode();  // 进入 RTC 的配置模式
	
	RTC_SetPrescaler(32767);  // 设置 RTC 的预分频值(硬件会默认+1<为了防止写 0 分频>)
	RTC_WaitForLastTask();  // 等待最近一次对 RTC 寄存器写操作完成

	set_time(t);  // 把形参时间转换成时间戳, 设置为 RTC 的计数值
	RTC_WaitForLastTask();  // 等待最近一次对 RTC 寄存器写操作完成
	
	RTC_ExitConfigMode();  // 退出 RTC 的配置模式
}

// 初始化RTC的函数
void rtc_init(struct time_st *t)
{
	NVIC_InitTypeDef Nvic_Value;
	
	// 1.配置 NVIC
	// 由于需要 RTC 每一秒钟计数器自增 1, 所以需要使用秒中断
	// (无论是否是第一次使用 RTC, 都需要使能秒中断)
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  // 选择抢占优先级和响应优先级的所占比例为 2 : 2
	Nvic_Value.NVIC_IRQChannel = RTC_IRQn;  // 选择 RTC 的中断号
	Nvic_Value.NVIC_IRQChannelCmd = ENABLE;  // 使能该中断
	Nvic_Value.NVIC_IRQChannelPreemptionPriority = 2;  // 选择抢占优先级的级别为 2
	Nvic_Value.NVIC_IRQChannelSubPriority = 2;  // 选择响应优先级的级别为 2
	NVIC_Init(&Nvic_Value);  // 按照上述配置初始化 NVIC
	
	// 2.判断是否第一次使用 RTC
	// 因为如果第一次使用 RTC 需要配置 RTC 的时钟频率和计数值
	// 如果不是第一次使用 RTC 那就不需要配置的操作
	if(BKP_ReadBackupRegister(BKP_DR3) != 0x1234)  // 第一次使用 RTC
	{
		rtc_config(t);  // 初始化 RTC
		BKP_WriteBackupRegister(BKP_DR3, 0x1234);  // 把特定值写进 3 号备份寄存器
	}
	else  // 不是第一次使用 RTC
	{
		RTC_WaitForSynchro();  // 等待 RTC 的时钟同步
		RTC_WaitForLastTask();  // 等待最近一次对 RTC 寄存器写操作完成
		RTC_ITConfig(RTC_IT_SEC, ENABLE);  // 使能 RTC 的秒中断
		RTC_WaitForLastTask();  // 等待最近一次对 RTC 寄存器写操作完成
	}
}

// 根据基准时间(1970-1-1 00:00:00)转换成时间戳(从基准时间到当前时间的秒数)
void set_time(struct time_st *t)
{
	u32 total_seconds = 0;  // 存储时间戳 (从 1970-01-01 开始的秒数)

	// 计算从 1970 年到当前年份的前一年的总秒数
	// 如 2026 年, 则计算 1970~2025 年之间的总秒数
	u16 year = 0;  // 循环变量, 存储年份
	for (year = 1970; year < t->Y; year++)  // 从 1970 年遍历到目标年份的前一年    
	{
		total_seconds += is_leap_year(year) ? 31622400UL : 31536000UL;  // 366 * 24 * 3600 : 365 * 24 * 3600
	}
	
	// 计算当前年份已过月份的总秒数
	// 如 5 月, 则计算 1~4 月的总天数对应的秒数
	u8 month = 0;  // 循环变量, 存储月份
	for(month = 1; month < t->M; month++)
	{
		total_seconds += get_month_days(t->Y, month) * 86400UL;  // 24 * 3600
	}
    	
	// 当前月份已过天数的总秒数
	// 如 5月9日, 则计算已过 8 天的总秒数
	total_seconds += (t->D - 1) * 86400UL; // 24 * 3600
	
	// 时分秒的总秒数
	// 如 5月9日 21:22, 将小时、分钟、秒都转换为秒数累加
	total_seconds += (t->h * 3600) + (t->m * 60) + t->s;
	
	// 通过 APB1 总线使能 PWR 和 BKP 的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	PWR_BackupAccessCmd(ENABLE);  // 通过 PWR 使能备份区域的写访问(关闭写保护)
	RTC_SetCounter(total_seconds);  // 为 RTC 设置计数值
	RTC_WaitForLastTask();  // 等待最近一次对 RTC 寄存器写操作完成
}

// 根据基准时间(1970-1-1 00:00:00)把时间戳转换成当前时间
void conv_time(struct time_st *t)
{
	u32 total_seconds = RTC_GetCounter();  // 获取 RTC 的计数值(时间戳)
	
	u32 days = total_seconds / 86400UL;  // 计算总天数 (从 1970-1-1 开始计算的)
	
	u32 remaining_seconds = total_seconds % 86400UL;  // 计算不足一天的秒数 (用于计算当天的时间)
	
	// 计算星期几
	u8 weekday = (days + 4) % 7;  // 1970年1月1日是星期四, 所以加 4 进行偏移
	t->W = (weekday == 0) ? 7 : weekday;  // 转换星期日是 7
	
	// 计算年份
	u16 year = 1970;  // 从 1970 年开始循环
	while(1)  // 无限循环，直到找到正确的年份  
	{
		u16 year_days = is_leap_year(year) ? 366 : 365;  // 获取当前年份的天数 (闰年 366 天，平年 365 天)
        if(days < year_days) break;  // 如果剩余天数小于当前年份的天数，说明当前年份就是目标年份, 跳出循环
        days -= year_days;  // 否则减去当前年份的天数, 继续下一年
        year++;  // 年份加1
	}
	t->Y = year;  // 存储计算出的年份
	
	// 计算月份和日期
	u8 month = 1;  // 从 1 月开始循环
	while(month <= 12)  // 循环处理 12 个月
    {
        u8 month_days_count = get_month_days(year, month);  // 获取当前月份的天数 (考虑闰年2月的情况)
        if(days < month_days_count) break;   // 如果剩余天数小于当前月份的天数, 说明当前月份就是目标月份, 跳出循环
        days -= month_days_count;  // 否则减去当前月份的天数, 继续下一个月
        month++;  // 月份加1
    } 
    t->M = month;  // 存储计算出的月份
    
	// 计算日期
	t->D = days + 1;  // days 现在是当月内已经过去的天数, 加 1 得到实际日期 (从 1 号开始数)
    
    // 计算时分秒
    t->h = remaining_seconds / 3600;
    t->m = (remaining_seconds % 3600) / 60;
    t->s = remaining_seconds % 60;
}

// 获取时间 
void get_time(struct time_st *t)
{
	memcpy(t, &T, sizeof(T));
}


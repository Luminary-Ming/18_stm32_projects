#include "rtc.h"


// 判断是否是闰年
static int is_leap_year(int year)
{
	return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// 根据基准时间(1970-1-1 00:00:00)转换成时间戳(从基准时间到当前时间的秒数)
void set_time(struct time_st *t)
{
	u16 year = 0;  // 循环变量, 存储年份
	u32 days = 0;  // 存储当前年份的月份总天数
	u32 total_seconds = 0;  // 存储时间戳

	for (year = 1970; year < t->Y; year++)  // 计算从 1970 年到当前年份的前一年的总秒数
	{
		total_seconds += is_leap_year(year) ? 366 * 24 * 3600 : 365 * 24 * 3600;
	}
	
	 switch (t->M)  // 计算月份总天数
	 {
		 case 12: days += 30;  // 11月总天数
		 case 11: days += 31;  // 10月总天数
		 case 10: days += 30;  //  9月总天数
		 case  9: days += 31;  //  8月总天数
		 case  8: days += 31;  //  7月总天数
		 case  7: days += 30;  //  6月总天数
		 case  6: days += 31;  //  5月总天数
		 case  5: days += 30;  //  4月总天数
		 case  4: days += 31;  //  3月总天数
		 case  3: days += (28 + is_leap_year(t->Y));  // 2月总天数
		 case  2: days += 31;  // 1月总天数
		 case  1: days += t->D;  // 当前月总天数
	 }
	 
	 total_seconds += days * 24 * 3600;  // 月份总天数的总秒数
	 
	 total_seconds += (t->h * 3600) + (t->m * 60) + t->s;  // 时分秒的总秒数
	
	 t->s = total_seconds;  // 把时间戳存进结构体
}

// 根据基准时间(1970-1-1 00:00:00)把时间戳转换成当前时间
void conv_time(struct time_st *t)
{
	u32 total_seconds = t->s;  // total_seconds 变量存储时间戳
	u32 days = 0;  // 存储总天数
	u32 remaining_seconds = 0;  // 存储不足一天的剩余秒数
	u8 weekday = 0;  // 存储星期几
	
	u16 year = 1970;
	u8 month = 1;
	u8 day = 0;
	
	// 计算总天数 (从 1970-1-1 开始计算的)
	days = total_seconds / (24 * 3600);
	
	// 计算不足一天的秒数
	remaining_seconds = total_seconds % (24 * 3600);
	
	// 计算星期几 (+4: 1970年1月1日是星期四)
	weekday = (days + 4) % 7;
	// 转换星期日是 7
	if(weekday == 0) 
		weekday = 7;  
	
	// 计算年份
	while(days >= 365) 
	{
		if(is_leap_year(year))  // 如果是闰年
		{
			if (days >= 366) 
			{
                days -= 366;
                year++;
			}
		}
		else
		{
			days -= 365;
			year++;
		}	
	}
	
	// 计算月份和星期
	while()
	
	
	
}
#ifndef __RTC_H
#define __RTC_H
/*
该项目工程使用的是STM32F103RBT6系列的芯片
内部已经集成了RTC外设,可以直接进行使用
芯片内部的RTC外设的计数器需要存储的是秒数
而且最多到2的32次方减1的大小,并不是无限大
我们这次所代表的的时间范围是
1970/1/1/ 00:00:00 ------ 2099/12/31 23:59:59
所以如果我们第一次驱动RTC的话需要设置时间
也就意味着将来计数器中需要存储
例子:如果需要存储2019-9-20 09:00:00
需要计算此时此刻到1970/1/1/ 00:00:00有多少秒
存储到计数器中
*/
#include "stm32f10x_conf.h"//包含了该头文件所有的外设头文件均已包含

struct time_st//定义了日期时间的结构体
{
	int Y;//年份
	int M;//月份
	int D;//日期
	int W;//星期
	int h;//小时
	int m;//分钟
	int s;//秒数
};

extern void rtc_init(struct time_st *t);//初始化RTC

extern void set_time(struct time_st *t);//设置时间

extern void conv_time(struct time_st* t);//转换时间

extern void get_time(struct time_st *t);//获取时间

#endif









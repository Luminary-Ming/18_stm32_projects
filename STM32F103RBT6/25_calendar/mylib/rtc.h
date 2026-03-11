#ifndef __RTC_H
#define __RTC_H
/*
    驱动RTC外设,由于RTC属于定时器资源
    时钟频率 : LSE / 分频 = 1Hz
    计数值 : 从基准时间到当前时间的秒数
*/
#include "stm32f10x_conf.h"

typedef uint32_t  u32;  // unsigned int
typedef uint16_t  u16;  // unsigned short
typedef uint8_t   u8;   // unsigned char

struct time_st
{
	int Y;  // 年份
	int M;  // 月份
	int D;  // 日期
	int W;  // 星期
	int h;  // 小时
	int m;  // 分钟
	int s;  // 秒数
};

extern void rtc_init(struct time_st *t);  // 初始化RTC的函数

// 根据基准时间(1970-1-1 00:00:00)转换成时间戳(从基准时间到当前时间的秒数)
extern void set_time(struct time_st *t);  // 设置时间

// 根据基准时间(1970-1-1 00:00:00)把时间戳转换成当前时间
extern void conv_time(struct time_st *t);  // 转换时间

extern void get_time(struct time_st *t);  // 获取时间 

#endif


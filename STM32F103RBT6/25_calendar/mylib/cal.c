#include "rtc.h"
#include "oled.h"
#include "stdio.h"
#include "string.h"

// 月份英文缩写
const char *month_names[] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

// 星期英文缩写（周一开始）
const char *weekdays[] = {
    "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"
};

// 显示简洁日历
void display_calendar(struct time_st *t)
{
    u8 buf[20];
    u16 year = t->Y;
    u8 month = t->M;
    u8 day = t->D;
    u8 wday = t->W;  // 1=周一, 7=周日
    u8 hour = t->h;
    u8 minute = t->m;
    u8 second = t->s;
    u8 font_size = 16;  // 16号字体
    u8 char_width = 8;   // 16号字体每个字符宽8像素
    
    // 清除屏幕
    OLED_Clear();
    
    // 第0行：月份年份（居中）
    // 例如 "Mar 2026"
    sprintf((char *)buf, "%s %d", month_names[month-1], year);
    u8 title_len = strlen((char *)buf) * char_width;
    u8 title_x = (128 - title_len) / 2;
    OLED_ShowString(title_x, 0, buf, font_size);
    
    // 第1行：星期 + 日期（居中）
    // 例如 "Tue 10"
    // wday: 1=周一, 2=周二, ..., 7=周日
    sprintf((char *)buf, "%s %d", weekdays[wday-1], day);
    u8 date_len = strlen((char *)buf) * char_width;
    u8 date_x = (128 - date_len) / 2;
    OLED_ShowString(date_x, 16, buf, font_size);
    
    // 第2行：当前时间（居中）
    // 例如 "14:25:30"
    sprintf((char *)buf, "%02d:%02d:%02d", hour, minute, second);
    u8 time_len = strlen((char *)buf) * char_width;
    u8 time_x = (128 - time_len) / 2;
    OLED_ShowString(time_x, 32, buf, font_size);
}


#include "delay.h"
#include "usart1.h"
#include "stdio.h"
#include "oled.h"
#include "sht30.h"
#include "eeprom.h"
#include "adj_res.h"
#include "rtc.h"
#include "string.h"

// 解析编译时间字符串
void parse_compile_time(struct time_st *t)
{
    // __DATE__ 格式: "Mar 10 2026"
    // __TIME__ 格式: "14:30:25"
    char month_str[4];
    int year, day, hour, minute, second;
    
    // 解析日期
    sscanf(__DATE__, "%s %d %d", month_str, &day, &year);
    
    // 月份转换
    const char *months[] = {"Jan","Feb","Mar","Apr","May","Jun",
                            "Jul","Aug","Sep","Oct","Nov","Dec"};
    for(int i = 0; i < 12; i++)
    {
        if(strcmp(month_str, months[i]) == 0)
        {
            t->M = i + 1;
            break;
        }
    }
    
    t->Y = year;
    t->D = day;
    
    // 解析时间
    sscanf(__TIME__, "%d:%d:%d", &hour, &minute, &second);
    t->h = hour;
    t->m = minute;
    t->s = second;
}

int main(void)
{	   
    u8 oled_buf1[64] = {0};
    u8 oled_buf2[64] = {0};
    
    struct time_st ot = {0};  // 初始时间
    struct time_st nt = {0};  // 当前时间

    delay_init();
    usart1_init();
    OLED_Init();
    sht_init();
    eeprom_init();
    adj_res_init();
    
	parse_compile_time(&ot);
	printf("11: %d-%02d-%02d %02d:%02d:%02d\r\n", 
		   ot.Y, ot.M, ot.D, ot.h, ot.m, ot.s);
        
	rtc_init(&ot);
    OLED_Clear();
    OLED_ShowString(0, 0, (unsigned char *)"RTC Test", 16);
    
    while(1)
    {	
        get_time(&nt);
  
        sprintf((char *)oled_buf1, "%d-%02d-%02d %d", nt.Y, nt.M, nt.D, nt.W);
        sprintf((char *)oled_buf2, "%02d:%02d:%02d", nt.h, nt.m, nt.s);
        
        OLED_ShowString(0, 2, (u8 *)oled_buf1, 16);
        OLED_ShowString(0, 4, (u8 *)oled_buf2, 16);
        
        delay_ms(500);
    }
}


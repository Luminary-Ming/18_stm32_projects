#include "delay.h"
#include "usart1.h"
#include "stdio.h"
#include "oled.h"
#include "sht30.h"
#include "eeprom.h"
#include "adj_res.h"
#include "rtc.h"
#include "cal.h"

int main(void)
{  
    struct time_st ot = {2026, 3, 10, 2, 9, 55, 0};  // 设置时间
    struct time_st nt = {0};
    
    delay_init();
    usart1_init();
    OLED_Init();
    sht_init();
    eeprom_init();
    adj_res_init();
    rtc_init(&ot);
    
    OLED_Clear();
    
    while(1)
    {
        get_time(&nt);
        
        // 显示简洁日历
        display_calendar(&nt);
        
        delay_ms(500);
    }
}


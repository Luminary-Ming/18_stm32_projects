#include "led.h"
#include "buzzer.h"
#include "button.h"
#include "delay.h"
#include "ldt.h"
#include "bitband.h"
#include "ds18b20.h"

int main(void)
{
    short temp_raw = 0;   // 存储 DS18B20 返回的放大 100 倍的温度值
    u8 show_buf[4] = {0}; // 数码管显示缓冲区
    u8 i;

    led_init();
    buzzer_init();
    button_init();
    delay_init();
    ldt_init();      // 初始化数码管
    ds18b20_init();  // 初始化 DS18B20

    // 开机提示
    for(i = 0; i < 2; i++) {
        buzzer_on();
        delay_ms(100);
        buzzer_off();
        delay_ms(100);
    }

    while(1)
    {
        // 获取温度 (假设 ds18b20_get_temp 返回 2556 代表 25.56度)
        temp_raw = ds18b20_get_temp();

        // 数据拆解与数码管逻辑处理
        if (temp_raw >= 0) {
            // 正数显示逻辑：例如 25.56 度 -> 显示 "25.5"
            show_buf[0] = temp_raw / 1000;          // 十位：2
            show_buf[1] = (temp_raw % 1000) / 100;  // 个位：5 (带小数点)
            show_buf[2] = (temp_raw % 100) / 10;    // 十分位：5
            show_buf[3] = temp_raw % 10;  
            
            // 调用数码管显示函数，注意：你需要修改 ldt_display 支持小数点
            // 这里假设你的 ldt_display(位置, 数值, 是否点亮小数点)
			for(int i = 0; i < 250; i++)
			{
				ldt_display(0, show_buf[0], 0); 
				ldt_display(1, show_buf[1], 1); // 第二位点亮小数点
				ldt_display(2, show_buf[2], 0);
				ldt_display(3, show_buf[3], 0);
			}
           
        } 
        else {
            // 负数显示逻辑：例如 -12.34 度 -> 显示 "-12.3"
            short abs_temp = -temp_raw;
            show_buf[0] = 16;                       // 16 是负号 '-'
            show_buf[1] = abs_temp / 1000;          // 十位
            show_buf[2] = (abs_temp % 1000) / 100;  // 个位 (带小数点)
            show_buf[3] = (abs_temp % 100) / 10;    // 十分位
            
			for(int i = 0; i < 250; i++)
			{
				ldt_display(0, 16, 0);          // 显示负号
				ldt_display(1, show_buf[1], 0);
				ldt_display(2, show_buf[2], 1); // 小数点
				ldt_display(3, show_buf[3], 0);
			} 
        }
    }
}


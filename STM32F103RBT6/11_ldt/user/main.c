#include "led.h"
#include "buzzer.h"
#include "button.h"
#include "delay.h"
#include "ldt.h"

int main(void)
{
	int i = 0;  // 循环变量
	int j = 0;  // 循环变量
	unsigned char ledcode[] = {0x3f,0x06,0x5b,0x4f,   // 0 1 2 3
								0x66,0x6d,0x7d,0x07,  // 4 5 6 7
								0x7f,0x6f,0x77,0x7c,  // 8 9 A b
								0x39,0x5e,0x79,0x71}; // c d E F
	
	led_init();  // 初始化 led
	buzzer_init();  // 初始化蜂鸣器                                                      
	button_init();  // 初始化 button
	delay_init();  // 初始化延时函数
	ldt_init();  // 初始化数码管
								
								
	for(i = 0; i < 2; i++)  // 开机提示：蜂鸣器响 2 声
	{
		buzzer_on();
		delay_ms(100);
		buzzer_off();
		delay_ms(100);
	}
	
	// 主循环：测试数码管显示
	while(1)
	{
		for(j = 0; j < 250; j++)  // 连续调用 250 次，让5555显示约1秒钟（250 * 4ms = 1000ms）
		{
			digit_show_data(5555);	 // 执行一轮4ms的扫描
		}
		
		// 测试1：逐位显示数字 1 2 3 4（每位持续0.5秒）
		// 第一位显示1
		hc138_out_data(0);
		hc595_send_data(ledcode[1]);
		delay_ms(500);
		
		// 第二位显示2
		hc138_out_data(1);
		hc595_send_data(ledcode[2]);
		delay_ms(500);
		
		// 第三位显示3
		hc138_out_data(2);
		hc595_send_data(ledcode[3]);
		delay_ms(500);
		
		// 第四位显示4
		hc138_out_data(3);
		hc595_send_data(ledcode[4]);
		delay_ms(500);
	
		// 全部熄灭0.5秒
		hc595_send_data(0);
		delay_ms(500);
	}
}


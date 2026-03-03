#include "led.h"
#include "buzzer.h"
#include "button.h"
#include "delay.h"
#include "ldt.h"
#include "bitband.h"
#include "dht.h"
#include "eint.h"
#include "iwdg.h"

int flag = 1;  // 定义全局变量 flag 作为标记

void h0(void)  // KEY0 按下要做的事情
{
	flag = 0;  // 标记变为 0
	led_on(0);
	led_on(1);
	led_on(2);
}

void h1(void)  // KEY1 按下要做的事情
{
	u8 i = 0;  // 循环变量
	for(i = 0; i < 2; i++)  // 蜂鸣器响 2 声
	{
		buzzer_on();
		delay_ms(100);
		buzzer_off();
		delay_ms(100);
	}
}

void h2(void)  // KEY2 按下要做的事情
{
	led_off(0);
	led_off(1);
	led_off(2);
}

int main(void)
{
	u8 i = 0;  // 循环变量
	u8 j = 0;  // 循环变量
	u8 dht_data[5] = {0};  // 存储 DHT11 检测的数据
	
	led_init();  // 初始化 led
	buzzer_init();  // 初始化蜂鸣器                                                      
	button_init();  // 初始化 button
	delay_init();  // 初始化延时函数
	ldt_init();  // 初始化数码管
	dht_init();  // 初始化 DHT11 传感器 
	eint_init();  // 初始化 EINT
	
			
	set_eint_handler(h0, h1, h2);  // 设置功能按键的回调函数
								
	//for(i = 0; i < 2; i++)  // 开机提示：蜂鸣器响 2 声
	//{
		//buzzer_on();
		//delay_ms(100);
		//buzzer_off();
		//delay_ms(100);
	//}
	
	buzzer_on();
	iwdg_init(6 * 625);  // 初始化 IWDG
	delay_ms(4000);
	buzzer_off();
	
	while(flag)
	{
		iwdg_feed_dog();  // 喂狗
		led_on(0);
		delay_ms(500);
		led_off(0);
		delay_ms(500);
	}
	
	// 主循环
	while(1)
	{
		get_dht_value(dht_data);  // 获取 DHT11 采集的数据
		
		// 只有当校验和正确时才更新显示
		if(dht_data[4]!= 0 && (dht_data[4] == (dht_data[0] + dht_data[1] + dht_data[2] + dht_data[3])))
		{
			for(j = 0; j < 250; j++)  // 连续调用 250 次，让5555显示约1秒钟（250 * 4ms = 1000ms）
			{
				//led_on(0);  // 开启 LED0 表示当前是温度
				digit_show_data(dht_data[2]);  // 显示温度
			}
			//led_off(0);
			
			for(j = 0; j < 250; j++)  // 连续调用 250 次，让5555显示约1秒钟（250 * 4ms = 1000ms）
			{
				//led_on(1);  // 开启 LED0 表示当前是温度
				digit_show_data(dht_data[0]);  // 显示湿度
			}
			//led_off(1);
		}
	}
}


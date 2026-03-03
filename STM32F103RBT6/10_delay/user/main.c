#include "led.h"
#include "buzzer.h"
#include "button.h"
#include "delay.h"

int main(void)
{
	int i = 0;  // 循环变量
	
	led_init();  // 初始化 led
	buzzer_init();  // 初始化蜂鸣器                                                      
	button_init();  // 初始化 button
	delay_init();  // 初始化延时函数
	
	for(i = 0; i < 3; i= (i + 1) % 3)
	{
		buzzer_on();
		delay_ms(100);
		buzzer_off();
		delay_ms(100);
	}
}


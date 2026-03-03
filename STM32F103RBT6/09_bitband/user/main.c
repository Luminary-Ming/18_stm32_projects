#include "led.h"
#include "buzzer.h"
#include "button.h"

int main(void)
{
	int i = 0;  // 循环变量
	int is_press_down = 0;  // 是否按下的标志 1按下 0抬起
	//int is_bright = 0;  // LED 灯是否明亮 1明亮 0熄灭
	
	led_init();  // 初始化 led
	buzzer_init();  // 初始化蜂鸣器
	button_init();  // 初始化 button
	
	for(i = 0; i < 3; i = (i + 1) % 3)  // 循环遍历三个按键 KEY0 KEY1 KEY2
	{
		
		is_press_down = button_status(i);  // 获取当前按钮的状态
		
		if(is_press_down == 1)  // 判断按键是否按下
		{
			buzzer_on();
			led_on(i);
		}
		else
		{
			buzzer_off();
			led_off(i);
		}
	}
}


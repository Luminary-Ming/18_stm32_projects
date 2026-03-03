#include "led.h"
#include "buzzer.h"

int main(void)
{
	int current_led = 0;  // 当前要点亮的 LED 灯编号
	
	led_init();  // 初始化 led
	buzzer_init();  // 初始化蜂鸣器
	
	while(1)  // 让单片机的程序死在 main() 中, 不让它结束
	{
		led_on(current_led);
		buzzer_on();  // 蜂鸣器响
		
		delay(1500);  // 保持亮和响的状态
		
		led_off(current_led);
		buzzer_off();  // 蜂鸣器关
		
		delay(1200);  //  保持灭和静音的状态
		
		current_led = (current_led + 1) % 3;  // 切换 LED
	}
}




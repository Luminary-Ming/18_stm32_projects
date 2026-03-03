#include "led.h"
#include "buzzer.h"
#include "button.h"

int main(void)
{
	int i = 0;  // 循环变量
	int led_status[3] = {0, 0 ,0};  // 记录三个 LED 的状态 1亮 0灭
	int last_button_status[3] = {0, 0, 0};  // 记录上一次扫描时按键的状态
	
	led_init();  // 初始化 led
	buzzer_init();  // 初始化蜂鸣器
	button_init();  // 初始化 button
	
	while(1)  // 主程序无限循环
	{
		for(i = 0; i < 3; i++)  // 依次检查三个按键（KEY0, KEY1, KEY2）
		{
			int current_status = button_status(i);  // 获取当前按键状态 1按下 0抬起
			
			if(current_status != last_button_status[i])  // 如果当前状态不等于上次记录的状态，说明按键动作发生了变化
			{
				delay(20);  // 消抖, 机械按键按下时会有 5-10ms 的杂乱抖动，延时 20ms 跳过这段抖动
				
				// 二次确认, 延时后再读一次，如果状态变了，说明刚才可能只是电磁干扰（杂波），直接跳过
				if(button_status(i) != current_status) continue;
				
				if(current_status == 1)  // 从 0 变到 1, 表示按键刚刚被按下
					buzzer_on();  // 蜂鸣器响
				else  // 从 1 变到 0, 表示按键刚刚被抬起
				{
					buzzer_off();  // 蜂鸣器关
					
					led_status[i] = !led_status[i];  // 翻转 LED 状态
					led_status[i] ? led_on(i) : led_off(i);
				}
			}
			last_button_status[i] = current_status;
		}
		delay(50);  // 扫描完一轮后稍微歇一下，防止 CPU 空转过热，增加系统稳定性
	}
}


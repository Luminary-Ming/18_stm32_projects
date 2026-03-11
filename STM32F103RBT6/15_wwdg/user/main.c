#include "led.h"
#include "buzzer.h"
#include "button.h"
#include "delay.h"
#include "ldt.h"
#include "bitband.h"
#include "dht.h"
#include "eint.h"
#include "iwdg.h"
#include "wwdg.h"

/*
	实验现象说明：
	如果系统正常运行（喂狗时间正确）：LED1 会持续规律闪烁
	如果复位（喂狗太早或太晚）：LED0 会在启动时快速闪烁 3 次
*/

int main(void)
{
	led_init();  // 初始化 led
	buzzer_init();  // 初始化蜂鸣器                                                      
	button_init();  // 初始化 button
	delay_init();  // 初始化延时函数
	ldt_init();  // 初始化数码管
	dht_init();  // 初始化 DHT11 传感器 
	eint_init();  // 初始化 EINT
	
	// --- 复位指示灯 ---
    // 每次重启时，LED0 快闪 3 次, 如果你看到它一直在闪, 说明 WWDG 一直在触发重启
    for(int i = 0; i < 3; i++) {
        led_on(0);
        delay_ms(100);
        led_off(0);
        delay_ms(100);
    }
	
	wwdg_init(); // 初始化 WWDG
	
	while (1)
    {
        /* * WWDG 核心逻辑模拟：
         * 假设你的 WWDG 配置是：计数器初值 0x7F，窗口值 0x5F。
         * 你必须等计数器从 0x7F 减到 0x5F 以下，才能喂狗。
         */
        
        // 步骤 A：进入等待区（避开“禁止喂狗区”）
        // 如果这个延迟太短（比如设为 1ms），系统会因为“喂狗过早”直接复位
        delay_ms(35); 
        
        // 步骤 B：执行喂狗
        // 如果步骤 A + 步骤 B 的总时间超过了计数器减到 0x3F 的时间，系统会因为“喂狗过晚”复位
        wwdg_feed_dog();
        
        // 步骤 C：工作指示灯
        // LED1 翻转，表示程序完整跑完了一圈，没被复位
		led_on(1);
		led_on(2);
        delay_ms(10);
		led_off(1);
		led_off(2);
    }
}


#ifndef WWDG_H
#define WWDG_H

/*
	驱动窗口看门狗
	时钟频率: PCLK1 / 4096 / 分频系数 (1 2 4 8)
	PCLK1 = 36MHz
	WWDG 时钟 = 36MHz / 4096 / 8 = 1098Hz
	计数周期 = 1 / 1098Hz ≈ 0.91ms
	计数值范围: 7bit 的计数器 (64 ~ 127)
	溢出时间 = (0x7F - 0x40) / 1098Hz = (127 - 64) / 1098Hz ≈58ms
	喂狗窗口 = (0x5F - 0x5F) / 1098Hz = (127 - 95) / 1098Hz ≈29ms
	
	计数器值: 0x7F(127) → ... → 0x5F(95) → ... → 0x40(64) → 复位
	时间轴:   0ms          ...    29ms        ...    58ms        超时
			  |<--- 禁止喂狗 --->|<--- 允许喂狗 --->|
			  |                  |                  |
		   不能喂狗           可以喂狗           即将复位
		   (喂狗会复位)        (正常喂狗)         (触发中断)
		   
	只能在 29ms ~ 58ms之间喂狗, 否则会复位
*/
#include "stm32f10x_conf.h"


extern void wwdg_init(void);  // 初始化 WWDG

extern void wwdg_feed_dog(void);  // 喂狗

#endif


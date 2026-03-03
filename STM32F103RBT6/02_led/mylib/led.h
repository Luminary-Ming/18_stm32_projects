#ifndef LED_H
#define LED_H

/*
	LED0 PC1
	LED1 PC2
	LED2 PC3
	输出高电平, 灯亮
	输出低电平, 灯灭
*/

#include "stm32f10x_conf.h"  // 包含这一个头文件, 所有外设的头文件就都包含了

#define GPIOC_CRL *(volatile unsigned int *)(GPIOC_BASE + 0x00)
#define GPIOC_ODR *(volatile unsigned int *)(GPIOC_BASE + 0x0C)
	
extern void led_init(void);  // 初始化 LED 使用的 GPIO 管脚
extern void led_on(int num);   // 通过形参 num 点亮响应的 LED 灯
extern void led_off(int num);  // 通过形参 num 熄灭响应的 LED 灯

#endif


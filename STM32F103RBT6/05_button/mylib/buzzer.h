#ifndef BUZZE_H
#define BUZZE_H

/*
	蜂鸣器 PC7
	输出高电平, 蜂鸣器响
	输出低电平, 蜂鸣器关
*/

#include "stm32f10x_conf.h"  // 包含这一个头文件, 所有外设的头文件就都包含了

#define GPIOC_CRL *(volatile unsigned int *)(GPIOC_BASE + 0x00)
#define GPIOC_ODR *(volatile unsigned int *)(GPIOC_BASE + 0x0C)

extern void buzzer_init(void);  // 初始化蜂鸣器
extern void buzzer_on(void);   // 打开蜂鸣器
extern void buzzer_off(void);  // 关闭蜂鸣器
extern void delay(unsigned int count);  // 简单的延时函数

#endif


#ifndef EINT_H
#define EINT_H

/*
	功能按键  管脚	EXIT号  中断号
	KEY0      PC9   EXTI9   EXTI9_5
	KEY1      PC8   EXTI8   EXTI9_5
	KEY2      PA0   EXTI0   EXTI0
	
	PC9 PC8 配置为上拉输入模式
	    PA0 配置为下拉输入模式
*/
#include "stm32f10x_conf.h"

typedef void (*eint_handler)(void);  // 定义函数指针类型

extern void eint_init(void);  // 初始化功能按键使用的 GPIO 管脚和外部中断以及中断控制器

extern void set_eint_handler(eint_handler h0, eint_handler h1, eint_handler h2);  // 设置回调函数

#endif


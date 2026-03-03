#ifndef BUTTON_H
#define BUTTON_H

/*
	KEY0  PC9
	KEY1  PC8
	KEY2  PA0
	PC9 PC8 配置为上拉输入模式
	PA0 配置为下拉输入模式
*/

#include "stm32f10x_conf.h"

#define GPIOA_CRL *(volatile unsigned int *)(GPIOA_BASE + 0x00)
#define GPIOA_IDR *(volatile unsigned int *)(GPIOA_BASE + 0x08)
#define GPIOA_ODR *(volatile unsigned int *)(GPIOA_BASE + 0x0C)

#define GPIOC_CRH *(volatile unsigned int *)(GPIOC_BASE + 0x04)
#define GPIOC_IDR *(volatile unsigned int *)(GPIOC_BASE + 0x08)
#define GPIOC_ODR *(volatile unsigned int *)(GPIOC_BASE + 0x0C)

extern void button_init(void);  // 初始化功能按键使用的 GPIO 管脚
extern int button_status(int num);  // 根据形参检测相应按键的电平高低

#endif


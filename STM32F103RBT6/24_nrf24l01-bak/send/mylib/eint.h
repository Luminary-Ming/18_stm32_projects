#ifndef __EINT_H
#define __EINT_H
/*
开发板设备		所连接的管脚		所使用的外部中断编号
KEY0					PC9							EXTI9
KEY1					PC8							EXTI8
从原理图可知
当功能按键按下检测到GPIO管脚为				低电平
当功能按键没有按下检测到GPIO管脚为		高电平
可以配置为下降沿触发外部中断
*/
#include "stm32f10x_conf.h"//包含了该头文件所有的外设头文件均已包含

typedef void (*eint_handler)(void);//通过typedef定义函数指针类型

extern void eint_init(void);//初始化的函数(初始化所连接的GPIO管脚和中断控制器以及外部中断)

extern void set_eint_handler(eint_handler h0, eint_handler h1);//设置回调函数

#endif









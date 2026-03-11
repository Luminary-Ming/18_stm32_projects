#ifndef __LED_H
#define __LED_H
/*
开发板设备		所连接的管脚
LED0					PC1
LED1					PC2
LED2					PC3
从原理图可知
GPIO管脚输出高电平	LED灯亮
GPIO管脚输出低电平	LED灯灭
*/
#include "stm32f10x_conf.h"//包含了该头文件所有的外设头文件均已包含

extern void led_init(void);//初始化的函数(初始化所连接的GPIO管脚)

extern void led_on(int nu);//根据参数nu点亮相应的led灯的函数

extern void led_off(int nu);//根据参数nu关闭相应的led灯的函数

#endif





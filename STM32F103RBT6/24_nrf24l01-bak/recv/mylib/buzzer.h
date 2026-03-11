#ifndef __BUZZER_H
#define __BUZZER_H
/*
开发板设备		所连接的管脚
BEEP					PC7
从原理图可知
GPIO管脚输出高电平	蜂鸣器响
GPIO管脚输出低电平	蜂鸣器不响
*/
#include "stm32f10x_conf.h"//包含了该头文件所有的外设头文件均已包含

extern void buzzer_init(void);//初始化的函数(初始化所连接的GPIO管脚)

extern void buzzer_on(void);//使蜂鸣器响的函数

extern void buzzer_off(void);//使蜂鸣器不响的函数

#endif







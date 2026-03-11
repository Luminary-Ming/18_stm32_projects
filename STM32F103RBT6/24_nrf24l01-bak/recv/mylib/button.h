#ifndef __BUTTON_H
#define __BUTTON_H
/*
开发板设备		所连接的管脚
KEY0					PC9
KEY1					PC8
从原理图可知
当功能按键按下检测到GPIO管脚为				低电平
当功能按键没有按下检测到GPIO管脚为		高电平
*/
#include "stm32f10x_conf.h"//包含了该头文件所有的外设头文件均已包含

extern void button_init(void);//初始化的函数(初始化所连接的GPIO管脚)

extern int button_status(int nu);//根据参数nu检测相应的功能按键是否被按下

#endif








#ifndef __IWDG_H
#define __IWDG_H
/*
有关独立看门狗的操作
从STM32F10X的官方手册可知,独立看门狗的时钟源可以是由LSI提供(可以经过分频)
从STM32F10X的官方手册可知,独立看门狗的计数值为0 ~ 2的12次方减1(0 ~ 4095)
*/
#include "stm32f10x_conf.h"//包含了该头文件所有的外设头文件均已包含

extern void iwdg_init(int ms);//初始化的函数(包括设置预分频器和计数值)

extern void iwdg_feed_dog(void);//喂狗的功能函数

#endif







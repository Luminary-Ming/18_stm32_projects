#ifndef __ADJ_RES_H
#define __ADJ_RES_H
/*
开发板设备			所连接的管脚
高精度可调电阻	PC0/ADC_IN10
已知芯片的管脚是多功能复用型的管脚
芯片的管脚会有 输入功能 输出功能 复用功能 模拟功能
从原理图可知开发板中有关AD转换的模块除了预留的GPIO接口以外
开发板上的高精度可调电阻所连接的GPIO管脚也有AD转换功能
ADC_VOL		PC0/ADC_IN10
*/
#include "stm32f10x_conf.h"//包含了该头文件所有的外设头文件均已包含

extern void adj_res_init(void);//初始化高精度可调电阻的函数(初始化所连接的GPIO管脚以及AD转换)

extern int get_adj_res_value(void);//获取AD转换之后的结果

#endif






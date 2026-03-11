#ifndef __LDT_H
#define __LDT_H
/*
开发板设备		所连接的管脚
HC138_A0			PC11
HC138_A1			PC12
HC138_A2			PD2
HC595_SCLK		PB5
HC595_LCLK		PB4
HC595_DATA		PB3
已知芯片的管脚是多功能复用型的管脚
芯片的管脚会有 输入功能 输出功能 复用功能 模拟功能
我们这次操作的数码管为四位共阴极的数码管
公共端连接了74HC138
LED灯的端连接了74HC595
*/
#include "stm32f10x_conf.h"//包含了该头文件所有的外设头文件均已包含

extern void ldt_init(void);//初始化数码管的函数(初始化所连接的GPIO管脚)

extern void hc138_out_data(uint8_t data);//控制74HC138模块选择公共端

extern void hc595_send_data(uint8_t data);//控制74HC595模块发送数据

extern void digit_show_data(int data);//在数码管中显示数据

#endif








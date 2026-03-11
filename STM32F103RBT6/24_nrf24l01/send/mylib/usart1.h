#ifndef __USART1_H
#define __USART1_H
/*
开发板设备		所连接的管脚
USART1_TX					PA9
USART1_RX					PA10
已知芯片的管脚是多功能复用型的管脚
芯片的管脚会有 输入功能 输出功能 复用功能 模拟功能
我们这次使用的是串口,所以选择复用功能
*/
#include "stm32f10x_conf.h"//包含了该头文件所有的外设头文件均已包含

typedef void (*usart1_handler)(unsigned char);//通过typedef定义函数指针类型

extern void usart_1_init(void);//初始化的函数(初始化所连接的GPIO管脚和串口功能以及中断控制器)

extern void usart_1_send_byte(unsigned char c);//发送一个字节数据

extern void usart_1_send_data(char *buf);//发送一个字符串数据

extern unsigned char usart_1_recv_byte(void);//接收一个字节数据(轮询的方式)

extern void set_usart1_handler(usart1_handler h);//设置回调函数

#endif










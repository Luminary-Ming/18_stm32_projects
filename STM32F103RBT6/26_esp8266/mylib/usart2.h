#ifndef USART2_H
#define USART2_H

/*
	发送  USART2_TX  PA2
	接收  USART2_RX  PA3
	
	PA2  配置为推挽复用模式
	PA3  配置为浮空输入模式
*/
#include "stm32f10x_conf.h"

typedef void (*usart2_handler)(u8);  // 定义了函数指针类型

extern void usart2_init(void);  // 初始化 USART2 使用的管脚

extern void usart2_send_byte(u8 data);  // 通过 USART2 发送 2byte 的数据

extern void usart2_send_data(u8 *buf);  // 通过 USART2 发送多个字节的数据

extern u8 usart2_recv_byte(void);   // 通过 USART2 接收 2byte 的数据

extern void set_usart2_handler(usart2_handler h);  // 设置回调函数

#endif


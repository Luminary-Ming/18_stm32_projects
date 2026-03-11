#ifndef USART1_H
#define USART1_H

/*
	发送  USART1_TX  PA9
	接收  USART1_RX  PA10
	
	PA9  配置为推挽复用模式
	PA10 配置为浮空输入模式
*/
#include "stm32f10x_conf.h"

typedef void (*usart1_handler)(u8);  // 定义了函数指针类型

extern void usart1_init(void);  // 初始化 USART1 使用的管脚

extern void usart1_send_byte(u8 data);  // 通过 USART1 发送 1byte 的数据

extern void usart1_send_data(u8 *buf);  // 通过 USART1 发送多个字节的数据

extern u8 usart1_recv_byte(void);   // 通过 USART1 接收 1byte 的数据

extern u8 *usart1_recv_data(void);  // 通过 USART1 接收多个字节的数据

extern void set_usart1_handler(usart1_handler h);  // 设置回调函数

#endif


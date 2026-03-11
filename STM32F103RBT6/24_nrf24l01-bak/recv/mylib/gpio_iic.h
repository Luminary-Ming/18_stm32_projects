#ifndef __GPIO_IIC_H
#define __GPIO_IIC_H
/*
用GPIO的方式来模拟IIC协议
这样做的优点是移植性是最好的
通过硬件原理图可知
时钟线	IIC_SCL		PB6
数据线	IIC_SDA		PB7
*/
#include "stm32f10x_conf.h"//包含了该头文件所有的外设头文件均已包含

typedef unsigned char u8;//通过typedef定义u8类型

extern void IIC_Init(void);									//初始化IIC使用的GPIO

extern void IIC_Start(void);								//产生开始信号

extern void IIC_Stop(void);									//产生结束信号

extern void IIC_Send_Byte(u8 txd);					//利用IIC总线发送数据(一位一位的发送)

extern u8 IIC_Recv_Byte(unsigned char ack);	//利用IIC总线接收数据

extern u8 IIC_Wait_Ack(void);								//等待ACK

extern void IIC_Ack(void);									//产生ACK

extern void IIC_NAck(void);									//不产生ACK

#endif

















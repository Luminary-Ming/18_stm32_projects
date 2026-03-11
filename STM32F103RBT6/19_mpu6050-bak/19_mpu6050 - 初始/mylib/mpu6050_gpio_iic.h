#ifndef MPU6050_GPIO_IIC_H
#define MPU6050_GPIO_IIC_H

// 用 GPIO 的方式来模拟 IIC 协议

#include "stm32f10x_conf.h"

typedef unsigned char u8;

void MPU6050_IIC_Init(void);  //初始化 IIC 使用的 GPIO	
void MPU6050_IIC_Start(void);  // 产生开始信号
void MPU6050_IIC_Stop(void);  // 产生结束信号
void MPU6050_IIC_Send_Byte(u8 txd);  // 利用 IIC 总线发送数据(一位一位的发送)
u8 MPU6050_IIC_Recv_Byte(unsigned char ack);// 利用 IIC 总线接收数据
u8 MPU6050_IIC_Wait_Ack(void);  // 等待 ACK
void MPU6050_IIC_Ack(void);  // 产生 ACK
void MPU6050_IIC_NAck(void);  // 不产生 ACK

#endif


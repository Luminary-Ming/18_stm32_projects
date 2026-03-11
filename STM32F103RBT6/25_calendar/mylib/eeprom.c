#include "eeprom.h"
#include "gpio_iic.h"

// 初始化 EEPROM
void eeprom_init(void)
{
	IIC_Init();
}

// 按字节写
void eeprom_byte_write(u8 daddr, u8 waddr, u8 data)
{
	IIC_Start();  // 产生开始信号
	IIC_Send_Byte(daddr | 0);  // 发送设备地址加写的标志
	while(IIC_Wait_Ack());  // 等待 ACK
	
	IIC_Send_Byte(waddr);  // 发送内部存储空间地址
	while(IIC_Wait_Ack());  // 等待 ACK
	
	IIC_Send_Byte(data);  // 发送 1byte 数据
	while(IIC_Wait_Ack());  // 等待 ACK
	
	IIC_Stop();  // 产生结束信号
}

// 按页写
void eeprom_page_write(u8 daddr, u8 waddr, u8 *data, u8 size)
{
	IIC_Start();  // 产生开始信号
	IIC_Send_Byte(daddr | 0);  // 发送设备地址加写的标志
	while(IIC_Wait_Ack());  // 等待 ACK
	
	IIC_Send_Byte(waddr);  // 发送内部存储空间地址
	while(IIC_Wait_Ack());  // 等待 ACK
	
	for (u8 i = 0; i < size; i++)  // 发送数据  
	{
        IIC_Send_Byte(data[i]);
        while(IIC_Wait_Ack());
    }
	
	IIC_Stop();  // 产生结束信号
}	

// 当前地址读
u8 eeprom_curr_read(u8 daddr)
{
	u8 data;
	
	IIC_Start();  // 产生开始信号
	IIC_Send_Byte(daddr | 1);  // 发送设备地址加读的标志
	while(IIC_Wait_Ack());  // 等待 ACK
	
	data = IIC_Recv_Byte(0);  // 接收数据, 不产生 ACK
	IIC_Stop();  // 产生结束信号
	
	return data;
}

// 随机地址读
u8 eeprom_rand_read(u8 daddr, u8 waddr)
{
	u8 data;
	
	// 假写
	IIC_Start();  // 产生开始信号
	IIC_Send_Byte(daddr | 0);  // 发送设备地址加写的标志
	while(IIC_Wait_Ack());  // 等待 ACK
	IIC_Send_Byte(waddr);  // 发送内部存储空间地址
	while(IIC_Wait_Ack());  // 等待 ACK
	
	// 读取数据
	IIC_Start();  // 产生开始信号
	IIC_Send_Byte(daddr | 1);  // 发送设备地址加读的标志
	while(IIC_Wait_Ack());  // 等待 ACK
	
	data = IIC_Recv_Byte(0);  // 接收数据, 不产生 ACK
	IIC_Stop();  // 产生结束信号
	
	return data;
}

// 顺序地址读
void eeprom_sequ_read(u8 daddr, u8 waddr, u8 *data, u8 size)
{
	u8 i = 0;  // 循环变量
	
	// 假写
	IIC_Start();  // 产生开始信号
	IIC_Send_Byte(daddr | 0);  // 发送设备地址加写的标志
	while(IIC_Wait_Ack());  // 等待 ACK
	IIC_Send_Byte(waddr);  // 发送内部存储空间地址
	while(IIC_Wait_Ack());  // 等待 ACK
	
	// 读取数据
	IIC_Start();  // 产生开始信号
	IIC_Send_Byte(daddr | 1);  // 发送设备地址加读的标志
	while(IIC_Wait_Ack());  // 等待 ACK
	
	
	for(i = 0; i < size; i++)
	{
		 if (i == size - 1)
            data[i] = IIC_Recv_Byte(0);  // 最后一个字节发 NACK
        else
            data[i] = IIC_Recv_Byte(1);  // 前面的字节发 ACK
	}
	
	IIC_Stop();  // 产生结束信号
}


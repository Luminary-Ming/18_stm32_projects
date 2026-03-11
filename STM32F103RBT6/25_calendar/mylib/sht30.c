#include "sht30.h"
#include "gpio_iic.h"
#include "stdio.h"

// 初始化 SHT30
extern void sht_init(void)
{
	IIC_Init();
}

// 给 SHT30 写一个采样指令
extern void sht_write_mode(void)
{
	IIC_Start();  // 产生开始信号
	IIC_Send_Byte(SLAVE_ADDR << 1 | 0);  // 发送设备地址加写的标志
	while(IIC_Wait_Ack());  // 等待 ACK
	IIC_Send_Byte(PERIODIC_MODE_HIGH >> 8);  // 发送采样指令的高字节
	while(IIC_Wait_Ack());  // 等待 ACK
	IIC_Send_Byte(PERIODIC_MODE_HIGH & 0xFF);  // 发送采样指令的低字节
	IIC_Stop();  // 产生结束信号
}

// 给 SHT30 写一个读取数据的指令
extern void sht_write_read_cmd(void)
{
	IIC_Start();  // 产生开始信号
	IIC_Send_Byte(SLAVE_ADDR << 1 | 0);  // 发送设备地址加写的标志
	while(IIC_Wait_Ack());  // 等待 ACK
	IIC_Send_Byte(FETCH_DATA >> 8);  // 发送读取数据指令的高字节
	while(IIC_Wait_Ack());  // 等待 ACK
	IIC_Send_Byte(FETCH_DATA & 0xFF);  // 发送读取数据指令的低字节
	while(IIC_Wait_Ack());  // 等待 ACK
}

// 获取 SHT30 传感器数据以及转换成温度和湿度
extern void sht_read_data(double sht_data[2])
{
	u8 i = 0;  // 循环变量
	u8 buf[6] = {0};  // 存储从 SHT30 读取出的 6byte 数据
	u16 temp = 0;  // 存储温度的高字节 + 低字节
	u16 hum = 0;   // 存储湿度的高字节 + 低字节
	
	IIC_Start();  // 产生开始信号
	IIC_Send_Byte(SLAVE_ADDR << 1 | 1);  // 发送设备地址加读的标志
	while(IIC_Wait_Ack());  // 等待 ACK
	for(i = 0; i < 6; i++)
	{
		if(i == 5)  // 最后一个字节数据不产生 ACK
			buf[i] = IIC_Recv_Byte(0);  // 读取最后一个字节的数据, 并且不产生 ACK
		else
			buf[i] = IIC_Recv_Byte(1);  // 读取之前字节的数据, 并且产生 ACK
	}
	IIC_Stop();  // 产生结束信号
	
	temp = buf[0] << 8 | buf[1];
	hum  = buf[3] << 8 | buf[4];
	
	sht_data[0] = (double)(-45 + 175 * (double)temp / 65535);
	sht_data[1] = (double)(100 * (double)hum / 65535);	
}


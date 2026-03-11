#include "eeprom.h"//包含头文件
#include "gpio_iic.h"//包含头文件

void eeprom_init(void)//初始化eeprom的函数
{
	IIC_Init();//初始化IIC使用的GPIO
}

void eeprom_byte_write(u8 daddr, u8 waddr, u8 data)//按字节来写数据
{
	IIC_Start();//产生开始信号
	IIC_Send_Byte(daddr);//利用IIC总线发送数据(发送设备地址)
	IIC_Wait_Ack();//等待ACK
	IIC_Send_Byte(waddr);//利用IIC总线发送数据(发送内部存储空间地址)
	IIC_Wait_Ack();//等待ACK
	IIC_Send_Byte(data);//利用IIC总线发送数据(发送数据)
	IIC_Wait_Ack();//等待ACK
	IIC_Stop();//产生结束信号
}

void eeprom_page_write(u8 daddr, u8 waddr, u8 *buf, u8 size)//按页来写数据
{
	u8 i = 0;//循环变量
	
	IIC_Start();//产生开始信号
	IIC_Send_Byte(daddr);//利用IIC总线发送数据(发送设备地址)
	IIC_Wait_Ack();//等待ACK
	IIC_Send_Byte(waddr);//利用IIC总线发送数据(发送内部存储空间地址)
	IIC_Wait_Ack();//等待ACK
	for(i = 0; i < size; i++)//轮询size次
	{
		IIC_Send_Byte(buf[i]);//利用IIC总线发送数据(发送数据)
		IIC_Wait_Ack();//等待ACK
	}
	IIC_Stop();//产生结束信号
}

u8 eeprom_cur_addr_read(u8 daddr)//当前地址读数据
{
	u8 data = 0;//用于接收数据
	
	IIC_Start();//产生开始信号
	IIC_Send_Byte(daddr | 1);//利用IIC总线发送数据(发送设备地址)
	IIC_Wait_Ack();//等待ACK
	data = IIC_Recv_Byte(0);//利用IIC总线接收数据
	IIC_Stop();//产生结束信号
	
	return data;//把获取到的数据返回
}

u8 eeprom_rand_read(u8 daddr, u8 waddr)//随即地址读数据
{
	u8 data = 0;//用于接收数据
	
	IIC_Start();//产生开始信号
	IIC_Send_Byte(daddr);//利用IIC总线发送数据(发送设备地址)
	IIC_Wait_Ack();//等待ACK
	IIC_Send_Byte(waddr);//利用IIC总线发送数据(发送内部存储空间地址)
	IIC_Wait_Ack();//等待ACK
	IIC_Start();//产生开始信号
	IIC_Send_Byte(daddr | 1);//利用IIC总线发送数据(发送设备地址)
	IIC_Wait_Ack();//等待ACK
	data = IIC_Recv_Byte(0);//利用IIC总线接收数据
	IIC_Stop();//产生结束信号
	
	return data;//把获取到的数据返回
}

void eeprom_sequ_read(u8 daddr, u8 waddr, u8 *buf, u8 size)//顺序地址读数据
{
	u8 i = 0;//循环变量
	
	IIC_Start();//产生开始信号
	IIC_Send_Byte(daddr);//利用IIC总线发送数据(发送设备地址)
	IIC_Wait_Ack();//等待ACK
	IIC_Send_Byte(waddr);//利用IIC总线发送数据(发送内部存储空间地址)
	IIC_Wait_Ack();//等待ACK
	IIC_Start();//产生开始信号
	IIC_Send_Byte(daddr | 1);//利用IIC总线发送数据(发送设备地址)
	IIC_Wait_Ack();//等待ACK
	for(i = 0; i < size; i++)//轮询size次
	{
		if(i == size - 1)//判断是否是最后一次读数据
			buf[i] = IIC_Recv_Byte(0);//利用IIC总线接收数据
		else
			buf[i] = IIC_Recv_Byte(1);//利用IIC总线接收数据
	}
	IIC_Stop();//产生结束信号
}









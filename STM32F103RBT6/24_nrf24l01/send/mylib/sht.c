#include "sht.h"//包含头文件
#include "gpio_iic.h"//包含头文件
#include "delay.h"//包含头文件
#include "led.h"//包含头文件

void sht_init(void)//初始化sht30的函数
{
	IIC_Init();//初始化IIC使用的GPIO
	delay_us(250);//延时250微秒
	IIC_Start();//产生开始信号
	IIC_Send_Byte(SLAVE_ADDR);//利用IIC总线发送数据(发送设备地址)
	while(IIC_Wait_Ack());//等待ACK
	IIC_Send_Byte(PERIODIC_MODE_MEDIUM_1PMPS >> 8);//利用IIC总线发送数据(发送sht30的读取速率)
	while(IIC_Wait_Ack());//等待ACK
	IIC_Send_Byte(PERIODIC_MODE_MEDIUM_1PMPS & 0xFF);//利用IIC总线发送数据(发送sht30的读取速率)
	while(IIC_Wait_Ack());//等待ACK
	IIC_Stop();//产生结束信号
	delay_us(150);//延时150微秒
}

void sht_write_mode(void)//配置sht30的读取速率
{
	IIC_Start();//产生开始信号
	IIC_Send_Byte(SLAVE_ADDR);//利用IIC总线发送数据(发送设备地址)
	while(IIC_Wait_Ack());//等待ACK
	IIC_Send_Byte(PERIODIC_MODE_MEDIUM_1PMPS >> 8);//利用IIC总线发送数据(发送sht30的读取速率)
	while(IIC_Wait_Ack());//等待ACK
	IIC_Send_Byte(PERIODIC_MODE_MEDIUM_1PMPS & 0xFF);//利用IIC总线发送数据(发送sht30的读取速率)
	while(IIC_Wait_Ack());//等待ACK
	IIC_Stop();//产生结束信号
	delay_ms(5);//延时5毫秒
}

void sht_write_read_cmd(void)//写入读SHT30数据的命令
{
	IIC_Start();//产生开始信号
	IIC_Send_Byte(SLAVE_ADDR);//利用IIC总线发送数据(发送设备地址)
	while(IIC_Wait_Ack());//等待ACK
	IIC_Send_Byte(READOUT_PERIODIC_MODE >> 8);//利用IIC总线发送数据(发送读SHT30数据的命令)
	while(IIC_Wait_Ack());//等待ACK
	IIC_Send_Byte(READOUT_PERIODIC_MODE & 0xFF);//利用IIC总线发送数据(发送读SHT30数据的命令)
	while(IIC_Wait_Ack());//等待ACK
}

void sht_read_data(double sht_data[2])//读SHT30采集到的数据
{
	uint8_t buf[6] = {0};//存储SHT30读出的数据
	uint16_t Temp = 0;//存储温度数据
	uint16_t Hum = 0;//存储湿度数据
	
	IIC_Start();//产生开始信号
	IIC_Send_Byte(SLAVE_ADDR | 0x01);//利用IIC总线发送数据(发送设备地址)
	delay_us(5);//延时5微秒
	while(IIC_Wait_Ack());//等待ACK
	buf[0] = IIC_Recv_Byte(1);//利用IIC总线接收数据
	buf[1] = IIC_Recv_Byte(1);//利用IIC总线接收数据
	buf[2] = IIC_Recv_Byte(1);//利用IIC总线接收数据
	buf[3] = IIC_Recv_Byte(1);//利用IIC总线接收数据
	buf[4] = IIC_Recv_Byte(1);//利用IIC总线接收数据
	buf[5] = IIC_Recv_Byte(0);//利用IIC总线接收数据
	IIC_Stop();//产生结束信号
	
	Temp = buf[0] & 0xFF;
	Temp <<= 8;
	Temp |= buf[1] & 0xFF;//把温度的高八位和低八位结合
	Hum = buf[3] & 0xFF;
	Hum <<= 8;
	Hum |= buf[4] & 0xFF;//把湿度的高八位和低八位结合
	
	sht_data[0] = (double)(-45 + 175 * (double)(Temp) / 65535);//通过转换公式转换成温度(小数点后两位)
	sht_data[1] = (double)(100 * (double)(Hum) / 65535);//通过转换公式转换成湿度(小数点后两位)
}









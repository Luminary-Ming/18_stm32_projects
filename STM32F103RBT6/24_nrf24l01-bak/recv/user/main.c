/****************************************************************
工程介绍	:	主要是通过开发板上的NRF无线通信模块进行一发一收的实验
						接收方接收发送方发过来的数据,并进行显示
使用方法	:	通过GPIO的方式模拟SPI协议从而驱动NRF无线通信模块
程序现象	:	可以把接收到的NRF发送放发送过来的数据通过OLED进行显示
****************************************************************/
#include "led.h"//包含有关led灯操作的头文件
#include "buzzer.h"//包含有关蜂鸣器操作的头文件
#include "button.h"//包含有关功能按键操作的头文件
#include "delay.h"//包含有关延时操作的头文件
#include "eint.h"//包含有关按键中断操作的头文件
#include "iwdg.h"//包含有关独立看门狗操作的头文件
#include "usart1.h"//包含有关窗口1操作的头文件
#include "stdio.h"//包含有关标准输入输出操作的头文件
#include "dht.h"//包含有关DHT11传感器操作的头文件
#include "eeprom.h"//包含有关EEPROM操作的头文件
#include "sht.h"//包含有关SHT30操作的头文件
#include "oled.h"//包含有关OLED操作的头文件
#include "ldt.h"//包含有关数码管操作的头文件
#include "adj_res.h"//包含有关高精度可调电阻操作的头文件
#include "rtc.h"//包含有关RTC操作的头文件
#include "nrf24l01.h"//包含有关NRF操作的头文件

int main(void)
{
	u8 ret;//接收返回值
	struct nrf_msg_st msg;//自定义结构体类型<32byte>
	char Sht_Data[12] = {0};//存储SHT30转换成字符串的数据
	u8 rxAddr[] = {0x10, 0x20, 0x30, 0x40, 0x53};//接受地址
	//u8 txAddr[] = {0x10, 0x20, 0x30, 0x40, 0x51};//发送地址
	
	led_init();//调用初始化led灯的函数
	delay_init();//调用初始化延时的函数
	OLED_Init();//调用初始化OLED的函数
	SPI_NRF_Init();//初始化NRF
	
	OLED_Clear();//清屏的函数
	
	ret = NRF_Check();//检测NRF是否存在
	if (ret == SUCCESS)//如果ret等于SUCCESS
	{
		OLED_ShowString(0, 0, (u8 *)"spi ok", 12);//显示spi ok的信息
		OLED_ShowString(0, 2, (u8 *)"R:10,20,30,40,53", 12);//显示接收地址
		OLED_ShowString(0, 4, (u8 *)"T:10,20,30,40,51", 12);//显示发送地址
	} 
	else 
	{
		OLED_ShowString(0, 0, (u8 *)"spi error", 12);//显示spi error的信息
	}
	
	NRF_RX_Mode(rxAddr, 11, sizeof(struct nrf_msg_st));//设置为接收模式
	
	delay_ms(2000);//延时2s
	OLED_Clear();//清屏的函数
	
	while(1)
	{
		ret = NRF_Rx_Dat((u8 *)&msg, sizeof(msg));//接收数据
		led_on(0);//0号LED灯亮
		if(ret == RX_DR)//判断是否接到数据
		{
			OLED_ShowString(0, 0, (u8 *)"rx bingo", 12);//显示rx bingo提示信息
			OLED_ShowString(0, 2, (u8 *)msg.buf, 12);//显示接收到的数据
			OLED_ShowNum(0, 4, msg.Dht_Hum, 2, 12);//显示接收到的数据
			OLED_ShowNum(30, 4, msg.Dht_Temp, 2, 12);//显示接收到的数据
			sprintf(Sht_Data, "%.2f %.2f", msg.Sht_Temp, msg.Sht_Hum);//转换接收到的数据
			OLED_ShowString(0, 6, (u8 *)Sht_Data, 12);//显示接收到的数据
		} 
		else
		{
			OLED_ShowString(0, 0, (u8 *)"rx error", 12);//显示rx error提示信息
		}
	}
}






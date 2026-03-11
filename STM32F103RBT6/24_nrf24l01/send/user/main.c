/****************************************************************
工程介绍	:	主要是通过开发板上的NRF无线通信模块进行一发一收的实验
						发送方可以把自己采集到的温度湿度AD转换的结果进行发送
使用方法	:	通过GPIO的方式模拟SPI协议从而驱动NRF无线通信模块
程序现象	:	可以把自己采集到的的温度 湿度 AD转换的值显示到自己的
						OLED显示屏中,并且可以把这些数据整合成数据包通过NRF
						发送到接收方
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
	char Dht_Data[5] = {0};//存储DHT11的数据
	double Sht_Data[2] = {0};//存储SHT30的数据
	char Sht_Buf[12] = {0};//存储转换SHT30之后的字符串
	int Adj_Res_Value = 0;//存储AD转换的数据
	u8 ret = 0;//接收返回值
	struct time_st t = {2019, 9, 17, 2, 22, 37, 0};//存储当前需要设置的时间
	int count = 0;//计数变量
	struct nrf_msg_st msg;//自定义结构体类型<32byte>
	//u8 rxAddr[] = {0x10, 0x20, 0x30, 0x40, 0x51};//接受地址
	u8 txAddr[] = {0x10, 0x20, 0x30, 0x40, 0x53};//发送地址
	
	led_init();//调用初始化led灯的函数
	buzzer_init();//调用初始化蜂鸣器的函数
	button_init();//调用初始化功能按键的函数
	delay_init();//调用初始化延时的函数
	eint_init();//调用初始化按键中断的函数
	usart_1_init();//调用初始化USART1的函数
	dht_init();//调用初始化DHT11的函数
	eeprom_init();//调用初始化eeprom的函数
	sht_init();//调用初始化SHT30的函数
	OLED_Init();//调用初始化OLED的函数
	ldt_init();//调用初始化数码管的函数
	adj_res_init();//调用初始化高精度可调电阻的函数
	rtc_init(&t);//调用初始化RTC的函数
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
	
	sht_write_mode();//配置sht30的读取速率
	
	NRF_TX_Mode(txAddr, 17);//设置为发送模式

	delay_ms(2000);//延时2s
	OLED_Clear();//清屏的函数8o989o nm 
	
	while(1)
	{
		get_dht_value(Dht_Data);//获取DHT11数据
		sht_write_read_cmd();//写入读SHT30数据的命令
		sht_read_data(Sht_Data);//读SHT30采集到的数据
		Adj_Res_Value = get_adj_res_value();//获取AD转换的结果
		msg.Dht_Hum = (int)Dht_Data[0];//DHT11的湿度转存到NRF数据包的结构体中
		msg.Dht_Temp = (int)Dht_Data[2];//DHT11的温度转存到NRF数据包的结构体中
		msg.Sht_Temp = (float)Sht_Data[0];//SHT30的温度转存到NRF数据包的结构体中
		msg.Sht_Hum = (float)Sht_Data[1];//SHT30的湿度转存到NRF数据包的结构体中
		sprintf((char *)msg.buf, "%d", Adj_Res_Value);//AD转换的数据存储到NRF数据包的结构体中
		OLED_ShowString(0, 2, (u8 *)msg.buf, 12);//显示自己采集到的AD值
		OLED_ShowNum(0, 4, msg.Dht_Hum, 2, 12);//显示自己采集到的DHT11的湿度
		OLED_ShowNum(30, 4, msg.Dht_Temp, 2, 12);//显示自己采集到的DHT11的温度
		sprintf(Sht_Buf, "%.2f %.2f", msg.Sht_Temp, msg.Sht_Hum);//转换SHT30采集到的数据
		OLED_ShowString(0, 6, (u8 *)Sht_Buf, 12);//显示SHT30采集到的的数据
		count = 0;
		send_again:
		delay_ms(20);//延时20ms
		ret = NRF_Tx_Dat((u8 *)&msg, sizeof(msg));//发送数据
		if(ret == TX_DS)//判断是否发送成功
		{
			OLED_ShowString(0, 0, (u8 *)"tx bingo", 12);//打印tx bingo提示信息
		} 
		else 
		{
			OLED_ShowString(0, 0, (u8 *)"tx error", 12);//打印tx error提示信息
			count++;
			if(count < 5)//如果5次没有发送成功
			{
				goto send_again;//跳转到send_again
			}
		}
		led_on(0);//0号led灯亮
		delay_ms(500);//延时1s
		led_off(0);//0号led灯灭
		delay_ms(500);//延时1s
	}
}






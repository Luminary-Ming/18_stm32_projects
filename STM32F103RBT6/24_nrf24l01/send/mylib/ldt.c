#include "ldt.h"//包含头文件
#include "bitband.h"//包含头文件
#include "delay.h"//包含头文件

#define	HC138_A0		PCOut(11)//把PC11的输出做成宏
#define	HC138_A1		PCOut(12)//把PC12的输出做成宏
#define	HC138_A2		PDOut(2)//把PD2的输出做成宏
#define	HC595_SCLK	PBOut(5)//把PB5的输出做成宏
#define	HC595_LCLK	PBOut(4)//把PB4的输出做成宏
#define	HC595_DATA	PBOut(3)//把PB3的输出做成宏

void ldt_init(void)//初始化所用到的GPIO管脚
{
	GPIO_InitTypeDef Gpio_Value;//定义了GPIO固件的结构体变量
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
	//通过APB2总线使能GPIOB组 GPIOC组 GPIOD组 和 AFIO 的时钟
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//改变指定管脚的映射 GPIO_Remap_SWJ_JTAGDisable,JTAG-DP 禁用 + SW-DP 使能 
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_Out_PP;//选择了推挽的输出模式
	Gpio_Value.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_4 | GPIO_Pin_3;//选择了5号 4号 3号GPIO管脚
	Gpio_Value.GPIO_Speed = GPIO_Speed_50MHz;//选择了50MHz的输出速度
	GPIO_Init(GPIOB, &Gpio_Value);//按照上述配置初始化GPIOB组的管脚
	
	Gpio_Value.GPIO_Pin = GPIO_Pin_11 |GPIO_Pin_12;//选择了11号 12号GPIO管脚
	GPIO_Init(GPIOC, &Gpio_Value);//按照上述配置初始化GPIOC组的管脚
	
	Gpio_Value.GPIO_Pin = GPIO_Pin_2;//选择了2号GPIO管脚
	GPIO_Init(GPIOD, &Gpio_Value);//按照上述配置初始化GPIOD组的管脚
	
	HC138_A0 = 0;//把HC138_A0所连接的GPIO管脚拉低
	HC138_A1 = 0;//把HC138_A1所连接的GPIO管脚拉低
	HC138_A2 = 0;//把HC138_A2所连接的GPIO管脚拉低
	HC595_SCLK = 0;//把HC595_SCLK所连接的GPIO管脚拉低
	HC595_LCLK = 0;//把HC595_LCLK所连接的GPIO管脚拉低
	HC595_DATA = 0;//把HC595_DATA所连接的GPIO管脚拉低
}

void hc138_out_data(uint8_t data)//控制74HC138模块选择公共端
{
	switch(data)//通过分支语句区分使用的是哪一个数码管
	{
		case 0 : HC138_A0 = 0; HC138_A1 = 0; HC138_A2 = 0; break;//0号数码管使能
		case 1 : HC138_A0 = 1; HC138_A1 = 0; HC138_A2 = 0; break;//1号数码管使能
		case 2 : HC138_A0 = 0; HC138_A1 = 1; HC138_A2 = 0; break;//2号数码管使能
		case 3 : HC138_A0 = 1; HC138_A1 = 1; HC138_A2 = 0; break;//3号数码管使能
	}
}

void hc595_send_data(uint8_t data)//控制74HC595模块发送数据
{
	int i = 0;//循环变量
	
	for(i = 0; i < 8; i++)//循环8次
	{
		HC595_SCLK = 0;//把HC595_SCLK拉低
		if(data & 0x80)//判断data数据中的最高位是否为真
			HC595_DATA = 1;//把HC595_DATA拉高
		else
			HC595_DATA = 0;//把HC595_DATA拉低
		data <<= 1;//data按位左移等于1
		HC595_SCLK = 1;//把HC595_SCLK拉高
	}
	HC595_LCLK = 0;//把HC595_LCLK拉低
	HC595_LCLK = 1;//把HC595_LCLK拉高
}

void digit_show_data(int data)//在数码管中显示数据
{
	uint8_t d[4] = {0};//存储显示数据的每一位
	uint8_t i = 0;//循环变量
	uint8_t table[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};
	//共阴极数码管显示0~F的数据
	
	d[0] = data / 1000;//取出data的千位
	d[1] = data / 100 % 10;//取出data的百位
	d[2] = data / 10 % 10;//取出data的十位
	d[3] = data % 10;//取出data的个位
	
	for(i = 0; i < 4; i++)//循环4次
	{
		hc138_out_data(i);//选择四位共阴极数码管的公共端
		hc595_send_data(table[d[i]]);//发送串行数据
		delay_ms(1);//延时1毫秒
		hc595_send_data(0);////发送串行数据
	}
}









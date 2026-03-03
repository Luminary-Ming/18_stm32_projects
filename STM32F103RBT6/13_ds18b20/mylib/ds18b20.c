#include "ds18b20.h"
#include "bitband.h"
#include "delay.h"


// 把 DS18B20 使用的 GPIO 管脚初始化为推挽输出
static void set_dht_gpio_output(void)
{
	GPIO_InitTypeDef Gpio_Value;  // 定义 GPIO 结构体类型的变量
	Gpio_Value.GPIO_Mode = GPIO_Mode_Out_PP;  // 推挽输出
	Gpio_Value.GPIO_Pin = GPIO_Pin_15;  // 使用第 15 号管脚
	Gpio_Value.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &Gpio_Value);
}

// 把 DS18B20 使用的 GPIO 管脚初始化为浮空输入
static void set_dht_gpio_input(void)
{
	GPIO_InitTypeDef Gpio_Value;  // 定义 GPIO 结构体类型的变量
	Gpio_Value.GPIO_Mode = GPIO_Mode_IN_FLOATING;  // 浮空输入
	Gpio_Value.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOA, &Gpio_Value);
}

// 控制数据线输出高低电平
static void ds18b20_output_status(int state)
{
	set_ds18b20_gpio_output();  // 把 PC10 管脚初始化为推挽输出
	
	if(state == 0)  // 判断是否拉低数据线
		PCOut(10) = 0;
	else
		PCOut(10) = 1;
}

// 获取数据线的电平高低
static int ds18b20_input_status(void)
{
	set_ds18b20_gpio_input();  // 把 PC10 管脚初始化为浮空输入
	
	return PCIn(10);
}




// DS18B20 复位 (握手)
static void ds18b20_reset()
{
	
}


// 初始化 DS18B20
void ds18b20_init(void)
{
	// 通过 APB2 总线使能 GPIOC 组的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	
	// 把 PA15 的调试功能禁用，作为普通 IO 使用
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	
	ds18b20_reset();
}
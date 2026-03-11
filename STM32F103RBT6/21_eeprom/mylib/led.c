#include "led.h"
#include "bitband.h"

// 初始化 LED 使用的 GPIO 管脚
void led_init(void)
{
	GPIO_InitTypeDef Gpio_Value;  // 定义 GPIO 结构体类型的变量
	
	// 通过 APB2 总线使能 GPIOC 组的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	// 初始化
	Gpio_Value.GPIO_Mode = GPIO_Mode_Out_PP;  // 选择推挽输出模式
	Gpio_Value.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;  // 选择 1 2 3 号管脚
	Gpio_Value.GPIO_Speed = GPIO_Speed_50MHz;  // 选择 50MHz 输出速率
	
	// 按照上述配置初始化 GPIOC 组的管脚
	GPIO_Init(GPIOC, &Gpio_Value);
	
	PCOut(1) = 0;  // 让 PC1 管脚输出低电平
	PCOut(2) = 0;  // 让 PC2 管脚输出低电平
	PCOut(3) = 0;  // 让 PC3 管脚输出低电平
}	

// 通过形参 num 点亮响应的 LED 灯
void led_on(int num)
{
	switch(num)
	{
		case 0: PCOut(1) = 1; break;  // 让 PC1 管脚输出高电平
		case 1: PCOut(2) = 1; break;  // 让 PC2 管脚输出高电平
		case 2: PCOut(3) = 1; break;  // 让 PC3 管脚输出高电平
	}
}	

// 通过形参 num 熄灭响应的 LED 灯
void led_off(int num)
{
	switch(num)
	{
		case 0: PCOut(1) = 0; break;  // 让 PC1 管脚输出低电平
		case 1: PCOut(2) = 0; break;  // 让 PC2 管脚输出低电平
		case 2: PCOut(3) = 0; break;  // 让 PC3 管脚输出低电平
	}                      
}


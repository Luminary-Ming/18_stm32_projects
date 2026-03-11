#include "buzzer.h"
#include "bitband.h"

// 初始化蜂鸣器
void buzzer_init(void)
{
	GPIO_InitTypeDef Gpio_Value;  // 定义 GPIO 结构体类型的变量
	
	// 通过 APB2 总线使能 GPIOB 组的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	// 初始化
	Gpio_Value.GPIO_Mode = GPIO_Mode_Out_PP;  // 选择推挽输出模式
	Gpio_Value.GPIO_Pin = GPIO_Pin_7;  // 选择 7 号管脚
	Gpio_Value.GPIO_Speed = GPIO_Speed_50MHz;  // 选择 50MHz 输出速率
	
	// 按照上述配置初始化 GPIOC 组的管脚 -> 7 号管脚, 推挽的输出模式, 50MHz 输出速率
	GPIO_Init(GPIOC, &Gpio_Value);
	
	PCOut(7) = 0;  // 让 PC7 输出低电平（蜂鸣器关闭）
}


// 打开蜂鸣器
void buzzer_on(void)
{
	PCOut(7) = 1;  // PC7 输出高电平
}	

// 关闭蜂鸣器
void buzzer_off(void)
{
	PCOut(7) = 0;  // PC7 输出低电平
}

// 简单的延时函数（通过空循环实现）
void delay(unsigned int count)
{
	unsigned int i, j;
	for(i = 0; i < count; i++)
	{
		for(j = 0; j < 1000; j++);  // 空循环
	}
}
	


#include "button.h"
#include "bitband.h"

// 初始化功能按键使用的 GPIO 管脚
void button_init(void)  
{
	GPIO_InitTypeDef Gpio_Value;  // 定义 GPIO 结构体类型的变量
	
	// 通过 APB2 总线使能 GPIOA 组和 GPIOC 组的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA, ENABLE);
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_IPU;  // 选择 上拉输入 模式
	Gpio_Value.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8; // 选择 9 8 号管脚
	GPIO_Init(GPIOC, &Gpio_Value);  // 按照上述配置初始化 GPIOC 组的管脚
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_IPD;  // 选择 下拉输入 模式
	Gpio_Value.GPIO_Pin = GPIO_Pin_0; // 选择 0 号管脚
	GPIO_Init(GPIOA, &Gpio_Value);  // 按照上述配置初始化 GPIOA 组的管脚
}

// 根据形参检测相应按键的电平高低
int button_status(int num)  
{
	int ret = 0;
	switch(num)
	{
		case 0: ret = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9); break;  // 获取 PC9 管脚的电平高低;
		case 1: ret = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8); break;  // 获取 PC8 管脚的电平高低;
		case 2: ret = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0); ret = !ret; break;  // 获取 PA0 管脚的电平高低;
	}
	
	return !ret;  // 在公司里, 1代表有状态(按下), 0代表无状态(没有按下)
}


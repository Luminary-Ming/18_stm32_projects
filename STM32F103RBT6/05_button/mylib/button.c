#include "button.h"

// 初始化功能按键使用的 GPIO 管脚
void button_init(void)  
{
	// 通过 APB2 总线使能 GPIOA 组和 GPIOC 组的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA, ENABLE);
	
	// 初始化 PC9 管脚
	GPIOC_CRH &= ~(15 << 4);
	GPIOC_CRH |= (2 << 6);  // 上拉输入人
	GPIOC_ODR |= (1 << 9);  // 输出高电平
	
	// 初始化 PC8 管脚
	GPIOC_CRH &= ~(15 << 0);
	GPIOC_CRH |= (2 << 2);  // 上拉输入
	GPIOC_ODR |= (1 << 8);  // 输出高电平
	
	// 初始化 PA0 管脚
	GPIOA_CRL &= ~(15 << 0);
	GPIOA_CRL |= (2 << 2);  // 下拉输入
	GPIOA_ODR &= ~(1 << 0); // 输出低电平    
}

// 根据形参检测相应按键的电平高低
int button_status(int num)  
{
	int ret = 0;
	switch(num)
	{
		case 0: ret = (GPIOC_IDR >> 9) & 0x01; break;
		case 1: ret = (GPIOC_IDR >> 8) & 0x01; break;
		case 2: ret = GPIOA_IDR & 0x01; ret = !ret; break;
	}
	
	return !ret;  // 在公司里, 1代表有状态(按下), 0代表无状态(没有按下)
}


#include "led.h"

// 初始化 LED 使用的 GPIO 管脚
void led_init(void)
{
	// 通过 APB2 总线使能 GPIOC 组的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	// 初始化 LED0 (PC1 管脚) -> 推挽的输出模式, 50MHz 输出速率
	GPIOC_CRL &= ~(15 << 4);
	GPIOC_CRL |= (3 << 4);
	GPIOC_ODR &= ~(1 << 1);  // 让 PC1 管脚输出低电平
	
	// 初始化 LED1 (PC2 管脚) -> 推挽的输出模式, 50MHz 输出速率
	GPIOC_CRL &= ~(15 << 8);
	GPIOC_CRL |= (3 << 8);
	GPIOC_ODR &= ~(1 << 2);  // 让 PC2 管脚输出低电平
	
	// 初始化 LED2 (PC3 管脚) -> 推挽的输出模式, 50MHz 输出速率
	GPIOC_CRL &= ~(15 << 12);
	GPIOC_CRL |= (3 << 12);
	GPIOC_ODR &= ~(1 << 3);  // 让 PC3 管脚输出低电平
}	

// 通过形参 num 点亮响应的 LED 灯
void led_on(int num)
{
	switch(num)
	{
		case 0: GPIOC_ODR |= (1 << 1); break;  // 让 PC1 管脚输出高电平
		case 1: GPIOC_ODR |= (1 << 2); break;  // 让 PC2 管脚输出高电平
		case 2: GPIOC_ODR |= (1 << 3); break;  // 让 PC3 管脚输出高电平
	}
}	

// 通过形参 num 熄灭响应的 LED 灯
void led_off(int num)
{
	switch(num)
	{
		case 0: GPIOC_ODR &= ~(1 << 1); break;  // 让 PC1 管脚输出低电平
		case 1: GPIOC_ODR &= ~(1 << 2); break;  // 让 PC2 管脚输出低电平
		case 2: GPIOC_ODR &= ~(1 << 3); break;  // 让 PC3 管脚输出低电平
	}                      
}


#include "buzzer.h"


// 初始化蜂鸣器
void buzzer_init(void)
{
	// 通过 APB2 总线使能 GPIOB 组的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	// 初始化蜂鸣器 (PB8 管脚) -> 推挽的输出模式, 50MHz 输出速率
	// PC7 在 CRH 寄存器中，每 4 位控制一个引脚 (8-15)，PB8 对应 bit0-3
	GPIOC_CRL &= ~((unsigned int)15 << 28);  // 清零 PC7 的 4 个配置位
	GPIOC_CRL |= (3 << 28);  // 设置为推挽输出模式
	GPIOC_ODR &= ~(1 << 7);  // 初始状态：PC7 输出低电平（蜂鸣器关闭）
}


// 打开蜂鸣器
void buzzer_on(void)
{
	GPIOC_ODR |= (1 << 7);  // PC7 输出高电平
}	

// 关闭蜂鸣器
void buzzer_off(void)
{
	GPIOC_ODR &= ~(1 << 7);  // PC7 输出低电平
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

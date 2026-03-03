#include "led.h"

void led_init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    
    // PC1, PC2, PC3 配置
    GPIOC_CRL &= ~(0xFFF << 4); // 清空 bit 4-15
    GPIOC_CRL |= (0x333 << 4);  // 设置为推挽输出
    GPIOC_ODR &= ~(0x7 << 1);   // 默认熄灭
}

void led_on(int num) 
{ 
	GPIOC_ODR |= (1 << (num + 1)); 
}
void led_off(int num) 
{ 
	GPIOC_ODR &= ~(1 << (num + 1)); 
}


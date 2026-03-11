#include "buzzer.h"

void buzzer_init(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIOC_CRL &= ~((unsigned int)15 << 28); // PC7
    GPIOC_CRL |= (3 << 28); 
    GPIOC_ODR &= ~(1 << 7); 
}

void play_music(unsigned int frequency, unsigned int duration_ms) {
    if (frequency == 0) {
        delay_ms(duration_ms);
        return;
    }
    
    // 修正：半周期时间(us) = 1000000 / (2 * frequency)
    unsigned int half_period = 500000 / frequency;  // 微秒
    
    // 修正：需要输出的周期数 = 总时间 / 周期时间
    // 周期时间(us) = 2 * half_period = 1000000 / frequency
    unsigned int cycles = (duration_ms * 1000) / (2 * half_period);
    
    for (unsigned int i = 0; i < cycles; i++) {
        GPIOC_ODR |= (1 << 7);   // 输出高电平
        delay_us(half_period);
        GPIOC_ODR &= ~(1 << 7);  // 输出低电平
        delay_us(half_period);
    }
}

void delay_us(unsigned int us) {
    while(us--) {
        // 72MHz 下，实测校准值
        unsigned int i = 12;  // 可能需要微调这个值
        while(i--);
    }
}

void delay_ms(unsigned int ms) {
    while(ms--) delay_us(1000);
}


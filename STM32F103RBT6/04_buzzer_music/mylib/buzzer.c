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
    // 算法校准
    unsigned int half_period = 500000 / frequency;
    unsigned int cycles = (frequency * duration_ms) / 1000;
    
    for (unsigned int i = 0; i < cycles; i++) {
        GPIOC_ODR |= (1 << 7);
        delay_us(half_period);
        GPIOC_ODR &= ~(1 << 7);
        delay_us(half_period);
    }
}

void delay_us(unsigned int us) {
    while(us--) {
        // 针对 72MHz 校准：i=5 到 6 之间最准
        unsigned char i = 6; 
        while(i--);
    }
}

void delay_ms(unsigned int ms) {
    while(ms--) delay_us(1000);
}



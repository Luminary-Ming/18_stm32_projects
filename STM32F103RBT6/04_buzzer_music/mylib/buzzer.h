#ifndef BUZZE_H
#define BUZZE_H

#include "stm32f10x_conf.h"

// 寄存器地址映射
#define GPIOC_CRL *(volatile unsigned int *)(GPIOC_BASE + 0x00)
#define GPIOC_ODR *(volatile unsigned int *)(GPIOC_BASE + 0x0C)

// 补全后的频率表 (Hz)
#define NOTE_0   0
#define NOTE_F4  349  // <--- 刚才漏掉的就是它！
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622 
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880

void buzzer_init(void);  
void play_music(unsigned int frequency, unsigned int duration_ms);
void delay_ms(unsigned int ms);
void delay_us(unsigned int us);

#endif


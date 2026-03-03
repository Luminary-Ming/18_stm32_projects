#ifndef LED_H
#define LED_H

#include "stm32f10x_conf.h"

#define GPIOC_CRL *(volatile unsigned int *)(GPIOC_BASE + 0x00)
#define GPIOC_ODR *(volatile unsigned int *)(GPIOC_BASE + 0x0C)
	
void led_init(void);
void led_on(int num);
void led_off(int num);

#endif


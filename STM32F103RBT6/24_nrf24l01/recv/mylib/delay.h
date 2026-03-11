#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f10x_conf.h"//包含了该头文件所有的外设头文件均已包含

#define SYSCLK 72					//系统时钟的基数(注:是72而不是72M)

typedef uint32_t  u32;		//通过typedef定义了u32类型
typedef uint16_t  u16;		//通过typedef定义了u16类型
typedef uint8_t   u8;			//通过typedef定义了u8类型
	
void delay_init(void);	//延时函数的初始化
void delay_ms(u16 nms);	//毫秒级的延时函数(参数为毫秒数)
void delay_us(u32 nus);	//微秒级的延时函数(参数为微秒数)

#endif






























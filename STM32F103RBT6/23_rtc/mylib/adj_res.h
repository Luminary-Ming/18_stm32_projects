#ifndef ADJ_RES_H
#define ADJ_RES_H

/*
	ADC_VOL   PC0 / ADC12_10
	PC0 管脚初始化为模拟输入
*/
#include "stm32f10x_conf.h"

typedef uint16_t  u16;  // unsigned short

extern void adj_res_init(void);  // 初始化高精度可调电阻使用的 GPIO 管脚以及 ADC 通道

extern u16 get_adj_res_value(void);  // 获取高精度可调电阻转换后的数值

extern float get_adj_res_voltage(void);  // 将采样值转换为实际的电压值

#endif


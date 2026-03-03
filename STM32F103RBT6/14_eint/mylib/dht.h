#ifndef DHT_H
#define DHT_H

/*
	DHT11  PC10
	管脚初始化为推挽输出和浮空输入
*/
#include "stm32f10x_conf.h"

extern void dht_init(void);  // 初始化 DHT11
extern void get_dht_value(u8 data[5]);  // 获取 DHT11 采集的数据

#endif


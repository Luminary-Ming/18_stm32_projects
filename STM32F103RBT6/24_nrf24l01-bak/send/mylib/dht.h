#ifndef __DHT_H
#define __DHT_H
/*
开发板设备		所连接的管脚
DHT11					PC10
已知芯片的管脚是多功能复用型的管脚
芯片的管脚会有 输入功能 输出功能 复用功能 模拟功能
我们这次操作的传感器是DHT11接口是GPIO
所以即需要输入模式也需要输出模式
*/
#include "stm32f10x_conf.h"//包含了该头文件所有的外设头文件均已包含

extern void dht_init(void);//初始化的函数(初始化所连接的GPIO管脚)

extern void get_dht_value(char *buf);//获取DHT11采集到的数据

#endif





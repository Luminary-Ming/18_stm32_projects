#ifndef __SHT_H
#define __SHT_H
/*
用GPIO的方式来模拟IIC协议
这样做的优点是移植性是最好的
通过硬件原理图可知
时钟线	IIC_SCL		PB6
数据线	IIC_SDA		PB7
sht.c 和 sht.h 是用来操作开发板上有关SHT30的实验
*/
#include "stm32f10x_conf.h"//包含了该头文件所有的外设头文件均已包含

#define	SLAVE_ADDR									0x88		//SHT30的设备地址
#define PERIODIC_MODE_MEDIUM_1PMPS	0x2737	//配置SHT30的读取速率
#define READOUT_PERIODIC_MODE				0xE000	//读SHT30数据的命令

extern void sht_init(void);//初始化sht30的函数

extern void sht_write_mode(void);//配置sht30的读取速率

extern void sht_write_read_cmd(void);//写入读SHT30数据的命令

extern void sht_read_data(double sht_data[2]);//读SHT30采集到的数据

#endif








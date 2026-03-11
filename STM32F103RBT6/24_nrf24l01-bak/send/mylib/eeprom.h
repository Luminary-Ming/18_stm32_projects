#ifndef __EEPROM_H
#define __EEPROM_H
/*
用GPIO的方式来模拟IIC协议
这样做的优点是移植性是最好的
通过硬件原理图可知
时钟线	IIC_SCL		PB6
数据线	IIC_SDA		PB7
eeprom.c 和 eeprom.h 是用来操作开发板上有关EEPROM的实验
*/
#include "stm32f10x_conf.h"//包含了该头文件所有的外设头文件均已包含

extern void eeprom_init(void);//初始化eeprom的函数

extern void eeprom_byte_write(u8 daddr, u8 waddr, u8 data);//按字节来写数据

extern void eeprom_page_write(u8 daddr, u8 waddr, u8 *buf, u8 size);//按页来写数据

extern u8 eeprom_cur_addr_read(u8 daddr);//当前地址读数据

extern u8 eeprom_rand_read(u8 daddr, u8 waddr);//随即地址读数据

extern void eeprom_sequ_read(u8 daddr, u8 waddr, u8 *buf, u8 size);//顺序地址读数据

#endif











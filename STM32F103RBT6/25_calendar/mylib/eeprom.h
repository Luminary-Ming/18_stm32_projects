#ifndef EEPROM_H
#define EEPROM_H

#include "stm32f10x_conf.h"

// daddr: 设备地址
// waddr: 起始内存地址
// data: 存储读取数据的缓冲区指针
// size: 要读取的字节数
extern void eeprom_init(void);  // 初始化 EEPROM
extern void eeprom_byte_write(u8 daddr, u8 waddr, u8 data);  // 按字节写
extern void eeprom_page_write(u8 daddr, u8 waddr, u8 *data, u8 size);  // 按页写
extern u8 eeprom_curr_read(u8 daddr);  // 当前地址读
extern u8 eeprom_rand_read(u8 daddr, u8 waddr);  // 随机地址读
extern void eeprom_sequ_read(u8 daddr, u8 waddr, u8 *data, u8 size);  // 顺序地址读

#endif


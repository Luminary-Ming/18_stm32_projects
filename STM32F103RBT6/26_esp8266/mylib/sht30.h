#ifndef SHT30_H
#define SHT30_H

#include "stm32f10x_conf.h"

#define SLAVE_ADDR 0x44  // 设备地址
#define PERIODIC_MODE_HIGH 0x2737  // 采样地址
#define FETCH_DATA 0XE000  // 获取数据指令

extern void sht_init(void);  // 初始化 SHT30
extern void sht_write_mode(void);  // 给 SHT30 写一个采样指令
extern void sht_write_read_cmd(void);  // 给 SHT30 写一个读取数据的指令
extern void sht_read_data(double sht_data[2]);  // 获取 SHT30 传感器数据以及转换成温度和湿度

#endif


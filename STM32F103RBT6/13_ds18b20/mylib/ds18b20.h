#ifndef DS18B20_H
#define DS18B20_H
/*
	DS18B20 测量温度
	PA6 管脚初始化为推挽输出和浮空输入
*/
#include "stm32f10x.h"

// DS18B20 指令集
#define DS18B20_CMD_SKIP_ROM    0xCC  // 跳过 ROM 匹配（单设备总线常用）
#define DS18B20_CMD_CONVERT_T   0x44  // 启动温度转换
#define DS18B20_CMD_READ_SCR    0xBE  // 读取暂存器（获取温度）

extern void ds18b20_init(void);  // 初始化 DS18B20
extern short ds18b20_get_temp(void);  // 获取温度（返回放大100倍的整数，如2550表示25.50℃）

#endif


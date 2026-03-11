#include "dht.h"
#include "bitband.h"
#include "delay.h"
#include "ds18b20.h"

// PA6 管脚初始化推挽输出
static void set_ds18b20_gpio_output(void)
{
	GPIO_InitTypeDef Gpio_Value;
	Gpio_Value.GPIO_Mode = GPIO_Mode_Out_PP;  // 推挽输出
	Gpio_Value.GPIO_Pin = GPIO_Pin_6;
	Gpio_Value.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &Gpio_Value);
}

// PA6 管脚初始化为浮空输入
static void set_ds18b20_gpio_input(void)
{
	GPIO_InitTypeDef Gpio_Value;
	Gpio_Value.GPIO_Mode = GPIO_Mode_IN_FLOATING;  // 浮空输入
	Gpio_Value.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOA, &Gpio_Value);
}

// 控制数据线输出高低电平
static void ds18b20_output_status(int state)
{
	set_ds18b20_gpio_output();
	
	if(state == 0)  // 判断是否拉低数据线
		PAOut(6) = 0;
	else
		PAOut(6) = 1;
}

// 获取数据线的电平高低
static int ds18b20_input_status(void)
{
	set_ds18b20_gpio_input();
	return PAIn(6);
}

// 初始化 ds18b20_init
void ds18b20_init(void)
{
	// 通过 APB2 总线使能 GPIOA 组的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
}

// 复位及存在检测 (握手)
static u8 ds18b20_reset(void)
{
	u8 presence = 0;  // 存在标志
	
	ds18b20_output_status(1);  // 拉高数据线
	ds18b20_output_status(0);  // 拉低数据线
	delay_us(480);  // 至少 480us
	ds18b20_output_status(1);  // 拉高数据线
	delay_us(70);  // 等待 15-60us 后设备会拉低
	
	if(ds18b20_input_status() == 0)  // 判断数据线是否被拉低
		presence = 1;  // 存在(握手成功)
	
	delay_us(410);  // 完成整个 480us 的接收时段
	
	return presence;
}

// 写一个字节
static void ds18b20_write_byte(u8 data)
{
	u8 i = 0;
	for(i = 0; i < 8; i++)
	{
		ds18b20_output_status(0);  // 拉低数据线
		delay_us(2);  // 保持极短时间表示起始
		if(data & 0x01)
			ds18b20_output_status(1);  // 拉高数据线
		else
			ds18b20_output_status(0);  // 拉低数据线
		delay_us(60);  // 持续 60us 以上
		ds18b20_output_status(1);  // 拉高数据线
		data >>= 1;
	}
}

// 读一个位
static u8 ds18b20_read_bit(void)
{
	u8 bit = 0;
	
	ds18b20_output_status(0);  // 拉低数据线
	delay_us(15);
	if(ds18b20_input_status())
		bit = 1;
	delay_us(50);  // 等待时隙结束
	
	return bit;
}

// 第一个字节
static u8 ds18b20_read_byte(void)
{
	u8 i = 0;
	u8 data = 0;
	for(i = 0; i < 8; i++)
	{
		if(ds18b20_read_bit())
			data |= (0x01 << i); // 从低位到高位依次填充
	}
	
	return data;
}

// 获取温度
short ds18b20_get_temp(void)
{
	u8 TL, TH = 0;
    short temp = 0;
	
	ds18b20_reset();
	ds18b20_write_byte(DS18B20_CMD_SKIP_ROM);   // 跳过 ROM
	ds18b20_write_byte(DS18B20_CMD_CONVERT_T);  // 启动温度转换
	
	// 这里需要等待转换完成。12位精度最大需750ms 
    // 简单处理可直接延时，或检测总线直到变高电平
    delay_ms(750);
	
	ds18b20_reset();
	ds18b20_write_byte(DS18B20_CMD_SKIP_ROM);
    ds18b20_write_byte(DS18B20_CMD_READ_SCR);  // 读取暂存器（获取温度）
	
	TL = ds18b20_read_byte();  // 温度低字节
	TH = ds18b20_read_byte();  // 温度高字节
	
	temp = (TH << 8) | TL;
	
	// 高5位是符号位(表示正负温度) + 中间7位是整数部分 + 低4位是小数部分 (精度为0.0625°C)
	// 标准公式：实际温度 = temp × 0.0625
	// temp × 0.0625 × 100 = temp × 6.25 转换为四位整数, 将来好在数码管上显示
	return (short)(temp * 6.25f);	
}


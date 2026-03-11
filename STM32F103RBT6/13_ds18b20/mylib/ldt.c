#include "ldt.h"
#include "bitband.h"
#include "delay.h"

#define HC138_A0 PCOut(11)
#define	HC138_A1 PCOut(12)
#define	HC138_A2 PDOut(2)
#define	HC595_SCLK PBOut(5)
#define	HC595_LCLK PBOut(4)
#define	HC595_DATA PBOut(3)

// 全局编码表
static const u8 ledcode[] = {
    0x3f, 0x06, 0x5b, 0x4f, // 0 1 2 3
    0x66, 0x6d, 0x7d, 0x07, // 4 5 6 7
    0x7f, 0x6f, 0x77, 0x7c, // 8 9 A b
    0x39, 0x5e, 0x79, 0x71, // c d E F
    0x40, 0x00              // [16]是负号'-', [17]是全灭
};

// 初始化数码管使用的 GPIO 管脚
void ldt_init(void)
{
	// 通过 APB2 总线使能 GPIOB GPIOC GPIOD AFIO(复用功能 IO) 组的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
	
	// 把 PB3 的调试功能禁用，作为普通 IO 使用
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	// 把这些管脚初始化为推挽输出
	GPIO_InitTypeDef Gpio_Value;  // 定义 GPIO 结构体类型的变量
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_Out_PP;  // 选择推挽输出模式
	Gpio_Value.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	Gpio_Value.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &Gpio_Value);
	
	Gpio_Value.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOD, &Gpio_Value);
	
	Gpio_Value.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_4 | GPIO_Pin_3;
	GPIO_Init(GPIOB, &Gpio_Value);
}

// 通过 74HC138 模块选择数码管的公共端
void hc138_out_data(u8 data)
{
	switch(data)
	{
		case 0: HC138_A2 = 0; HC138_A1 = 0; HC138_A0 = 0; break;  // 让 CH1 公共端有效
		case 1: HC138_A2 = 0; HC138_A1 = 0; HC138_A0 = 1; break;  // 让 CH2 公共端有效
		case 2: HC138_A2 = 0; HC138_A1 = 1; HC138_A0 = 0; break;  // 让 CH3 公共端有效
		case 3: HC138_A2 = 0; HC138_A1 = 1; HC138_A0 = 1; break;  // 让 CH4 公共端有效
	}
}	

// 通过 74HC595 模块把串行的编码转换成并行的数据
void hc595_send_data(u8 data)
{
	u8 i = 0;// 循环变量
	for(i = 0; i < 8; i++)  // 循环取每一位编码数据
	{
		HC595_SCLK = 0;  // 拉低移位寄存器的时钟线
		if(data & 0x80)  // 取形参 data 的最高位数据
			HC595_DATA = 1;  // 通过串行数据线输入 1
		else
			HC595_DATA = 0;  // 通过串行数据线输入 0
		data <<= 1;  // 让形参 data 的每一位数据左移一位
		HC595_SCLK = 1;  // 拉高移位寄存器的时钟线
	}
	HC595_LCLK = 0;  // 拉低存储寄存器的时钟线
	HC595_LCLK = 1;  // 拉高存储寄存器的时钟线
}

// 把形参的数字通过四位数码管显示
void digit_show_data(int data)
{
	u8 d[4] = {0};  // 存储四位数字的每一位
	u8 i = 0;  // 循环变量
	
	// 把 data 的每一位存到数组
	d[0] = data / 1000;
	d[1] = data / 100 % 10;
	d[2] = data / 10 % 10;
	d[3] = data % 10;
	
	for(i = 0; i < 4; i++)
	{
		hc138_out_data(i);  // 循环选择公共端有效
		hc595_send_data(ledcode[d[i]]);  // 把该位置上的数字的编码转换成并行数字
		delay_ms(1);  // 延时 1ms
		hc595_send_data(0);  // 手动清空 74HC595 的寄存器
	}
}

/**
 * @brief 在指定位置显示一个数字，并选择是否点亮小数点
 * @param pos      位置 (0-3)
 * @param number   要显示的数字 (0-15), 16为负号, 17为灭
 * @param show_dot 1:点亮小数点, 0:不亮
 */
void ldt_display(u8 pos, u8 number, u8 show_dot)
{
    u8 seg_code;
    
    if(number > 17) return;
    
    seg_code = ledcode[number];
    
    // 如果需要显示小数点，将第7位（DP）置1
    if(show_dot) 
        seg_code |= 0x80;
    
    hc138_out_data(pos);     // 选择位数
    hc595_send_data(seg_code); // 发送段码
    delay_ms(1);             // 扫描延时
    hc595_send_data(0x00);   // 消隐，防止重影
}



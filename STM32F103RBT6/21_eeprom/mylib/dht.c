#include "dht.h"
#include "bitband.h"
#include "delay.h"

// 把 DHT11 使用的 GPIO 管脚初始化为推挽输出
static void set_dht_gpio_output(void)
{
	GPIO_InitTypeDef Gpio_Value;  // 定义 GPIO 结构体类型的变量
	Gpio_Value.GPIO_Mode = GPIO_Mode_Out_PP;  // 推挽输出
	Gpio_Value.GPIO_Pin = GPIO_Pin_10;
	Gpio_Value.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &Gpio_Value);
}

// 把 DHT11 使用的 GPIO 管脚初始化为浮空输入
static void set_dht_gpio_input(void)
{
	GPIO_InitTypeDef Gpio_Value;  // 定义 GPIO 结构体类型的变量
	Gpio_Value.GPIO_Mode = GPIO_Mode_IN_FLOATING;  // 浮空输入
	Gpio_Value.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOC, &Gpio_Value);
}

// 控制数据线输出高低电平
static void dht_output_status(int state)
{
	set_dht_gpio_output();  // 把 PC10 管脚初始化为推挽输出
	
	if(state == 0)  // 判断是否拉低数据线
		PCOut(10) = 0;
	else
		PCOut(10) = 1;
}

// 获取数据线的电平高低
static int dht_input_status(void)
{
	set_dht_gpio_input();  // 把 PC10 管脚初始化为浮空输入
	
	return PCIn(10);
}

// 初始化 DHT11
void dht_init(void)
{
	// 通过 APB2 总线使能 GPIOC 组的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
}

// 获取 DHT11 采集的数据
void get_dht_value(u8 data[5])
{
	u8 flag = 0;  // 用来获取数据线的电平高低
	u8 time = 0;  // 计数变量
	u8 i = 0, j = 0;  // 循环变量
	
	dht_output_status(1);  // 拉高数据线
	dht_output_status(0);  // 拉低数据线
	delay_ms(20);  // 延时 20ms
	dht_output_status(1);  // 拉高数据线
	
	// 以超时处理的方式等待 DHT11 拉低数据线
	do
	{
		flag = dht_input_status();  // 读取数据线的电平高低
		delay_us(2);  // 延时 2us
		time++;  // 计数变量自增
	}while(flag == 1 && time <= 20);  // 如果获取数据线的电平是低或者超时了跳出循环
	if(time > 20)  // 判断是否超时 (大于 40us)
		return;  // 由于超时结束函数
	
	while(!dht_input_status());  // 以死等的方式等待数据线被拉高
	
	// 循环接收 40bit 数据, 高位先出
	for(i = 0; i < 5; i++)  // 遍历数组, 每个元素存储 1 字节
	{
		data[i] = 0;  // 清零
		for(j = 0; j < 8; j++)  // 遍历每位, 1 字节 8 位
		{
			while(dht_input_status());   // 以死等方式等待 DHT11 拉低数据线 (等待前一周期的高电平结束)
			while(!dht_input_status());  // 以死等方式等待 DHT11 拉高数据线 (等待当前位的高电平开始)
			
			delay_us(40);  // 延时 40us 后采样 (26us-28us 表示0   70us 表示1)
			
			if(dht_input_status() == 1)
				data[i] |= (0x80 >> j);
		}
	}
	dht_output_status(1);  // 最后拉高数据线, 数据传送结束
}


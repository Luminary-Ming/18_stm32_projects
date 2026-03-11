#include "dht.h"//包含头文件
#include "bitband.h"//包含头文件
#include "delay.h"//包含头文件

void set_dht_gpio_output(void)//把DHT11所连接的GPIO管脚配置成输出模式
{
	GPIO_InitTypeDef Gpio_Value;//定义了GPIO固件的结构体变量
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_Out_PP;//选择了推挽的输出模式
	Gpio_Value.GPIO_Pin = GPIO_Pin_10;//选择了10号GPIO管脚
	Gpio_Value.GPIO_Speed = GPIO_Speed_50MHz;//选择了50MHz的输出速度
	GPIO_Init(GPIOC, &Gpio_Value);//按照上述配置初始化GPIOC组的管脚
}

void set_dht_gpio_input(void)//把DHT11所连接的GPIO管脚配置成输入模式
{
	GPIO_InitTypeDef Gpio_Value;//定义了GPIO固件的结构体变量
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_IN_FLOATING;//选择了浮空的输入模式
	Gpio_Value.GPIO_Pin = GPIO_Pin_10;//选择了10号GPIO管脚
	GPIO_Init(GPIOC, &Gpio_Value);//按照上述配置初始化GPIOC组的管脚
}

void dht_output_state(int status)//通过参数status把所连接的GPIO管脚输出高电平或者输出低电平
{
	set_dht_gpio_output();//把DHT11所连接的GPIO管脚配置成输出模式
	if(status == 0)//判断参数status是否为0
		PCOut(10) = 0;//通过位带把PC10管脚输出低电平
	else
		PCOut(10) = 1;//通过位带把PC10管脚输出高电平
}

int dht_input_state(void)//检测DHT11所连接的GPIO管脚的高低电平
{
	set_dht_gpio_input();//把DHT11所连接的GPIO管脚配置成输入模式
	return PCIn(10);//通过位带把PC10管脚采集到的高低电平返回
}

void dht_init(void)//初始化的函数(使能所连接的GPIO管脚的时钟)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);//通过APB2总线使能GPIOC组的时钟
}

void get_dht_value(char *buf)//获取DHT11采集到的数据
{
	char flag = 0;//获取状态的变量
	char time = 0;//做超时处理所需的变量
	char i = 0;//循环变量
	char tmp = 0;//获取采集到的每一位数据
	
	dht_output_state(1);//把数据线拉高
	dht_output_state(0);//把数据线拉低
	delay_ms(20);				//延时20毫秒
	dht_output_state(1);//把数据线拉高
	/*接下来等待数据线被DHT11拉低(死等 超时处理)*/
	//while(dht_input_state());//死等
	do//超时处理
	{
		flag = dht_input_state();//获取DHT11所连接的GPIO管脚的高低电平
		delay_us(2);//延时2微秒
		time++;//time自增
	}while(flag == 1 && time <= 20);//判断flag是否是1并且time小于等于20
	if(time > 20)//如果time大于20,超时
		return ;//结束功能函数
	
	while(!dht_input_state());//等待数据线拉高
	
	for(i = 0; i < 40; i++)//一次完整数据有40位=8位湿度整数+8位湿度小数+8位温度整数+8位温度小数+8位校验位
	{
		while(dht_input_state());//等待数据线拉低
		while(!dht_input_state());//等待数据线拉高
		delay_us(40);//延时40微秒
		tmp <<= 1;//按位左移1位
		if(dht_input_state())//判断检测到的电信号是否为1
			tmp |= 1;//tmp按位或上1
		if((i + 1) % 8 == 0)//当i = 7 15 23 31 39进入if把独到的8位数据保存到buf中
		{
			buf[i / 8] = tmp;//把tmp存入到buf中
			tmp = 0;//tmp清零
		}
	}
	dht_output_state(1);//把数据线拉高
}








#include "buzzer.h"//包含头文件
#include "bitband.h"//包含有关位带的头文件

void buzzer_init(void)//初始化的函数(初始化所连接的GPIO管脚)
{
	GPIO_InitTypeDef Buzzer_Value;//定义了GPIO固件的结构体变量
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);//通过APB2总线使能GPIOC组的时钟
	
	Buzzer_Value.GPIO_Mode = GPIO_Mode_Out_PP;//选择推挽的输出模式
	Buzzer_Value.GPIO_Pin = GPIO_Pin_7;//选择 7 号管脚
	Buzzer_Value.GPIO_Speed = GPIO_Speed_50MHz;//选择了50MHz的输出速度
	GPIO_Init(GPIOC, &Buzzer_Value);//按照上述配置初始化GPIOC组的管脚
	
	PCOut(7) = 0;//使用位带使PC7管脚拉低
}

void buzzer_on(void)//使蜂鸣器响的函数
{
	PCOut(7) = 1;//使用位带使PC7管脚拉低
}

void buzzer_off(void)//使蜂鸣器不响的函数
{
	PCOut(7) = 0;//使用位带使PC7管脚拉低
}






#include "led.h"//包含头文件
#include "bitband.h"//包含有关位带的头文件

void led_init(void)//初始化的函数(初始化所连接的GPIO管脚)
{
	GPIO_InitTypeDef Led_Value;//定义了GPIO固件的结构体变量
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//通过APB2总线使能GPIOC组的时钟
	
	Led_Value.GPIO_Mode = GPIO_Mode_Out_PP;//选择推挽的输出模式
	Led_Value.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;//选择 1 2 3 号管脚
	Led_Value.GPIO_Speed = GPIO_Speed_50MHz;//选择了50MHz的输出速度
	GPIO_Init(GPIOC, &Led_Value);//按照上述配置初始化GPIOC组的管脚
	
	PCOut(1) = 0;//使用位带使PC1管脚拉低
	PCOut(2) = 0;//使用位带使PC2管脚拉低
	PCOut(3) = 0;//使用位带使PC3管脚拉低
}

void led_on(int nu)//根据参数nu点亮相应的led灯的函数
{
	switch(nu)
	{
		case 0 : PCOut(1) = 1; break;//把PC1管脚拉高
		case 1 : PCOut(2) = 1; break;//把PC2管脚拉高
		case 2 : PCOut(3) = 1; break;//把PC3管脚拉高
	}
}

void led_off(int nu)//根据参数nu关闭相应的led灯的函数
{
	switch(nu)
	{
		case 0 : PCOut(1) = 0; break;//把PC1管脚拉低
		case 1 : PCOut(2) = 0; break;//把PC2管脚拉低
		case 2 : PCOut(3) = 0; break;//把PC3管脚拉低
	}
}





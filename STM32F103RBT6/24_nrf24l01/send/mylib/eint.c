#include "eint.h"//包含头文件

eint_handler e_h0, e_h1;//定义两个全局的函数指针

void eint_init(void)//初始化的函数(初始化所连接的GPIO管脚和中断控制器以及外部中断)
{
	GPIO_InitTypeDef Gpio_Value;//定义了GPIO固件的结构体变量
	NVIC_InitTypeDef Nvic_Value;//定义了中断控制器固件的结构体变量
	EXTI_InitTypeDef Exti_Value;//定义了外部中断固件的结构体变量
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
	//通过APB2总线使能GPIOC组的和AFIO的时钟
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_IPU;//选择上拉输入模式
	Gpio_Value.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8;//选择管脚9 8
	Gpio_Value.GPIO_Speed = GPIO_Speed_50MHz;//选择50MHz输出速度
	GPIO_Init(GPIOC, &Gpio_Value);//按照上述配置初始化GPIOC组的管脚
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource9);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource8);
	//为外部中断9选择由PC9触发 为外部中断8选择由PC8触发
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//配置组优先级和子优先级的所占比例
	Nvic_Value.NVIC_IRQChannel = EXTI9_5_IRQn;//选择中断号
	Nvic_Value.NVIC_IRQChannelCmd = ENABLE;//使能该中断
	Nvic_Value.NVIC_IRQChannelPreemptionPriority = 2;//配置组优先级
	Nvic_Value.NVIC_IRQChannelSubPriority = 2;//配置子优先级
	NVIC_Init(&Nvic_Value);//按照上述配置初始化NVIC
	
	Exti_Value.EXTI_Line = EXTI_Line9 | EXTI_Line8;//选择外部中断号9 8
	Exti_Value.EXTI_LineCmd = ENABLE;//使能该外部中断
	Exti_Value.EXTI_Mode = EXTI_Mode_Interrupt;//选择中断的模式
	Exti_Value.EXTI_Trigger = EXTI_Trigger_Falling;//选择下降沿触发中断
	EXTI_Init(&Exti_Value);//按照上述匹配初始化EXTI
}

void EXTI9_5_IRQHandler(void)//异常向量表中有关外部中断9_5的中断处理函数
{
	if(EXTI_GetITStatus(EXTI_Line9) == SET)//判断是否是由外部中断9触发的中断
	{
		e_h0();//调用按键按下之后要做的处理
		EXTI_ClearITPendingBit(EXTI_Line9);//清除外部中断9中断标志位
	}
	if(EXTI_GetITStatus(EXTI_Line8) == SET)//判断是否是由外部中断8触发的中断
	{
		e_h1();//调用按键按下之后要做的处理
		EXTI_ClearITPendingBit(EXTI_Line8);//清除外部中断8中断标志位
	}
}

void set_eint_handler(eint_handler h0, eint_handler h1)//设置回调函数
{
	e_h0 = h0;//把形参h0所存储的地址保存到全局变量e_h0中
	e_h1 = h1;//把形参h1所存储的地址保存到全局变量e_h1中
}







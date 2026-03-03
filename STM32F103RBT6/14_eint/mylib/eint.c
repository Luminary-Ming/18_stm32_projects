#include "eint.h"
#include "bitband.h"

eint_handler e_h0, e_h1, e_h2;  // 定义全局函数指针

// 初始化功能按键使用的 GPIO 管脚和外部中断以及中断控制器
void eint_init(void)
{
	GPIO_InitTypeDef Gpio_Value;  // 定义 GPIO 结构体类型的变量
	NVIC_InitTypeDef Nvic_Value;  // 定义 NVIC 结构体类型的变量
	EXTI_InitTypeDef Exti_Value;  // 定义 EXTI 结构体类型的变量
	 
	// 通过 APB2 总线使能 GPIOA 组和 GPIOC 组以及 AFIO 的时钟
	// GPIOA 和 GPIOC 时钟使能：是因为要使用这两个端口的引脚作为输入
	// AFIO 时钟使能：是因为外部中断需要重映射引脚（将普通 GPIO 连接到 EXTI），所以必须开启 AFIO 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_IPU;  // 上拉输入
	Gpio_Value.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;  // 使用第 8 9 号管脚
	GPIO_Init(GPIOC, &Gpio_Value);  // 按照上述配置初始化 GPIOC 组的管脚
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_IPD;  // 下拉输入
	Gpio_Value.GPIO_Pin = GPIO_Pin_0;  // 使用第 0 号管脚
	GPIO_Init(GPIOA, &Gpio_Value);  // 按照上述配置初始化 GPIOA 组的管脚
	

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource9);  // 为 EXTI9 选择由 GPIOC 的 PC9 管脚触发
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource8);  // 为 EXTI8 选择由 GPIOC 的 PC8 管脚触发
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);  // 为 EXTI0 选择由 GPIOA 的 PA0 管脚触发
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  // 选择抢占优先级和响应优先级的所占比例为 2 : 2
	Nvic_Value.NVIC_IRQChannel = EXTI9_5_IRQn;  // 选择 EXTI9_5 中断号
	Nvic_Value.NVIC_IRQChannelCmd = ENABLE;  // 使能该中断
	Nvic_Value.NVIC_IRQChannelPreemptionPriority = 2;  // 选择抢占优先级的级别为 2 (范围 0 ~ 3 <00 ~ 11>)
	Nvic_Value.NVIC_IRQChannelSubPriority = 2;  // 选择响应优先级的级别为 2 (范围 0 ~ 3 <00 ~ 11>)
	// 抢占优先级相同, 响应优先级也相同, 所以最终的优先级以中断号为标准
	NVIC_Init(&Nvic_Value);  // 按照上述配置初始化 NVIC
	 
	Nvic_Value.NVIC_IRQChannel = EXTI0_IRQn;  // 选择 EXTI0 中断号
	NVIC_Init(&Nvic_Value);  // 按照上述配置初始化 NVIC
	 
	Exti_Value.EXTI_Line = EXTI_Line9 | EXTI_Line8;  // 选择 EXTI9 和 EXTI8 外部中断
	Exti_Value.EXTI_LineCmd = ENABLE;  // 使能该外部中断
	Exti_Value.EXTI_Mode = EXTI_Mode_Interrupt;  // 选择中断模式
	Exti_Value.EXTI_Trigger = EXTI_Trigger_Falling;  // 选择下降沿触发
	EXTI_Init(&Exti_Value);  // 按照上述配置初始化 EXTI9 和 EXTI8
	
	Exti_Value.EXTI_Line = EXTI_Line0;  // 选择 EXTI0 外部中断
	Exti_Value.EXTI_Trigger = EXTI_Trigger_Rising;  // 选择上升沿触发
	EXTI_Init(&Exti_Value);  // 按照上述配置初始化 EXTI0
}

// EXTI9_5 的中断处理函数
void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line9) == SET)  // 判断是否是 EXTI9 触发的中断
	{
		e_h0();  // 调用客户指定的函数功能
		EXTI_ClearITPendingBit(EXTI_Line9);  // 清除 EXTI9 的中断标志位
	}
	
	if(EXTI_GetITStatus(EXTI_Line8) == SET)  // 判断是否是 EXTI8 触发的中断
	{
		e_h1();  // 调用客户指定的函数功能
		EXTI_ClearITPendingBit(EXTI_Line8);  // 清除 EXTI8 的中断标志位
	}
}

// EXTI0 的中断处理函数
void EXTI0_IRQHandler(void)
{
	e_h2();  // 调用客户指定的函数功能
	EXTI_ClearITPendingBit(EXTI_Line0);  // 清除 EXTI0 的中断标志位
}

// 设置回调函数
void set_eint_handler(eint_handler h0, eint_handler h1, eint_handler h2)
{
	e_h0 = h0;  // 把形参 h0 存储的地址, 转存到全局函数指针 e_h0 中
	e_h1 = h1;  // 把形参 h1 存储的地址, 转存到全局函数指针 e_h1 中
	e_h2 = h2;  // 把形参 h2 存储的地址, 转存到全局函数指针 e_h2 中
}


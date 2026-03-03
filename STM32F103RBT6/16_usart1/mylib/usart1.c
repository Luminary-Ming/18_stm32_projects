#include "usart1.h"

// 初始化 USART1 使用的管脚
void usart1_init(void)
{
	GPIO_InitTypeDef Gpio_Value;  // 定义 GPIO 结构体类型的变量
	USART_InitTypeDef Usart_Value;
	
	// 通过 APB2 总线使能 GPIOA 组和 USART1 的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_AF_PP;  // 推挽复用
	Gpio_Value.GPIO_Pin = GPIO_Pin_9;
	Gpio_Value.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &Gpio_Value);
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_IN_FLOATING;  // 浮空输入
	Gpio_Value.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &Gpio_Value);
	
	Usart_Value.USART_BaudRate = 115200;  // 选择 115200 波特率
	Usart_Value.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	Usart_Value.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  // 选择发送和接收
	Usart_Value.USART_Parity = USART_Parity_No;  // 选择没有奇偶校验
	Usart_Value.USART_StopBits = USART_StopBits_1;  // 选择 1 个停止位
	Usart_Value.USART_WordLength = USART_WordLength_8b;  // 选择 8 个停止位
	
	USART_Init(USART1, &Usart_Value);  // 按照上述配置初始化 USART1
	
	USART_Cmd(USART1, ENABLE);  // 使能 USART1 的功能
}	

// 通过 USART1 发送 1byte 的数据
void usart1_send_byte(u8 data)
{
	USART1->SR;  // 防止第一个字节数据丢失
	
	USART_SendData(USART1, data);  // 把形参 data 通过 USART1 进行发送	
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);  // 以死等的方式等待 USART1 发送数据成功
	USART_ClearFlag(USART1, USART_FLAG_TC);  // 清除 USART1 发送数据成功的标志状态	
}	

// 通过 USART1 接收 1byte 的数据
u8 usart1_recv_byte(void)
{
	u8 data = 0;  // 存储 USART1 接收到的数据
	
	if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)  == SET)  // 判断 USART1 是否接收到了数据
	{
		data = USART_ReceiveData(USART1);  // 获取 USART1 接收到的数据
		USART_ClearFlag(USART1, USART_FLAG_RXNE);  // 清除 USART1 接收数据成功的标志状态	
	}
	
	return data;  // 返回 USART1 接收到的数据
}


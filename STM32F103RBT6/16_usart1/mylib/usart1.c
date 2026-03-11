#include "usart1.h"
#include "stdio.h"
#include "string.h"

#define USART1_RECV_BUF_SIZE  200  // 接收缓冲区大小

// 定义全局函数指针
usart1_handler u1_h;  

// 禁用半主机模式 (禁用后就没有 stdin stdout 了)
#pragma import(__use_no_semihosting)

struct __FILE  // 弱定义 FILE 类型
{
	int a;
};

FILE __stdout;  // 弱定义标准输出
FILE __stdin;  // 弱定义标准输入

// 加上这个函数, 否则编译不过
// .\Objects\test.axf: Error: L6915E: Library reports error: __use_no_semihosting was requested, but _sys_exit was referenced
void _sys_exit(void) {}

// 加上这个函数, 否则编译不过
// .\Objects\test.axf: Error: L6915E: Library reports error: __use_no_semihosting was requested, but _ttywrch was referenced
void _ttywrch(int ch) {}	

// 使用 USART1 重新实现 fputc
int fputc(int c, FILE *fp)
{
	usart1_send_byte(c);  // 调用 USART1 发送 1byte 的数据
	return c;
}

// 初始化 USART1 使用的管脚
void usart1_init(void)
{
	GPIO_InitTypeDef Gpio_Value;  // 定义 GPIO 结构体类型的变量
	USART_InitTypeDef Usart_Value;
	NVIC_InitTypeDef Nvic_Value;  // 定义 NVIC 结构体类型的变量
	
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
	Usart_Value.USART_WordLength = USART_WordLength_8b;  // 选择 8 个数据位
	
	USART_Init(USART1, &Usart_Value);  // 按照上述配置初始化 USART1
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  // 选择抢占优先级和响应优先级的所占比例为 2 : 2
	Nvic_Value.NVIC_IRQChannel = USART1_IRQn;  // 选择 USART1_IRQn 中断号
	Nvic_Value.NVIC_IRQChannelCmd = ENABLE;  // 使能该中断
	Nvic_Value.NVIC_IRQChannelPreemptionPriority = 2;  // 选择抢占优先级的级别为 2
	Nvic_Value.NVIC_IRQChannelSubPriority = 2;  // 选择响应优先级的级别为 2
	NVIC_Init(&Nvic_Value);  // 按照上述配置初始化 NVIC
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  // 使能 USART1 接收数据的中断
	 
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

// 通过 USART1 发送多个字节的数据
void usart1_send_data(u8 *buf)
{
	USART1->SR;  // 防止第一个字节数据丢失
	
	while(*buf != '\0')
	{
		usart1_send_byte((u8)(*buf));  // 把形参 data 通过 USART1 进行发送
		buf++;
	}
}	

// 中断处理函数
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)  // 判断是否是由 USART1 接收数据触发的中断
	{
		u1_h(USART_ReceiveData(USART1));  // 获取 USART1 接收的数据
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);  // 清除 USART1 接收数据的中断标志位
	}
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

// 通过 USART1 接收多个字节的数据 (精简版)
u8 *usart1_recv_data(void)
{
	u16 len = 0;
	
	static u8 recv_buf[100];  // 静态缓冲区
	
	// 循环获取 USART1 接收到的数据
	while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)
	{
		recv_buf[len++] = USART_ReceiveData(USART1);  // 获取 USART1 接收到的数据
		USART_ClearFlag(USART1, USART_FLAG_RXNE);  // 清除 USART1 接收数据成功的标志状态	
	}
	
	recv_buf[len] = '\0';  // 添加字符串结束符
	
	return recv_buf;
}

// 设置回调函数
void set_usart1_handler(usart1_handler h)
{
	u1_h = h;  // 把形参 h 存储的地址, 转存到全局函数指针 u1_h 中
}


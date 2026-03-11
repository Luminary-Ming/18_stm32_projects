#include "usart1.h"//包含头文件

usart1_handler u1_h;//定义一个全局的函数指针

void usart_1_init(void)//初始化的函数(初始化所连接的GPIO管脚和串口功能以及中断控制器)
{
	GPIO_InitTypeDef Gpio_Value;//定义了GPIO固件的结构体变量
	USART_InitTypeDef Usart_Value;//定义了串口固件的结构体变量
	NVIC_InitTypeDef Nvic_Value;//定义了中断控制器固件的结构体变量
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
	//通过APB2总线使能GPIOA组和USART1的时钟
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_AF_PP;//选择了推挽的复用模式
	Gpio_Value.GPIO_Pin = GPIO_Pin_9;//选择了管脚9(PA9是发送端)
	Gpio_Value.GPIO_Speed = GPIO_Speed_50MHz;//选择了50MHz的输出速度
	GPIO_Init(GPIOA, &Gpio_Value);//按照上述配置初始化GPIOA组的管脚
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_IN_FLOATING;//选择了浮空的输入模式
	Gpio_Value.GPIO_Pin = GPIO_Pin_10;//选择了管脚10(PA10是接收端)
	GPIO_Init(GPIOA, &Gpio_Value);//按照上述配置初始化GPIOA组的管脚
	
	Usart_Value.USART_BaudRate = 115200;//选择了115200的波特率
	Usart_Value.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//选择关闭了硬件流控
	Usart_Value.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//选择了发送和接收模式
	Usart_Value.USART_Parity = USART_Parity_No;//选择没有奇偶检验位
	Usart_Value.USART_StopBits = USART_StopBits_1;//选择1个停止位
	Usart_Value.USART_WordLength = USART_WordLength_8b;//选择8个数据位
	USART_Init(USART1, &Usart_Value);//按照上述配置初始化USART1
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//配置组优先级和子优先级的所占比例
	Nvic_Value.NVIC_IRQChannel = USART1_IRQn;//选择中断号
	Nvic_Value.NVIC_IRQChannelCmd = ENABLE;//使能该中断
	Nvic_Value.NVIC_IRQChannelPreemptionPriority = 2;//配置组优先级
	Nvic_Value.NVIC_IRQChannelSubPriority = 2;//配置子优先级
	NVIC_Init(&Nvic_Value);//按照上述配置初始化NVIC
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//使能了USART1的接收触发中断
	
	USART_Cmd(USART1, ENABLE);//使能USART1的功能
}

void usart_1_send_byte(unsigned char c)//发送一个字节数据
{
	USART_SendData(USART1, c);//通过USART1发送变量c所存储的数据
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);//检验USART1的发送数据是否成功
	USART_ClearFlag(USART1, USART_FLAG_TC);//清除USART1的发送成功的标志
}

void usart_1_send_data(char *buf)//发送一个字符串数据
{
	while(*buf)//轮询判断是否碰到字符串尾零的位置
	{
		usart_1_send_byte(*buf);//调用发送一个字节数据的函数把获取到的字符发送
		buf++;//指针偏移到下一个字符
	}
}

unsigned char usart_1_recv_byte(void)//接收一个字节数据(轮询的方式)
{
	unsigned char ret = 0;//定义ret变量接收串口接到的数据
	
	if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)//判断USART1是否接到了数据
	{
		ret = USART_ReceiveData(USART1);//把接收到的数据存储到ret变量中
		USART_ClearFlag(USART1, USART_FLAG_RXNE);//清除USART1的标志状态
	}
	
	return ret;//返回接收到的数据
}

void USART1_IRQHandler(void)//异常向量表中有关USART1的中断处理函数
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)//判断是否是USART1的接收触发的中断
	{
		u1_h(USART_ReceiveData(USART1));//接收数据,并把接收到的数据传递给回调函数
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);//清除USART1的接收中断标志
	}
}

void set_usart1_handler(usart1_handler h)//设置回调函数
{
	u1_h = h;//把形参h所存储的地址保存到全局变量u1_h中
}





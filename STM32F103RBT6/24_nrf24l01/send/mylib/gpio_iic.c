#include "gpio_iic.h"//包含头文件
#include "delay.h"//包含头文件
#include "bitband.h"//包含头文件

/*
用GPIO的方式模拟IIC总线这种方法,和硬件连接的GPIO有关
我的设备接到不同的GPIO上,将来就需要配置响应的GPIO
因为我们做的实验有OLED EEPROM SHT30,而这些设备都是连接在了相同的管脚上
所以,在使用时从设备地址一定不要写错
*/

//以下代码是硬件相关的，移植时候需要修改
GPIO_InitTypeDef  GPIO_InitForSCL;//定义了GPIO固件的结构体变量
GPIO_InitTypeDef  GPIO_InitForSDAOut;//定义了GPIO固件的结构体变量
GPIO_InitTypeDef  GPIO_InitForSDAIn;//定义了GPIO固件的结构体变量

#define IIC_SCL					PBOut(6)//把PB6的输出做成宏
#define IIC_SDA					PBOut(7)//把PB7的输出做成宏
#define IIC_SDA_STATUS 	PBIn(7)//把PB7的输入做成宏

void IIC_Init(void)//初始化IIC使用的GPIO
{			
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//通过APB2总线使能GPIOB组的时钟
	
	//PB6 SCL
  GPIO_InitForSCL.GPIO_Mode = GPIO_Mode_Out_PP;//选择了推挽的输出模式
	GPIO_InitForSCL.GPIO_Pin = GPIO_Pin_6;//选择了6号GPIO管脚
	GPIO_InitForSCL.GPIO_Speed = GPIO_Speed_50MHz;//选择了50MHz的输出速度
	
	//PB7 SDA OUT
	GPIO_InitForSDAOut.GPIO_Mode = GPIO_Mode_Out_PP;//选择了推挽的输出模式
  GPIO_InitForSDAOut.GPIO_Pin = GPIO_Pin_7;//选择了7号GPIO管脚
	GPIO_InitForSDAOut.GPIO_Speed = GPIO_Speed_50MHz;//选择了50MHz的输出速度
	
  //PB7 SDA IN
  GPIO_InitForSDAIn.GPIO_Mode = GPIO_Mode_IPU;//选择了上拉的输入模式
  GPIO_InitForSDAIn.GPIO_Pin = GPIO_Pin_7;//选择了7号GPIO管脚
	GPIO_InitForSDAIn.GPIO_Speed = GPIO_Speed_50MHz;//选择了50MHz的输出速度
	
  GPIO_Init(GPIOB, &GPIO_InitForSDAOut);//按照上述配置初始化GPIOB组的管脚
  GPIO_Init(GPIOB, &GPIO_InitForSCL);//按照上述配置初始化GPIOB组的管脚
	
	IIC_SCL = 1;//把时钟线拉高
	IIC_SDA = 1;//把数据线拉高
}

void IIC_Sda_In(void)//把数据线所使用的GPIO管脚配置为输入模式
{
	GPIO_Init(GPIOB, &GPIO_InitForSDAIn);//按照上述配置初始化GPIOB组的管脚
}

void IIC_Sda_Out(void)//把数据线所使用的GPIO管脚配置为输出模式
{
	GPIO_Init(GPIOB, &GPIO_InitForSDAOut);//按照上述配置初始化GPIOB组的管脚
}
//以上代码是硬件相关的，移植时候需要修改

//以下代码硬件无关，只跟iic协议有关
void IIC_Start(void)//产生开始信号
{
	IIC_Sda_Out();//把数据线所使用的GPIO管脚配置为输出模式
	IIC_SDA = 1;//拉高数据线
	IIC_SCL = 1;//拉高时钟线
	delay_us(2);//延时2微秒
 	IIC_SDA = 0;//拉低数据线
	delay_us(2);//延时2微秒
	IIC_SCL = 0;//拉低时钟线
}	  

void IIC_Stop(void)//产生结束信号
{
	IIC_Sda_Out();//把数据线所使用的GPIO管脚配置为输出模式
	IIC_SCL = 0;//拉低时钟线
	IIC_SDA = 0;//拉低数据线
 	delay_us(2);//延时2微秒
	IIC_SCL = 1;//拉高时钟线
	delay_us(1);//延时1微秒
	IIC_SDA = 1;//拉高数据线
	delay_us(2);//延时2微秒
}

u8 IIC_Wait_Ack(void)//等待ACK
{
	u8 ucErrTime = 0;//超时处理的计数变量
	
	IIC_Sda_In();//把数据线所使用的GPIO管脚配置为输入模式
	IIC_SCL = 0;//拉低时钟线
	delay_us(1);//延时1微秒
	IIC_SCL = 1;//拉高时钟线
	delay_us(1);//延时1微秒
	while(IIC_SDA_STATUS)//判断数据线是高电平还是低电平
	{
		ucErrTime++;//计数变量自增
		if(ucErrTime > 250)//判断计数变量的值是否超过特定的值
		{
			IIC_Stop();//产生结束信号
			return 1;//返回数值1
		}
	}
	IIC_SCL = 0;//拉低时钟线
	return 0;//返回数值0
} 

void IIC_Ack(void)//产生ACK
{
	IIC_SCL = 0;//拉低时钟线
	IIC_Sda_Out();//把数据线所使用的GPIO管脚配置为输出模式
	IIC_SDA = 0;//拉低数据线
	delay_us(2);//延时2微秒
	IIC_SCL = 1;//拉高时钟线
	delay_us(2);//延时2微秒
	IIC_SCL = 0;//拉低时钟线
}
    
void IIC_NAck(void)//不产生ACK
{
	IIC_SCL = 0;//拉低时钟线
	IIC_Sda_Out();//把数据线所使用的GPIO管脚配置为输出模式
	IIC_SDA = 1;//拉高数据线
	delay_us(2);//延时2微秒
	IIC_SCL = 1;//拉高时钟线
	delay_us(2);//延时2微秒
	IIC_SCL = 0;//拉低时钟线
}					 				     

void IIC_Send_Byte(u8 txd)//利用IIC总线发送数据(一位一位的发送)
{                        
  u8 t;//循环变量
	
	IIC_Sda_Out();//把数据线所使用的GPIO管脚配置为输出模式
  for (t = 0;t < 8; t++)//循环8次
	{  
		IIC_SCL = 0;//拉低时钟线
		delay_us(1);//延时1微秒
    IIC_SDA = (txd & 0x80) >> 7;//把数据txd按照一位一位的方式发送(先发送高位)
    txd <<= 1;//txd按位左移等于1
		delay_us(1);//延时1微秒
		IIC_SCL = 1;//拉高时钟线
		delay_us(2);//延时2微秒
  }
	IIC_SCL = 0;//拉低时钟线
} 	    

u8 IIC_Recv_Byte(unsigned char ack)//利用IIC总线接收数据
{
	unsigned char i = 0;//循环变量
	unsigned char receive = 0;//保存接收到的IIC的数据
	
	IIC_Sda_In();//把数据线所使用的GPIO管脚配置为输入模式
  for(i = 0; i < 8; i++)
	{
    IIC_SCL = 0;//拉低时钟线
    delay_us(2);//延时2微秒
		IIC_SCL = 1;//拉高时钟线
		delay_us(2);//延时2微秒
    receive <<= 1;//receive变量左移等于1
    if(IIC_SDA_STATUS)//判断数据线是高电平还是低电平
			receive |= 1;//receive按位或等于1
  }					 
	IIC_SCL = 0;//拉低时钟线
  if (!ack)//判断形参ack取非是否为真
		IIC_NAck();//不产生ACK
  else
		IIC_Ack();//产生ACK
	
  return receive;//把获取到的一个字节数据返回
}

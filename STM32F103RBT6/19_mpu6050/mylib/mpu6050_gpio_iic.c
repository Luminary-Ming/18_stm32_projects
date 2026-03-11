#include "mpu6050_gpio_iic.h"
#include "delay.h"
#include "bitband.h"

/*
	用GPIO的方式模拟IIC总线这种方法,和硬件连接的GPIO有关
	我的设备接到不同的GPIO上,将来就需要配置响应的GPIO
	因为我们做的实验有OLED EEPROM SHT30,而这些设备都是连接在了相同的管脚上
	所以,在使用时从设备地址一定不要写错
	
	SCL PA5
	SDA PA4
*/

// 以下代码是硬件相关的，移植时候需要修改
GPIO_InitTypeDef  MPU6050_PIO_InitForSCL;
GPIO_InitTypeDef  MPU6050_GPIO_InitForSDAOut;
GPIO_InitTypeDef  MPU6050_GPIO_InitForSDAIn;

#define IIC_SCL			PAOut(5)
#define IIC_SDA			PAOut(4)
#define IIC_SDA_STATUS 	PAIn(4)

void MPU6050_IIC_Init(void)
{			
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	// PA5 SCL
	MPU6050_PIO_InitForSCL.GPIO_Mode = GPIO_Mode_Out_PP;
	MPU6050_PIO_InitForSCL.GPIO_Pin = GPIO_Pin_5;
	MPU6050_PIO_InitForSCL.GPIO_Speed = GPIO_Speed_50MHz;
	
	// PA4 SDA OUT
	MPU6050_GPIO_InitForSDAOut.GPIO_Mode = GPIO_Mode_Out_PP;
	MPU6050_GPIO_InitForSDAOut.GPIO_Pin = GPIO_Pin_4;
	MPU6050_GPIO_InitForSDAOut.GPIO_Speed = GPIO_Speed_50MHz;
	
  // PA4 SDA IN
	MPU6050_GPIO_InitForSDAIn.GPIO_Mode = GPIO_Mode_IPU;
	MPU6050_GPIO_InitForSDAIn.GPIO_Pin = GPIO_Pin_4;
	MPU6050_GPIO_InitForSDAIn.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &MPU6050_GPIO_InitForSDAOut);
	GPIO_Init(GPIOA, &MPU6050_PIO_InitForSCL);
	
	IIC_SCL = 1;
	IIC_SDA = 1;
}

void MPU6050_IIC_Sda_In(void)
{
	GPIO_Init(GPIOA, &MPU6050_GPIO_InitForSDAIn);
}

void MPU6050_IIC_Sda_Out(void)
{
	GPIO_Init(GPIOA, &MPU6050_GPIO_InitForSDAOut);
}
// 以上代码是硬件相关的，移植时候需要修改

// 以下代码硬件无关，只跟 iic 协议有关
void MPU6050_IIC_Start(void)
{
	MPU6050_IIC_Sda_Out();
	IIC_SDA=1;	  	  
	IIC_SCL=1;  
	delay_us(2);
 	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=0;
}	  

void MPU6050_IIC_Stop(void)
{
	MPU6050_IIC_Sda_Out();
	IIC_SCL=0;
	IIC_SDA=0;
 	delay_us(2);
	IIC_SCL=1; 
	delay_us(1);
	IIC_SDA=1;
	delay_us(2);
}

u8 MPU6050_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	MPU6050_IIC_Sda_In();
	IIC_SCL = 0;
	delay_us(1);	   
	IIC_SCL=1;
	delay_us(1);
	while(IIC_SDA_STATUS) {
		ucErrTime++;
		if(ucErrTime>250) {
			MPU6050_IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0; 	   
	return 0;  
} 

void MPU6050_IIC_Ack(void)
{
	IIC_SCL=0;
	MPU6050_IIC_Sda_Out();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
    
void MPU6050_IIC_NAck(void)
{
	IIC_SCL=0;
	MPU6050_IIC_Sda_Out();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}					 				     

void MPU6050_IIC_Send_Byte(u8 txd)
{                        
  u8 t;  
	
	MPU6050_IIC_Sda_Out();    

  for (t = 0;t < 8; t++) {  
		IIC_SCL = 0;
		delay_us(1);	
    IIC_SDA = (txd & 0x80) >> 7;
    txd <<= 1; 	  
		delay_us(1);   
		IIC_SCL=1;
		delay_us(2); 
   }	
	IIC_SCL = 0;
} 	    

u8 MPU6050_IIC_Recv_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	MPU6050_IIC_Sda_In();
  for(i=0;i<8;i++ ) {
    IIC_SCL=0; 
    delay_us(2);
		IIC_SCL=1;
		delay_us(2);
    receive<<=1;
    if(IIC_SDA_STATUS)
			receive |= 1;   
  }					 
	IIC_SCL = 0;
  if (!ack)
		MPU6050_IIC_NAck();
  else
		MPU6050_IIC_Ack(); 
  return receive;
}


#ifndef __SPI_NRF_H__
#define __SPI_NRF_H__
/*
开发板设备		所连接的管脚
CE						PC5
CSN						PC4
SCK						PA7
MOSI					PA6
MISO					PA5
IRQ						PA4
已知芯片的管脚是多功能复用型的管脚
芯片的管脚会有 输入功能 输出功能 复用功能 模拟功能
我们这次操作的是开发板上的无线通信模块Nrf24L01
已知该模块的接口是SPI接口,我们需要把模块接入到有SPI接口的管脚上
但是为了考虑到移植性的问题,我们还是采取使用GPIO的方式模拟SPI协议
*/
#include "stm32f10x_conf.h"//包含了该头文件所有的外设头文件均已包含
//让Nrf模块之间通信(一次发送数据<=32字节)
//用下面的这个结构体(32字节)进行通信
//里面的成员的含义自己说了算
struct nrf_msg_st//定义的发送数据包的结构体(不能超过32字节)
{
	u32 Dht_Hum;//存储DHT11的湿度
	u32 Dht_Temp;//存储DHT11的温度
	float Sht_Temp;//存储SHT30的温度
	float Sht_Hum;//存储SHT30的湿度
	u8 buf[16];//存储其他数据
};
//用GPIO来模拟SPI总线(移植性好)
//做代码移植时这部分需要修改(打开硬件原理图)
//这块也可以修改成位带操作
#define NRF_CSN_HIGH()      GPIO_SetBits(GPIOC, GPIO_Pin_4)//片选拉高(不能操作设备)
#define NRF_CSN_LOW()      	GPIO_ResetBits(GPIOC, GPIO_Pin_4)//片选拉低(可以操作设备)
#define NRF_CE_HIGH()	   		GPIO_SetBits(GPIOC, GPIO_Pin_5)//拉高CE(打开射频)
#define NRF_CE_LOW()	   		GPIO_ResetBits(GPIOC, GPIO_Pin_5)//拉低CE(关闭射频)
#define NRF_Read_IRQ()			GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4)//读中断的管脚(无论是发送数据还是接受数据都是低电平)

#define NRF_MOSI_HIGH()			GPIO_SetBits(GPIOA, GPIO_Pin_6)//MOSI拉高 
#define NRF_MOSI_LOW()			GPIO_ResetBits(GPIOA, GPIO_Pin_6)//MOSI拉低
#define NRF_CLK_HIGH()			GPIO_SetBits(GPIOA, GPIO_Pin_7)//CLK拉高
#define NRF_CLK_LOW()				GPIO_ResetBits(GPIOA, GPIO_Pin_7)//CLK拉低
#define NRF_Read_MISO()			GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5)//读MISO(主入从出)

#define TX_ADR_WIDTH 		5  	//发射地址宽度(5个字节)

#define RX_ADR_WIDTH    5		//每一个Nrf都会有一个地址(3 4 5bytes)

// SPI(nRF24L01) commands
#define NRF_READ_REG    0x00  // Define read command to register
#define NRF_WRITE_REG   0x20  // Define write command to register
#define RD_RX_PLOAD 		0x61  // Define RX payload register address
#define WR_TX_PLOAD 		0xA0  // Define TX payload register address
#define FLUSH_TX    		0xE1  // Define flush TX register command
#define FLUSH_RX    		0xE2  // Define flush RX register command
#define REUSE_TX_PL 		0xE3  // Define reuse TX payload register command
#define NOP         		0xFF  // Define No Operation, might be used to read status register

// SPI(nRF24L01) registers(addresses)
#define CONFIG      		0x00  // 'Config' register address
#define EN_AA       		0x01  // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR   		0x02  // 'Enabled RX addresses' register address
#define SETUP_AW    		0x03  // 'Setup address width' register address
#define SETUP_RETR  		0x04  // 'Setup Auto. Retrans' register address
#define RF_CH       		0x05  // 'RF channel' register address
#define RF_SETUP    		0x06  // 'RF setup' register address
#define STATUS      		0x07  // 'Status' register address
#define OBSERVE_TX  		0x08  // 'Observe TX' register address
#define CD          		0x09  // 'Carrier Detect' register address
#define RX_ADDR_P0  		0x0A  // 'RX address pipe0' register address
#define RX_ADDR_P1  		0x0B  // 'RX address pipe1' register address
#define RX_ADDR_P2  		0x0C  // 'RX address pipe2' register address
#define RX_ADDR_P3  		0x0D  // 'RX address pipe3' register address
#define RX_ADDR_P4  		0x0E  // 'RX address pipe4' register address
#define RX_ADDR_P5  		0x0F  // 'RX address pipe5' register address
#define TX_ADDR     		0x10  // 'TX address' register address
#define RX_PW_P0    		0x11  // 'RX payload width, pipe0' register address
#define RX_PW_P1    		0x12  // 'RX payload width, pipe1' register address
#define RX_PW_P2    		0x13  // 'RX payload width, pipe2' register address
#define RX_PW_P3    		0x14  // 'RX payload width, pipe3' register address
#define RX_PW_P4    		0x15  // 'RX payload width, pipe4' register address
#define RX_PW_P5    		0x16  // 'RX payload width, pipe5' register address
#define FIFO_STATUS 		0x17  // 'FIFO Status Register' register address

#define MAX_RT      		0x10 //达到最大重发次数中断标志位(超时)
#define TX_DS						0x20 //发送完成中断标志位	  // 发送OK

#define RX_DR						0x40 //接收到数据中断标志位 //接收OK

void SPI_NRF_Init(void);//对Nrf进行初始化
u8 SPI_NRF_RW(u8 dat);//对Nrf进行读写
//(从SPI总线考虑Nrf的读和写时序一样,只不过第一个字节中的读写命令不一样)
u8 SPI_NRF_ReadReg(u8 reg);//读Nrf寄存器
u8 SPI_NRF_WriteReg(u8 reg,u8 dat);//写Nrf寄存器
u8 SPI_NRF_ReadBuf(u8 reg,u8 *pBuf,u8 bytes);//读Buf(可以一次对于多个寄存器进行操作<必须是连续的寄存器>)
u8 SPI_NRF_WriteBuf(u8 reg ,u8 *pBuf,u8 bytes);	//写Buf()

void NRF_RX_Mode(u8 *rxAddr, u8 channel, u8 recvLen);//把Nrf设置为接收模式(自己的地址,自己的通道,自己接收数据长度)
void NRF_TX_Mode(u8 *txAddr, u8 channel);//把Nrf设置为发送模式(发送的地址,通道)<发送地址和自己的接收地址一致>
u8 NRF_Rx_Dat(u8 *rxbuf, u8 recvLen);//接收数据
u8 NRF_Tx_Dat(u8 *txbuf, u8 recvLen);//发送数据
u8 NRF_Check(void); //检查STM32和NRF硬件上有没有连接好
//比如说给Nrf某个寄存器写个10再读一下这个寄存器,读回来的值没问题就说明OK
//上述函数实现用的都是轮询的方式(S5P6818上用的是中断的方式)
#endif 






















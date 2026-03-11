#include "nrf24l01.h"//包含头文件

void SPI2_For_Nrf24l01(void)//初始化NRF所使用到的SPI接口的管脚
{
	GPIO_InitTypeDef GPIO_InitStructure;//定义了GPIO固件的结构体变量
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
	//通过APB2总线使能GPIOA组和GPIOC组的时钟
	
	//CSN PC4 OUT
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//选择了推挽的输出模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//选择了4号管脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//选择了50MHz输出速度
	GPIO_Init(GPIOC, &GPIO_InitStructure);//按照上述配置初始化GPIOC组的管脚
	GPIO_SetBits(GPIOC, GPIO_Pin_4);//默认是高电平(低电平有效)
	
	//CE PC5 OUT
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//选择了推挽的输出模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//选择了5号管脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//选择了50MHz输出速度
	GPIO_Init(GPIOC, &GPIO_InitStructure);//按照上述配置初始化GPIOC组的管脚
	GPIO_ResetBits(GPIOC, GPIO_Pin_5);//默认是低电平(既不发送也不接收)

	//MOSI PA6 OUT
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//选择了推挽的输出模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//选择了6号管脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//选择了50MHz输出速度
	GPIO_Init(GPIOA, &GPIO_InitStructure);//按照上述配置初始化GPIOA组的管脚
	
	//SPI2_CLK PA7 OUT
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//选择了7号管脚
	GPIO_Init(GPIOA, &GPIO_InitStructure);//按照上述配置初始化GPIOA组的管脚
	
	//MISO PA5 IN
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//选择了浮空输入模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//选择了5号管脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//选择了50MHz的输出速度
	GPIO_Init(GPIOA, &GPIO_InitStructure);//按照上述配置初始化GPIOA组的管脚
	
	//IRQ PA4 IN
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//选择了上拉输入模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//选择了5号管脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//选择了50MHz的输出速度
	GPIO_Init(GPIOA, &GPIO_InitStructure);//按照上述配置初始化GPIOA组的管脚
}
//可以修改成delay_ms delay_us
void Delay(__IO u32 nCount)//延时函数
{
  for(; nCount != 0; nCount--);
} 
//----------------------------------------------------------------//


//----------以下代码与芯片无关------------------------------------//
//我们使用的是SPI总线的
//SCK初始低电平上升沿有效
/*
 * 函数名：SPI_NRF_RW
 * 描述  ：对Nrf进行读写
 * 输入  ：dat:将要写入的数据
 * 输出  ：写每位数据时的状态
 * 调用  ：内部调用
 */
u8 SPI_NRF_RW(u8 dat)//可以一边发一边收
{  	
	int count = 0;

	/*每一次循环写一位数据,并且获取一位数据*/
	for(count = 0; count < 8; count++){
		NRF_CLK_LOW();
		if(dat & 0x80) {
			NRF_MOSI_HIGH();
		} else {
			NRF_MOSI_LOW();
		}
		dat <<= 1;
		NRF_CLK_HIGH();
		dat |= NRF_Read_MISO();	
	}
	NRF_CLK_LOW();	

	return dat;
}



/*
 * 函数名：SPI_NRF_Init
 * 描述  ：用于对Nrf进行初始化
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void SPI_NRF_Init(void)
{
	/*初始化Nrf24l01所使用的GPIO*/
	SPI2_For_Nrf24l01();
	/*默认不操作的状态下把片选拉高*/
	NRF_CSN_HIGH(); 
}

/*
 * 函数名：SPI_NRF_WriteReg
 * 描述  ：用于向NRF特定的寄存器写入数据
 * 输入  ：	reg:NRF的命令+寄存器地址。
						dat:将要向寄存器写入的数据
 * 输出  ：NRF的status寄存器的状态
 * 调用  ：内部调用
 */
u8 SPI_NRF_WriteReg(u8 reg,u8 dat)
{
	u8 status;
	/*关闭射频*/
	NRF_CE_LOW();
	/*置低CSN，使能SPI传输*/
	NRF_CSN_LOW();
				
	/*发送命令及寄存器号 */
	status = SPI_NRF_RW(reg);//在写的时候也读回来状态了
		 
	/*向寄存器写入数据*/
	SPI_NRF_RW(dat); 
	          
	/*CSN拉高，完成*/	   
	NRF_CSN_HIGH();	
		
	/*返回状态寄存器的值*/
	return(status);
}


/*
 * 函数名：SPI_NRF_ReadReg
 * 描述  ：用于从NRF特定的寄存器读出数据
 * 输入  ：reg:NRF的命令+寄存器地址。
 * 输出  ：寄存器中的数据
 * 调用  ：内部调用
 */
u8 SPI_NRF_ReadReg(u8 reg)
{
 	u8 reg_val;

	/*关闭射频*/
	NRF_CE_LOW();
	/*置低CSN，使能SPI传输*/
 	NRF_CSN_LOW();
				
	/*发送寄存器号*/
	SPI_NRF_RW(reg); 

	/*读取寄存器的值 */
	reg_val = SPI_NRF_RW(NOP);
	            
	/*CSN拉高，完成*/
	NRF_CSN_HIGH();		
   	
	return reg_val;
}	


/*
 * 函数名：SPI_NRF_ReadBuf
 * 描述  ：用于从NRF的寄存器中读出一串数据
 * 输入  ：	reg:NRF的命令+寄存器地址。
						pBuf:用于存储将被读出的寄存器数据的数组，外部定义
						bytes:pBuf的数据长度	
 * 输出  ：NRF的status寄存器的状态
 * 调用  ：外部调用
 */
u8 SPI_NRF_ReadBuf(u8 reg,u8 *pBuf,u8 bytes)
{
 	u8 status, byte_cnt;

	NRF_CE_LOW();
	
	/*置低CSN，使能SPI传输*/
	NRF_CSN_LOW();
		
	/*发送寄存器号*/		
	status = SPI_NRF_RW(reg); 

 	/*读取缓冲区数据*/
	for(byte_cnt=0;byte_cnt<bytes;byte_cnt++)		  
		pBuf[byte_cnt] = SPI_NRF_RW(NOP); //从NRF24L01读取数据  

	/*CSN拉高，完成*/
	NRF_CSN_HIGH();	
		
 	return status;		//返回寄存器状态值
}


/*
 * 函数名：SPI_NRF_WriteBuf
 * 描述  ：用于向NRF的寄存器中写入一串数据
 * 输入  ：reg:NRF的命令+寄存器地址。
 		   pBuf：存储了将要写入写寄存器数据的数组，外部定义
		   bytes: pBuf的数据长度	
 * 输出  ：NRF的status寄存器的状态
 * 调用  ：外部调用
 */
u8 SPI_NRF_WriteBuf(u8 reg ,u8 *pBuf,u8 bytes)
{
	u8 status,byte_cnt;
	
	NRF_CE_LOW();
	/*置低CSN，使能SPI传输*/
	NRF_CSN_LOW();			

	/*发送寄存器号*/	
	status = SPI_NRF_RW(reg); 
 	
	/*向缓冲区写入数据*/
	for(byte_cnt=0;byte_cnt<bytes;byte_cnt++)
		SPI_NRF_RW(*pBuf++);	//写数据到缓冲区 	 
	  	   
	/*CSN拉高，完成*/
	NRF_CSN_HIGH();			
  
	return (status);	//返回NRF24L01的状态 		
}

/*
 * 函数名：NRF_RX_Mode
 * 描述  ：配置并进入接收模式
 * 输入  ：无	
 * 输出  ：无
 * 调用  ：外部调用
 */
void NRF_RX_Mode(u8 *rxAddr, u8 channel, u8 recvLen)
{
	NRF_CE_LOW();	

	SPI_NRF_WriteBuf(NRF_WRITE_REG+RX_ADDR_P0, rxAddr, RX_ADR_WIDTH);//设置RX_ADDR_P0的地址
	SPI_NRF_WriteReg(NRF_WRITE_REG+EN_AA, 0x01);    //使能通道0的自动应答    
	SPI_NRF_WriteReg(NRF_WRITE_REG+EN_RXADDR, 0x01);//使能通道0的接收地址    
	SPI_NRF_WriteReg(NRF_WRITE_REG+SETUP_AW, 0x03);//地址为5个字节
	SPI_NRF_WriteReg(NRF_WRITE_REG+RF_CH, channel); //设置RF通信频率    
	SPI_NRF_WriteReg(NRF_WRITE_REG+RX_PW_P0, recvLen);//选择通道0的有效数据宽度      
	SPI_NRF_WriteReg(NRF_WRITE_REG+RF_SETUP, 0x0f); //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
	SPI_NRF_WriteReg(NRF_WRITE_REG+CONFIG, 0x0f);  //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式 
	
	/*CE拉高，进入接收模式,射频开始工作*/	
  NRF_CE_HIGH();
	Delay(0xffff); 
}    

/*
 * 函数名：NRF_TX_Mode
 * 描述  ：配置发送模式
 * 输入  ：无	
 * 输出  ：无
 * 调用  ：外部调用
 */
void NRF_TX_Mode(u8 *txAddr, u8 channel)
{  
	NRF_CE_LOW();		

	SPI_NRF_WriteBuf(NRF_WRITE_REG+TX_ADDR, txAddr, TX_ADR_WIDTH);//设置TX_ADDR地址 
	SPI_NRF_WriteBuf(NRF_WRITE_REG+RX_ADDR_P0, txAddr, RX_ADR_WIDTH);//设置RX节点地址,主要为了使能ACK   
	SPI_NRF_WriteReg(NRF_WRITE_REG+EN_AA, 0x01);     //使能通道0的自动应答    
	SPI_NRF_WriteReg(NRF_WRITE_REG+EN_RXADDR, 0x01); //使能通道0的接收地址 
	SPI_NRF_WriteReg(NRF_WRITE_REG+SETUP_AW, 0x03);//地址为5位的
	SPI_NRF_WriteReg(NRF_WRITE_REG+SETUP_RETR, 0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
	SPI_NRF_WriteReg(NRF_WRITE_REG+RF_CH, channel);       //设置RF通道为CHANAL
	SPI_NRF_WriteReg(NRF_WRITE_REG+RF_SETUP, 0x0f);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
	SPI_NRF_WriteReg(NRF_WRITE_REG+CONFIG, 0x0e);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,发射模式,开启所有中断
	
	/*CE拉高，进入发送模式*/	
  NRF_CE_HIGH();
	Delay(0xffff); //CE要拉高一段时间才进入发送模式
}


/*
 * 函数名：NRF_Check
 * 描述  ：主要用于NRF与MCU是否正常连接
 * 输入  ：无	
 * 输出  ：SUCCESS/ERROR 连接正常/连接失败
 * 调用  ：外部调用
 */
u8 NRF_Check(void)
{
	u8 buf[5]={0xC2,0xC2,0xC2,0xC2,0xC2};
	u8 buf1[5];
	u8 i; 
	 
	/*写入5个字节的地址.  */  
	SPI_NRF_WriteBuf(NRF_WRITE_REG+TX_ADDR,buf,5);

	/*读出写入的地址 */
	SPI_NRF_ReadBuf(TX_ADDR,buf1,5); 
	 
	/*比较*/               
	for(i=0;i<5;i++){
		if(buf1[i]!=0xC2)
			break;
	} 
	       
	if(i==5){
		return SUCCESS ;        //MCU与NRF成功连接 
	}else{
		return ERROR ;        //MCU与NRF不正常连接
	}
}

 /*
 * 函数名：NRF_Tx_Dat
 * 描述  ：用于向NRF的发送缓冲区中写入数据
 * 输入  ：txBuf：存储了将要发送的数据的数组，外部定义	
 * 输出  ：发送结果，成功返回TXDS,失败返回MAXRT或ERROR
 * 调用  ：外部调用
 */ 
u8 NRF_Tx_Dat(u8 *txbuf, u8 sendLen)
{
	u8 state;
  
	/*ce为低，进入待机模式1*/
	NRF_CE_LOW();

	/*写数据到TX BUF 最大 32个字节*/						
	SPI_NRF_WriteBuf(WR_TX_PLOAD,txbuf,sendLen);

	/*CE为高，txbuf非空，发送数据包 */   
	NRF_CE_HIGH();
	  	
	while(NRF_Read_IRQ()!=0); //可以修改成中断处理
		                         
	state = SPI_NRF_ReadReg(STATUS);
   
	/*清除TX_DS或MAX_RT中断标志*/                  
	SPI_NRF_WriteReg(NRF_WRITE_REG+STATUS,state); 	
	SPI_NRF_WriteReg(FLUSH_TX,NOP);    //清除TX FIFO寄存器 

	/*判断中断类型*/    
	if(state&MAX_RT){                     //达到最大重发次数
			 return MAX_RT; 
	}else if(state&TX_DS){                  //发送完成
		 	return TX_DS;
	}else{						  
			return ERROR;                 //其他原因发送失败
	}
} 

 /*
 * 函数名：NRF_Rx_Dat
 * 描述  ：用于从NRF的接收缓冲区中读出数据
 * 输入  ：rxBuf：用于接收该数据的数组，外部定义	
 * 输出  ：接收结果，
 * 调用  ：外部调用
 */ 
u8 NRF_Rx_Dat(u8 *rxbuf, u8 recvLen)
{
	u8 state; 
	NRF_CE_HIGH();	 //进入接收状态
	 /*等待接收中断*/
	while(NRF_Read_IRQ()!=0); 
	
	NRF_CE_LOW();  	 //进入待机状态
	/*读取status寄存器的值  */               
	state=SPI_NRF_ReadReg(STATUS);
	 
	/* 清除中断标志*/      
	SPI_NRF_WriteReg(NRF_WRITE_REG+STATUS,state);

	/*判断是否接收到数据*/
	if(state&RX_DR){                                 //接收到数据
	  SPI_NRF_ReadBuf(RD_RX_PLOAD,rxbuf,recvLen);//读取数据
		SPI_NRF_WriteReg(FLUSH_RX,NOP);          //清除RX FIFO寄存器
	  return RX_DR; 
	}else{    
		return ERROR;                    //没收到任何数据
	}
}


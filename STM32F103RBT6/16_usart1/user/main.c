#include "led.h"
#include "buzzer.h"
#include "delay.h"
#include "usart1.h"
#include "stdio.h"
#include "stdlib.h"

struct data  // 数据包
{
	int dht_hum;  // DHT11 湿度
	int dht_temp;  // DHT11 温度
	float sht_temp;  // SHT30 温度
	float sht_hum;  // SHT30 湿度
	u8 buf[16];  // 额外数据
};

/**********************************************************************/

void recv_handler1(u8 recv)
{
	switch(recv)
	{
		case '0': led_on(0); break;
		case '1': led_on(1); break;
		case '2': led_on(2); break;
		case '3': buzzer_on(); break;
		case '4': led_off(0); led_off(1); led_off(2); buzzer_off(); break;
	}	
}

/**********************************************************************/
/*
	接收到 ON  打开蜂鸣器
	接收到 OFF 打开蜂鸣器
*/
#define O_FLAG 0
#define N_FLAG 1
#define F_FLAG 2
int flag = O_FLAG;  // 全局变量, 默认 O_FLAG

void recv_handler2(u8 recv)
{
	switch(flag)
	{
		case O_FLAG: 
			if(recv == 'O')   // 判断接收到的数据是否是 'O'
				flag = N_FLAG;  // 切换 N_FLAG 的状态
			break;
			
		case N_FLAG: 
			if(recv == 'N')
			{
				buzzer_on();  // 打开蜂鸣器
				flag = O_FLAG;  // 切换到 O_FLAG 初始状态
			}
			else if(recv == 'F')
				flag = F_FLAG;  // 切换到 F_FLAG 的状态
			else
				flag = O_FLAG;  // 切换到 O_FLAG 初始状态
			break;
			
		case F_FLAG: 
			if(recv == 'F')
				buzzer_off();   // 关闭蜂鸣器
			flag = O_FLAG;  // 切换到 O_FLAG 初始状态
			break;
	}
}

/**********************************************************************/
// 定义解析状态
#define IPD_PLUS 0  // 接收 '+' 状态
#define IPD_I 1     // 接收 'I' 状态
#define IPD_P 2     // 接收 'P' 状态
#define IPD_D 3     // 接收 'D' 状态
#define IPD_COMMA 4 // 接收 ',' 状态
#define IPD_LEN 5   // 接收长度状态 (10)
#define IPD_DATA 6  // 接收数据状态 (ABCDEFGHIJ)

int ipd_state = IPD_PLUS;  // 状态变量, 初始化为接收 '+' 的状态
u8 idp_len = 0;  // 存储解析出来的数据长度
u8 data_count = 0;  // 已经接收到的数据字节数计数器
u8 len_str[10];  // 临时存储数字字符串的缓冲区
int len_idx = 0;  // 数字字符串的缓冲区的索引
u8 ipd_finished = 0;  // 完成标志, 当完整接收一包数据后置1, 通知主循环处理
u8 final_data[100];  // 存储最终接收到的数据内容
	
// 解析 "+IPD,10:ABCDEFGHIJ"
void recv_handler(u8 recv)
{	
	switch(ipd_state)  // 根据当前状态进行不同的处理
    {
        case IPD_PLUS: 
            if(recv == '+') {
                ipd_state = IPD_I;  // 进入下一个状态，准备接收 'I'	  
                len_idx = 0;  // 重置长度字符串索引   
                data_count = 0;  // 重置数据计数器
                idp_len = 0;  // 重置数据长度
            }
            break;
            
        case IPD_I: 
            if(recv == 'I') ipd_state = IPD_P;  // 如果收到 'I', 进入下一个状态, 准备接收 'P'
            else if(recv == '+') ipd_state = IPD_I;  // 如果又收到 '+'（可能是新的开始）, 回到接收 'I' 的状态
            else ipd_state = IPD_PLUS;  // 收到其他字符, 返回初始状态
            break;
            
        case IPD_P: 
            if(recv == 'P') ipd_state = IPD_D; 
            else if(recv == '+') ipd_state = IPD_I; 
            else ipd_state = IPD_PLUS; 
            break;
            
        case IPD_D:
            if(recv == 'D') ipd_state = IPD_COMMA;
            else if(recv == '+') ipd_state = IPD_I;
            else ipd_state = IPD_PLUS;
            break;
            
        case IPD_COMMA:
            if(recv == ',') ipd_state = IPD_LEN;
            else if(recv == '+') ipd_state = IPD_I;
            else ipd_state = IPD_PLUS;
            break;
            
        case IPD_LEN:
            if(recv >= '0' && recv <= '9')  // 如果收到的是数字字符
			{
                if(len_idx < 9)  // 防止缓冲区溢出（最多存9个数字）   
					len_str[len_idx++] = recv;  // 将数字字符存入长度字符串, 索引+1
            }
            else if(recv == ':')  // 如果收到 ':'，表示长度数字结束
			{  
                len_str[len_idx] = '\0';  // 在字符串末尾添加结束符
                idp_len = atoi((char *)len_str);  // 将数字字符串转换为整数, 得到数据长度
                data_count = 0;  // 重置数据计数器，准备接收数据
                ipd_state = (idp_len > 0) ? IPD_DATA : IPD_PLUS;  // 如果长度>0，进入数据接收状态，否则返回初始状态
            }
            break;

        case IPD_DATA:  // 接收实际数据
            final_data[data_count++] = recv; // 将接收到的字节存入最终数据数组，计数器+1
            if(data_count >= idp_len)   // 如果已经收到足够长度的数据
			{
                final_data[data_count] = '\0';  // 在数据末尾添加字符串结束符
                ipd_finished = 1;  // 置完成标志位, 通知主循环处理这包数据
                ipd_state = IPD_PLUS;  // 返回初始状态，准备接收下一包数据
            }
            break;
    }
}

int main(void)
{	
	//u8 *recv_data = NULL;  // 接收数据
	
	led_init();  // 初始化 led
	buzzer_init();  // 初始化蜂鸣器                                                      
	delay_init();  // 初始化延时函数
	usart1_init();  // 初始化 USART1
	
	//struct data d = {12, 34, 34.12, 12.34, "DHT Other Data"};  // 假数据
	
	usart1_send_data((u8 *)"hello xinming");

	set_usart1_handler(recv_handler);  // 设置 USART1 回调函数
	
	while(1){
			
		if(ipd_finished) {
            printf("Received IPD:\r\nlen = %d\r\ndata = %s\r\n", idp_len, final_data);
            ipd_finished = 0; // 清除标志
        }
		// 打印 DHT11 假数据
		//printf("Dht:%d/%d Sht:%.2f/%.2f Other:%s\n", d.dht_hum, d.dht_temp, d.sht_temp, d.sht_hum, d.buf);
				
		//recv_data = usart1_recv_data();
		
		//recv_data = usart1_recv_data();  // 主动接收多字节数据
        //printf("%s", recv_data);
			
		//delay_ms(1000);  // 延时 1 秒
	}
}


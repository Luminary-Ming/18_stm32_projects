#include "led.h"
#include "buzzer.h"
#include "delay.h"
#include "usart1.h"
#include "stdio.h"
#include "oled.h"

struct data  // 数据包
{
	int dht_hum;  // DHT11 湿度
	int dht_temp;  // DHT11 温度
	float sht_temp;  // SHT30 温度
	float sht_hum;  // SHT30 湿度
	u8 buf[16];  // 额外数据
};

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

// 解析 "+IPD,10:ABCDEFGHIJ"
void recv_handler(u8 recv)
{	
	static u8 buf[128];
    static u8 idx = 0;

    buf[idx++] = recv;
    // 假设以换行符判定一包数据结束
    if (recv == '\n' || idx >= 127) {
        buf[idx] = '\0';
        // 在这里调用你之前的解析函数
        // parse_ipd_data(buf); 
        printf("Recv: %s", buf); // 这样才能保证实时打印
        idx = 0;
    }
}

int main(void)
{	
	u8 *recv_data = NULL;  // 接收数据
	
	led_init();  // 初始化 led
	buzzer_init();  // 初始化蜂鸣器                                                      
	delay_init();  // 初始化延时函数
	usart1_init();  // 初始化 USART1
	OLED_Init();  // 初始化 OLED
	
	OLED_Clear();  // 清屏 
	
	//OLED_ShowChar(0, 0, '$', 16);  // 在(0,0) 的位置显示一个 16 号字体的符号 $
	//OLED_ShowNum(0, 2, 114511, 6, 16);  // 在(0,2) 的位置显示一个 16 号字体的数字 114511
	//OLED_ShowString(0, 4, (unsigned char *)"ta fei", 16);  // 在(0,4) 的位置显示一个 16 号字体的字符串 ta fei
	OLED_ShowCHinese(0, 4, 4);  // 在(0,2) 的位置显示汉字
	
	//picture_1();  // 显示图片
	
	//struct data d = {12, 34, 34.12, 12.34, "DHT Other Data"};  // 假数据
	//char buf[] = "Dht:xx/xx Sht:xx.xx/xx.xx";  // 存储转换的字符串

	set_usart1_handler(recv_handler);  // 设置 USART1 回调函数
	
	while(1){
			
		//printf("Dht:%d/%d Sht:%.2f/%.2f Other:%s\n", d.dht_hum, d.dht_temp, d.sht_temp, d.sht_hum, d.buf);
		
		//usart1_send_data((unsigned char *)buf);
		
		//recv_data = usart1_recv_data();
		
		//recv_data = usart1_recv_data();  // 主动接收多字节数据
        //printf("%s", recv_data);
			
		//delay_ms(1000);  // 延时 1 秒
	}
}


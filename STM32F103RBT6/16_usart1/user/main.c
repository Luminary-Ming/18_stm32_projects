#include "led.h"
#include "buzzer.h"
#include "delay.h"
#include "usart1.h"

int main(void)
{
	u8 c = 'A';
	u8 recv = 0;
	
	led_init();  // 初始化 led
	buzzer_init();  // 初始化蜂鸣器                                                      
	delay_init();  // 初始化延时函数
	usart1_init();  // 初始化 USART1

	while(1){
		
		delay_ms(1000);  // 延时1秒
		
		usart1_send_byte(c++);
		
		if(c > 'Z')
			c = 'A';
		
		recv = usart1_recv_byte();
		
		switch(recv)
		{
			case '0': led_on(0); break;
			case '1': led_on(1); break;
			case '2': led_on(2); break;
			case '3': buzzer_on(); break;
			case '4': led_off(0); led_off(1); led_off(2); buzzer_off(); break;
		}	
	}
}


#include "stm32f10x_conf.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "bitband.h"
#include "delay.h"
#include "morse.h"
#include "display.h"
#include "dht11.h"
#include "interrupt.h"
#include "iwdg.h"
#include "usart1.h"
#include "stdio.h"
#include "oled.h"
#include "sg90.h"


struct data{
	int dht_hum;
	int dht_temp;
	float sht_temp;
	float sht_hum;
	u8 buf[16];
};

#define O_FLAG 0
#define N_FLAG 1
#define F_FLAG 2
int flag = O_FLAG;

void recv_handler(u8 recv){
	switch(flag){
		case O_FLAG: 
			if(recv == 'O')
				flag = N_FLAG;
			break;
		case N_FLAG:
			if(recv == 'N'){
				beep_on();
				flag = O_FLAG;
			}else if(recv == 'F')
				flag = F_FLAG;
			else 
				flag = O_FLAG;
			break;
		case F_FLAG:
			if(recv == 'F')
				beep_off();
			flag = O_FLAG;
			break;
	}
}
int main(void)
{
	struct data d ={12, 34, 21.23, 23.21, "Hello World!"};
	char buf[64] = "Dht:xx/xx Sht:xx.xx/xx.xx";
	int angle = 0;
	//Servo_T servo1, servo2;
	int i;
	led_init();
	beep_init();
	key_init();
	delay_init();
	display_init();
	dht11_init();
	eint_init();
	usart1_init();
	sg90_init();
	//OLED_Init();
	//servo_init(&servo1, TIM3, TIM_Channel_3, 500, 2500);
	//servo_init(&servo2, TIM3, TIM_Channel_4, 500, 2500);
	
	
	//OLED_Clear();
	//OLED_ShowChar(0, 0, '%', 16);
	//OLED_ShowNum(0, 1, 9527, 4, 16);
	//OLED_ShowString(0,3, "MegaGatlingPea", 12);
	//OLED_ShowCHinese(0, 6, 10);
	//set_usart1_handler(recv_handler);
	//picture_jh();
	while(1)
	{
		// 测试1: 两个舵机同步转到0度
        sg90_set_angle1(0);
        sg90_set_angle2(0);
        delay_ms(2000);
        
        // 转到180度
        sg90_set_angle1(180);
        sg90_set_angle2(180);
        delay_ms(2000);
        
        // 转到90度
        sg90_set_angle1(90);
        sg90_set_angle2(90);
        delay_ms(2000);
        
        // 测试2: 两个舵机相反方向
        for(i = 0; i <= 180; i += 30)
        {
            sg90_set_angle1(i);
            sg90_set_angle2(180 - i);
            delay_ms(500);
        }
        
        for(i = 180; i >= 0; i -= 30)
        {
            sg90_set_angle1(i);
            sg90_set_angle2(180 - i);
            delay_ms(500);
		}
	}
}




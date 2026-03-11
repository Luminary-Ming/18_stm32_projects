#include "delay.h"
#include "usart1.h"
#include "stdio.h"
#include "oled.h"
#include "sht30.h"
#include "eeprom.h"
#include "adj_res.h"

int main(void)
{	   
	u16 adj_res_value = 0;
	float adj_res_vol = 0;
	u8 oled_buf1[64] = {0};  // 储存显示在 OLED 屏上的数据
	u8 oled_buf2[64] = {0};  // 储存显示在 OLED 屏上的数据
                                              
    delay_init();  // 初始化延时函数
    usart1_init(); // 初始化 USART1
	OLED_Init();   // 初始化 OLED
	sht_init();    // 初始化 SHT30
	eeprom_init(); // 初始化 EEPROM
	adj_res_init();  // 初始化高精度可调电阻
 
	OLED_Clear();  // 清屏
    OLED_ShowString(0, 0, (unsigned char *)"ADC Test", 16);
   
    while(1){	
		// 获取 ADC 转换结果
		adj_res_value = get_adj_res_value();  // 获取高精度可调电阻的数字量
		adj_res_vol = get_adj_res_voltage();  // 获取高精度可调电阻的电压值
  
		// 在 OLED 上显示
		sprintf((char *)oled_buf1, "value:%hd", adj_res_value);
		sprintf((char *)oled_buf2, "voltage:%f", adj_res_vol);
        
        OLED_ShowString(0, 2, (u8 *)oled_buf1, 16);
		OLED_ShowString(0, 4, (u8 *)oled_buf2, 16);

        delay_ms(500);
    }
}


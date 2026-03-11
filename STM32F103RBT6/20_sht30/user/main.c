#include "led.h"
#include "buzzer.h"
#include "delay.h"
#include "usart1.h"
#include "stdio.h"
#include "oled.h"
#include "sht30.h"

int main(void)
{	
    double sht30_data[2] = {0};  // 存储 SHT30 的温度湿度
	u8* temp_buf[64];  // 存储显示在 OLED 上的 SHT30 的温度数据
	u8* hum_buf[64];   // 存储显示在 OLED 上的 SHT30 的湿度数据
                                                         
    delay_init();  // 初始化延时函数
    usart1_init();  // 初始化 USART1
	OLED_Init();  // 初始化 OLED
	sht_init();  // 初始化 SHT30
	
	OLED_Clear();  // 清屏
	
    OLED_ShowString(0, 0, (unsigned char *)"SHT30 Test", 16);
   
    sht_write_mode();  // 设置 SHT30 的采用频率
    
    while(1){	
        sht_write_read_cmd();  // 发送获取 SHT30 数据的指令
        sht_read_data(sht30_data);  // 获取 SHT30 采集的数据
        
		// 在串口工具上打印
        printf("Temp:%.2f C, Hum:%.2f %%\r\n\r\n", sht30_data[0], sht30_data[1]);
        
		// 在OLED上显示
		sprintf((char *)temp_buf, "Temp:%.2f C", sht30_data[0] );
		sprintf((char *)hum_buf, " Hum:%.2f %%", sht30_data[1]);
        OLED_ShowString(0, 2, (unsigned char *)temp_buf, 16);
        OLED_ShowString(0, 4, (unsigned char *)hum_buf, 16);

        delay_ms(1000);  // 每秒读取一次
    }
}


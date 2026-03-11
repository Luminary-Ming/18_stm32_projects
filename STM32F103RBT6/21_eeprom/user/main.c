#include "delay.h"
#include "usart1.h"
#include "stdio.h"
#include "oled.h"
#include "sht30.h"
#include "eeprom.h"

int main(void)
{	
    u8 daddr = 0xA0;  // A2/A1/A0 都接地
	u8 waddr1 = 0x10;  // 假设要写入的第一个地址是 0x10
	u8 waddr2 = 0x20;  // 假设要写入的第二个地址是 0x20
	u8 data[6] = {1, 1, 4, 5, 1, 1};  // 假数据
	u8 buf[6] = {0};  // 储存顺序读的数据
	u8 oled_buf1[64] = {0};  // 储存显示在 OLED 屏上的 0x10 地址数据
	u8 oled_buf2[64] = {0};  // 储存显示在 OLED 屏上的 0x20 地址数据
    u8 val = 0;  // 接收读取的数据
                                                     
    delay_init();  // 初始化延时函数
    usart1_init(); // 初始化 USART1
	OLED_Init();   // 初始化 OLED
	sht_init();    // 初始化 SHT30
	eeprom_init(); // 初始化 EEPROM

	
	//eeprom_byte_write(daddr, waddr1, 0x55);  // 写一个字节到地址 0x10
    //delay_ms(5);  // 等待写入完成 (数据手册 P5 Table5 中 tWR<写入周期时间> 时间是 5ms)
	
	//eeprom_page_write(daddr, waddr2, data, sizeof(data) / sizeof(data[0]));  // 页写 6 个字节到地址 0x20
	//delay_ms(5);  // 等待页写入完成
    
	OLED_Clear();  // 清屏
    OLED_ShowString(0, 0, (unsigned char *)"EEPROM Test", 16);
   
    while(1){	
        // 重新读取 EEPROM 数据
        val = eeprom_rand_read(daddr, waddr1);    // 从地址 0x10 读取
        eeprom_sequ_read(daddr, waddr2, buf, 6);  // 从地址 0x20 读取
        
		// 在串口工具上打印
        printf("0x10 data: %d\r\n", val);
        printf("0x20 data: ");
        for(int i = 0; i < 6; i++) {
            printf("%d ", buf[i]);
        }
        printf("\r\n");
        
		// 在 OLED 上显示
		sprintf((char *)oled_buf1, "0x10:%d", val);
		sprintf((char *)oled_buf2, "0x20:%d %d %d %d %d %d", 
                buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);
        
        OLED_ShowString(0, 0, (u8 *)"EEPROM Test", 16);
        OLED_ShowString(0, 2, (u8 *)oled_buf1, 16);
        OLED_ShowString(0, 4, (u8 *)oled_buf2, 16);

        delay_ms(500);
    }
}


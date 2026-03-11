#include "led.h"
#include "delay.h"
#include "usart1.h"
#include "stdio.h"
#include "oled.h"
#include "mpu6050.h"

int main(void)
{	
    MPU6050_Data mpu_data;
    u8 mpu_init_status;
    char display_buf[20]; // 用于暂存格式化后的字符串
    
    delay_init();
    usart1_init(); 
    OLED_Init(); 
    OLED_Clear(); 
	
	//picture_1();  // 开机页面
	//delay_ms(1000);
      
    mpu_init_status = MPU6050_Init();
    
    if(mpu_init_status == 0) {
        OLED_Clear();
        OLED_ShowString(0, 0, (u8*)"Init OK!", 16);
        delay_ms(1000);
    } else {
        OLED_Clear();
        sprintf(display_buf, "Err Status:%d", mpu_init_status);
        OLED_ShowString(0, 0, (u8*)display_buf, 12);
        while(1); // 初始化失败停止
    }
    
    OLED_Clear();
    
    while(1)
    {
        // 1. 获取数据
        MPU6050_Get_Data(&mpu_data);
        
        // 2. 显示加速度
        // 使用 sprintf 处理负数和格式化
        sprintf(display_buf, "AX:%0.2f AY:%0.2f", 
                mpu_data.accel_x/16384.0f, mpu_data.accel_y/16384.0f);
        OLED_ShowString(0, 0, (u8*)display_buf, 12);
        
        sprintf(display_buf, "AZ:%0.2f", mpu_data.accel_z/16384.0f);
        OLED_ShowString(0, 2, (u8*)display_buf, 12);
        
        // 3. 显示陀螺仪
        sprintf(display_buf, "GX:%d GY:%d", mpu_data.gyro_x, mpu_data.gyro_y);
        OLED_ShowString(0, 4, (u8*)display_buf, 12);
        
        sprintf(display_buf, "GZ:%d", mpu_data.gyro_z);
        OLED_ShowString(0, 5, (u8*)display_buf, 12);

        // 4. 显示温度
        sprintf(display_buf, "Temp:%.2f C", mpu_data.temp_value);
        OLED_ShowString(0, 7, (u8*)display_buf, 12);
        
        delay_ms(200); // 刷新频率设快一点，屏幕看起来更实时
    }
}
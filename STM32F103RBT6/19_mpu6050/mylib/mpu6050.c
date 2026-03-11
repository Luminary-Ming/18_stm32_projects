#include "mpu6050.h"
#include "mpu6050_gpio_iic.h"
#include "delay.h"
#include "stdio.h"

// 写一个字节到MPU6050寄存器
u8 MPU6050_Write_Byte(u8 reg, u8 data)
{
    MPU6050_IIC_Start();
    MPU6050_IIC_Send_Byte((MPU6050_ADDR << 1) | 0);  // 发送器件地址+写命令
    if(MPU6050_IIC_Wait_Ack()) {
        MPU6050_IIC_Stop();
        return 1;
    }
    
    MPU6050_IIC_Send_Byte(reg);  // 发送寄存器地址
    MPU6050_IIC_Wait_Ack();
    
    MPU6050_IIC_Send_Byte(data);  // 发送数据
    MPU6050_IIC_Wait_Ack();
    
    MPU6050_IIC_Stop();
    return 0;
}

// 从MPU6050读一个字节
u8 MPU6050_Read_Byte(u8 reg)
{
    u8 data;
    
    MPU6050_IIC_Start();
    MPU6050_IIC_Send_Byte((MPU6050_ADDR << 1) | 0);  // 发送器件地址+写命令
    MPU6050_IIC_Wait_Ack();
    
    MPU6050_IIC_Send_Byte(reg);  // 发送寄存器地址
    MPU6050_IIC_Wait_Ack();
    
    MPU6050_IIC_Start();
    MPU6050_IIC_Send_Byte((MPU6050_ADDR << 1) | 1);  // 发送器件地址+读命令
    MPU6050_IIC_Wait_Ack();
    
    data = MPU6050_IIC_Recv_Byte(0);  // 读取数据，最后发送NAK
    MPU6050_IIC_Stop();
    
    return data;
}

// 连续读取多个字节
u8 MPU6050_Read_Len(u8 reg, u8 len, u8 *buf)
{
    u8 i;
    
    MPU6050_IIC_Start();
    MPU6050_IIC_Send_Byte((MPU6050_ADDR << 1) | 0);  // 发送器件地址+写命令
    if(MPU6050_IIC_Wait_Ack()) {
        MPU6050_IIC_Stop();
        return 1;
    }
    
    MPU6050_IIC_Send_Byte(reg);  // 发送寄存器地址
    MPU6050_IIC_Wait_Ack();
    
    MPU6050_IIC_Start();
    MPU6050_IIC_Send_Byte((MPU6050_ADDR << 1) | 1);  // 发送器件地址+读命令
    MPU6050_IIC_Wait_Ack();
    
    for(i = 0; i < len-1; i++) {
        buf[i] = MPU6050_IIC_Recv_Byte(1);  // 读取数据，发送ACK
    }
    buf[len-1] = MPU6050_IIC_Recv_Byte(0);  // 读取最后一个字节，发送NAK
    
    MPU6050_IIC_Stop();
    return 0;
}

// 初始化MPU6050
u8 MPU6050_Init(void)
{
    u8 id;
    
    MPU6050_IIC_Init();  // 初始化IIC
    
    // 检查器件ID
    id = MPU6050_Read_Byte(WHO_AM_I);
	printf("Debug: Read WHO_AM_I = 0x%02X\r\n", id);
	
    if(id != 0x68 && id != 0x70) {
        return 1;  // 器件不存在
    }
    
    // 复位MPU6050
    MPU6050_Write_Byte(PWR_MGMT_1, 0x80);
    delay_ms(100);
    
    // 唤醒MPU6050，选择X轴陀螺作为时钟源
    MPU6050_Write_Byte(PWR_MGMT_1, 0x01);
    delay_ms(10);
    
    // 配置陀螺仪满量程范围为±2000°/s
    MPU6050_Write_Byte(GYRO_CONFIG, 0x18);  // 0x18 = 3<<3
    
    // 配置加速度计满量程范围为±2g
    MPU6050_Write_Byte(ACCEL_CONFIG, 0x00);
    
    // 配置采样率，设置数字低通滤波器
    MPU6050_Write_Byte(CONFIG, 0x03);       // DLPF_CFG=3，带宽约44Hz
    MPU6050_Write_Byte(SMPLRT_DIV, 0x13);   // 采样率 = 1kHz/(1+19) = 50Hz
    
    // 关闭中断、FIFO等
    MPU6050_Write_Byte(0x6A, 0x00);          // USER_CTRL
    MPU6050_Write_Byte(0x23, 0x00);          // FIFO_EN
    
    return 0;
}

// 获取MPU6050数据
void MPU6050_Get_Data(MPU6050_Data *data)
{
    u8 buf[14];
    
    // 连续读取加速度、温度、陀螺仪数据 (从0x3B开始，共14个字节)
    MPU6050_Read_Len(ACCEL_XOUT_H, 14, buf);
    
    // 解析加速度数据
    data->accel_x = (buf[0] << 8) | buf[1];
    data->accel_y = (buf[2] << 8) | buf[3];
    data->accel_z = (buf[4] << 8) | buf[5];
    
    // 解析温度数据
    data->temperature = (buf[6] << 8) | buf[7];
    //data->temp_value = (data->temperature / 340.0f) + 36.53f;  // 转换为摄氏度 (6050)
    data->temp_value = (data->temperature / 333.87f) + 21.0f;  // 转换为摄氏度 (6500)
	
    // 解析陀螺仪数据
    data->gyro_x = (buf[8] << 8) | buf[9];
    data->gyro_y = (buf[10] << 8) | buf[11];
    data->gyro_z = (buf[12] << 8) | buf[13];
}
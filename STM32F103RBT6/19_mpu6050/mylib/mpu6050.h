#ifndef MPU6050_H
#define MPU6050_H

#include "stm32f10x_conf.h"

// MPU6050 寄存器地址
#define SMPLRT_DIV     0x19  // 陀螺仪采样率分频
#define CONFIG         0x1A  // 配置寄存器 (用于设置外部同步和DLPF(数字低通滤波器))
#define GYRO_CONFIG    0x1B  // 陀螺仪配置 (用于设置陀螺仪量程范围(±250/500/1000/2000°/s))
#define ACCEL_CONFIG   0x1C  // 加速度计配置 (用于设置加速度计量程范围(±2/4/8/16g) )
#define ACCEL_XOUT_H   0x3B  // 加速度计X轴数据高8位
#define ACCEL_XOUT_L   0x3C  // 加速度计X轴数据低8位
#define ACCEL_YOUT_H   0x3D  // 加速度计Y轴数据高8位
#define ACCEL_YOUT_L   0x3E  // 加速度计Y轴数据低8位
#define ACCEL_ZOUT_H   0x3F  // 加速度计Z轴数据高8位
#define ACCEL_ZOUT_L   0x40  // 加速度计Z轴数据低8位
#define TEMP_OUT_H     0x41  // 温度传感器数据高8位
#define TEMP_OUT_L     0x42  // 温度传感器数据低8位
#define GYRO_XOUT_H    0x43  // 陀螺仪X轴数据高8位
#define GYRO_XOUT_L    0x44  // 陀螺仪X轴数据低8位
#define GYRO_YOUT_H    0x45  // 陀螺仪Y轴数据高8位
#define GYRO_YOUT_L    0x46  // 陀螺仪Y轴数据低8位
#define GYRO_ZOUT_H    0x47  // 陀螺仪Z轴数据高8位
#define GYRO_ZOUT_L    0x48  // 陀螺仪Z轴数据低8位
#define PWR_MGMT_1     0x6B  // 电源管理寄存器1 (用于控制芯片复位、睡眠模式和时钟源)
#define PWR_MGMT_2     0x6C  // 电源管理寄存器2 (用于控制各轴待机模式)
#define WHO_AM_I       0x75  // 器件ID (只读，固定返回0x68)

// MPU6050 器件地址
#define MPU6050_ADDR   0x68  // I2C设备地址(7位)，AD0 引脚接地时的地址

// 数据结构体
typedef struct {
    short accel_x;      // 加速度X轴原始数据
    short accel_y;      // 加速度Y轴原始数据
    short accel_z;      // 加速度Z轴原始数据
    short gyro_x;       // 陀螺仪X轴原始数据
    short gyro_y;       // 陀螺仪Y轴原始数据
    short gyro_z;       // 陀螺仪Z轴原始数据
    short temperature;  // 温度原始数据
    float temp_value;   // 实际温度值(°C)
} MPU6050_Data;

// 函数声明 
u8 MPU6050_Init(void);
u8 MPU6050_Read_Byte(u8 reg);
u8 MPU6050_Write_Byte(u8 reg, u8 data);
u8 MPU6050_Read_Len(u8 reg, u8 len, u8 *buf);
void MPU6050_Get_Data(MPU6050_Data *data);

#endif


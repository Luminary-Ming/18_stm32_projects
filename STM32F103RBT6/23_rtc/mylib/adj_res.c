#include "adj_res.h"

// 初始化高精度可调电阻使用的 GPIO 管脚以及 ADC 通道
void adj_res_init(void)
{
	GPIO_InitTypeDef Gpio_Vaule;  // 定义 GPIO_InitTypeDef 结构体类型的变量
	ADC_InitTypeDef ADC_Vaule;  // 定义 ADC_InitTypeDef 结构体类型的变量
	
	// 通过 APB2 总线使能 GPIOC 组以及 ADC1 的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC, ENABLE);
	
	Gpio_Vaule.GPIO_Mode = GPIO_Mode_AIN;  // 模拟输入
	Gpio_Vaule.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOC, &Gpio_Vaule);
	
	ADC_Vaule.ADC_ContinuousConvMode = DISABLE;  // 禁用连续转换模式
	ADC_Vaule.ADC_DataAlign = ADC_DataAlign_Right;  // 数据右对齐
	ADC_Vaule.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  // 关闭外部触发转换
	ADC_Vaule.ADC_Mode = ADC_Mode_Independent;  // 独立转换模式
	ADC_Vaule.ADC_NbrOfChannel = 10;  // 选择 10 号通道
	ADC_Vaule.ADC_ScanConvMode = DISABLE;  // 禁用扫描转换模式
	ADC_Init(ADC1, &ADC_Vaule);  // 初始化 ADC1
	
	// 配置 ADC1 的 10 号通道, 等级为 1, 采样时间为 239.5
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_239Cycles5);
	
	ADC_Cmd(ADC1, ENABLE);  // 使能 ADC1 的转换功能
	ADC_ResetCalibration(ADC1);  // 复位 ADC1 的校准寄存器
	while(ADC_GetResetCalibrationStatus(ADC1));  // 死等 ADC1 的校准复位操作成功
	ADC_StartCalibration(ADC1);  // 启动 ADC1 的自校准功能
	while(ADC_GetCalibrationStatus(ADC1));  // 死等 ADC1 的自校准功能操作成功
}

// 获取高精度可调电阻转换后的数值
u16 get_adj_res_value(void)
{
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);  // 以软件的方式开启 ADC1 的转换
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);  // 以死等的方式等待转换结束的标志
	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);  // 清除 ADC1 转换结束的标志
	
	return ADC_GetConversionValue(ADC1);  // 把 ADC1 转换的结果返回
}	

// 将采样值转换为实际的电压值
float get_adj_res_voltage(void)
{
	u16 adc_val = get_adj_res_value();  // 获取 ADC1 转换的结果
	
	return (float)adc_val * 3.3 / 4095;
}
 

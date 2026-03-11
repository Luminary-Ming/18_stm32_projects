#include "adj_res.h"//包含头文件

void adc_init(void)//初始化高精度可调电阻所使用的的GPIO管脚以及AD转换功能
{
	GPIO_InitTypeDef Gpio_Value;//定义了GPIO固件的结构体变量
	ADC_InitTypeDef Adc_Value;//定义了ADC固件的结构体变量
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_ADC1, ENABLE);
	//通过APB2总线使能GPIOC组和ADC1的时钟
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_AIN;//选择了模拟输入模式
	Gpio_Value.GPIO_Pin = GPIO_Pin_0;//选择了0号管脚
	Gpio_Value.GPIO_Speed = GPIO_Speed_50MHz;//选择了50MHz的输出速度
	GPIO_Init(GPIOC, &Gpio_Value);//按照上述配置初始化GPIOC	组的管脚
	
	Adc_Value.ADC_ContinuousConvMode = DISABLE;//选择关闭连续转换模式
	Adc_Value.ADC_DataAlign = ADC_DataAlign_Right;//选择数据右对齐
	Adc_Value.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//选择关闭外部触发
	Adc_Value.ADC_Mode = ADC_Mode_Independent;//选择单独转换模式
	Adc_Value.ADC_NbrOfChannel = 10;//选择通道10
	Adc_Value.ADC_ScanConvMode = DISABLE;//关闭扫描模式
	ADC_Init(ADC1, &Adc_Value);//按照上述配置初始化ADC1
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_239Cycles5);
	//配置ADC1的通道10
	
	ADC_Cmd(ADC1, ENABLE);//使能ADC1
}

void adc_start(void)//软件开启AD转换
{
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//软件开启AD转换
}

void adc_wait(void)//等待AD转换结束
{
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);//死等转换结束(可以改为超时处理)
}

int get_adc_value(void)//获取转换结果
{
	return ADC_GetConversionValue(ADC1);//获取ADC1转换的数值
}

void adj_res_init(void)//初始化高精度可调电阻的函数(初始化所连接的GPIO管脚以及AD转换)
{
	adc_init();//调用初始化函数
}

int get_adj_res_value(void)//获取AD转换之后的结果
{
	adc_start();//软件开启AD转换
	adc_wait();//等待AD转换结束
	return get_adc_value();//返回获取转换结果
}









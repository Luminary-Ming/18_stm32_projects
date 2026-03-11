#include "button.h"//包含头文件
#include "bitband.h"//包含有关位带的头文件

void button_init(void)//初始化的函数(初始化所连接的GPIO管脚)
{
	GPIO_InitTypeDef Button_Value;//定义了GPIO固件的结构体变量
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//通过APB2使能GPIOC组的时钟
	
	Button_Value.GPIO_Mode = GPIO_Mode_IPU;//选择了上拉输入的模式
	Button_Value.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8;//选择了 8 9 号管脚
	Button_Value.GPIO_Speed = GPIO_Speed_50MHz;//选择了50MHz的输出速度
	GPIO_Init(GPIOC, &Button_Value);//按照上述的配置初始化GPIOC组的管脚
}

int button_status(int nu)//根据参数nu检测相应的功能按键是否被按下
{
	int ret = 0;//定义接收获取值的变量
	
	switch(nu)
	{
		case 0 : ret = PCIn(9); break;//读取GPIOC组的9号管脚的电平并保存到ret变量中
		case 1 : ret = PCIn(8); break;//读取GPIOC组的8号管脚的电平并保存到ret变量中
	}
	return !ret;//把获取到的值做逻辑非的操作并返回
}







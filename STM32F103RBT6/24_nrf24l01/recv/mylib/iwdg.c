#include "iwdg.h"//包含头文件

void iwdg_init(int ms)//初始化的函数(包括设置预分频器和计数值)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//开启写访问(关闭写保护)
	IWDG_SetPrescaler(IWDG_Prescaler_64);//64分频 40KHz / 64 = 625Hz
	IWDG_SetReload(ms);//设置重载值
	IWDG_ReloadCounter();//喂狗
	IWDG_Enable();//开启看门狗
}

void iwdg_feed_dog(void)//喂狗的功能函数
{
	IWDG_ReloadCounter();//喂狗
}







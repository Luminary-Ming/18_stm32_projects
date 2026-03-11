#include "iwdg.h"

// 初始化 IWDG
void iwdg_init(int ms)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);  // 打开 PR 寄存器和 RLR 寄存器的写访问
	IWDG_SetPrescaler(IWDG_Prescaler_64);  // 选择 64 分频 LSI(40KHz) / 64 = 625Hz
	IWDG_SetReload(ms);  // 设置重装载值 (计数值)
	IWDG_ReloadCounter();  // 喂狗的操作
	IWDG_Enable();  // 使能看门狗
}

// 喂狗
void iwdg_feed_dog(void)
{
	IWDG_ReloadCounter();  // 喂狗的操作
}	


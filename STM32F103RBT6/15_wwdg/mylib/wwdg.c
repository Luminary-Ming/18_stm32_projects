#include "wwdg.h"


// 初始化 WWDG
void wwdg_init(void)
{
	// 通过 APB1 总线使能 WWDG 的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
	
	WWDG_SetPrescaler(WWDG_Prescaler_8);  // 8 分频
	WWDG_SetWindowValue(0x5F);  // 窗口值
	WWDG_SetCounter(0x7F);  // 计数器初始值
	WWDG_Enable(0x7F);  // 从 0x7F 开始往下数
}	

 // 喂狗
void wwdg_feed_dog(void)
{
	WWDG_SetCounter(0x7F);  // 重新设置计数
}
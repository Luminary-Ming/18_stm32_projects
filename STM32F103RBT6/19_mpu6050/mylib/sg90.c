#include "sg90.h"
void sg90_init(void)
{
    // 1. 使能时钟
    RCC->APB2ENR |= 1<<3;      // GPIOB时钟
    RCC->APB1ENR |= 1<<1;      // TIM3时钟
    
    // 2. 配置PB0和PB1为复用推挽输出 (GPIOB_CRL)
    GPIOB->CRL &= ~(0xF<<0);   // 清除PB0配置
    GPIOB->CRL &= ~(0xF<<4);   // 清除PB1配置
    GPIOB->CRL |= (0xB<<0);    // PB0: 1011 = 复用推挽输出 50MHz
    GPIOB->CRL |= (0xB<<4);    // PB1: 1011 = 复用推挽输出 50MHz
    
    // 3. 配置TIM3时基
    TIM3->PSC = 72 - 1;         // 预分频: 72MHz/72 = 1MHz (1us/计数)
    TIM3->ARR = 20000 - 1;      // 自动重装: 20000 = 20ms
    TIM3->CR1 |= 0x80;          // ARPE使能
    TIM3->CR1 |= 0x01;          // 使能计数器
    
    // 4. 配置通道3 (PB0)
    TIM3->CCMR2 |= (6<<4);      // OC3M = 110 (PWM模式1)
    TIM3->CCMR2 |= (1<<3);      // OC3PE = 1 (预装载使能)
    TIM3->CCER |= (1<<8);       // CC3E = 1 (输出使能)
    TIM3->CCR3 = 1500;          // 初始1.5ms (90度)
    
    // 5. 配置通道4 (PB1)
    TIM3->CCMR2 |= (6<<12);     // OC4M = 110 (PWM模式1)
    TIM3->CCMR2 |= (1<<11);     // OC4PE = 1 (预装载使能)
    TIM3->CCER |= (1<<12);      // CC4E = 1 (输出使能)
    TIM3->CCR4 = 1500;          // 初始1.5ms (90度)
    
    // 6. 生成更新事件，加载所有寄存器
    TIM3->EGR |= 0x01;
}

// PB0舵机角度设置
void sg90_set_angle1(u16 angle)
{
    uint16_t compare;
    
    if(angle > 180) angle = 180;
    
    // 500(0.5ms=0°) 到 2500(2.5ms=180°)
    compare = 500 + (angle * 2000 / 180);
    
    TIM3->CCR3 = compare;  // 直接写寄存器
}

// PB1舵机角度设置
void sg90_set_angle2(u16 angle)
{
    uint16_t compare;
    
    if(angle > 180) angle = 180;
    
    compare = 500 + (angle * 2000 / 180);
    
    TIM3->CCR4 = compare;  // 直接写寄存器
}


/*
void tim3_pwm_init(void){
	GPIO_InitTypeDef GPIO_Value;
	TIM_TimeBaseInitTypeDef TIM_TB;
	TIM_OCInitTypeDef TIM_OC;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB1Periph_TIM3, ENABLE);
	GPIO_Value.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Value.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Value.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_Value);
	
	TIM_TB.TIM_Period = 19999;
	TIM_TB.TIM_Prescaler = 71;
	TIM_TB.TIM_ClockDivision = 0;
	TIM_TB.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TB);
	
	TIM_OC.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OC.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC.TIM_Pulse = 1500;
	TIM_OC.TIM_OCPolarity = TIM_OCPolarity_High;
	
	TIM_OC3Init(TIM3, &TIM_OC);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_OC4Init(TIM3, &TIM_OC);
    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
	
}
	
void servo_init(Servo_T *servo, TIM_TypeDef *TIMx, uint16_t Channel, uint16_t min_us, uint16_t max_us){
	servo->TIMx = TIMx;
	servo->Channel = Channel;
	servo->min_pulse = min_us;
	servo->max_pulse = max_us;
	servo_set_angle(servo, 90);
}
void servo_set_angle(Servo_T *servo, float angle){
	u16 pulse_us;
	u16 compare;
	
	if(angle < 0) angle = 0;
	if(angle > 180) angle = 180;

	pulse_us = servo->min_pulse + (u16)((servo->max_pulse - servo->min_pulse) * angle / 180.0f);
	compare = pulse_us;
	if(servo->TIMx == TIM3){
		switch(servo->Channel){
			case TIM_Channel_3: TIM_SetCompare3(TIM3, compare); break;
			case TIM_Channel_4: TIM_SetCompare4(TIM3, compare); break;
		}
	}
}
*/


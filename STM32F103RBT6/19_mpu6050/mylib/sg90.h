#ifndef __SG90_H
#define __SG90_H

#include "stm32f10x_conf.h"
typedef struct {
    TIM_TypeDef *TIMx;
    uint16_t Channel;
    uint16_t min_pulse;  // 最小脉宽(us)
    uint16_t max_pulse;  // 最大脉宽(us)
} Servo_T;

extern void sg90_init(void);
extern void sg90_set_angle1(u16 angle);
extern void sg90_set_angle2(u16 angle);
#endif



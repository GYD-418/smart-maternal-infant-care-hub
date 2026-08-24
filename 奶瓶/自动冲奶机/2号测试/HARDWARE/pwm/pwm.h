#ifndef _PWM_H
#define _PWM_H

void PWM_Init(u16 arr,u16 psc);
void PWM_SetCompare3(uint16_t Compare);
void TIM3_PWM_Init(u16 arr,u16 psc);
void Motor1_ZHENG(void);           //电机1正转
void Motor1_FAN(void);          //电机1反转
void Motor1_enable(void);			// 电机1使能
void Motor1_disable(void);			// 电机1失能
void maichong1(int mb);

#endif

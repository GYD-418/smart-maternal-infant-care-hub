#include "stm32f10x.h"                  // Device header
#include "pwm.h"

void Motor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	PWM_Init();
}

void Motor_SetSpeed(int8_t Speed)
{
	if(Speed>=0)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_5);
		GPIO_ResetBits(GPIOB,GPIO_Pin_6);
		PWM_SetCompare3(Speed);
	}
	else
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_5);
		GPIO_SetBits(GPIOB,GPIO_Pin_6);
		PWM_SetCompare3(-Speed);
	}
}

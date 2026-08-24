#include "stm32f10x.h"                  // Device header

void PWM_Init(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=100-1;
	TIM_TimeBaseInitStruct.TIM_Prescaler=36-1;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
	
	
	
	TIM_OCStructInit(&TIM_InitStructure);
	TIM_InitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_InitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_InitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_InitStructure.TIM_Pulse=0;
	TIM_OC3Init(TIM2,&TIM_InitStructure);
	
	TIM_Cmd(TIM2,ENABLE);
}

void PWM_SetCompare3(uint16_t Compare)
{
	TIM_SetCompare3(TIM2,Compare);
}

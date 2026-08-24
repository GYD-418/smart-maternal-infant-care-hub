#include "stm32f10x.h"                  // Device header

extern int PWM_num1;
extern int mai1;
extern int mubiao1;

void PWM_Init(u16 arr,u16 psc)
{ 
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
//	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=arr;
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;
//	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void TIM3_PWM_Init(u16 arr,u16 psc)///pwm
{
	GPIO_InitTypeDef GPIO_InitStrue;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
 
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStrue.GPIO_Pin=GPIO_Pin_0;//PB0->CH3  PB1->CH4 TIM3 
	GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStrue);
	
	TIM_TimeBaseStructure.TIM_Period = arr; 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;		//设置输出比较模式为PWM1模式。在这种模式下，定时器的输出比较寄存器将产生一个脉宽调制信号。
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High ; 	//设置脉冲宽度为0。这意味着在PWM模式下，输出比较寄存器的值将被设置为0，从而产生一个低电平信号
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;   //设置输出比较极性为高。这意味着当输出比较寄存器的值大于等于比较值时，输出引脚将产生高电平信号
	TIM_OC3Init(TIM3,&TIM_OCInitStructure);
	
	TIM_ClearFlag(TIM3,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
	NVIC_Init(&NVIC_InitStructure);
	
	
	TIM_ITConfig(TIM3,TIM_IT_CC3,ENABLE);
	TIM_OC3FastConfig(TIM3,TIM_OCFast_Disable);//禁用快速模式，快速模式允许定时器在输出比较事件发生时立即切换输出状态，从而提高PWM信号的频率。在这里，它被禁用以保持默认的行为。
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_Cmd(TIM3,ENABLE);

}

void Motor_Init(void)
 {
  GPIO_InitTypeDef GPIO_InitStructure;  
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
 }
 
 void Motor1_ZHENG(void)           //电机1正转
{
 GPIO_ResetBits(GPIOB,GPIO_Pin_1);
}
 
 
void Motor1_FAN(void)           //电机1反转
{
 GPIO_SetBits(GPIOB,GPIO_Pin_1);
}

void Motor1_enable(void)			// 电机1使能
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_2,Bit_RESET);
}

void Motor1_disable(void)			// 电机1失能
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_2,Bit_SET);
}

void maichong1(int mb)
{
	if(mb>PWM_num1)
	{
		Motor1_ZHENG();
		mai1=1;
		TIM_CCxCmd(TIM3,TIM_Channel_3,ENABLE);		//打开TIM3通道3输出PWM
	}
	else if(mb<PWM_num1)
	{
		Motor1_FAN();
		mai1=0;
		TIM_CCxCmd(TIM3,TIM_Channel_3,ENABLE);		//打开TIM3通道3输出PWM
	}
	mubiao1=mb;
}

void PWM_SetCompare3(uint16_t Compare)
{
	TIM_SetCompare3(TIM2,Compare);
}

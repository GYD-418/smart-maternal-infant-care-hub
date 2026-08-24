#include "pwm.h"
#include "led.h"
#include "usart.h"
#include "Motor.h"

void Motor_STEP_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	// 3. 配置引脚复用映射
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3);    // PA6 -> TIM3_CH1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM3);    // PA7 -> TIM3_CH2

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM3);    // PA6 -> TIM3_CH1
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM3);    // PA7 -> TIM3_CH2
}

void Motor_DIR_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//上拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化
}

void TIM3_PWM1_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
//	TIM_InternalClockConfig(TIM3);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = 65536-1; 
	TIM_TimeBaseStructure.TIM_Prescaler =TIM_PRESCALER; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; 						//高级定时器功能
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_Toggle;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High ; 
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;	
	TIM_OC1Init(TIM3,&TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Disable);    //关闭预装载
	
	TIM_CCxCmd(TIM3,TIM_Channel_1,TIM_CCx_Disable);
	TIM_ClearFlag(TIM3,TIM_FLAG_CC1);																//清除定时器3的通道1的标志位
	TIM_ITConfig(TIM3,TIM_IT_CC1,ENABLE); //开启定时器3比较中断
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);

}

void TIM3_PWM2_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
//	TIM_InternalClockConfig(TIM3);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = 65536-1; 
	TIM_TimeBaseStructure.TIM_Prescaler =TIM_PRESCALER; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; 						//高级定时器功能
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_Toggle;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High ; 
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;	
	TIM_OC2Init(TIM3,&TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Disable);    //关闭预装载
	
	TIM_CCxCmd(TIM3,TIM_Channel_2,TIM_CCx_Disable);
	TIM_ClearFlag(TIM3,TIM_FLAG_CC2);																//清除定时器3的通道1的标志位
	TIM_ITConfig(TIM3,TIM_IT_CC2,ENABLE); //开启定时器3比较中断
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);

}


void TIM3_PWM3_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
//	TIM_InternalClockConfig(TIM3);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = 65536-1; 
	TIM_TimeBaseStructure.TIM_Prescaler =TIM_PRESCALER; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; 						//高级定时器功能
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_Toggle;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High ; 
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;	
	TIM_OC3Init(TIM3,&TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Disable);    //关闭预装载
	
	TIM_CCxCmd(TIM3,TIM_Channel_3,TIM_CCx_Disable);
	TIM_ClearFlag(TIM3,TIM_FLAG_CC3);																//清除定时器3的通道1的标志位
	TIM_ITConfig(TIM3,TIM_IT_CC3,ENABLE); //开启定时器3比较中断
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);

}

void TIM3_PWM4_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
//	TIM_InternalClockConfig(TIM3);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = 65536-1; 
	TIM_TimeBaseStructure.TIM_Prescaler =TIM_PRESCALER; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; 						//高级定时器功能
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_Toggle;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High ; 
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;	
	TIM_OC4Init(TIM3,&TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM3,TIM_OCPreload_Disable);    //关闭预装载
	
	TIM_CCxCmd(TIM3,TIM_Channel_4,TIM_CCx_Disable);
	TIM_ClearFlag(TIM3,TIM_FLAG_CC4);																//清除定时器3的通道1的标志位
	TIM_ITConfig(TIM3,TIM_IT_CC4,ENABLE); //开启定时器3比较中断
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);

}


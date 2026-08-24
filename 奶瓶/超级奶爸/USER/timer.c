#include "timer.h"
#include "stm32f4xx.h"                  // Device header

void TIM6_PID_X(int AR, int PS) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

    // ??????
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = AR - 1;       // ARR = AR - 1
    TIM_TimeBaseInitStructure.TIM_Prescaler = PS - 1;    // PSC = PS - 1
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStructure);

    TIM_ClearFlag(TIM6, TIM_FLAG_Update); // ??????
    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);

    // ???????(??main???????)
    // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    // ??????
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM6, ENABLE);
}

void TIM7_PID_Y(int AR , int PS)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;    //结构体	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);  //开启时钟
	
//	TIM_InternalClockConfig(TIM7);     //选择内部时钟

	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //指定时钟分频 采样频率 一分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数器模式 向上计数
	TIM_TimeBaseInitStructure.TIM_Period = AR - 1;   //周期 ARR自动重装器的值  0——65535         定时频率 = 72M / (PSC+1) / (ARR+1)  10k频率计10000个数
	TIM_TimeBaseInitStructure.TIM_Prescaler = PS - 1;  //PSC预分频器的值         0——65535      预分频多 自动重装少 频率低计较少的数
//	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;  //重复计数器的值
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseInitStructure);    //初始化时基单元
	
//	TIM_ClearFlag(TIM7, TIM_FLAG_Update);     //手动清除更新中断标志位  防止立刻更新中断进入中断从1开始
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);//使能中断
	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;   //中断通道  定时器2   NVIC通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;    //响应优先级
	NVIC_Init(&NVIC_InitStructure);         //初始化结构体
	
	TIM_Cmd(TIM7, ENABLE);
}

/**				
  * 函    数：该函数用于旋转时的PID计算，30ms进入一次中断
  * 参    数：无
  * 返 回 值：无
  */
void TIM_PID_XUAN(void)
{
	TIM_TimeBaseInitTypeDef TIM_BASE_Init;
	NVIC_InitTypeDef NVIC_Inital;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	
	TIM_BASE_Init.TIM_ClockDivision = TIM_CounterMode_Up;
	TIM_BASE_Init.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_BASE_Init.TIM_Period = 30000;
	TIM_BASE_Init.TIM_Prescaler = 8300;
	TIM_BASE_Init.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1,&TIM_BASE_Init);
	
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);	
		
	NVIC_Inital.NVIC_IRQChannel=TIM1_UP_TIM10_IRQn;           //定时器2中断
	NVIC_Inital.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Inital.NVIC_IRQChannelPreemptionPriority=2;//最高优先级
	NVIC_Inital.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_Inital);
	
	
	TIM_Cmd(TIM1,DISABLE);
	
}

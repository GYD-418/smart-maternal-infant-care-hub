#include "led.h" 
#include "delay.h"

void LED_Init(void)
{    	 
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOG, ENABLE);//使能GPIOG时钟
	//PG13、PG14和PG15初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;         //LED0、LED1和LED2对应IO口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                  //普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;             //100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                   //上拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);                         //初始化GPIO
	GPIO_ResetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10);   
	GPIO_SetBits(GPIOE,GPIO_Pin_5 | GPIO_Pin_6);
}

void beep(void)
{
	GPIO_ResetBits(GPIOE,GPIO_Pin_5);
	delay_ms(100);
	GPIO_SetBits(GPIOE,GPIO_Pin_5);
	delay_ms(100);
}

void kk_turn(void)
{
	GPIO_SetBits(GPIOE, GPIO_Pin_10);//可控硅打开奶粉仓
	delay_ms(5500);
	GPIO_ResetBits(GPIOE, GPIO_Pin_10);
}

// 初始化过零检测中断
void ZeroCross_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    // 使能GPIOA时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    // 使能SYSCFG时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    
    // 配置过零检测引脚为输入
    GPIO_InitStructure.GPIO_Pin = ZERO_CROSS_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; // 根据电路设计选择上/下拉
    GPIO_Init(GPIO_PORT, &GPIO_InitStructure);
    
    // 配置外部中断
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
    
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; // 根据实际电路选择边沿
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    
    // 配置中断优先级
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

// 初始化可控硅控制引脚
void Triac_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 配置可控硅触发引脚为推挽输出
    GPIO_InitStructure.GPIO_Pin = TRIAC_TRIGGER_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_ResetBits(GPIO_PORT, TRIAC_TRIGGER_PIN); // 初始状态关闭
}


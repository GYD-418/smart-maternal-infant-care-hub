#include "stm32f10x.h"                  // Device header
#include "hot.h"
#include "delay.h"
void fz_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
}

void hot_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11 | GPIO_Pin_10 ;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_ResetBits(GPIOA,GPIO_Pin_11 | GPIO_Pin_10);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	GPIO_ResetBits(GPIOB,GPIO_Pin_7);
}	 

void water1_on(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_11);
}

void water1_off(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_11);
}

void water2_on(void)
{
	GPIO_SetBits(GPIOB,GPIO_Pin_7);
}

void water2_off(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_7);
}

void light_on(void)
{
	GPIO_SetBits(GPIOB,GPIO_Pin_8);
}

void light_off(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);
}

void hot_on(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_10);
}

void hot_off(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_10);
}
void fz_ok(void)
{
		GPIO_SetBits(GPIOA,GPIO_Pin_1);
	delay_ms(20);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
}

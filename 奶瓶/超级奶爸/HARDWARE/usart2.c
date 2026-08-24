#include "stm32f4xx.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
uint8_t Serial_RxData_2;
uint8_t Serial_RxFlag_2;

void USART1_Init(uint32_t bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 1. 使能时钟（F4 的 USART1 挂在 APB2，GPIOA 挂在 AHB1）
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);  // USART1 时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);   // GPIOA 时钟
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);   // GPIOA 时钟

    // 2. 配置 GPIO（F4 需要明确设置复用功能）
    // PA9 (USART1_TX) - 复用推挽输出
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;            // 复用模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         // 推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;           // 上拉
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    // PA10 (USART1_RX) - 复用输入（上拉）
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;           // 上拉输入
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    // 3. 配置引脚复用（F4 必须手动映射 AF 功能）
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5);   // PA9 -> USART1_TX
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);  // PA10 -> USART1_RX

    // 4. 配置 USART1
    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_Init(UART5, &USART_InitStructure);

    // 5. 使能接收中断
    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);

    // 6. 配置 NVIC（中断优先级分组不变）
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);

    // 7. 使能 USART1
    USART_Cmd(UART5, ENABLE);
}
void Serial_SendByte_2(uint8_t Byte)
{
	USART_SendData(UART5, Byte);
	while (USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET);
}

void Serial_SendArray_2(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte_2(Array[i]);
	}
}

void Serial_SendString_2(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)
	{
		Serial_SendByte_2(String[i]);
	}
}

uint32_t Serial_Pow_2(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y --)
	{
		Result *= X;
	}
	return Result;
}

void Serial_SendNumber_2(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte_2(Number / Serial_Pow_2(10, Length - i - 1) % 10 + '0');
	}
}

int fputc(int ch, FILE *f)
{
	Serial_SendByte_2(ch);
	return ch;
}

void Serial_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_SendString_2(String);
}

uint8_t Serial_GetRxFlag_2(void)
{
	if (Serial_RxFlag_2 == 1)
	{
		Serial_RxFlag_2 = 0;
		return 1;
	}
	return 0;
}

uint8_t Serial_GetRxData(void)
{
	return Serial_RxData_2;
}


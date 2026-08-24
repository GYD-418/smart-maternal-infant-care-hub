#include "stm32f4xx.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
#include "string.h"

void UART4_Init(uint32_t bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    // 1. 使能时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    // 2. 配置GPIO
    // PC10作为UART4_TX，复用推挽输出
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    // PC11作为UART4_RX，复用输入
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; // 上拉输入
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    // 3. 引脚复用映射
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);

    // 4. 复位UART4（可选）
//    USART_DeInit(UART4);

    // 5. 配置UART
    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_Init(UART4, &USART_InitStructure);
    
    // 6. 使能接收中断
    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
    
    // 7. 配置NVIC
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);
    
    // 8. 使能UART
    USART_Cmd(UART4, ENABLE);
}

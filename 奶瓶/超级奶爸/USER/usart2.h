#ifndef __USART2_H
#define __USART2_H

#include <stdio.h>

void USART1_Init(uint32_t bound);
void Serial_SendByte_2(uint8_t Byte);
void Serial_SendArray_2(uint8_t *Array, uint16_t Length);
void Serial_SendString_2(char *String);
void Serial_SendNumber_2(uint32_t Number, uint8_t Length);
void Serial_Printf(char *format, ...);

uint8_t Serial_GetRxFlag_2(void);
uint8_t Serial_GetRxData_2(void);

#endif

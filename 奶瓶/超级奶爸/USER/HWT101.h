#ifndef __HWT101_H
#define __HWT101_H

#include "stm32f4xx.h"

/* ?????????? ---------------------------------------*/
void UART2_Init(uint32_t baudrate);
void Serial_SendByte_UART2(uint8_t Byte);
void UART2_Put_Char(uint8_t DataToSend);
void UART2_Put_String(uint8_t *Str);
void UART2_SendHalfWord(USART_TypeDef *pUSARTx, uint16_t ch);
void MyUART2_SendArr(uint16_t *str, uint8_t count);
void sendcmd(char cmd[]);

/* IMU ????? ---------------------------------------------*/
struct SAcc   { short a[3]; short T; };
struct SGyro  { short w[3]; short T; };
extern char angle_buf[30];
struct SAngle { short Angle[3]; short T; };

#endif

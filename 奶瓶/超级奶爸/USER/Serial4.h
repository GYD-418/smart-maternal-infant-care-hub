#ifndef __SERIAL4_H
#define __SERIAL4_H

void Serial4_Init(void);

void Usart4_SendByte(uint8_t Byte);
 
void Usart4_SendString(char *String);
void Usart4_SendArray(uint8_t *array, uint16_t length);
 
void Usart4_Printf(char *format, ...);
void ParseAndPrintData(uint8_t *data, uint16_t length);
uint8_t CalculateChecksum(uint8_t *data, uint16_t length, uint8_t type);

#endif

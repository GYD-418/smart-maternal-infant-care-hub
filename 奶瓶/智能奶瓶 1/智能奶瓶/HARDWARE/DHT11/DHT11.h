#ifndef _DHT11_H
#define _DHT11_H
 
void DHT1_Init_InPut(void);
void DHT1_Init_OutPut(void);
void DHT1_Start(void);
uint16_t DHT1_Scan(void);
uint16_t DHT1_ReadBit(void);
uint16_t DHT1_ReadByte(void);
uint16_t DHT1_ReadData(uint8_t buffer1[5]);
 
#endif

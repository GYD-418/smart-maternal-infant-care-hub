#ifndef _DHT11_DOUBLE_H
#define _DHT11_DOUBLE_H
 
void DHT2_Init_InPut(void);
void DHT2_Init_OutPut(void);
void DHT2_Start(void);
uint16_t DHT2_Scan(void);
uint16_t DHT2_ReadBit(void);
uint16_t DHT2_ReadByte(void);
uint16_t DHT2_ReadData(uint8_t buffer2[5]);
 
#endif

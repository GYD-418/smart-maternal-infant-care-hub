#ifndef WEIXI_H
#define WEIXI_H

#include "stm32f4xx.h"  


void Motor_go(int16_t maichong);
void Mortor_Zi_Zhuan_dir(uint8_t dir);
void Mortor_Zi_Zhuan_speed(int16_t speed);
void Mortor_Stop(void);

#endif  

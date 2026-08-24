#ifndef __LED_H
#define __LED_H
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	


//LED端口定义
#define LED0 PFout(9)	// DS0
#define LED1 PFout(10)	// DS1	 
#define ZERO_CROSS_PIN     GPIO_Pin_0   // 过零检测引脚 PA0
#define TRIAC_TRIGGER_PIN  GPIO_Pin_1   // 可控硅触发引脚 PA1
#define GPIO_PORT          GPIOA

void LED_Init(void);//初始化		
void kk_turn(void);
void beep(void);
void ZeroCross_Init(void);
void Triac_Init(void);
#endif

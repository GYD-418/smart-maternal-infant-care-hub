#include "stm32f4xx.h"
#include  "timer.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "Emm_V5.h"
#include "led.h"
#include "usart2.h"
#include "usart4.h"
#include <stdio.h>
#include "usart6.h"

//GPIO_SetBits(GPIOE, GPIO_Pin_0);混合仓电机12V 
//GPIO_SetBits(GPIOE, GPIO_Pin_1);饮水水泵12V
//GPIO_SetBits(GPIOE, GPIO_Pin_2);冲洗水泵12V
//GPIO_SetBits(GPIOE, GPIO_Pin_3);消毒灯5V
//GPIO_SetBits(GPIOE, GPIO_Pin_4);灯12V
//GPIO_SetBits(GPIOE, GPIO_Pin_7);奶粉仓电机12V
//GPIO_SetBits(GPIOE, GPIO_Pin_8);风扇12V
//GPIO_SetBits(GPIOE, GPIO_Pin_9);搅拌仓灯12V
//GPIO_SetBits(GPIOE, GPIO_Pin_10);可控硅打开奶粉仓
//GPIO_ResetBits(GPIOE,GPIO_Pin_6);低电平触发扫码模块
//#define ZERO_CROSS_PIN     GPIO_Pin_0   // 过零检测引脚 PA0
//#define TRIAC_TRIGGER_PIN  GPIO_Pin_1   // 可控硅触发引脚 PA1
// 控制状态变量
volatile uint8_t triac_enabled = 0;     // 可控硅开关状态 (0=关闭, 1=开启)

int aa,bb,cc,dd,ee,flag,Temp,ii,aaaa,bbbb,cccc,eeee;
uint8_t kk,S_RxData,res,data,kkkk;

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	//设置中断优先级分组为组2
	SysTick_Init();  
	usart_init(); 
	LED_Init();
	ZeroCross_Init();
	Triac_Init();
	USART1_Init(115200);
	UART4_Init(9600);
	USART6_Init(115200);
	TIM_PID_XUAN();
	triac_enabled = 0;
	Temp=19;
	while(1)
	{ 
		Serial_Printf("page0.t1.txt=\"%d\"\xff\xff\xff",Temp);
		if(flag==1)
		{
			Serial_Printf("page4.t0.txt=\"%d\"\xff\xff\xff",6);
			delay_ms(20);
			GPIO_SetBits(GPIOE,GPIO_Pin_6);
			flag=0;
			res=0;
		}
		if(kk==4)
		{
			beep();
			GPIO_SetBits(GPIOE,GPIO_Pin_6);
			kk=0;
		}
		if(kk==1)
		{
			beep();
			GPIO_SetBits(GPIOE, GPIO_Pin_1);
			if(aa%2==0)
			{
				GPIO_ResetBits(GPIOE,GPIO_Pin_1);
			}
			kk=0;
			data=0;
		}
		if(kkkk==1)
		{
			beep();
			GPIO_SetBits(GPIOE, GPIO_Pin_1);
			if(aaaa%2==0)
			{
				GPIO_ResetBits(GPIOE,GPIO_Pin_1);
			}
			kkkk=0;
			data=0;
		}		
		if(kk==2)
		{
			GPIO_ResetBits(GPIOE,GPIO_Pin_6);
		}
		if(kk==3)
		{
			beep();
			triac_enabled = 1;
			if(ee%2==0)
			{
				beep();
				triac_enabled = 0;
			}
			kk=0;
			data=0;
		}	
		if(kkkk==3)
		{
			beep();
			triac_enabled = 1;
			if(eeee%2==0)
			{
				triac_enabled = 0;
			}
			kkkk=0;
			data=0;
		}	

		if(kk==5)
		{
			beep();
			GPIO_SetBits(GPIOE, GPIO_Pin_2);
			if(bb%2==0)
			{
				GPIO_ResetBits(GPIOE,GPIO_Pin_2);
			}
			kk=0;
			data=0;
		}
		if(kkkk==5)
		{
			beep();
			GPIO_SetBits(GPIOE, GPIO_Pin_2);
			if(bbbb%2==0)
			{
				GPIO_ResetBits(GPIOE,GPIO_Pin_2);
			}
			kkkk=0;
			data=0;
		}
		if(kk==6)
		{
			beep();
			GPIO_SetBits(GPIOE, GPIO_Pin_3);
			if(cc%2==0)
			{
				GPIO_ResetBits(GPIOE,GPIO_Pin_3);
			}
			kk=0;
			data=0;
		}
		if(kkkk==6)
		{
			beep();
			GPIO_SetBits(GPIOE, GPIO_Pin_3);
			if(cccc%2==0)
			{
				GPIO_ResetBits(GPIOE,GPIO_Pin_3);
			}
			kkkk=0;
			data=0;
		}
		if(kk==7)
		{
			beep();
			GPIO_SetBits(GPIOE, GPIO_Pin_4);
			kk=0;
		}
		if(kk==8)
		{
			beep();
			GPIO_ResetBits(GPIOE, GPIO_Pin_4);
			kk=0;
		}
		if(kk==17)
		{
			beep();
			kk_turn();
			if(dd%2==0)
			{
				kk_turn();
			}
			kk=0;
		}
		if(kk==18)
		{
			beep();
			GPIO_SetBits(GPIOE, GPIO_Pin_1);//饮水水泵12V
			GPIO_SetBits(GPIOE, GPIO_Pin_0);//混合仓电机12V 
			triac_enabled = 1;
			delay_ms(7000);
			GPIO_ResetBits(GPIOE, GPIO_Pin_1);
			GPIO_ResetBits(GPIOE, GPIO_Pin_0);
			triac_enabled = 0;
			kk=0;
		}
		if(kk==40)
		{
			beep();  
			
			kk_turn();//打开奶粉仓盖
//先进奶粉
			GPIO_SetBits(GPIOE, GPIO_Pin_7);//奶粉仓电机12V
			GPIO_SetBits(GPIOE, GPIO_Pin_8);//风扇12V
			triac_enabled = 1;
			delay_ms(2000);
			GPIO_ResetBits(GPIOE,GPIO_Pin_7);
			kk_turn();//关闭奶粉仓盖
//搅拌
			GPIO_SetBits(GPIOE, GPIO_Pin_1);//饮水水泵12V
			GPIO_SetBits(GPIOE, GPIO_Pin_9);//搅拌仓灯12V
			
			delay_ms(1000);
			GPIO_SetBits(GPIOE, GPIO_Pin_0);//混合仓电机12V 
			delay_ms(5000);
			GPIO_ResetBits(GPIOE, GPIO_Pin_1);
			triac_enabled = 0;
			delay_ms(2000);
			GPIO_ResetBits(GPIOE, GPIO_Pin_8);//风扇12V
			GPIO_ResetBits(GPIOE, GPIO_Pin_0);
			GPIO_ResetBits(GPIOE, GPIO_Pin_9);
			
			
			data=0;
			kk=0;
			
		}
		if(kk==41)
		{
	beep();  
			
			kk_turn();
//先进奶粉
			GPIO_SetBits(GPIOE, GPIO_Pin_7);//奶粉仓电机12V
			GPIO_SetBits(GPIOE, GPIO_Pin_8);//风扇12V
			triac_enabled = 1;
			delay_ms(4000);
			GPIO_ResetBits(GPIOE,GPIO_Pin_7);
			kk_turn();//关闭奶粉仓盖
//搅拌
			GPIO_SetBits(GPIOE, GPIO_Pin_1);//饮水水泵12V
			GPIO_SetBits(GPIOE, GPIO_Pin_9);//搅拌仓灯12V
			
			delay_ms(1000);
			GPIO_SetBits(GPIOE, GPIO_Pin_0);//混合仓电机12V 
			delay_ms(7500);
			GPIO_ResetBits(GPIOE, GPIO_Pin_1);
			triac_enabled = 0;
			delay_ms(2000);
			GPIO_ResetBits(GPIOE, GPIO_Pin_8);//风扇12V
			GPIO_ResetBits(GPIOE, GPIO_Pin_0);
			GPIO_ResetBits(GPIOE, GPIO_Pin_9);
			
			data=0;
			kk=0;
		}
		if(kk==42)
		{
	beep();  
			
			kk_turn();
//先进奶粉
			GPIO_SetBits(GPIOE, GPIO_Pin_7);//奶粉仓电机12V
			GPIO_SetBits(GPIOE, GPIO_Pin_8);//风扇12V
			triac_enabled = 1;
			delay_ms(7000);
			GPIO_ResetBits(GPIOE,GPIO_Pin_7);
			kk_turn();//关闭奶粉仓盖
//搅拌
			GPIO_SetBits(GPIOE, GPIO_Pin_1);//饮水水泵12V
			GPIO_SetBits(GPIOE, GPIO_Pin_9);//搅拌仓灯12V
			
			delay_ms(1000);
			GPIO_SetBits(GPIOE, GPIO_Pin_0);//混合仓电机12V 
			delay_ms(15000);
			GPIO_ResetBits(GPIOE, GPIO_Pin_1);
			triac_enabled = 0;
			delay_ms(2500);
			GPIO_ResetBits(GPIOE, GPIO_Pin_0);
			GPIO_ResetBits(GPIOE, GPIO_Pin_9);
			GPIO_ResetBits(GPIOE, GPIO_Pin_8);//风扇12V
			data=0;
			kk=0;
		}
		if(kkkk==42)
		{
	beep();  
			
			kk_turn();
//先进奶粉
			GPIO_SetBits(GPIOE, GPIO_Pin_7);//奶粉仓电机12V
			GPIO_SetBits(GPIOE, GPIO_Pin_8);//风扇12V
			triac_enabled = 1;
			delay_ms(7000);
			GPIO_ResetBits(GPIOE,GPIO_Pin_7);
			kk_turn();//关闭奶粉仓盖
//搅拌
			GPIO_SetBits(GPIOE, GPIO_Pin_1);//饮水水泵12V
			GPIO_SetBits(GPIOE, GPIO_Pin_9);//搅拌仓灯12V
			
			delay_ms(1000);
			GPIO_SetBits(GPIOE, GPIO_Pin_0);//混合仓电机12V 
			delay_ms(15000);
			GPIO_ResetBits(GPIOE, GPIO_Pin_1);
			triac_enabled = 0;
			delay_ms(2500);
			GPIO_ResetBits(GPIOE, GPIO_Pin_0);
			GPIO_ResetBits(GPIOE, GPIO_Pin_9);
			GPIO_ResetBits(GPIOE, GPIO_Pin_8);//风扇12V
			data=0;
			kk=0;
		}
		if(kk==43)
		{
	beep();  
			
			kk_turn();
//先进奶粉
			GPIO_SetBits(GPIOE, GPIO_Pin_7);//奶粉仓电机12V
			GPIO_SetBits(GPIOE, GPIO_Pin_8);//风扇12V
			triac_enabled = 1;
			delay_ms(10000);
			GPIO_ResetBits(GPIOE,GPIO_Pin_7);
			kk_turn();//关闭奶粉仓盖
//搅拌
			GPIO_SetBits(GPIOE, GPIO_Pin_1);//饮水水泵12V
			GPIO_SetBits(GPIOE, GPIO_Pin_9);//搅拌仓灯12V
			
			delay_ms(1000);
			GPIO_SetBits(GPIOE, GPIO_Pin_0);//混合仓电机12V 
			delay_ms(20000);
			GPIO_ResetBits(GPIOE, GPIO_Pin_1);
			triac_enabled = 0;
			delay_ms(3000);
			GPIO_ResetBits(GPIOE, GPIO_Pin_0);
			GPIO_ResetBits(GPIOE, GPIO_Pin_9);
			GPIO_ResetBits(GPIOE, GPIO_Pin_8);//风扇12V
			data=0;
			kk=0;
		}
		if(kk==44)
		{
	beep();  
			
			kk_turn();
//先进奶粉
			GPIO_SetBits(GPIOE, GPIO_Pin_7);//奶粉仓电机12V
			GPIO_SetBits(GPIOE, GPIO_Pin_8);//风扇12V
			triac_enabled = 1;
			delay_ms(14500);
			GPIO_ResetBits(GPIOE,GPIO_Pin_7);
			kk_turn();//关闭奶粉仓盖
//搅拌
			GPIO_SetBits(GPIOE, GPIO_Pin_1);//饮水水泵12V
			GPIO_SetBits(GPIOE, GPIO_Pin_9);//搅拌仓灯12V
			
			delay_ms(1000);
			GPIO_SetBits(GPIOE, GPIO_Pin_0);//混合仓电机12V 
			delay_ms(22500);
			GPIO_ResetBits(GPIOE, GPIO_Pin_1);
			triac_enabled = 0;
			delay_ms(3000);
			GPIO_ResetBits(GPIOE, GPIO_Pin_0);
			GPIO_ResetBits(GPIOE, GPIO_Pin_9);
			GPIO_ResetBits(GPIOE, GPIO_Pin_8);//风扇12V
			data=0;
			kk=0;
		}

	}
}

void UART5_IRQHandler(void)
{
	if (USART_GetITStatus(UART5, USART_IT_RXNE) == SET)
	{
		S_RxData = USART_ReceiveData(UART5);
		if(S_RxData==01)
		{
			kk=1;	
			aa++;
		}
		if(S_RxData==64)
		{
			kk=40;
		}
		if(S_RxData==65)
		{
			kk=41;
		}
		if(S_RxData==66)
		{
			kk=42;
		}
		if(S_RxData==67)
		{
			kk=43;
		}
		if(S_RxData==68)
		{
			kk=44;
		}
		if(S_RxData==69)
		{
			kk=45;
		}		
		if(S_RxData==70)
		{
			kk=46;
		}		
		if(S_RxData==03)
		{
			kk=3;
			ee++;
		}
		if(S_RxData==02)
		{
			kk=2;
		}
		if(S_RxData==04)
		{
			kk=4;
		}
		if(S_RxData==05)
		{
			kk=5;
			bb++;
		}
		if(S_RxData==06)
		{
			kk=6;
			cc++;
		}
		if(S_RxData==07)
		{
			kk=7;
		}
		if(S_RxData==00)
		{
			kk=8;
		}
		if(S_RxData==23)
		{
			kk=17;
			dd++;
		}
		if(S_RxData==24)
		{
			kk=18;
		}
	}
	USART_ClearITPendingBit(UART5, USART_IT_RXNE);
}

void UART4_IRQHandler(void)
{
	if (USART_GetITStatus(UART4, USART_IT_RXNE) == SET)
	{
		res = USART_ReceiveData(UART4);
		if(res!=0)
		{
			flag=1;
		}
	}
	USART_ClearITPendingBit(UART4, USART_IT_RXNE);
}


void TIM1_UP_TIM10_IRQHandler()
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
	{	
		
	}
}

void USART6_IRQHandler(void)
{
    if (USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)  // 接收中断
    {
        data = USART_ReceiveData(USART6);  // 读取数据
				if(data==01)
				{
					kkkk=3;
					eeee++;
					data=0;
				}
				if(data==02)
				{
					kkkk=1;
					aaaa++;
					data=0;
				}
				if(data==03)
				{
					kkkk=5;
					bbbb++;
					data=0;
				}
				if(data==04)
				{
					kkkk=6;
					cccc++;
					data=0;
				}
				if(data==05)
				{
					kkkk=42;
				}
    }
	 USART_ClearITPendingBit(USART6, USART_IT_RXNE);  // 清除中断标志
	
}

// 过零检测中断服务函数
void EXTI0_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
				volatile int i;
        // 如果可控硅使能，在过零点触发
        if(triac_enabled)
        {
            GPIO_SetBits(GPIO_PORT, TRIAC_TRIGGER_PIN); // 触发可控硅
            // 保持触发信号足够长的时间（根据实际需求调整）
            for(i = 0; i < 100; i++); // 简单延时
            GPIO_ResetBits(GPIO_PORT, TRIAC_TRIGGER_PIN);
        }
        
        EXTI_ClearITPendingBit(EXTI_Line0); // 清除中断标志
    }
}


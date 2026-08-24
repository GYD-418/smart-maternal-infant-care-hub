#include "stm32f10x.h"                  // Device header
#include "led.h"
#include "delay.h"
#include "sys.h"
//#include "usart.h"
//#include "lcd.h"
#include "HX711.h"
#include "hot.h"
//#include "led.h"
#include "Serial.h"
#include "ds18b20.h"
#include "pwm.h"

int temperture,temp;
int time,time_set;
uint8_t kk;
uint8_t S_RxData;
extern s32 Weight_Shiwu1,Weight_Shiwu2,Weight_Shiwu3;  //放置重物体后，读取数据存储变量 

 int main(void)
 { 		
	 
	Serial_Init(); 			//115200
	delay_init();	    	 //延时函数初始化	  
	DS18B20_Init();	 
	Init_HX711pin();
	HX711_Init3();
	hot_Init(); 
	PWM_Init(7200-1,10000-1);			//  s
	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级	
	Get_Maopi1();				//称毛皮重量
	Get_Maopi2();				//称毛皮重量
  Get_Maopi3();				//称毛皮重量
	 
  	while(1) 
	{			
		temperture=DS18B20_Get_Temp();
		temp=temperture/10;
		printf("page0.t1.txt=\"%d\"\xff\xff\xff",temp);
		
		Weight_Shiwu1=Get_Weight1();
		Weight_Shiwu2=Get_Weight2();
		Weight_Shiwu3=Get_Weight3();
		printf("page0.t3.txt=\"%d\"\xff\xff\xff",Weight_Shiwu1-628);
		printf("page0.t5.txt=\"%d\"\xff\xff\xff",Weight_Shiwu2-124);
		printf("page0.t7.txt=\"%d\"\xff\xff\xff",Weight_Shiwu3-473);
		
		
		if(kk==1)
		{
			time_set=8;
			TIM_Cmd(TIM2,ENABLE);
		}
		
		if(kk==2)
		{
			time_set=8;
			TIM_Cmd(TIM2,ENABLE);
		}
		if(kk==3)
		{
			hot_on();
		}
		if(kk==4)
		{
			hot_off();
		}
		if(kk==5)
		{
			water2_on();
		}
		if(kk==6)
		{
			water2_off();
		}
		if(kk==7)
		{
			light_on();
		}
		if(kk==8)
		{
			light_off();
		}
		
	}	
}
 
void USART2_IRQHandler(void)
{

}

void USART3_IRQHandler(void)
{
	if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
	{
		S_RxData = USART_ReceiveData(USART3);
		if(S_RxData==01)
		{
			kk=1;
		}
		if(S_RxData==02)
		{
			kk=2;
		}
		if(S_RxData==03)
		{
			kk=3;
		}
		if(S_RxData==04)
		{
			kk=4;
		}
		if(S_RxData==05)
		{
			kk=5;
		}
		if(S_RxData==06)
		{
			kk=6;
		}
		if(S_RxData==07)
		{
			kk=7;
		}
		if(S_RxData==00)
		{
			kk=8;
		}
	}
	USART_ClearITPendingBit(USART3, USART_IT_RXNE);
}

void TIM2_IRQHandler(void) 
{
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) 
	{
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // 清除TIM2更新中断标志
		time=time+1;
		if(time==2)
		{
			water1_on();	
		}
	
		if(time==time_set)
		{
			time=0;
			kk=0;
			water1_off();
			TIM_Cmd(TIM2,DISABLE);	
		}
	}
}

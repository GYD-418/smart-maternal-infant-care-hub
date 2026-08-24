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

int aa=0;
int PWM_num1 = 0;
int tingzhi1=0;
int mai1 = 2;
int mubiao1=0;
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
	 
	fz_Init(); 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级	
	Get_Maopi1();				//称毛皮重量
	Get_Maopi2();				//称毛皮重量
  Get_Maopi3();				//称毛皮重量
	 
//  TIM3_PWM_Init(2000-1,90-1);					//这个电机1的速度就不用改了
//	TIM_SetCompare3(TIM3,90);
//	TIM_CCxCmd(TIM3,TIM_Channel_3,ENABLE);		//打开TIM3通道3输出PWM
//	Motor1_FAN();
   	while(1) 
	{			
		temperture=DS18B20_Get_Temp();
		temp=temperture/10;
		printf("page0.t1.txt=\"%d\"\xff\xff\xff",temp);
		
		Weight_Shiwu1=Get_Weight1();
		Weight_Shiwu2=Get_Weight2();
		Weight_Shiwu3=Get_Weight3();
		printf("page0.t3.txt=\"%d\"\xff\xff\xff",Weight_Shiwu1-475);
		printf("page0.t5.txt=\"%d\"\xff\xff\xff",Weight_Shiwu2-755);
		printf("page0.t7.txt=\"%d\"\xff\xff\xff",Weight_Shiwu3-240);
		
		if(tingzhi1==1)										//让电机1停止，在定时器里的写法停不了，就写了个标志位到while里来判断，杰哥牛逼
		 {
				TIM_CCxCmd(TIM3,TIM_Channel_3,DISABLE);			 
		 }
		
		if(kk==1)
		{
			time_set=10;
			TIM_Cmd(TIM2,ENABLE);
		}
		
		if(kk==40)
		{
			TIM3_PWM_Init(2000-1,16-1);					//这个电机1的速度就不用改了
			TIM_SetCompare3(TIM3,50);
			TIM_CCxCmd(TIM3,TIM_Channel_3,ENABLE);		//打开TIM3通道3输出PWM
			maichong1(2000);	
			time_set=4;
			TIM_Cmd(TIM2,ENABLE);
		}
		if(kk==41)
		{
			TIM3_PWM_Init(2000-1,16-1);					//这个电机1的速度就不用改了
			TIM_SetCompare3(TIM3,50);
			TIM_CCxCmd(TIM3,TIM_Channel_3,ENABLE);		//打开TIM3通道3输出PWM
			maichong1(2000);
			time_set=5;
			TIM_Cmd(TIM2,ENABLE);
		}
		if(kk==42)
		{
			TIM3_PWM_Init(2000-1,16-1);					//这个电机1的速度就不用改了
			TIM_SetCompare3(TIM3,50);
			TIM_CCxCmd(TIM3,TIM_Channel_3,ENABLE);		//打开TIM3通道3输出PWM
			maichong1(2000);
			time_set=8;
			TIM_Cmd(TIM2,ENABLE);
		}
		if(kk==43)
		{
			TIM3_PWM_Init(2000-1,16-1);					//这个电机1的速度就不用改了
			TIM_SetCompare3(TIM3,50);
			TIM_CCxCmd(TIM3,TIM_Channel_3,ENABLE);		//打开TIM3通道3输出PWM
			maichong1(2000);
			time_set=10;
			TIM_Cmd(TIM2,ENABLE);
		}
		if(kk==44)
		{
			TIM3_PWM_Init(2000-1,16-1);					//这个电机1的速度就不用改了
			TIM_SetCompare3(TIM3,50);
			TIM_CCxCmd(TIM3,TIM_Channel_3,ENABLE);		//打开TIM3通道3输出PWM
			maichong1(2000);
			time_set=12;
			TIM_Cmd(TIM2,ENABLE);
		}
		if(kk==45)
		{
			TIM3_PWM_Init(2000-1,16-1);					//这个电机1的速度就不用改了
			TIM_SetCompare3(TIM3,50);
			TIM_CCxCmd(TIM3,TIM_Channel_3,ENABLE);		//打开TIM3通道3输出PWM
			maichong1(2000);
			time_set=14;
			TIM_Cmd(TIM2,ENABLE);
		}
		if(kk==3)
		{
			hot_on();
		}
		if(kk==46)
		{
			hot_off();
		}
		if(kk==4)
		{
			hot_off();	
			if(aa%2==0)
				{
					
				fz_ok();
					if(aa%2==0)
					{
						aa=1;
					}
				}
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
 
void TIM3_IRQHandler(void)   //自己debug一下看数值变化就看懂了
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{	
//		count1++;
		if(mai1==1)
		{
			PWM_num1++;
			if(PWM_num1==mubiao1)
			{
				//TIM_CCxCmd(TIM3,TIM_Channel_3,DISABLE);		//关闭TIM3通道3输出PWM
				tingzhi1=1;
				mai1=2;
			}
		}
		if(mai1==0)
		{
			PWM_num1--;
			if(PWM_num1==mubiao1)
			{
        tingzhi1=1;
				//TIM_CCxCmd(TIM3,TIM_Channel_3,DISABLE);		//关闭TIM3通道3输出PWM
				mai1=2;
			}
		}
		if(mai1==2)
		{
			PWM_num1=0;
		}
		TIM_ClearITPendingBit(TIM3,TIM_IT_CC3);
  TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 
	}	
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
		}
		if(S_RxData==04)
		{
			kk=4;aa+=1;
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
			water1_off();
//			TIM3_PWM_Init(2000-1,90-1);					//这个电机1的速度就不用改了
//			TIM_SetCompare3(TIM3,50);
//			TIM_CCxCmd(TIM3,TIM_Channel_3,ENABLE);		//打开TIM3通道3输出PWM
//			maichong1(5000);
			time=0;
			kk=0;
			
			TIM_Cmd(TIM2,DISABLE);	
		}
	}
}

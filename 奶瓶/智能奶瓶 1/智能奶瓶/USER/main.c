#include "stm32f10x.h"                  // Device header
#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
//#include "lcd.h"
#include "DHT11.h"
#include "HX711.h"
#include "DHT11_double.h"
#include "Buzzer.h"
#include "hot.h"
#include "Motor.h"
//#include "led.h"
#include "Serial.h"

int d=8;
uint8_t S_RxData;
uint8_t S_RxData2;
int RxData;
uint8_t kk;
extern s32 Weight_Shiwu;  //放置重物体后，读取数据存储变量 
u8 Temp_Set=30;

 int main(void)
 { 		
//	u8 lcd_id[12];			//存放LCD ID字符串
	uint8_t buffer1[5];uint8_t buffer2[5];
	float  t1, h2;
	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先
	 
	hot_Init();
	Motor_Init();
	Serial_Init(); 			//115200
	delay_init();	    	 //延时函数初始化	  
	yuying_Init();
//	uart_init(9600);	 	//串口初始化为9600
	USART2_Init(9600);
//	uart5_init(9600);
	LED_Init();		  		//初始化与LED连接的硬件接口
// 	LCD_Init();                             
	Buzzer_Init();
//	POINT_COLOR=RED; 
//	sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//将LCD ID打印到lcd_id数组。	
	 
	 
	Init_HX711pin(); 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级	
	Get_Maopi();				//称毛皮重量
	delay_ms(1000);
	delay_ms(1000);
	Get_Maopi();				//重新获取毛皮重量
	

	 
/******************************
     宝宝卫士--“智能奶瓶”
******************************/	
//	printf("宝宝卫士");
//	Chinese_Show_two(5,5,0,16,0);Chinese_Show_two(25,5,1,16,0);Chinese_Show_two(45,5,2,16,0);Chinese_Show_two(65,5,3,16,0);
//	Chinese_Show_three(85,5,0,16,0);Chinese_Show_three(105,5,1,16,0);Chinese_Show_three(125,5,2,16,0);Chinese_Show_three(145,5,3,16,0);
//	Chinese_Show_four(165,5,0,16,0);Chinese_Show_four(185,5,1,16,0);Chinese_Show_four(205,5,2,16,0);Chinese_Show_four(225,5,3,16,0);
//****************************//	 
	 
  	while(1) 
	{	
		
		if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==SET)
		{
			RxData=USART_ReceiveData(USART1);
			if(RxData== 69)
			{
				hot_on();
				printf("page0.t11.txt=\"OPEN\"\xff\xff\xff");
			}
			if(RxData== 70)
			{
				hot_off();
				printf("page0.t11.txt=\"CLOSE\"\xff\xff\xff");
			}
			if(RxData== 71)
			{
				LED_OFF();
//				USART_Cmd (USART3 ,DISABLE );
			}
			if(RxData== 72)
			{
				LED_ON();
//				USART_Cmd (USART3,ENABLE );
			}
		}
//		if(kk==6)
//		{
//			hot_on();
//			printf("page0.t11.txt=\"OPEN\"\xff\xff\xff");
//		}
		
/******************************
     温度湿度显示
******************************/	 
//		Chinese_Show_five(5,40,0,16,0);Chinese_Show_five(22,40,1,16,0);Chinese_Show_one(39,40,0,16,0);Chinese_Show_one(56,40,1,16,0);LCD_ShowString(74,40,200,16,16,":");  //温度，size=16
//		Chinese_Show_five(5,70,0,16,0);Chinese_Show_five(22,70,1,16,0);Chinese_Show_one(39,70,2,16,0);Chinese_Show_one(56,70,3,16,0);LCD_ShowString(74,70,200,16,16,":");	//湿度	
		if(DHT1_ReadData(buffer1) == 0)
		{
//			h1 = buffer1[0] + buffer1[1] / 10.0;					//湿 度
			t1 = buffer1[2] + buffer1[3] / 10.0;					//温度
 
//			LCD_ShowFloat(79,40,16,t1,3,2);						//温度
//			LCD_ShowFloat(79,70,16,h1,3,2);						//湿度
			
//			Serial_SendByte(buffer1[0]);
//			printf("h1");
//			delay_ms(1000);
	
//			USART_SendData(USART2,(int)(Weight_Shiwu/3.77)/(3));
//			delay_ms(1000);
//			if(h1>80){Buzzer_ON(); }else{Buzzer_OFF();}
				printf("page0.t1.txt=\"%0.1f\"\xff\xff\xff",t1);
			
		}
		
		if(DHT2_ReadData(buffer2) == 0)
		{
			h2 = buffer2[0] + buffer2[1] / 10.0;					//温度
//			t2 = buffer2[2] + buffer2[3] / 10.0;					//湿度
 
//			LCD_ShowFloat(79,40,16,t1,3,2);						//温度
//			LCD_ShowFloat(79,70,16,h2,3,2);						//湿度
			
//			USART_SendData(USART2,(int)buffer1[0]);
//			delay_ms(1000);
			
			if(h2>80){Buzzer_ON(); }else{Buzzer_OFF();}
				printf("page0.t3.txt=\"%0.1f\"\xff\xff\xff",h2);
		}
		Serial_SendByte(0xA5);
		Serial_SendByte(t1);
		Serial_SendByte(h2);
		Serial_SendByte(t1+h2);
		Serial_SendByte(0x5A);
		
		                                      
		
/***************************		
重量显示  剩余水量
***************************/			
//		Chinese_Show_six(5,100,0,16,0);Chinese_Show_six(22,100,1,16,0);Chinese_Show_six(39,100,2,16,0);Chinese_Show_six(56,100,3,16,0);LCD_ShowString(74,100,200,16,16,":");
//		LCD_ShowString(135,100,200,16,16,"g");
		Weight_Shiwu=Get_Weight();
//		LCD_ShowFloat(79,100,16,Weight_Shiwu/3.77,4,2);
			printf("page0.t5.txt=\"%0.1f\"\xff\xff\xff",Weight_Shiwu/3.77);
		
	
		
/***************************		                                                                                                                                                                                                                                                                                    
        恒温温度
***************************/		
		
//		Chinese_Show_seven(5,130,0,16,0);Chinese_Show_seven(22,130,1,16,0);Chinese_Show_seven(39,130,2,16,0);Chinese_Show_seven(56,130,3,16,0);LCD_ShowString(74,130,200,16,16,":");
//		
//		LCD_ShowNum(89,130,Temp_Set,2,16);
		
		
/***************************		
				恒温状态
***************************/					
		
//		Chinese_Show_ten(5,160,0,16,0);Chinese_Show_ten(22,160,1,16,0);Chinese_Show_ten(39,160,2,16,0);Chinese_Show_ten(56,160,3,16,0);LCD_ShowString(74,160,200,16,16,":");

//		if(res==1)
//	{
//		hot_on();
//		Chinese_Show_eleven(90,160,2,16,0);	
//		if(t1>=Temp_Set)
//		{
//			hot_off();
//		}		
//	}
//		if(res==3)
//	{
//		Chinese_Show_eleven(90,160,3,16,0);
//		hot_off();
//	}
				
		
/***************************		
				加热状态
***************************/				
		
//		Chinese_Show_eight(5,190,0,16,0);Chinese_Show_eight(22,190,1,16,0);Chinese_Show_eight(39,190,2,16,0);Chinese_Show_eight(56,190,3,16,0);LCD_ShowString(74,190,200,16,16,":");
//		printf("page0.t11.txt=\"CLOSE\"\xff\xff\xff");
		
		if(kk==1)
		{
			hot_on();printf("page0.t11.txt=\"OPEN\"\xff\xff\xff");
		}
		
		if(kk==2)
		{
			hot_off();printf("page0.t11.txt=\"CLOSE\"\xff\xff\xff");
		}
		if(kk==3)
		{
			LED_OFF();
		}
		if(kk==4)
		{
			LED_ON();
		}
/***************************		
				奶瓶水位
***************************/					
		
//		Chinese_Show_nine(5,220,0,16,0);Chinese_Show_nine(22,220,1,16,0);Chinese_Show_nine(39,220,2,16,0);Chinese_Show_nine(56,220,3,16,0);LCD_ShowString(74,220,200,16,16,":");

//		LCD_ShowNum(90,220,((Weight_Shiwu/3.77))/(3),2,16);
//		LCD_ShowString(130,220,200,16,16,"%");
		
			printf("page0.t7.txt=\"%0.1f\"\xff\xff\xff",((Weight_Shiwu/3.77))/(3));
	} 
}
 
void USART2_IRQHandler()
{

    if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)  
    { 
			S_RxData2 = USART_ReceiveData(USART2);
			if(S_RxData2==12)
			{
				hot_on();kk=1;
			}
			if(S_RxData2==13)
			{
				hot_off();kk=2;
			}
			if(S_RxData2==14)
			{
				LED_OFF();kk=3;
			}
			if(S_RxData2==15)
			{
				LED_ON();kk=4;
			}
    }  	
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
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
	}
	USART_ClearITPendingBit(USART3, USART_IT_RXNE);
}

//void USART1_IRQHandler(void)
//{
//	if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==SET)
//		{
//			RxData=USART_ReceiveData(USART1);
//			if(RxData==45)
//			{
//				kk=6;
//			}
//		}
//		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
//}

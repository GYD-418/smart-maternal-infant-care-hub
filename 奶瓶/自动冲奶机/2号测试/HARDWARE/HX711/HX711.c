#include "stm32f10x.h"                  // Device header
#include "HX711.h"
#include "delay.h"

u32 HX711_Buffer1,HX711_Buffer2,HX711_Buffer3;
u32 Weight_Maopi1,Weight_Maopi2,Weight_Maopi3;
s32 Weight_Shiwu1,Weight_Shiwu2,Weight_Shiwu3;
//u8 Flag_Error1 = 0;
//u8 Flag_Error2 = 0;
//u8 Flag_Error3 = 0;

//校准参数
//因为不同的传感器特性曲线不是很一致，因此，每一个传感器需要矫正这里这个参数才能使测量值很准确。
//当发现测试出来的重量偏大时，增加该数值。
//如果测试出来的重量偏小时，减小改数值。
//该值可以为小数
#define GapValue 106.5


void Init_HX711pin(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PF端口时钟
	
	//HX711_SCK
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_4 ;				 // 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB
	
	//HX711_DOUT
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_5 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//输入上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

	GPIO_SetBits(GPIOA,GPIO_Pin_6 | GPIO_Pin_4);					//初始化设置为0
	
}

void HX711_Init3(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PF端口时钟
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 ;				 // 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				 // 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB
	
		GPIO_SetBits(GPIOB,GPIO_Pin_5);					//初始化设置为0
}


u32 HX711_Read1(void)	//增益128
{
	unsigned long count; 
	unsigned char i; 
  	HX711_SCK=0;  //拉低时钟引脚
  	count=0; 
	delay_us(1);
  	while(HX711_DOUT);   //读取数据引脚状态，等待AD芯片准备好，为低电平时是准备好了
  	for(i=0;i<24;i++)  //时钟周期500Khz
	{ 
	  	HX711_SCK=1; 
	  	count=count<<1; 
		delay_us(1);
		HX711_SCK=0; 
	  	if(HX711_DOUT)
			count++; 
		delay_us(1);
	} 
 	HX711_SCK=1; 
    count=count^0x800000;//第25个脉冲下降沿来时，转换数据 
	delay_us(1);
	HX711_SCK=0;  
	return(count);
}

/****************************************************
获取毛皮重量函数
****************************************************/
void Get_Maopi1(void)
{
	Weight_Maopi1 = HX711_Read1();	
} 

/****************************************************
称重函数
****************************************************/
s32 Get_Weight1(void)
{
		HX711_Buffer1 = HX711_Read1();
		Weight_Shiwu1 = HX711_Buffer1;	
		Weight_Shiwu1 = ((s32)((float)Weight_Shiwu1/GapValue)-79084)/3.77; 	//计算实物的实际重量
	
		return 	Weight_Shiwu1;	//因为不同的传感器特性曲线不一样，因此，每一个传感器需要矫正这里的GapValue这个除数。																//当发现测试出来的重量偏大时，增加该数值																	//如果测试出来的重量偏小时，减小改数值。

}

u32 HX711_Read2(void)	//增益128
{
	unsigned long count; 
	unsigned char i; 
  	HX711_SCK2=0;  //拉低时钟引脚
  	count=0; 
	delay_us(1);
  	while(HX711_DOUT2);   //读取数据引脚状态，等待AD芯片准备好，为低电平时是准备好了
  	for(i=0;i<24;i++)  //时钟周期500Khz
	{ 
	  	HX711_SCK2=1; 
	  	count=count<<1; 
		delay_us(1);
		HX711_SCK2=0; 
	  	if(HX711_DOUT2)
			count++; 
		delay_us(1);
	} 
 	HX711_SCK2=1; 
    count=count^0x800000;//第25个脉冲下降沿来时，转换数据 
	delay_us(1);
	HX711_SCK2=0;  
	return(count);
}

/****************************************************
获取毛皮重量函数
****************************************************/
void Get_Maopi2(void)
{
	Weight_Maopi2 = HX711_Read2();	
} 

/****************************************************
称重函数
****************************************************/
s32 Get_Weight2(void)
{
		HX711_Buffer2 = HX711_Read2();
		Weight_Shiwu2 = HX711_Buffer2;	
		Weight_Shiwu2 = ((s32)((float)Weight_Shiwu2/GapValue)-74041)/3.74; 	//计算实物的实际重量
	
		return 	Weight_Shiwu2;	//因为不同的传感器特性曲线不一样，因此，每一个传感器需要矫正这里的GapValue这个除数。																//当发现测试出来的重量偏大时，增加该数值																	//如果测试出来的重量偏小时，减小改数值。

}

u32 HX711_Read3(void)	//增益128
{
	unsigned long count; 
	unsigned char i; 
  	HX711_SCK3=0;  //拉低时钟引脚
  	count=0; 
	delay_us(1);
  	while(HX711_DOUT3);   //读取数据引脚状态，等待AD芯片准备好，为低电平时是准备好了
  	for(i=0;i<24;i++)  //时钟周期500Khz
	{ 
	  	HX711_SCK3=1; 
	  	count=count<<1; 
		delay_us(1);
		HX711_SCK3=0; 
	  	if(HX711_DOUT3)
			count++; 
		delay_us(1);
	} 
 	HX711_SCK3=1; 
    count=count^0x800000;//第25个脉冲下降沿来时，转换数据 
	delay_us(1);
	HX711_SCK3=0;  
	return(count);
}

/****************************************************
获取毛皮重量函数
****************************************************/
void Get_Maopi3(void)
{
	Weight_Maopi3 = HX711_Read3();	
} 

/****************************************************
称重函数
****************************************************/
s32 Get_Weight3(void)
{
		HX711_Buffer3 = HX711_Read3();
		Weight_Shiwu3 = HX711_Buffer3;	
		Weight_Shiwu3 = ((s32)((float)Weight_Shiwu3/GapValue)-84007)/3.80; 	//计算实物的实际重量
	
		return 	Weight_Shiwu3;	//因为不同的传感器特性曲线不一样，因此，每一个传感器需要矫正这里的GapValue这个除数。																//当发现测试出来的重量偏大时，增加该数值																	//如果测试出来的重量偏小时，减小改数值。

}


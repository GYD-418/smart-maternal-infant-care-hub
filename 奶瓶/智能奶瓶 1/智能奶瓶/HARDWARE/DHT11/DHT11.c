#include "stm32f10x.h"                  // Device header
#include "Delay.h"
 
 
void DHT1_Init_InPut(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
 
void DHT1_Init_OutPut(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
 
void DHT1_Start(void)
{
	DHT1_Init_OutPut();
	GPIO_ResetBits(GPIOB, GPIO_Pin_14); // 拉低总线
	delay_us(19000);
	GPIO_SetBits(GPIOB, GPIO_Pin_14); // 拉高总线
	delay_us(20);
	DHT1_Init_InPut();
}
 
uint16_t DHT1_Scan(void)
{
	return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14); // 返回读取数据
}
 
uint16_t DHT1_ReadBit(void)
{
	while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == RESET);
	delay_us(40);
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == SET)
	{
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == SET);
		return 1;
	}
	else
	{
		return 0;
	}
}
 
uint16_t DHT1_ReadByte(void)
{
	uint16_t i, data = 0;
	for(i = 0; i < 8; i++)
	{
		data <<= 1; // 向左进一位
		data |= DHT1_ReadBit();
	}
	return data;
}
 
uint16_t DHT1_ReadData(uint8_t buffer1[5])
{
	uint16_t i =0;
	uint8_t check1 = buffer1[0] + buffer1[1] + buffer1[2] + buffer1[3];
	DHT1_Start();
	if(DHT1_Scan() == RESET)
	{
		while(DHT1_Scan() == RESET);
		while(DHT1_Scan() == SET);
		for(i = 0; i < 5; i++)
		{
			buffer1[i] = DHT1_ReadByte();
		}
		// DHT11输出的40位数据
		while(DHT1_Scan() == RESET);
		DHT1_Init_OutPut();
		GPIO_SetBits(GPIOB, GPIO_Pin_11);
		
		
		if(check1 != buffer1[4])
		{
			return 1; // 数据出错
		}
	}
	return  0;
}

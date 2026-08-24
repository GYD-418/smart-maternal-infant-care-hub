#include "stm32f10x.h"                  // Device header
#include "Delay.h"
 
 
void DHT2_Init_InPut(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
 
void DHT2_Init_OutPut(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
 
void DHT2_Start(void)
{
	DHT2_Init_OutPut();
	GPIO_ResetBits(GPIOB, GPIO_Pin_15); // 拉低总线
	delay_us(19000);
	GPIO_SetBits(GPIOB, GPIO_Pin_15); // 拉高总线
	delay_us(20);
	DHT2_Init_InPut();
}
 
uint16_t DHT2_Scan(void)
{
	return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15); // 返回读取数据
}
 
uint16_t DHT2_ReadBit(void)
{
	while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) == RESET);
	delay_us(40);
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) == SET)
	{
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) == SET);
		return 1;
	}
	else
	{
		return 0;
	}
}
 
uint16_t DHT2_ReadByte(void)
{
	uint16_t i, data = 0;
	for(i = 0; i < 8; i++)
	{
		data <<= 1; // 向左进一位
		data |= DHT2_ReadBit();
	}
	return data;
}
 
uint16_t DHT2_ReadData(uint8_t buffer2[5])
{
	uint16_t i =0;
	uint8_t check2 = buffer2[0] + buffer2[1] + buffer2[2] + buffer2[3];
	DHT2_Start();
	if(DHT2_Scan() == RESET)
	{
		while(DHT2_Scan() == RESET);
		while(DHT2_Scan() == SET);
		for(i = 0; i < 5; i++)
		{
			buffer2[i] = DHT2_ReadByte();
		}
		// DHT11输出的40位数据
		while(DHT2_Scan() == RESET);
		DHT2_Init_OutPut();
		GPIO_SetBits(GPIOB, GPIO_Pin_10);
		
		
		if(check2 != buffer2[4])
		{
			return 1; // 数据出错
		}
	}
	return  0;
}

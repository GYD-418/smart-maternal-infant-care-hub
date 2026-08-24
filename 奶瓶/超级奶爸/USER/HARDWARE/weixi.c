#include "stm32f4xx.h"                  // Device header
#include "Emm_V5.h"
#include "delay.h"
#include "usart.h"
//Emm_V5_Pos_Control(1, 0, 50, 0, 16000, 0, 0); //左前
//delay_ms(10);
//Emm_V5_Pos_Control(4, 1, 50, 0, 16000, 0, 0);//右前
//delay_ms(10);			
//Emm_V5_Pos_Control(2, 1, 50, 0, 16000, 0, 0);//右后
//delay_ms(10);	
//Emm_V5_Pos_Control(3, 0, 50, 0, 16000, 0, 0);//左后
//delay_ms(10);	
//while(rxFrameFlag == false); 
//rxFrameFlag = false;

void Motor_go(int16_t maichong)
{
	uint8_t a,b;
	if(maichong > 0){a = 0x00;b=0x01;}
	else {a = 0x01;b=0x00;maichong = -maichong;}
	Emm_V5_Pos_Control(1, a, 170, 180, maichong, 0, 0); //左前
	delay_ms(10);
	Emm_V5_Pos_Control(4, b, 170, 180, maichong, 0, 0);//右前
	delay_ms(10);			
	Emm_V5_Pos_Control(2, b, 170, 180, maichong, 0, 0);//右后
	delay_ms(10);	
	Emm_V5_Pos_Control(3, a, 170, 180, maichong, 0, 0);//左后
	delay_ms(10);	
	while(rxFrameFlag == false); 
	rxFrameFlag = false;
}
/**					调用会立即执行
  * 函    数：原地旋转90度，当转过90度后，使能PID算法，进一步进行校正
  * 参    数：方向：0X01/0X00
  * 返 回 值：无
  */
void Mortor_Zi_Zhuan_dir(uint8_t dir)
{
	
	usart_SendByte(0X00);
	usart_SendByte(0XFD);
	usart_SendByte(dir);
	usart_SendByte(0X01);
	usart_SendByte(0XF4);		//速度
	usart_SendByte(0X78);		//加速度
	usart_SendByte(0X00);
	usart_SendByte(0X00);
	usart_SendByte(0X11);
	usart_SendByte(0X40);
	usart_SendByte(0X00);
	usart_SendByte(0X00);
	usart_SendByte(0X6B);
	delay_ms(1000);
	while(rxFrameFlag == false); 
	rxFrameFlag = false;

}

/**					调用会立即执行
  * 函    数：原地旋转,用于PID算法
  * 参    数：方向：0X01/0X00
  * 返 回 值：无
  */
void Mortor_Zi_Zhuan_speed(int16_t speed)
{
	uint8_t dir;
	if(speed < 0)
	{dir = 0x00; speed = -speed;}
	else {dir = 0x01; }
	
	usart_SendByte(0X00);
	usart_SendByte(0XF6);
	usart_SendByte(dir);
	usart_SendByte(0X00);
	usart_SendByte(speed);		//速度
	usart_SendByte(0X04);		//加速度
	usart_SendByte(0X00);
	usart_SendByte(0X6B);
	delay_ms(20);
}

void Mortor_Stop(void)
{
	
	Emm_V5_Stop_Now(0,0);
	delay_ms(1000);
	
}

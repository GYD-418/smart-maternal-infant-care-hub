#include "sys.h"
#include "usart.h"	  
#include "Key.h"
#include "delay.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif

//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
//int fputc(int ch, FILE *f)
//{      
//	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
//    USART1->DR = (u8) ch;      
//	return ch;
//}
#endif 

/*使用microLib的方法*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/
 
#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	  
  
void uart_init(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART1, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART1, ENABLE);                    //使能串口1 

}



u8 res;
//void USART1_IRQHandler(void)  
//{  
//  
////    if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)  
////    { 
////        res= USART_ReceiveData(USART1);   
////        USART_SendData(USART1,res);
////                
////    }  
//	USART_ClearITPendingBit(USART1,USART_IT_RXNE);
//}
//void USART1_IRQHandler(void)                	//串口1中断服务程序
//	{
//	u8 Res;
//#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
//	OSIntEnter();    
//#endif
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
//		{
//		Res =USART_ReceiveData(USART1);	//读取接收到的数据
//		
//		if((USART_RX_STA&0x8000)==0)//接收未完成
//			{
//			if(USART_RX_STA&0x4000)//接收到了0x0d
//				{
//				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
//				else USART_RX_STA|=0x8000;	//接收完成了 
//				}
//			else //还没收到0X0D
//				{	
//				if(Res==0x0d)USART_RX_STA|=0x4000;
//				else
//					{
//					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
//					USART_RX_STA++;
//					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
//					}		 
//				}
//			}   		 
//     } 
//#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
//	OSIntExit();  											 
//#endif
//} 
#endif	

//void USART2_Init(int bound)
//{
//  GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	 
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//使能USART1，GPIOA时钟
// 
//	//USART1_TX   GPIOA.9
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.9
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
//  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//   
//  //USART1_RX	  GPIOA.10初始化
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA10
//  GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;//浮空输入
//  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

//  //Usart1 NVIC 配置
//  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
//  
//   //USART 初始化设置

//	USART_InitStructure.USART_BaudRate = bound;//串口波特率
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
//	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

//  USART_Init(USART2, &USART_InitStructure); //初始化串口1
//  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口接受中断
//  USART_Cmd(USART2, ENABLE);                    //使能串口1 
//}


//串口5 PC12 PD2 ,APB1时钟
void uart5_init(u32 bound) {
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);    //使能UART5，GPIOC时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);    //使能UART5，GPIOC时钟

    //UART5_TX   GPIOC.12
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //PC.12
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //复用推挽输出
    GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOC.12

    //UART5_RX	  GPIOD.2初始化
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//PD2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIOD.2

    //Usart5 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;        //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);    //根据指定的参数初始化VIC寄存器

    //USART 初始化设置
    USART_InitStructure.USART_BaudRate = bound;//串口波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    //收发模式

    USART_Init(UART5, &USART_InitStructure); //初始化串口5
    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//开启串口接受中断
    USART_Cmd(UART5, ENABLE);                    //使能串口5
}

//void u5_printf(char *fmt, ...) {
//    int i;
//    int len;
//    char buffer[50]; //足够容纳才可以，可以搞大点
//    va_list args;
//    va_start(args, fmt);
//    vsprintf(buffer, fmt, args);
//    va_end(args);

//    len = strlen(buffer);

//    for (i = 0; i < len; i++) {
//        while (USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET); //等待发送完毕
//        USART_SendData(UART5, buffer[i]); //发送一个字符
//    }
//}

//void UART5_IRQHandler(void)                    //串口5中断服务程序
//{
//    u8 Res;
//    if (USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
//    {
//        Res = USART_ReceiveData(UART5);    //读取接收到的数据

//        u5_printf("收到了一个字符: %c\r\n", Res);
//    }

//}


//void USART2_IRQHandler()
//{
//    if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)  
//    { 
////        res= USART_ReceiveData(USART2);   
////        USART_SendData(USART2,res);
//    }  	
//		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
//}

void yuying_Init(void)
{
	
		GPIO_InitTypeDef GPIO_InitStructure;
	  USART_InitTypeDef USART_InitStructure;
	//开启时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入模式 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
  
	USART_InitStructure.USART_BaudRate=9600;  //波特率
	USART_InitStructure.USART_HardwareFlowControl= USART_HardwareFlowControl_None ;//硬件流控制
	USART_InitStructure.USART_Mode=USART_Mode_Rx;  //串口模式
	USART_InitStructure.USART_Parity= USART_Parity_No  ;//校验位
	USART_InitStructure.USART_StopBits=USART_StopBits_1;//停止位
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;//字长
	USART_Init(USART1,&USART_InitStructure);
	
	USART_Cmd(USART1,ENABLE);
	
}	


void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART2,Byte);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
}

//void Send(uint16_t a) //a为16进制
//{
//	delay_ms(100);
//	Serial_SendByte(0xA5);
//		Serial_SendByte(a);
//		Serial_SendByte(a);
//		Serial_SendByte(0x5A);
//	delay_ms(100);
//}

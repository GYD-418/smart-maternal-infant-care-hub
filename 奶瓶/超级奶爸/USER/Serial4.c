#include "stm32f4xx.h"                  // Device header
#include <stdio.h>
#include "Serial4.h"
#include "string.h"
#include "delay.h"
#include <stdarg.h>

extern volatile float global_angle;
extern volatile uint8_t new_data_received;
extern volatile float angular_velocity_y;
extern volatile float angular_velocity_z;
extern uint8_t received_data_packet[11];

 
//配置命令数组
//解锁寄存器 FF AA 69 88 B5
//Z轴角度归零FF AA 76 00 00
//设置200HZ输出 FF AA 03 0B 00
//设置波特率115200 FF AA 04 06 00
//保存 FF AA 00 00 00
//重启 FF AA 00 FF 00
uint8_t unlock_register[] = {0xFF, 0xAA, 0x69, 0x88, 0xB5};
uint8_t reset_z_axis[] = {0xFF, 0xAA, 0x76, 0x00, 0x00};
uint8_t set_output_200Hz[] = {0xFF, 0xAA, 0x03, 0x0B, 0x00};
uint8_t set_baudrate_115200[] = {0xFF, 0xAA, 0x04, 0x06, 0x00};
uint8_t save_settings[] = {0xFF, 0xAA, 0x00, 0x00, 0x00};
uint8_t restart_device[] = {0xFF, 0xAA, 0x00, 0xFF, 0x00};


void Serial4_Init(void)
{
	 GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(UART4, &USART_InitStructure);
	
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(UART4, ENABLE);
	
	// 发送配置命令
    Usart4_SendArray(unlock_register, sizeof(unlock_register));
    Usart4_SendArray(reset_z_axis, sizeof(reset_z_axis));
    Usart4_SendArray(set_output_200Hz, sizeof(set_output_200Hz));
    Usart4_SendArray(set_baudrate_115200, sizeof(set_baudrate_115200));
    Usart4_SendArray(save_settings, sizeof(save_settings));
    Usart4_SendArray(restart_device, sizeof(restart_device));
}


void Usart4_SendByte(uint8_t Byte) {
    USART_SendData(UART4, Byte); //将字节数据写入数据寄存器，写入后USART自动生成时序波形
    while (USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET); //等待发送完成
}
 
void Usart4_SendString(char *String) {
    uint8_t i = 0;
    while (String[i]) { // 遍历字符数组（字符串），遇到字符串结束标志位后停止
        Usart4_SendByte(String[i]); // 依次调用Usart3_SendByte发送每个字节数据
        i++;
    }
}
 
void Usar4_Printf(char *format, ...) {
    char String[100]; //定义字符数组
    va_list arg; //定义可变参数列表数据类型的变量arg
    va_start(arg, format); //从format开始，接收参数列表到arg变量
    vsprintf(String, format, arg); //使用vsprintf打印格式化字符串和参数列表到字符数组中
    va_end(arg); //结束变量arg
    Usart4_SendString(String); //串口发送字符数组（字符串）
}
 
void Usart4_SendArray(uint8_t *array, uint16_t length) 
	{
		uint16_t i;
    for (i = 0; i < length; i++) {
        Usart4_SendByte(array[i]);
    }
    Usart4_SendByte(0x0D); // 发送回车符
    Usart4_SendByte(0x0A); // 发送换行符
	delay_ms(200);
	
}

void UART4_IRQHandler(void)
{
   static uint8_t rx_buffer[11];
    static uint8_t rx_index = 0;
    static uint8_t state = 0; // 0: 寻找包头，1: 接收数据
 
    if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET) {
        uint8_t data = USART_ReceiveData(UART4);
 
        if (state == 0) { // 寻找包头
            if (data == 0x55) {
                rx_buffer[0] = data;
                rx_index = 1;
                state = 1;
            }
        } else if (state == 1) { // 接收数据
            rx_buffer[rx_index++] = data;
            if (rx_index == 2 && rx_buffer[1] != 0x53 && rx_buffer[1] != 0x52) { // 如果第二字节不是0x53或0x52，则继续寻找下一个0x55
                if (rx_buffer[1] == 0x55) {
                    rx_index = 1;
                } else {
                    state = 0;
                    rx_index = 0;
                }
            } else if (rx_index == 11) { // 接收到了完整的数据包
                ParseAndPrintData(rx_buffer, 11);
                rx_index = 0;
                state = 0; // 重新寻找包头
            }
        }
 
        USART_ClearITPendingBit(UART4, USART_IT_RXNE);
			}
}

void ParseAndPrintData(uint8_t *data, uint16_t length) {
		uint8_t yaw_l,yaw_h,wy_l,wy_h,wz_l,wz_h;
		int16_t yaw,wy,wz;
		float angle;
    if (length == 11) {
        uint8_t checksum = CalculateChecksum(data, length - 1, data[1]);
        if (checksum != data[length - 1]) {
//            Usart4_Printf("Checksum error\r\n");
            return;
        }
 
        if (data[0] == 0x55 && data[1] == 0x53) {
            // 数据部分从索引 6 开始，低字节在前，高字节在后
            yaw_l = data[6];
            yaw_h = data[7];
            yaw = (int16_t)((yaw_h << 8) | yaw_l);
 
            // 将解析后的整数值转换为角度
						yaw = (float)yaw;
            angle = (yaw / 32768.0) * 180.0;
            global_angle = angle;
            new_data_received = 1;
        } else if (data[0] == 0x55 && data[1] == 0x52) {
            // 解析角速度数据
            wy_l = data[4];
            wy_h = data[5];
            wy = (int16_t)((wy_h << 8) | wy_l);
						wy = (float)wy;
            angular_velocity_y = (wy / 32768.0) * 2000.0;
 
            wz_l = data[6];
            wz_h = data[7];
            wz = (int16_t)((wz_h << 8) | wz_l);
						wz = (float)wz;
            angular_velocity_z = (wz / 32768.0) * 2000.0;
 
            new_data_received = 1;
        }
    }
}

uint8_t CalculateChecksum(uint8_t *data, uint16_t length, uint8_t type) 
{
		uint16_t i;
		uint8_t checksum;
    checksum = 0;
    for (i = 0; i < length; i++) {
        checksum += data[i];
    }
    return checksum;
}


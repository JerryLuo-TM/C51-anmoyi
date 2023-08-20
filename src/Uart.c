#include "STC15W4K60S4.h"
#include "intrins.h"
#include "Uart.H"

#define S1_S0 0x40
#define S1_S1 0x80

bit busy = 0;		  // 发送占忙标志
unsigned char rc = 0; // 接受数据缓存

void Usart_Init() // 波特率115200  18.432MHz
{
	SCON = 0x50;  // 8位数据,可变波特率
	AUXR |= 0x01; // 串口1选择定时器2为波特率发生器
	AUXR |= 0x04; // 定时器2时钟为Fosc,即1T
	T2L = 0xD8;	  // 设定定时初值
	T2H = 0xFF;	  // 设定定时初值
	AUXR |= 0x10; // 启动定时器2
	ES = 1;
	EA = 1;
}

unsigned int uint_to_str(unsigned long number, unsigned char *str_array)
{
	unsigned int i, value, bits;
	unsigned char byte_array[16];

	bits = 0;
	value = number;
	do
	{
		byte_array[bits++] = value % 10;
		value /= 10;
	} while (value);

	for (i = 0; i < bits; i++)
	{
		str_array[i] = byte_array[bits - i - 1] + '0';
	}

	return bits;
}

void SendData(unsigned char dat)
{
	while (busy)
		;
	ACC = dat;
	busy = 1;
	SBUF = ACC;
}

void SendString(char *s)
{
	while (*s)
	{
		SendData(*s++);
	}
}

void Uart() interrupt 4 using 1
{
	if (TI)
	{
		TI = 0;
		busy = 0;
	}
	if (RI)
	{
		rc = SBUF;
		RI = 0;
	}
}

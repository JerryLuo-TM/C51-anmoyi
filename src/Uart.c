#include "STC15W4K60S4.h"
#include "intrins.h"
#include "Uart.H"

#define S1_S0 0x40
#define S1_S1 0x80

bit busy = 0;		  // ����ռæ��־
unsigned char rc = 0; // �������ݻ���

void Usart_Init() // ������115200  18.432MHz
{
	SCON = 0x50;  // 8λ����,�ɱ䲨����
	AUXR |= 0x01; // ����1ѡ��ʱ��2Ϊ�����ʷ�����
	AUXR |= 0x04; // ��ʱ��2ʱ��ΪFosc,��1T
	T2L = 0xD8;	  // �趨��ʱ��ֵ
	T2H = 0xFF;	  // �趨��ʱ��ֵ
	AUXR |= 0x10; // ������ʱ��2
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

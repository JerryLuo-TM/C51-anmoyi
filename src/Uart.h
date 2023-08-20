#ifndef __UART_H_
#define __UART_H_

void Usart_Init();
void SendData(unsigned char dat);
void SendString(char *s);

unsigned int uint_to_str(unsigned long number, unsigned char *str_array);

#endif
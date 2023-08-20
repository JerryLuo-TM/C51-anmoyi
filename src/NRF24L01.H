#ifndef __NRF24L01_H
#define __NRF24L01_H
typedef unsigned char uchar;
typedef unsigned char uint;	
/*
sbit 	MISO	=P0^6;  //LQM固件
sbit 	MOSI	=P0^7;
sbit	SCK	  =P6^0;
sbit	CE	  =P6^2;
sbit	CSN		=P6^1;
sbit	IRQ		=P0^5;
*/
/*
sbit	CE	    =P1^3;	  
sbit	CSN		=P1^4;     //LJ固件
sbit	SCK	    =P6^3;    
sbit 	MOSI	=P1^0;     
sbit 	MISO	=P0^6;     
sbit	IRQ		=P0^7;
*/
// 最新接收  
//sbit	CE	    =P1^5;	  
//sbit	CSN		=P1^4;
//sbit	SCK	    =P1^3;    
//sbit 	MOSI	=P1^2;     
//sbit 	MISO	=P4^7;
//sbit	IRQ		=P1^1;
//战车驱动
sbit	CE	    =P3^4;	  
sbit	CSN		=P5^0;
sbit	SCK	    =P6^7;    
sbit 	MOSI	=P3^3;     
sbit 	MISO	=P6^5;
sbit	IRQ		=P6^6;


void Delay(unsigned int s);
void inerDelay_us(unsigned char n);
void init_NRF24L01();
uint SPI_RW(uint uchar);
uchar SPI_Read(uchar reg);
void SetRX_Mode(void);
uint SPI_RW_Reg(uchar reg, uchar value);
uint SPI_Read_Buf(uchar reg, uchar *pBuf, uchar uchars);
uint SPI_Write_Buf(uchar reg, uchar *pBuf, uchar uchars);
unsigned char nRF24L01_RxPacket(unsigned char* rx_buf);
void nRF24L01_TxPacket(unsigned char * tx_buf);

#endif  
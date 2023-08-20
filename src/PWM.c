#include "STC15W4K60S4.h"
#include "PWM.H"
#include "intrins.h"
void PWM_INT()         
{
//[PWM2:P3.7, PWM3:P2.1, PWM4:P2.2, PWM5:P2.3, PWM6:P1.6, PWM7:P1.7]  
//********************端口配置寄存器*******************
	  P_SW2=0x80;
//********************PWM配置寄存器*********************
	  PWMCFG=0x00;         //PEM输出端口初始电平为低电平
//********************PWM 控制寄存器********************
	  PWMCR=0x83;          //1 0 00 0011
    //PWMCR=0x8f;
//*********************PWM中断标志寄存器****************
		PWMIF=0x00;          //00000000
//*********************PWM外部异常控制寄存器PWMFDCR****
		PWMFDCR=0x00;        //00 0 0 0 0 0 0 (全关闭)
//**********************PWM时钟选择寄存器****************
	  PWMCKS=0x10;            //0000 000  PWM时钟源为系统时钟经分频器分频之后的时钟   由T2分频，低4位置 0000 
//  PWMCKS=0x00;      // 同上,此处为，内部时钟分频    时钟频率/后三位+1=分频频率  当前为系统设定分频
//***********************PWM2 即P3^7 控制寄存器**********
    PWM2CR=0x00;
		PWM3CR=0x00;
/*-------------------------------高八位和低八位控制--------------------------------*/	
    PWMCH=0x03;          //高 7 位    可设1~32767  此处设置   1001份精度
	  PWMCL=0xE9;          //低 8 位		
}

/********************************翻转***************************************************/
void PWMGO(unsigned int PWM2,unsigned int PWM3)
{
	

	   PWM2T1H=PWM2>>8;            //PWM第二次反转计数器 (高 7 位)  此次翻转后为低电平
	   PWM2T1L=PWM2;           //PWM第二次反转计数器 (高 7 位)    
	   PWM2T2H=0x03;
     PWM2T2L=0xE9;
	   
	   PWM3T1H=PWM3>>8;            //PWM第二次反转计数器 (高 7 位)  此次翻转后为低电平
	   PWM3T1L=PWM3;           //PWM第二次反转计数器 (高 7 位)    
		 PWM3T2H=0x03;
     PWM3T2L=0xE9;   
}

void Timer2Init(void)		//1us@28MHz
{
	AUXR |= 0x04;
	T2L = 0xE4;  // 0xFE; 0.0714us   14k   0xE4;   1US   1k
	T2H = 0xFF;
	AUXR |= 0x10;
}

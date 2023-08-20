#include "STC15W4K60S4.h"
#include "intrins.h"
#include "CCP.H"
unsigned char cnt;                           //存储PCA计时溢出次数
unsigned long count0;                       //记录上一次的捕获值
unsigned long count1;                       //记录本次的捕获值
unsigned long length;                       //存储信号的时间长度(count1 - count0)

void Init_CCP()
{
    P_SW1 &= 0xcf;                  //(P1.2/ECI, P1.1/CCP0, P1.0/CCP1, P3.7/CCP2)
    CCON = 0;                       //初始化PCA控制寄存器PCA定时器停止清除CF标志清除模块中断标志
    CL = 0;                         //复位PCA寄存器
    CH = 0;
    CCAP0L = 0;
    CCAP0H = 0;
    CMOD = 0x09;                    //设置PCA时钟源为系统时钟,且使能PCA计时溢出中断
  //CCAPM0 = 0x21;                  //PCA模块0为16位捕获模式(上升沿捕获,可测从高电平开始的整个周期),且产生捕获中断
  //CCAPM0 = 0x11;                  //PCA模块0为16位捕获模式(下降沿捕获,可测从低电平开始的整个周期),且产生捕获中断
    CCAPM0 = 0x31;                  //PCA模块0为16位捕获模式(上升沿/下降沿捕获,可测高电平或者低电平宽度),且产生捕获中
    CR = 1;                         //PCA定时器开始工作
    EA = 1;

    cnt = 0;
    count0 = 0;
    count1 = 0;
}

void PCA_isr() interrupt 7
{
    if (CF)
    {
        CF = 0;
        cnt++;                      //PCA计时溢出次数+1
    }
    if (CCF0)
    {
        CCF0 = 0;
        count0 = count1;            //备份上一次的捕获值
        ((unsigned char *)&count1)[3] = CCAP0L;  //保存本次的捕获值
        ((unsigned char *)&count1)[2] = CCAP0H;
        ((unsigned char *)&count1)[1] = cnt;
        ((unsigned char *)&count1)[0] = 0;
        length = count1 - count0;   //计算两次捕获的差值,即得到时间长度
    }
}


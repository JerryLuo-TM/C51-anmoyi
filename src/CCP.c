#include "STC15W4K60S4.h"
#include "intrins.h"
#include "CCP.H"
unsigned char cnt;                           //�洢PCA��ʱ�������
unsigned long count0;                       //��¼��һ�εĲ���ֵ
unsigned long count1;                       //��¼���εĲ���ֵ
unsigned long length;                       //�洢�źŵ�ʱ�䳤��(count1 - count0)

void Init_CCP()
{
    P_SW1 &= 0xcf;                  //(P1.2/ECI, P1.1/CCP0, P1.0/CCP1, P3.7/CCP2)
    CCON = 0;                       //��ʼ��PCA���ƼĴ���PCA��ʱ��ֹͣ���CF��־���ģ���жϱ�־
    CL = 0;                         //��λPCA�Ĵ���
    CH = 0;
    CCAP0L = 0;
    CCAP0H = 0;
    CMOD = 0x09;                    //����PCAʱ��ԴΪϵͳʱ��,��ʹ��PCA��ʱ����ж�
  //CCAPM0 = 0x21;                  //PCAģ��0Ϊ16λ����ģʽ(�����ز���,�ɲ�Ӹߵ�ƽ��ʼ����������),�Ҳ��������ж�
  //CCAPM0 = 0x11;                  //PCAģ��0Ϊ16λ����ģʽ(�½��ز���,�ɲ�ӵ͵�ƽ��ʼ����������),�Ҳ��������ж�
    CCAPM0 = 0x31;                  //PCAģ��0Ϊ16λ����ģʽ(������/�½��ز���,�ɲ�ߵ�ƽ���ߵ͵�ƽ���),�Ҳ���������
    CR = 1;                         //PCA��ʱ����ʼ����
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
        cnt++;                      //PCA��ʱ�������+1
    }
    if (CCF0)
    {
        CCF0 = 0;
        count0 = count1;            //������һ�εĲ���ֵ
        ((unsigned char *)&count1)[3] = CCAP0L;  //���汾�εĲ���ֵ
        ((unsigned char *)&count1)[2] = CCAP0H;
        ((unsigned char *)&count1)[1] = cnt;
        ((unsigned char *)&count1)[0] = 0;
        length = count1 - count0;   //�������β���Ĳ�ֵ,���õ�ʱ�䳤��
    }
}


#include "STC15W4K60S4.h"
#include "intrins.h"
#include "Uart.H"

#define ADC_POWER 0x80
#define ADC_FLAG 0x10
#define ADC_START 0x08
#define ADC_SPEEDLL 0x00
#define ADC_SPEEDL 0x20
#define ADC_SPEEDH 0x40
#define ADC_SPEEDHH 0x60

#define ID_ADDR_ROM 0x1ff7 // 8K ROM MCU
#define ID_ADDR_RAM 0xEF   // 256byte RAMMCU 0EFH

sbit POWER = P1 ^ 2;
sbit LED = P1 ^ 3;
sbit M0 = P1 ^ 4;
sbit M1 = P1 ^ 5;
sbit KEY = P3 ^ 2;

typedef enum
{
	SYS_MODE_STOP = 0,
	SYS_MODE_1,
	SYS_MODE_2,
	SYS_MODE_END,
} mode_type_t;

typedef enum
{
	OK = 0,
	FAIL,
} status_t;

unsigned int bandgap_adc, bandgap_vol;
unsigned long timestamp = 0, timestamp_1 = 0;
mode_type_t WORK_MODE = SYS_MODE_STOP;

unsigned int current;
unsigned char buffer[16];

void Delayms(unsigned int time) //@18.432MHz
{
	unsigned char i, j;
	do
	{
		i = 18;
		j = 235;
		do
		{
			while (--j)
				;
		} while (--i);
	} while (time--);
}

void Timer0Init(void)
{
	AUXR |= 0x80;
	TMOD &= 0xF0;
	TL0 = 0x00;
	TH0 = 0xB8;
	TF0 = 0;
	TR0 = 1;
	ET0 = 1;
	EA = 1;
}

void adc_init()
{
	P1ASF = 0x02; // P1.1
	ADC_RES = 0;
	ADC_RESL = 0;
	ADC_CONTR = ADC_POWER | ADC_SPEEDLL;
	Delayms(1);
}

unsigned int getADCResult(char ch)
{
	unsigned int adc = 0;
	if (ch == 0)
	{
		P1ASF = 0x00;
	}
	else
	{
		P1ASF = 0x02;
	}
	ADC_RES = 0;
	ADC_RESL = 0;
	ADC_CONTR = ADC_POWER | ADC_SPEEDL | ch | ADC_START;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	while (!(ADC_CONTR & ADC_FLAG))
		;
	ADC_CONTR &= ~ADC_FLAG;
	adc = (unsigned int)ADC_RES;
	adc = (adc << 2) | (ADC_RESL & 3);
	return adc;
}

void Power_switch(unsigned char on)
{
	if (on == 0x00)
	{
		POWER = 0;
	}
	else
	{
		POWER = 1;
	}
}

void Motor_Start(unsigned char dir)
{
	if (dir == 0x00)
	{
		M0 = 0;
		M1 = 1;
	}
	else
	{
		M0 = 1;
		M1 = 0;
	}
}

void Motor_Stop(void)
{
	M0 = 0;
	M1 = 0;
}

status_t get_key_mode(unsigned int time)
{
	if (KEY == 0)
	{
		Delayms(time);
		if (KEY == 0)
		{
			return OK;
		}
	}
	else
	{
		return FAIL;
	}
	return FAIL;
}

void system_stop(void)
{
	Power_switch(0);
	Motor_Stop();
	LED = 0;
	timestamp = 0;
}

void system_mode_1(void)
{
	unsigned long lc_time = 0;
	Power_switch(1);
	LED = 1;

	lc_time = timestamp / 1000ul;
	if (lc_time < 3 * 60)
	{
		if (((lc_time / 30) % 2) == 0)
		{
			Motor_Start(0);
		}
		else
		{
			Motor_Start(1);
		}
	}
	else if (lc_time < 6 * 60)
	{
		if (((lc_time / 20) % 2) == 0)
		{
			Motor_Start(0);
		}
		else
		{
			Motor_Start(1);
		}
	}
	else if (lc_time < 9 * 60)
	{
		if (((lc_time / 10) % 2) == 0)
		{
			Motor_Start(0);
		}
		else
		{
			Motor_Start(1);
		}
	}
	else if (lc_time < 12 * 60)
	{
		if (((lc_time / 20) % 2) == 0)
		{
			Motor_Start(0);
		}
		else
		{
			Motor_Start(1);
		}
	}
	else if (lc_time < 15 * 60)
	{
		if (((lc_time / 30) % 2) == 0)
		{
			Motor_Start(0);
		}
		else
		{
			Motor_Start(1);
		}
	}
}

void system_mode_2(void)
{
	unsigned long lc_time = 0;
	Power_switch(1);
	LED = 1;

	lc_time = timestamp / 1000ul;

	if (lc_time < 3 * 60)
	{
		if (((lc_time / 60) % 2) == 0)
		{
			Motor_Start(0);
		}
		else
		{
			Motor_Start(1);
		}
	}
	else if (lc_time < 6 * 60)
	{
		if (((lc_time / 40) % 2) == 0)
		{
			Motor_Start(0);
		}
		else
		{
			Motor_Start(1);
		}
	}
	else if (lc_time < 9 * 60)
	{
		if (((lc_time / 20) % 2) == 0)
		{
			Motor_Start(0);
		}
		else
		{
			Motor_Start(1);
		}
	}
	else if (lc_time < 12 * 60)
	{
		if (((lc_time / 40) % 2) == 0)
		{
			Motor_Start(0);
		}
		else
		{
			Motor_Start(1);
		}
	}
	else if (lc_time < 15 * 60)
	{
		if (((lc_time / 60) % 2) == 0)
		{
			Motor_Start(0);
		}
		else
		{
			Motor_Start(1);
		}
	}
}

void main(void)
{
	status_t status = FAIL;
	unsigned char *iptr;
	unsigned int i, len;

	P0M0 = 0x00;
	P0M1 = 0x00;
	P1M0 = 0x3C;
	P1M1 = 0x00;
	P2M0 = 0x00;
	P2M1 = 0x00;
	P3M0 = 0x03;
	P3M1 = 0x00;
	P4M0 = 0x00;
	P4M1 = 0x00;
	P5M0 = 0x00;
	P5M1 = 0x00;

	Delayms(10);
	Usart_Init();
	Timer0Init();
	adc_init();

	system_stop();

	KEY = 1;

	iptr = ID_ADDR_RAM;
	bandgap_vol = ((unsigned int)*iptr << 8) | (unsigned int)(*iptr + 1); // mv

	SendString("STC15F408AS Test! \r\n");
	while (1)
	{
		status = get_key_mode(200);
		if (status == OK)
		{
			WORK_MODE++;
			if (WORK_MODE >= SYS_MODE_END)
			{
				WORK_MODE = SYS_MODE_STOP;
			}
		}

		switch (WORK_MODE)
		{
		case SYS_MODE_STOP:
		{
			system_stop();
		}
		break;

		case SYS_MODE_1:
		{
			system_mode_1();
		}
		break;

		case SYS_MODE_2:
		{
			system_mode_2();
		}
		break;

		default:
			break;
		}

		if (timestamp > 900000ul) // 1000 * 60 * 15
		{
			timestamp = 0;
			WORK_MODE = SYS_MODE_STOP;
		}

		bandgap_adc = getADCResult(0);
		if (bandgap_adc != 0)
		{
			current = (bandgap_vol * getADCResult(1)) / bandgap_adc * 10;
		}

		if (current > 3000)
		{
			WORK_MODE = SYS_MODE_STOP;
		}

		if (timestamp_1 > 1000ul)
		{
			timestamp_1 = 0;
			for (i = 0; i < sizeof(buffer); i++)
			{
				buffer[i] = 0;
			}
			len = uint_to_str(current, &buffer[0]);
			buffer[len] = '\r';
			buffer[len + 1] = '\n';
			buffer[len + 2] = '\0';
			SendString(buffer);
		}
	}
}

/* Timer0 interrupt routine */
void tm0_isr() interrupt 1
{
	timestamp++;
	timestamp_1++;
}

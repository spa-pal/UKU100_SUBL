#include "pcf8563.h"
#include "main.h"
#include <LPC21XX.H>
#include "i2c.h"

//-----------------------------------------------
//Время
s time_union,time_union1;

signed char sec_bcd;
signed char min_bcd;
signed char hour_bcd;
signed char day_bcd;
signed char month_bcd;
signed char year_bcd;

#define sec__  time_union.time_str.sec___
#define min__  time_union.time_str.min___
#define hour__  time_union.time_str.hour___
#define day__  time_union.time_str.day___
#define month__  time_union.time_str.month___
#define year__  time_union.time_str.year___


//-----------------------------------------------
void i2c_init_soft(void)
{
if(SDA__<16)
	{
	PINSEL0&=~(1<<(SDA__*2))&~(1<<((SDA__*2)+1));
	}
else 	
	{
	PINSEL1&=~(1<<((SDA__-16)*2))&~(1<<(((SDA__-16)*2)+1));
	}

if(SCL__<16)
	{
	PINSEL0&=~(1<<(SCL__*2))&~(1<<((SCL__*2)+1));
	}
else 	
	{
	PINSEL1&=~(1<<((SCL__-16)*2))&~(1<<(((SCL__-16)*2)+1));
	}
	
IO0DIR&=~((1UL<<SDA__)||(1UL<<SCL__));	
IO0CLR=(1UL<<SDA__)||(1UL<<SCL__);
}

//-----------------------------------------------
void i2c_start_soft(void)
{

IO0CLR=(1UL<<SDA__)||(1UL<<SCL__);	
IO0DIR|=(1UL<<SDA__);
IO0DIR|=(1UL<<SDA__);
IO0DIR|=(1UL<<SCL__);
IO0DIR|=(1UL<<SCL__);

}

//-----------------------------------------------
void i2c_stop_soft(void)
{
IO0CLR=(1UL<<SDA__)||(1UL<<SCL__);	
IO0DIR&=~(1UL<<SCL__);
IO0DIR&=~(1UL<<SCL__);
IO0DIR&=~(1UL<<SDA__);
IO0DIR&=~(1UL<<SDA__);
}


//-----------------------------------------------
char* pcf8563_read_bytes_soft(char adr,char nums)
{
//long temp;
i2c_init_soft();
i2c_start_soft();
i2c_stop_soft();

}

//-----------------------------------------------
char* pcf8563_read(char nums)
{
char temp;
	
i2c_Start();
i2c_SendAddress(0xA2,0);
i2c_ReadAcknowledge();
i2c_SendByte(0x02);
i2c_ReadAcknowledge();

i2c_Restart();
i2c_SendAddress(0xA3,1);
i2c_ReadAcknowledge();

sec_bcd=i2c_ReadByte();
temp=(((sec_bcd&0x70)>>4)*10)+(sec_bcd&0x0f);
i2c_SendAcknowledge(1);
gran_char(&temp,0,59);
sec__=temp;

min_bcd=i2c_ReadByte();
temp=(((min_bcd&0x70)>>4)*10)+(min_bcd&0x0f);
i2c_SendAcknowledge(1);
gran_char(&temp,0,59);
min__=temp;

hour_bcd=i2c_ReadByte();
temp=(((hour_bcd&0x30)>>4)*10)+(hour_bcd&0x0f);
i2c_SendAcknowledge(1);
gran_char(&temp,0,23);
hour__=temp;

day_bcd=i2c_ReadByte();
temp=(((day_bcd&0x30)>>4)*10)+(day_bcd&0x0f);
i2c_SendAcknowledge(1);
gran_char(&temp,0,31);
day__=temp;

i2c_ReadByte();
i2c_SendAcknowledge(1);

month_bcd=i2c_ReadByte();
temp=(((month_bcd&0x10)>>4)*10)+(month_bcd&0x0f);
i2c_SendAcknowledge(1);
gran_char(&temp,0,12);
month__=temp;

year_bcd=i2c_ReadByte();
temp=(((year_bcd&0xf0)>>4)*10)+(year_bcd&0x0f);
gran_char(&temp,0,99);
i2c_SendAcknowledge(0);
year__=temp;

i2c_Stop();
}

//-----------------------------------------------
char* pcf8563_write(char adr, char in)
{

__disable_irq();		
i2c_Start();
i2c_SendAddress(0xA2,0);
i2c_ReadAcknowledge();
i2c_SendByte(adr);
i2c_ReadAcknowledge();
i2c_SendByte(in);
i2c_ReadAcknowledge();
i2c_Stop();
__enable_irq();

}

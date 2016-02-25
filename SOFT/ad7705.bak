#include "ad7705.h"
#include "25lc640.h"
#include <LPC21xx.H>
#include "main.h"

unsigned short ad7705_res1,ad7705_res2;
unsigned short ad7705_buff[2][16],ad7705_buff_[2];
unsigned short ad7705_res;
char b7705ch,ad7705_wrk_cnt;
unsigned short cnt_ad7705_vis,cnt_ad7705_vis_wrk;

//-----------------------------------------------
void ad7705_reset(void)
{
//short i;
IO1DIR|=(1UL<<22);
IO1CLR|=(1UL<<22);
delay_ms(10);
IO1SET|=(1UL<<22);
}

//----------------------------------------------- 
//настройка SPI1
void spi1_ad7705_config(void)
{
SET_REG( PINSEL1, 2, 1*2, 2);
SET_REG( PINSEL1, 2, 2*2, 2);
SET_REG( PINSEL1, 2, 3*2, 2);
SET_REG( PINSEL1, 2, 4*2, 2); 

S1SPCCR=10;
S1SPCR=0x3f;
}


//-----------------------------------------------
void ad7705_write(char in)
{
char i;
IO0DIR|=(1UL<<10);
IO0SET|=(1UL<<10);
spi1_ad7705_config();
for(i=0;i<5;i++)IO0CLR|=(1UL<<10);
spi1(in);
for(i=0;i<5;i++)IO0CLR|=(1UL<<10);
for(i=0;i<5;i++)IO0SET|=(1UL<<10);
spi1_unconfig();                   
}


//-----------------------------------------------
void ad7705_read(char num)
{
//char temp;
char i;
 
IO0DIR|=(1UL<<10);

IO0SET|=(1UL<<10);
spi1_ad7705_config();

for(i=0;i<5;i++)IO0CLR|=(1UL<<10);

ad7705_res=0;
if(num==1) 
	{
	ad7705_res=spi1(0);
	}
else if(num==2)
	{
	*(((char*)(&ad7705_res))+1)=spi1(0); 
	*(((char*)(&ad7705_res)))=spi1(0);
	}	   

for(i=0;i<5;i++)IO0CLR|=(1UL<<10);

for(i=0;i<5;i++)IO0SET|=(1UL<<10);

spi1_unconfig();                                            
}

//-----------------------------------------------
void ad7705_drv(void)
{
__disable_irq();

IO0DIR|=(1UL<<10);

	{
	ad7705_write(0x38);
	ad7705_read(2);
	
	if(!b7705ch)
		{
		ad7705_buff[0][ad7705_wrk_cnt]=ad7705_res;
		ad7705_res1=ad7705_res;
		}
	else if(b7705ch) 
		{
		ad7705_buff[1][ad7705_wrk_cnt]=ad7705_res;
		ad7705_res2=ad7705_res;
		}
 
	if(b7705ch)
		{
		b7705ch=0;
		ad7705_wrk_cnt++;
		if(ad7705_wrk_cnt>=16)ad7705_wrk_cnt=0;
		
		if((ad7705_wrk_cnt&0x01)==0)
			{
			unsigned temp_U;
			char i,ii;

			for(i=0;i<2;i++)
				{
				temp_U=0;
				for(ii=0;ii<16;ii++)
					{
					temp_U+=(unsigned long)ad7705_buff[i][ii];
					}
				ad7705_buff_[i]=(unsigned short)(temp_U>>4);
				}	
			}
		}	
		
		
		
	else b7705ch=1;  
	
	if(!b7705ch) ad7705_write(0x20);
	else if(b7705ch) ad7705_write(0x21);

	ad7705_write(BIN8(1101)); 

	if(!b7705ch) ad7705_write(0x10);
	else if(b7705ch) ad7705_write(0x11);

	ad7705_write(0x44);	
	}

	__enable_irq();

	
}

 


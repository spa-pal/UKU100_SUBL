#include "lcd_AGM1232_uku100.h"
#include <LPC21xx.H>
#include "main.h"



//-----------------------------------------------
void lcd1_chk(void)
{
IO0DIR&=(~((1<<D0)|(1<<D1)|(1<<D2)|(1<<D3)|(1<<D4)|(1<<D5)|(1<<D6)|(1<<D7)));
IO0SET|=(1<<RW);

__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();

IO1CLR|=(1<<A0);

__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();

IO0SET|=(1<<E1);

chk1:

if(IO0PIN&(1<<D7)) goto chk1;

}

//-----------------------------------------------
void lcd2_chk(void)
{
IO0DIR&=(~((1<<D0)|(1<<D1)|(1<<D2)|(1<<D3)|(1<<D4)|(1<<D5)|(1<<D6)|(1<<D7)));
IO0SET|=(1<<RW);

__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();

IO1CLR|=(1<<A0);

__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();

IO1SET|=(1<<E2);

chk2:

if(IO0PIN&(1<<D7)) goto chk2;

}

//-----------------------------------------------
void lcd1_wr(char in)
{

IO0DIR|=((1UL<<E1)|(1UL<<RW));
IO1DIR|=(1UL<<A0);

lcd1_chk();

__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();

IO1CLR|=(1UL<<A0);

__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();

IO0CLR|=(1UL<<RW);

__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();

IO0DIR|=((1<<D0)|(1<<D1)|(1<<D2)|(1<<D3)|(1<<D4)|(1<<D5)|(1<<D6)|(1<<D7));
IO0SET|=(((long)in)<<D0);
IO0CLR|=(((long)(~in))<<D0)&(0xffUL<<D0);

__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();

IO0CLR|=(1<<E1);

__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();

}



//-----------------------------------------------
void lcd2_wr(char in)
{

IO1DIR|=((1<<A0)|(1<<E2));
IO0DIR|=(1<<RW);


lcd2_chk();
     
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();

IO1CLR|=(1<<A0);

__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();

IO0CLR|=(1<<RW);

__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();

IO0DIR|=((1<<D0)|(1<<D1)|(1<<D2)|(1<<D3)|(1<<D4)|(1<<D5)|(1<<D6)|(1<<D7));
IO0SET|=(((long)in)<<D0);
IO0CLR|=(((long)(~in))<<D0)&(0xffUL<<D0);

__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();

IO1CLR|=(1<<E2);

__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();

}

//-----------------------------------------------
char data1_wr(char in)
{
//__disable_irq();		    
IO0DIR|=((1<<RW)|(1<<E1));
IO1DIR|=(1<<A0);

lcd1_chk();

__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();

IO1SET|=(1<<A0);

__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();

IO0CLR|=(1<<RW);

__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();

IO0DIR|=((1<<D0)|(1<<D1)|(1<<D2)|(1<<D3)|(1<<D4)|(1<<D5)|(1<<D6)|(1<<D7));
IO0SET|=(((long)in)<<D0);
IO0CLR|=(((long)(~in))<<D0)&(0xffUL<<D0);

__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();

IO0CLR|=(1<<E1);

__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();

return in;
}

//-----------------------------------------------
void data2_wr(char in)
{
IO1DIR|=((1<<A0)|(1<<E2));
IO0DIR|=(1<<RW);

lcd2_chk();

__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();

IO1SET|=(1<<A0);

__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();

IO0CLR|=(1<<RW);

__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();

IO0DIR|=((1<<D0)|(1<<D1)|(1<<D2)|(1<<D3)|(1<<D4)|(1<<D5)|(1<<D6)|(1<<D7));
IO0SET|=(((long)in)<<D0);
IO0CLR|=(((long)(~in))<<D0)&(0xffUL<<D0);

__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();

IO1CLR|=(1<<E2);

__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();
__nop();

}

//-----------------------------------------------
void lcd_set_page(char in)
{
lcd1_wr(_SET_PAGE_|in);
lcd2_wr(_SET_PAGE_|in);
}

//-----------------------------------------------
void lcd_set_col(char in)
{
lcd1_wr(_SET_COLUMN_|in);
lcd2_wr(_SET_COLUMN_|in);
}    

//-----------------------------------------------
void lcd_set_raw(char in)
{
lcd1_wr(_SET_RAW_|in);
lcd2_wr(_SET_RAW_|in);
}    

//-----------------------------------------------
void lcd_init(void) 
{
int i/*,ii*/;




#if(RES<16)
	{
	PINSEL0&=~(1<<(RES*2))&~(1<<((RES*2)+1));
	}
#else 	
	{
	PINSEL1&=~(1<<((RES-16)*2))&~(1<<(((RES-16)*2)+1));
	}
#endif

#if(E1<16)
PINSEL0&=~(1<<((E1)*2))&~(1<<(((E1)*2)+1));
#else
PINSEL1&=~(1<<((E1-16)*2))&~(1<<(((E1-16)*2)+1));
#endif
	
PINSEL2&=~(1<<((E2-16)*2))&~(1<<(((E2-16)*2)+1));

/*#if(A0<16)
PINSEL0&=~(1<<((A0)*2))&~(1<<(((A0)*2)+1));
#else
PINSEL1&=~(1<<((A0-16)*2))&~(1<<(((A0-16)*2)+1));
#endif*/

PINSEL2&=~(1<<((A0-16)*2))&~(1<<(((A0-16)*2)+1));

//PINSEL2&=~(1<<((RW-16)*2))&~(1<<(((RW-16)*2)+1));
				
#if(RW<16)
PINSEL0&=~(1<<((RW)*2))&~(1<<(((RW)*2)+1));
#else
PINSEL1&=~(1<<((RW-16)*2))&~(1<<(((RW-16)*2)+1));
#endif




#if(D0<16)
	{
	PINSEL0&=~(1<<(D0*2))&~(1<<((D0*2)+1));
	}
#else 	
	{
	PINSEL1&=~(1<<((D0-16)*2))&~(1<<(((D0-16)*2)+1));
	}
#endif

#if(D1<16)
	{
	PINSEL0&=~(1<<(D1*2))&~(1<<((D1*2)+1));
	}
#else 	
	{
	PINSEL1&=~(1<<((D1-16)*2))&~(1<<(((D1-16)*2)+1));
	}
#endif
	
#if(D2<16)
	{
	PINSEL0&=~(1<<(D2*2))&~(1<<((D2*2)+1));
	}
#else 	
	{
	PINSEL1&=~(1<<((D2-16)*2))&~(1<<(((D2-16)*2)+1));
	}
#endif
	
#if(D3<16)
	{
	PINSEL0&=~(1<<(D3*2))&~(1<<((D3*2)+1));
	}
#else 	
	{
	PINSEL1&=~(1<<((D3-16)*2))&~(1<<(((D3-16)*2)+1));
	}	
#endif
	
#if(D4<16)
	{
	PINSEL0&=~(1<<(D4*2))&~(1<<((D4*2)+1));
	}
#else 	
	{
	PINSEL1&=~(1<<((D4-16)*2))&~(1<<(((D4-16)*2)+1));
	}
#endif

#if(D5<16)
	{
	PINSEL0&=~(1<<(D5*2))&~(1<<((D5*2)+1));
	}
#else 	
	{
	PINSEL1&=~(1<<((D5-16)*2))&~(1<<(((D5-16)*2)+1));
	}
#endif
	
#if(D6<16)
	{
	PINSEL0&=~(1<<(D6*2))&~(1<<((D6*2)+1));
	}
#else 	
	{
	PINSEL1&=~(1<<((D6-16)*2))&~(1<<(((D6-16)*2)+1));
	}
#endif
	
#if(D7<16)
	{
	PINSEL0&=~(1<<(D7*2))&~(1<<((D7*2)+1));
	}
#else 	
	{
	PINSEL1&=~(1<<((D7-16)*2))&~(1<<(((D7-16)*2)+1));
	}
#endif	


IO0DIR|=(1<<D0)|(1<<D1)|(1<<D2)|(1<<D3)|(1<<D4)|(1<<D5)|(1<<D6)|(1<<D7)|(1<<RES);
IO1DIR|=(1<<E2)|(1<<A0);
IO0DIR|=(1<<E1)|(1<<RW);

IO0CLR|=(1<<D0)|(1<<D1)|(1<<D2)|(1<<D3)|(1<<D4)|(1<<D5)|(1<<D6)|(1<<D7)|(1<<RES);
IO1CLR|=(1<<E2)|(1<<A0);
IO0CLR|=(1<<E1)|(1<<RW);


IO1DIR|=(1<<22);
IO1CLR|=(1<<22);

delay_ms(10);


IO1DIR|=(1<<22);
IO1SET|=(1<<22);


for(i=0;i<166000;i++)
	{
	__nop();
	}

	
					
IO0DIR|=(1<<D0)|(1<<D1)|(1<<D2)|(1<<D3)|(1<<D4)|(1<<D5)|(1<<D6)|(1<<D7)|(1<<RES);
IO1DIR|=(1<<E2)|(1<<A0);
IO0DIR|=(1<<E1)|(1<<RW);

for(i=0;i<100;i++)
	{
	IO0CLR|=(1<<RES);
	}
for(i=0;i<166000;i++)
	{
	__nop();
	}
for(i=0;i<50;i++)
	{
	IO0SET|=(1<<RES);
	}	

 for(i=0;i<166000;i++)
	{
	__nop();
	}
}

//-----------------------------------------------
void status(void)
{

IO1CLR=(1<<A0);
IO0SET=(1<<RW);
IO0SET=(1<<E1);
__nop();
stat1: 
__nop();
__nop();
__nop();

if(IO0PIN&(1<<D7)) goto stat1;
IO0CLR=(1<<E1);

}

//-----------------------------------------------
void delay(void)
{
signed short i;

for(i=0;i<100;i++)
	{
	__nop();
	}
}

//-----------------------------------------------
void ltstrobe(char in)
{
status();
IO0SET=(1<<RW);
IO1CLR=(1<<A0);
IO0SET=(1<<E1);
IO1SET=(1<<E2);
IO0SET|=(((long)in)<<D0);
IO0CLR|=(((long)(~in))<<D0)&(0xffUL<<D0);
IO0CLR=(1<<E1);
IO1CLR=(1<<E2);
IO0CLR=(1<<RW);
}



//-----------------------------------------------
void lcd_clear(void)
{
char page,col/*,i*/;

for(page=0;page<=Max_page;page++)

	{
	lcd_set_page(page);
	lcd_set_col(0);
	for(col=0;col<=Max_Col;col++)
         	{
         	data1_wr(0x00);
	    	data2_wr(0x00);
     	}
     }  
}





//-----------------------------------------------
void lcd_on(void) 
{
lcd1_wr(_DISPLAY_ON_);
lcd2_wr(_DISPLAY_ON_);
}

//-----------------------------------------------
void lcd_off(void)
{
lcd1_wr(_DISPLAY_OFF_);
lcd2_wr(_DISPLAY_OFF_);
}

//-----------------------------------------------
void lcd_out(char* adr)
{
char* ptr0;
char* ptr1;
char i,n;
SET_REG(PINSEL0,0,(6*2),2);

ptr0=adr;
ptr1=adr+61;
lcd_set_raw(0);

for(n=0;n<=3;n++)
	{
	ptr0=adr+(122*(unsigned)n);
	ptr1=ptr0+61;
	lcd_set_page(n);
	lcd_set_col(0);
	for(i=0;i<61;i++)
		{
		data1_wr(*ptr0);
		data2_wr(*ptr1);
		ptr0++;
		ptr1++;
	    	}
	}
}


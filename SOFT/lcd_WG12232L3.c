#include "lcd_AGM1232_uku100.h"
#include <LPC21xx.H>
#include "main.h"

char lcd_bitmap2[512];

const char bit_mask_const[8]={0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

//-----------------------------------------------
void bitmap_hndl_2(void)
{
char row, row_;
signed char col, col_;
short ptr,ptr_;

char i;
short ii;



bitmap_hndl();


for(row=0;row<4;row++)	 //по 4 текстовым строкам
	{
	for(col=0;col<16;col++)	 //по 16 8-битным столбикам
		{
		for(row_=0;row_<8;row_++)	 //по каждому из 8 пикселей в строке
			{
			ptr_=((short)row*128)+((short)row_*16)+((short)col);
	
			lcd_bitmap2[ptr_]=0;//0x0f+row_;//row+16;

		   	ptr=((short)row*122)+((short)col*8);

			if((row==1)&&(col==0))
				{
/*				bh2_plazma[10]=row;
				bh2_plazma[11]=col;
				bh2_plazma[12]=(char)ptr;
				bh2_plazma[13]=(char)(ptr>>8);
				bh2_plazma[5]=lcd_bitmap[128+0];
				bh2_plazma[6]=lcd_bitmap[128+1];
				bh2_plazma[7]=lcd_bitmap[128+2];
				bh2_plazma[8]=lcd_bitmap[128+3];
				bh2_plazma[9]=lcd_bitmap[128+4];  */
				}
			if(col==15)
				{
				for(col_=7;col_>=5;col_--)	 //по каждому из 8 пикселей в этом столбике
					{
					//ptr=((short)row*128)+((short)col*8)+((short)col_);
					//if(ptr_==384) lcd_bitmap2[ptr_]|=bit_mask_const[col_];
					if(lcd_bitmap[ptr]&0x01) lcd_bitmap2[ptr_]|=bit_mask_const[col_];
					lcd_bitmap[ptr++]>>=1;
					}
				}
			else 
				{
				for(col_=7;col_>=0;col_--)	 //по каждому из 8 пикселей в этом столбике
					{
					//ptr=((short)row*128)+((short)col*8)+((short)col_);
					//if(ptr_==384) lcd_bitmap2[ptr_]|=bit_mask_const[col_];
					if(lcd_bitmap[ptr]&0x01) lcd_bitmap2[ptr_]|=bit_mask_const[col_];
					lcd_bitmap[ptr++]>>=1;
					}
				}
			}
		}
	}

/*for(ii=0;ii<128;ii++)
	{ 
	lcd_bitmap2[ii]=0;
	}

lcd_bitmap2[5]=0x01;
lcd_bitmap2[7]=0x05;
lcd_bitmap2[9]=0x15;
lcd_bitmap2[11]=0x55;
lcd_bitmap2[12]=0x01;
lcd_bitmap2[13]=0x55;

lcd_bitmap2[5+32]=0x55;
lcd_bitmap2[7+32]=0x55;
lcd_bitmap2[9+32]=0x55;
lcd_bitmap2[11+32]=0x55;
lcd_bitmap2[13+32]=0x55;*/


/*
lcd_bitmap2[5+64]=bh2_plazma[0];
lcd_bitmap2[7+64]=bh2_plazma[1];
lcd_bitmap2[9+64]=bh2_plazma[2];
lcd_bitmap2[11+64]=bh2_plazma[3];
lcd_bitmap2[13+64]=bh2_plazma[4];
lcd_bitmap2[5+96]=bh2_plazma[5];
lcd_bitmap2[7+96]=bh2_plazma[6];
lcd_bitmap2[9+96]=bh2_plazma[7];
lcd_bitmap2[11+96]=bh2_plazma[8];
lcd_bitmap2[13+96]=bh2_plazma[9];
lcd_bitmap2[5+112]=bh2_plazma[10];
lcd_bitmap2[7+112]=bh2_plazma[11];
lcd_bitmap2[9+112]=bh2_plazma[12];
lcd_bitmap2[11+112]=bh2_plazma[13];
lcd_bitmap2[13+112]=bh2_plazma[14];	*/
//lcd_bitmap2[0]|=0xff;
//lcd_bitmap2[128]|=0xff;
//lcd_bitmap2[256]|=0xff;
//lcd_bitmap2[384]|=0xff;
//LPC_GPIO0->FIOCLR|=(1<<8);
}


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
//lcd2_wr(_SET_PAGE_|in);
}

//-----------------------------------------------
void lcd_set_col(char in)
{
lcd1_wr(_SET_COLUMN_|in);
//lcd2_wr(_SET_COLUMN_|in);
}    

//-----------------------------------------------
void lcd_set_raw(char in)
{
lcd1_wr(_SET_RAW_|in);
//lcd2_wr(_SET_RAW_|in);
}    

//-----------------------------------------------
void lcd_init(void) 
{
int i/*,ii*/;


delay_ms(100);




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

/*
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

*/	
					
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

lcd1_wr(0x3f);
delay_us(150);
//strob_us(10);	
/*
for(i=0;i<166000;i++)
	{
	__nop();
	}*/

lcd1_wr(0x3f);

delay_us(50);
//strob_us(10);
/*
for(i=0;i<166000;i++)
	{
	__nop();
	}	*/

lcd1_wr(0x0c);

delay_us(150);
//strob_us(10);

//for(i=0;i<166000;i++)
/*	{
	__nop();
	}*/



/*
for(i=0;i<166000;i++)
	{
	__nop();
	}*/

/*lcd1_wr(0x01);

for(i=0;i<166000;i++)
	{
	__nop();
	} */

/*lcd1_wr(0x04);

for(i=0;i<166000;i++)
	{
	__nop();
	}*/
//lcd_reset();
//E1d=1;
//E2d=1;
//A0d=1;
//RWd=1;

//E1=0;
//E2=0;
/*IO1CLR|=(1<<E1);
IO1CLR|=(1<<E2);
//A0=1;
IO1SET|=(1<<A0);
//RW=1;
IO1SET|=(1<<RW);

lcd1_wr(_RESET_);
lcd2_wr(_RESET_);*/
     //LCD_PORTc=0xFF;
     //LCD_PORT=0xFF;
//disp_page0(); 

//IO0CLR|=(1<<A0)|(1<<RW);
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
char row,col,i;

for(row=0;row<32;row++)
	{
	lcd1_wr(0x80+row);
	lcd1_wr(0x80);

/*for(page=0;page<=Max_page;page++)

	{
	lcd_set_page(page);
	lcd_set_col(0);
	for(col=0;col<=Max_Col;col++)
         	{
         	data1_wr(0x01);
	    	//data2_wr(0x00);
     	}
     }  */

for (i=0;i<8;i++)
	{
	data1_wr(0x00);
	data1_wr(0x00);
	}
}
/*
data1_wr(0x01);
data1_wr(0x00);
data1_wr(0x00);
data1_wr(0x00);
data1_wr(0x00);
data1_wr(0x05);
data1_wr(0x15);*/
}





//-----------------------------------------------
void lcd_on(void) 
{
lcd1_wr(_DISPLAY_ON_);
//lcd2_wr(_DISPLAY_ON_);
}

//-----------------------------------------------
void lcd_off(void)
{
lcd1_wr(_DISPLAY_OFF_);
//lcd2_wr(_DISPLAY_OFF_);
}

//-----------------------------------------------
void lcd_out(char* adr)
{
static char mem;
short lcd_ptr;

char d1,d2;
char row,col,i;
//strob_us(1);

bitmap_hndl_2();

if(mem)
	{
	mem=0;
	d1=0x55;
	d2=0xff;
	}
else 
	{
	mem=1;
	d2=0x55;
	d1=0xff;
	} 

lcd_ptr=0;

for(row=0;row<32;row++)
	{
	//strob_us(10);
	lcd1_wr(0x80+row);
	//strob_us(10);
	lcd1_wr(0x80);
   	//strob_us(10);
/*for(page=0;page<=Max_page;page++)

	{
	lcd_set_page(page);
	lcd_set_col(0);
	for(col=0;col<=Max_Col;col++)
         	{
         	data1_wr(0x01);
	    	//data2_wr(0x00);
     	}
     }  */

	/*for (i=0;i<8;i++)
		{
		if(i==0) strob_us(10);
		data1_wr(d1);
		if(i==0) strob_us(10);
		data1_wr(d2);
		} */

	for (i=0;i<16;i++)
		{
		//if(i==0) strob_us(1);
		data1_wr(lcd_bitmap2[lcd_ptr++]);
		//data1_wr(0x01);
		}

	}

}


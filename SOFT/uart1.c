#include "uart1.h"
#include <LPC21XX.H> 
#include "main.h"
#include "25lc640.h"
#include "eeprom_map.h"
#include "control.h"
#include "gran.h"
#include "common_func.h"


char bRXIN1;
char UIB1[20];
char flag1;
char rx_buffer1[RX_BUFFER_SIZE1];
char tx_buffer1[TX_BUFFER_SIZE1];
unsigned char rx_wr_index1,rx_rd_index1,rx_counter1;
unsigned char tx_wr_index1,tx_rd_index1,tx_counter1;
char rx_buffer_overflow1;
char plazma_uart1;
char uart1_mess[10];
char data_rs1[40];
enum_usart1_router_stat usart1_router_stat;
char usart1_router_wrk;
char memo_out0[100];
char suzz[4];
char UIB10[30];
char usart1_router_cnt;
char _485_last_cnt;
char UIB[30];
extern char plazma;

char tx_wd_cnt=100;
unsigned long BAUD_RATE_1;
char rxrxrx, txtxtx;

//-----------------------------------------------
void putchar1(char c)
{
while (tx_counter1 == TX_BUFFER_SIZE1);
if (tx_counter1 || ((U1LSR & 0x60)==0))
   {
   tx_buffer1[tx_wr_index1]=c;
   if (++tx_wr_index1 == TX_BUFFER_SIZE1) tx_wr_index1=0;
   ++tx_counter1;
   }
else 
	{


	U1THR=c;
	tx_wd_cnt=100;
	}
}

//-----------------------------------------------
void uart_out1 (char num,char data0,char data1,char data2,char data3,char data4,char data5)
{
char i,t=0;
//char *ptr=&data1;
char UOB1[16]; 
UOB1[0]=data0;
UOB1[1]=data1;
UOB1[2]=data2;
UOB1[3]=data3;
UOB1[4]=data4;
UOB1[5]=data5;

for (i=0;i<num;i++)
	{
	t^=UOB1[i];
	}    
UOB1[num]=num;
t^=UOB1[num];
UOB1[num+1]=t;
//UOB1[num+2]=END;

for (i=0;i<num+3;i++)
	{
	putchar1(UOB1[i]);
	}   	
}

//-----------------------------------------------
void uart_out_adr1 (char *ptr, char len)
{
char UOB[110]/*={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}*/;
char i,t=0;

for(i=0;i<len;i++)
	{
	UOB[i]=ptr[i];
	}

//puts(UOB);

SET_REG(PINSEL1,0,(22-16)*2,2); //���� EN � 485
IO0DIR|=(1<<22);
IO0SET|=(1<<22); 
for (i=0;i<len;i++)
	{//txtxtx++;	
	putchar1(UOB[i]);
	}   
}
//-----------------------------------------------
void uart1_init(void)
{

BAUD_RATE_1=(unsigned long)(BAUDRATE)*100UL;


SET_REG(PINSEL0,1,8*2,2);
SET_REG(PINSEL0,1,9*2,2);

SET_REG(U1LCR,1,7/*DLAB*/,1);//U0LCR_bit.DLAB=1;
U1DLL=60000000UL/(BAUD_RATE_1*16UL);
U1DLM=60000000UL/(BAUD_RATE_1*16UL*256UL);
SET_REG(U1LCR,0,7/*DLAB*/,1);//U0LCR_bit.DLAB=0;
U1LCR=0x03;
U1FCR=0;

VICProtection = 0;
VICIntEnClr |= (1 << VIC_UART1); 
VICIntSelect &= ~(1 << VIC_UART1);
VICVectAddr3=(unsigned)uart1_interrupt;
VICVectCntl3 = 0x20 | VIC_UART1;
VICIntEnable |= (1  << VIC_UART1);

U1IER=0x03;

}


//-----------------------------------------------
char getchar1(void)
{
char data;
while (rx_counter1==0);
data=rx_buffer1[rx_rd_index1];
if (++rx_rd_index1 == RX_BUFFER_SIZE1) rx_rd_index1=0;
--rx_counter1;
return data;
}

//***********************************************
__irq void uart1_interrupt(void)
{
char status,u1iir,data;

//plazma++;

status=U1LSR;
u1iir=U1IIR;
   	
if((u1iir&0x0f)==4)
	{
	rxrxrx++;
	data=U1RBR;
	rx_buffer1[rx_wr_index1]=data;
   	//bRXIN1=1;
  	if (++rx_wr_index1 == RX_BUFFER_SIZE1) rx_wr_index1=0;
   	if (++rx_counter1 == RX_BUFFER_SIZE1)
      	{
      	rx_counter1=0;
      	rx_buffer_overflow1=1;
      	}

	if(data==0x0d)_485_last_cnt=5;
	else _485_last_cnt=0;
     
	
   	}
else if((u1iir&0x0f)==2)
	{
	txtxtx++;
	if (tx_counter1)
   		{
   		//usart1_router_wrk=1;
   		--tx_counter1;
   		U1THR=tx_buffer1[tx_rd_index1];
   		if (++tx_rd_index1 == TX_BUFFER_SIZE1) tx_rd_index1=0;


		tx_wd_cnt=100;
   		}
   	else 
		{
		SET_REG(PINSEL1,0,(22-16)*2,1); //���� PV � 485
		IO0DIR|=(1<<22);
		IO0CLR|=(1<<22);
		}
			
	} 
VICVectAddr = 0;  	
}
 

//-----------------------------------------------
void uart_in_an1(void)
{
char i;

char adrh,adrl;



adrl=ABCDEF[ADR%16];
adrh=ABCDEF[ADR/16];

//plazma=UIB1[1];
//

if((UIB1[0]=='Z'))
	{
	//uart_out1(3,1,2,3,4,5,6);/
	if((UIB1[1]==adrh)&&(UIB1[2]==adrl))
		{
	 	if((UIB1[3]=='N')&&(UIB1[5]==0x0d))
			{
			if(UIB1[4]=='N')
				{
				char i;
				plazma++;
				memo_out0[0]='!';
				memo_out0[1]=adrh;
				memo_out0[2]=adrl;
				memo_out0[3]='N';
				memo_out0[4]='>';
				//memo_out0[5]='>';
				i=0;
				while(name_of_blok[i])
					{
					memo_out0[5+i]=name_of_blok[i];
					i++;
					}
				memo_out0[5+i]=0x0d;
				uart_out_adr1(memo_out0,6+i);
				//uart_out1(3,1,2,3,4,5,6);
				}
			}

	 	else if((UIB1[3]=='4')/*&&(UIB1[5]==0x0d)*/)
			{
			signed short tempSS;
			char temp;
			//memo_out0[5]=0x0d;
			//uart_out_adr1(memo_out0,6);
			if(UIB1[4]=='1')    //������� ���������� � ���
				{
				char i;
				plazma++;
				memo_out0[0]='!';
				memo_out0[1]=adrh;
				memo_out0[2]=adrl;
				memo_out0[3]='4';
				memo_out0[4]='>';
				memo_out0[5]='1';
				//memo_out0[5]='>';
			/*	i=0;
				while(name_of_blok[i])
					{
					memo_out0[5+i]=name_of_blok[i];
					i++;
					}
				memo_out0[5+i]=0x0d;*/
				memo_out0[6]='+';
				memo_out0[7]='0'+ (U/10000);
				tempSS=U%10000;

				memo_out0[8]='0'+ (tempSS/1000);
				tempSS=tempSS%1000;

				memo_out0[9]='0'+ (tempSS/100);
				tempSS=tempSS%100;

				memo_out0[10]='0'+ (tempSS/10);
				tempSS=tempSS%10;

				memo_out0[11]='0'+ tempSS;
				
				tempSS=Ires/100;


				memo_out0[12]='+';

				memo_out0[13]='0'+ (tempSS/10000);
				tempSS=tempSS%10000;

				memo_out0[14]='0'+ (tempSS/1000);
				tempSS=tempSS%1000;

				memo_out0[15]='0'+ (tempSS/100);
				tempSS=tempSS%100;
				
				memo_out0[16]=',';
				
				memo_out0[17]='0'+ (tempSS/10);
				tempSS=tempSS%10;
				
				memo_out0[18]='0'+ tempSS;
				
				memo_out0[19]=0x0d;

				uart_out_adr1(memo_out0,20);
				//uart_out1(3,1,2,3,4,5,6);
				}

/*			else if(UIB1[4]=='2') //���������� ���������� � ��������� ����������
				{
				char i;
				memo_out0[0]='!';
				memo_out0[1]=adrh;
				memo_out0[2]=adrl;
				memo_out0[3]='4';
				memo_out0[4]='>';
				memo_out0[5]='2';

				memo_out0[6]='+';
				memo_out0[7]='0'+ (U1/10000);
				tempSS=U1%10000;

				memo_out0[8]='0'+ (tempSS/1000);
				tempSS=tempSS%1000;

				memo_out0[9]='0'+ (tempSS/100);
				tempSS=tempSS%100;

				memo_out0[10]='0'+ (tempSS/10);
				tempSS=tempSS%10;

				memo_out0[11]='0'+ tempSS;
				

				memo_out0[12]='+';
				memo_out0[13]='0'+ (U2/10000);
				tempSS=U2%10000;

				memo_out0[14]='0'+ (tempSS/1000);
				tempSS=tempSS%1000;

				memo_out0[15]='0'+ (tempSS/100);
				tempSS=tempSS%100;

				memo_out0[16]='0'+ (tempSS/10);
				tempSS=tempSS%10;

				memo_out0[17]='0'+ tempSS;

			
				memo_out0[18]=0x0d;

				uart_out_adr1(memo_out0,19);

				}*/

/*			else if(UIB1[4]=='3') //���������� ������������ ��������
				{
				char i;
				plazma++;
				memo_out0[0]='!';
				memo_out0[1]=adrh;
				memo_out0[2]=adrl;
				memo_out0[3]='4';
				memo_out0[4]='>';
				memo_out0[5]='3';

				memo_out0[6]='+';
				memo_out0[7]='0'+ (T/100);
				tempSS=T%100;

				memo_out0[8]='0'+ (tempSS/10);
				tempSS=tempSS%10;

				memo_out0[9]='0'+ tempSS;

                    memo_out0[10]='+';
				memo_out0[11]='0';
				
				memo_out0[12]=0x0d;

				uart_out_adr1(memo_out0,13);
				//uart_out1(3,1,2,3,4,5,6);
				}
			 */
			}




	 	else if((UIB1[3]=='1')&&(UIB1[5]==0x0d))    //���������-����������
			{
			if(UIB1[4]=='E')
				{
				char i;

				wrk_state=wrkON;
		///		lc640_write_int(EE_WRK_STAT,wrk_state);
				wrk_cnt=0;

				memo_out0[0]='!';
				memo_out0[1]=adrh;
				memo_out0[2]=adrl;
				memo_out0[3]='1';
				memo_out0[4]=0x0d;
				uart_out_adr1(memo_out0,5);
				
				}

			else if(UIB1[4]=='D')
				{
				char i;

				wrk_state=wrkOFF;
	///			lc640_write_int(EE_WRK_STAT,wrk_state);

				memo_out0[0]='!';
				memo_out0[1]=adrh;
				memo_out0[2]=adrl;
				memo_out0[3]='1';
				memo_out0[4]=0x0d;
				uart_out_adr1(memo_out0,5);
				}

			}

	 	}
	 
	}

/*if((UIB1[0]==0x55)&&(UIB1[1]==0x66))
	{
	
	uart_out1(2,0x57,0x66,0,0,0,0);
	}*/

/*else if((UIB1[0]==CMND)&&(UIB1[1]==1))
	{
	adc_buff_out_[0]=UIB1[2]+(UIB1[3]*256);
	adc_buff_out_[1]=UIB1[4]+(UIB1[5]*256);
	}

else if((UIB1[0]==CMND)&&(UIB1[1]==2))
	{
	adc_buff_out_[2]=UIB1[2]+(UIB1[3]*256);
	in_stat_out[0]=UIB1[4];
	in_stat_out[1]=UIB1[5];
	}
	
else if((UIB1[0]==CMND)&&(UIB1[1]==QWEST)&&(UIB1[2]==PUTTM))
	{
	adc_buff_out_[0]=UIB1[3]+(UIB1[4]*256);
	adc_buff_out_[1]=UIB1[5]+(UIB1[6]*256);
	adc_buff_out_[2]=UIB1[7]+(UIB1[8]*256);
	in_stat_out[0]=UIB1[9];
	in_stat_out[1]=UIB1[10];
	in_stat_out[2]=UIB1[11];
	in_stat_out[3]=UIB1[12];	
	}*/	
		
}
 
//-----------------------------------------------
char index_offset1 (signed char index,signed char offset)
{
index=index+offset;
if(index>=RX_BUFFER_SIZE1) index-=RX_BUFFER_SIZE1; 
if(index<0) index+=RX_BUFFER_SIZE1;
return index;
}

//-----------------------------------------------
char control_check1(char index)
{
char i=0,ii=0,iii;

//if(rx_buffer1[index]!=END) goto error_cc;

ii=rx_buffer1[index_offset1(index,-2)];
iii=0;
for(i=0;i<=ii;i++)
	{
	iii^=rx_buffer1[index_offset1(index,-2-ii+i)];
	}
if (iii!=rx_buffer1[index_offset1(index,-1)]) goto error_cc;	


//success_cc:
return 1;
//goto end_cc;
error_cc:
return 0;
//goto end_cc;

//end_cc:
//__nop();
}




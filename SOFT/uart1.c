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

SET_REG(PINSEL1,0,(22-16)*2,2); //Вход EN у 485
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

plazma_a++;

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
		SET_REG(PINSEL1,0,(22-16)*2,1); //Вход PV у 485
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

	 	else if(UIB1[3]=='1')         //программирование каналов
			{
			signed short tempSS;
			char temp;
			unsigned char *s;
			char answer_code='!';
			
			if(UIB1[4]=='C')	//старт в режиме алгоритма 1    
				{
				if((UIB1[5]=='1')||(UIB1[5]=='2')||(UIB1[5]=='3')||(UIB1[5]=='4')||(UIB1[5]=='5')||(UIB1[5]=='6')||(UIB1[5]=='7')||(UIB1[5]=='8'))	//один из каналов
					{
					chNumber=(short)UIB1[5]-49;

				
					if(!start_chanel_num(chNumber))
						{
						answer_code='D';
						}
					}
				}
			else if(UIB1[4]=='R')	//старт в режиме алгоритма 2    
				{
				if(!((!Konset[0])&&(!Konset[1])&&(!Konset[2])&&(!Konset[3])&&(!Konset[4])&&(!Konset[5])&&(!Konset[6])&&(!Konset[7])))
					{
					start_ALG2();
					}
				else
					{
					answer_code='D';
					}

				}
			else if(UIB1[4]=='S')	//стоп    
				{
				stop_process();
				wrk_pause_time=0;
				}
			else 
				{
				answer_code='C';
				}
			memo_out0[0]=answer_code;
			memo_out0[1]=adrh;
			memo_out0[2]=adrl;
			memo_out0[3]='1';
			memo_out0[4]=0x0d;
			uart_out_adr1(memo_out0,5);	
			}

	 	else if(UIB1[3]=='2')         //установки
			{
			signed short tempSS;
			char temp;
			unsigned char *s;
			char answer_code='!';

 				if((UIB1[4]=='A')&&(UIB1[5]=='L'))    //Алгоритм работы
					{
					strIng_ptr_start=6;
					strIng_ptr_end=pal_strspn(UIB1,'\r');
					strIng_len=strIng_ptr_end-strIng_ptr_start;
 					memcpy(strIng, &UIB1[strIng_ptr_start], strIng_len);
					strIng[strIng_len]=0;
					strIng[strIng_len+1]=0;
 
					strIng_ptr_komma=pal_strspn(strIng,'.');
					
					if(strIng_ptr_komma!=255) 
						{
						answer_code='D';
						} 
					if(answer_code!='D') 
						{
						setData=(signed short)atoi(strIng);
						
						if((setData==1)||(setData==2))
							{
							lc640_write_int(EE_ALG_MODE,setData);
							}
						else 
							{
							answer_code='V';
							}
						}
					}
				else if(UIB1[4]=='Z')    //Задержка переключения каналов 
                	{
					strIng_ptr_start=5;
					strIng_ptr_end=pal_strspn(UIB1,'\r');
					strIng_len=strIng_ptr_end-strIng_ptr_start;
 					memcpy(strIng, &UIB1[strIng_ptr_start], strIng_len);
					strIng[strIng_len]=0;
					strIng[strIng_len+1]=0;
 
					strIng_ptr_komma=pal_strspn(strIng,'.');
					
					if(strIng_ptr_komma!=255) 
						{
						answer_code='D';
						} 
					if(answer_code!='D') 
						{
						setData=(signed short)atoi(strIng);
						
						if((setData>=1)&&(setData<=500))
							{
							lc640_write_int(EE_TZPK,setData);
							}
						else 
							{
							answer_code='V';
							}
						}
                    }
						
				else if(UIB1[4]=='R')    //Количество повторов для алгоритма 2 
                	{
					strIng_ptr_start=5;
					strIng_ptr_end=pal_strspn(UIB1,'\r');
					strIng_len=strIng_ptr_end-strIng_ptr_start;
 					memcpy(strIng, &UIB1[strIng_ptr_start], strIng_len);
					strIng[strIng_len]=0;
					strIng[strIng_len+1]=0;
 
					strIng_ptr_komma=pal_strspn(strIng,'.');
					
					if(strIng_ptr_komma!=255) 
						{
						answer_code='D';
						} 
					if(answer_code!='D') 
						{
						setData=(signed short)atoi(strIng);
						
						if((setData>=1)&&(setData<=99))
							{
							lc640_write_int(EE_REP_NUM_ALG_2,setData);
							}
						else 
							{
							answer_code='V';
							}
						}
                    }	
				else if((UIB1[4]=='A')&&(UIB1[5]=='D'))    //Адрес 
					{
					strIng_ptr_start=6;
					strIng_ptr_end=pal_strspn(UIB1,'\r');
					strIng_len=strIng_ptr_end-strIng_ptr_start;
 					memcpy(strIng, &UIB1[strIng_ptr_start], strIng_len);
					strIng[strIng_len]=0;
					strIng[strIng_len+1]=0;
 
					strIng_ptr_komma=pal_strspn(strIng,'.');
					
					if(strIng_ptr_komma!=255) 
						{
						answer_code='D';
						} 
					if(answer_code!='D') 
						{
						setData=	  (signed short)strtol(strIng, 0, 16);	 // (signed short)atoi(strIng);
						
						if((setData>=16)&&(setData<=80))
							{
							lc640_write_int(EE_ADR,setData);
							}
						else 
							{
							answer_code='V';
							}
						}
					}

				else if((UIB1[4]=='B')&&(UIB1[5]=='R'))    //Адрес 
					{
					int setData_;
					strIng_ptr_start=6;
					strIng_ptr_end=pal_strspn(UIB1,'\r');
					strIng_len=strIng_ptr_end-strIng_ptr_start;
 					memcpy(strIng, &UIB1[strIng_ptr_start], strIng_len);
					strIng[strIng_len]=0;
					strIng[strIng_len+1]=0;
					
 
					strIng_ptr_komma=pal_strspn(strIng,'.');
					
					if(strIng_ptr_komma!=255) 
						{
						answer_code='D';
						} 
					if(answer_code!='D') 
						{
						setData_=(int)atoi(strIng);
						
						if((setData_==9600)||(setData_==19200)||(setData_==38400)||(setData_==57600)||(setData_==115200))
							{
							lc640_write_int(EE_BAUDRATE,(signed short)(setData_/100));
							uart1_init();
							}
						else 
							{
							answer_code='V';
							}
						}
					}
																	
				else 
					{
					answer_code='?';
					}								

			memo_out0[0]=answer_code;
			memo_out0[1]=adrh;
			memo_out0[2]=adrl;
			memo_out0[3]='2';
			memo_out0[4]=0x0d;
			uart_out_adr1(memo_out0,5);	
			}

	 	else if(UIB1[3]=='3')         //программирование каналов
			{
			signed short tempSS;
			char temp;
			unsigned char *s;
			char answer_code='!';

			if((UIB1[4]=='1')||(UIB1[4]=='2')||(UIB1[4]=='3')||(UIB1[4]=='4')||(UIB1[4]=='5')||(UIB1[4]=='6')||(UIB1[4]=='7')||(UIB1[4]=='8'))	//один из каналов
				{
				chNumber=(short)UIB1[4]-49;

				if(UIB1[5]=='I')    //Установка тока 
					{
					strIng_ptr_start=6;
					strIng_ptr_end=pal_strspn(UIB1,'\r');
					strIng_len=strIng_ptr_end-strIng_ptr_start;
 					memcpy(strIng, &UIB1[strIng_ptr_start], strIng_len);
					strIng[strIng_len]=0;
					strIng[strIng_len+1]=0;
 
					strIng_ptr_komma=pal_strspn(strIng,'.');

					if(strIng_ptr_komma==255) strIng[strIng_len]='0';
					else
						{
						if(strIng_len-strIng_ptr_komma>=3)
							{
							answer_code='D';
							}
						else
							{
							strcpy(&strIng[strIng_ptr_komma],&strIng[strIng_ptr_komma+1]);
							}
						} 

					if(answer_code!='D') 
						{
						setData=(signed short)atoi(strIng);
						
						if((setData>=250)&&(setData<=550))
							{
							lc640_write_int(ADR_EE_I_SET[chNumber],setData);
							}
						else 
							{
							answer_code='V';
							}
						}
					}
				else if((UIB1[5]=='T')&&(UIB1[6]=='p'))   //Установка времени подьема 
					{
					strIng_ptr_start=7;
					strIng_ptr_end=pal_strspn(UIB1,'\r');
					strIng_len=strIng_ptr_end-strIng_ptr_start;
 					memcpy(strIng, &UIB1[strIng_ptr_start], strIng_len);
					strIng[strIng_len]=0;
					strIng[strIng_len+1]=0;
 
					strIng_ptr_komma=pal_strspn(strIng,'.');
					
					if(strIng_ptr_komma!=255) 
						{
						answer_code='D';
						} 
					if(answer_code!='D') 
						{
						setData=(signed short)atoi(strIng);
						
						if((setData>=5)&&(setData<=60))
							{
							lc640_write_int(ADR_EE_TP_SET[chNumber],setData);
							}
						else 
							{
							answer_code='V';
							}
						}
					}

				else if((UIB1[5]=='T')&&(UIB1[6]=='v'))   //Установка времени выдержки 
					{
					strIng_ptr_start=7;
					strIng_ptr_end=pal_strspn(UIB1,'\r');
					strIng_len=strIng_ptr_end-strIng_ptr_start;
 					memcpy(strIng, &UIB1[strIng_ptr_start], strIng_len);
					strIng[strIng_len]=0;
					strIng[strIng_len+1]=0;
 
					strIng_ptr_komma=pal_strspn(strIng,'.');
					
					if(strIng_ptr_komma!=255) 
						{
						answer_code='D';
						} 
					if(answer_code!='D') 
						{
						setData=(signed short)atoi(strIng);
						
						if((setData>=1)&&(setData<=200))
							{
							lc640_write_int(ADR_EE_TV_SET[chNumber],setData);
							}
						else 
							{
							answer_code='V';
							}
						}
					}

				else if(UIB1[5]=='R')    //Установка количества повторов 
                	{
					strIng_ptr_start=6;
					strIng_ptr_end=pal_strspn(UIB1,'\r');
					strIng_len=strIng_ptr_end-strIng_ptr_start;
 					memcpy(strIng, &UIB1[strIng_ptr_start], strIng_len);
					strIng[strIng_len]=0;
					strIng[strIng_len+1]=0;
 
					strIng_ptr_komma=pal_strspn(strIng,'.');
					
					if(strIng_ptr_komma!=255) 
						{
						answer_code='D';
						} 
					if(answer_code!='D') 
						{
						setData=(signed short)atoi(strIng);
						
						if((setData>=1)&&(setData<=99))
							{
							lc640_write_int(ADR_EE_AVT_REP_SET[chNumber],setData);
							}
						else 
							{
							answer_code='V';
							}
						}
                    }	
				else if(UIB1[5]=='P')    //Установка периода повторов 
                	{
					strIng_ptr_start=6;
					strIng_ptr_end=pal_strspn(UIB1,'\r');
					strIng_len=strIng_ptr_end-strIng_ptr_start;
 					memcpy(strIng, &UIB1[strIng_ptr_start], strIng_len);
					strIng[strIng_len]=0;
					strIng[strIng_len+1]=0;
 
					strIng_ptr_komma=pal_strspn(strIng,'.');
					
					if(strIng_ptr_komma!=255) 
						{
						answer_code='D';
						} 
					if(answer_code!='D') 
						{
						setData=(signed short)atoi(strIng);
						
						if((setData>=((Tvset[chNumber]+Tpset[chNumber]+59)/60))&&(setData<=1440))
							{
							lc640_write_int(ADR_EE_AVT_PER_SET[chNumber],setData);
							}
						else 
							{
							answer_code='V';
							}
						}
                    }

				else if(UIB1[5]=='E')    //Включение/выключение канала в список работающик (активация) 
                	{
					strIng_ptr_start=6;
					strIng_ptr_end=pal_strspn(UIB1,'\r');
					strIng_len=strIng_ptr_end-strIng_ptr_start;
 					memcpy(strIng, &UIB1[strIng_ptr_start], strIng_len);
					strIng[strIng_len]=0;
					strIng[strIng_len+1]=0;
 
					strIng_ptr_komma=pal_strspn(strIng,'.');
					
					if(strIng_ptr_komma!=255) 
						{
						answer_code='D';
						} 
					if(answer_code!='D') 
						{
						setData=(signed short)atoi(strIng);
						
						if((setData==0)||(setData==1))
							{
							lc640_write_int(ADR_EE_KON_SET[chNumber],setData);
							}
						else 
							{
							answer_code='V';
							}
						}
                    }
												
				else 
					{
					answer_code='?';
					}								
				}
			else if(UIB1[4]=='A') //Установка алгоритма работы
				{
				}
			else 
				{
				answer_code='C';
				}
			memo_out0[0]=answer_code;
			memo_out0[1]=adrh;
			memo_out0[2]=adrl;
			memo_out0[3]='3';
			memo_out0[4]=0x0d;
			uart_out_adr1(memo_out0,5);	
			}

	 	else if((UIB1[3]=='4')/*&&(UIB1[5]==0x0d)*/)
			{
			signed short tempSS;
			char temp;
			//memo_out0[5]=0x0d;
			//uart_out_adr1(memo_out0,6);
			if(UIB1[4]=='1')    //Текущее напряжение и ток
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
				memo_out0[7]='0'+ (Ui/1000);
				tempSS=Ui%1000;

				memo_out0[8]='0'+ (tempSS/100);
				tempSS=tempSS%100;
				
				memo_out0[9]='0'+ (tempSS/10);
				tempSS=tempSS%10;

				memo_out0[10]=',';


				memo_out0[11]='0'+ tempSS;
				
				tempSS=Ii;
				
				memo_out0[12]='+';

				memo_out0[13]='0'+ (tempSS/1000);
				tempSS=tempSS%1000;

				memo_out0[14]='0'+ (tempSS/100);
				tempSS=tempSS%100;

				memo_out0[15]='0'+ (tempSS/10);
				tempSS=tempSS%10;
				
				memo_out0[16]=',';
				
				memo_out0[17]='0'+ tempSS;
				
				memo_out0[18]=0x0d;

				uart_out_adr1(memo_out0,19);
				//uart_out1(3,1,2,3,4,5,6);
				}

			else 
				{
				memo_out0[0]='?';
				memo_out0[1]=adrh;
				memo_out0[2]=adrl;
				memo_out0[3]='4';
				memo_out0[4]=0x0d;
				uart_out_adr1(memo_out0,5);
				}
			}
	 	else if((UIB1[3]=='R')/*&&(UIB1[5]==0x0d)*/)
			{
			bRESET=1;
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




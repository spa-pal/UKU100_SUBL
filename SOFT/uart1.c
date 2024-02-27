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
char modbus_timeout_cnt;
char bMODBUS_TIMEOUT;
char modbus_tx_buff[100];
unsigned char modbus_an_buffer[30];    	//Буфер, куда они потом копируются для анализа
unsigned char modbus_rx_counter;		//Количество принятых байт, используется при анализе целостности посылки и при расшифровке
short modbus_rtu_plazma[6];
char bUART1TX;

//-----------------------------------------------
unsigned short CRC16_2(char* buf, short len)
{
unsigned short crc = 0xFFFF;
short pos;
short i;

for (pos = 0; pos < len; pos++)
  	{
    	crc ^= (unsigned short)buf[pos];          // XOR byte into least sig. byte of crc

    	for ( i = 8; i != 0; i--) 
		{    // Loop over each bit
      	if ((crc & 0x0001) != 0) 
			{      // If the LSB is set
        		crc >>= 1;                    // Shift right and XOR 0xA001
        		crc ^= 0xA001;
      		}
      	else  crc >>= 1;                    // Just shift right
    		}
  	}
  // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
return crc;
}

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

//modbus_rtu_plazma[0]++;

status=U1LSR;
u1iir=U1IIR;
   	
if((u1iir&0x0f)==4)
	{
	modbus_rtu_plazma[2]++;
	data=U1RBR;
	if(bUART1TX==0)
		{
		
		rx_buffer1[rx_wr_index1]=data;
	   	//bRXIN1=1;
	  	if (++rx_wr_index1 == RX_BUFFER_SIZE1) rx_wr_index1=0;
	   	if (++rx_counter1 == RX_BUFFER_SIZE1)
	      	{
	      	rx_counter1=0;
	      	rx_buffer_overflow1=1;
	      	}
	
		if((data==0x0d)||(data==0x0a))_485_last_cnt=5;
		else _485_last_cnt=0;
	    
		modbus_timeout_cnt=0;
		}
	//else  modbus_rtu_plazma[3]++;
	//plazma_uart11++;
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
		bUART1TX=1;

		tx_wd_cnt=100;
   		}
   	else 
		{
		SET_REG(PINSEL1,0,(22-16)*2,1); //Вход PV у 485
		IO0DIR|=(1<<22);
		IO0CLR|=(1<<22);
		bUART1TX=0;
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
void modbus_hold_coils_transmit(unsigned char adr,unsigned char func,unsigned short reg_adr,unsigned short reg_quantity, char prot)
{
signed char modbus_registers[150];
//char modbus_tx_buff[150];
unsigned short crc_temp;
char i;

Ires=123456;
U=654;
//modbus_rtu_plazma[0]++;//=modbus_rx_counter;

modbus_registers[20]=(signed char)(*(((char*)(&Ires))+1));					//Рег11	Старший байт младшего слова Iвых
modbus_registers[21]=(signed char)(*(((char*)(&Ires))));					//    	Младший байт младшего слова Iвых
modbus_registers[22]=(signed char)(*(((char*)(&Ires))+3));					//Рег12	Старший байт старшего слова Iвых
modbus_registers[23]=(signed char)(*(((char*)(&Ires))+2));					//   	Младший байт старшего слова Iвых
modbus_registers[24]=(signed char)(*(((char*)(&U))+1));						//Рег13	Старший байт младшего слова Uвых
modbus_registers[25]=(signed char)(*(((char*)(&U))));						//    	Младший байт младшего слова Uвых
modbus_registers[26]=0;														//Рег14	Старший байт старшего слова Uвых
modbus_registers[27]=0;														//   	Младший байт старшего слова Uвых
modbus_registers[28]=(signed char)(*(((char*)(&I_MAX))+1));					//Рег15	Старший байт младшего слова уставки тока
modbus_registers[29]=(signed char)(*(((char*)(&I_MAX))));					//    	Младший байт младшего слова уставки тока
modbus_registers[30]=0;														//Рег16	Старший байт старшего слова уставки тока
modbus_registers[31]=0;														//   	Младший байт старшего слова уставки тока
modbus_registers[32]=(signed char)(*(((char*)(&U_MAX))+1));					//Рег17	Старший байт младшего слова уставки напряжения
modbus_registers[33]=(signed char)(*(((char*)(&U_MAX))));					//    	Младший байт младшего слова уставки напряжения
modbus_registers[34]=0;														//Рег18	Старший байт старшего слова уставки напряжения
modbus_registers[35]=0;														//   	Младший байт старшего слова уставки напряжения


/*modbus_registers[28]=(char)((LPC_RTC->MIN)>>8);			//Рег15  Время, минуты
modbus_registers[29]=(char)((LPC_RTC->MIN));
modbus_registers[30]=(char)((LPC_RTC->SEC)>>8);			//Рег16  Время, секунды
modbus_registers[31]=(char)((LPC_RTC->SEC));
modbus_registers[38]=(char)(NUMIST>>8);				//Рег20  Количество выпрямителей в структуре
modbus_registers[39]=(char)(NUMIST);
//modbus_registers[40]=(char)(PAR>>8);					//Рег21  Параллельная работа выпрямителей вкл./выкл.
//modbus_registers[41]=(char)(PAR);
modbus_registers[42]=(char)(ZV_ON>>8);					//Рег22  Звуковая аварийная сигнализация вкл./выкл.
modbus_registers[43]=(char)(ZV_ON);
modbus_registers[46]=(char)(UBM_AV>>8);				//Рег24  Аварийный уровень отклонения напряжения средней точки батареи, %
modbus_registers[47]=(char)(UBM_AV);
modbus_registers[58]=(char)(TBAT>>8);					//Рег30  Период проверки цепи батареи, минут.
modbus_registers[59]=(char)(TBAT);
modbus_registers[60]=(char)(UMAX>>8);					//Рег31  Максимальное (аварийное) напряжение выпрямителей, 0.1В
modbus_registers[61]=(char)(UMAX);
//modbus_registers[62]=(char)((UB20-DU)>>8);				//Рег32  Минимальное (аварийное) напряжение выпрямителей, 0.1В
//modbus_registers[63]=(char)((UB20-DU));
//modbus_registers[64]=(char)(UB0>>8);					//Рег33  Напряжение содержания батареи при 0гЦ, 0.1В
//modbus_registers[65]=(char)(UB0);
//modbus_registers[66]=(char)(UB20>>8);					//Рег34  Напряжение содержания батареи при 20гЦ, 0.1В
//modbus_registers[67]=(char)(UB20);
modbus_registers[68]=(char)(USIGN>>8);					//Рег35  Минимальное (сигнальное) напряжение батареи, 1В
modbus_registers[69]=(char)(USIGN);
modbus_registers[70]=(char)(UMN>>8);					//Рег36  Минимальное (аварийное) напряжение питающей сети, 1В
modbus_registers[71]=(char)(UMN);
modbus_registers[72]=(char)(U0B>>8);					//Рег37  Рабочее напряжение при невведенных батареях, 0.1В
modbus_registers[73]=(char)(U0B);
modbus_registers[74]=(char)(IKB>>8);					//Рег38  Ток контроля наличия батареи, 0.1а
modbus_registers[75]=(char)(IKB);
modbus_registers[76]=(char)(IZMAX>>8);					//Рег39  Ток заряда батареи максимальный, 0.1А
modbus_registers[77]=(char)(IZMAX);
modbus_registers[78]=(char)(IMAX>>8);					//Рег40  Ток переключения на большее кол-во выпрямителей, 0.1А
modbus_registers[79]=(char)(IMAX);
modbus_registers[80]=(char)(IMIN>>8);					//Рег41  Ток переключения на меньшее кол-во выпрямителей, 0.1А
modbus_registers[81]=(char)(IMIN);
//modbus_registers[82]=(char)(UVZ>>8);					//Рег42  Напряжение выравнивающего заряда, 0.1В
//modbus_registers[83]=(char)(UVZ);
modbus_registers[84]=(char)(TZAS>>8);					//Рег43  Время задержки включения выпрямителей, сек
modbus_registers[85]=(char)(TZAS);
modbus_registers[86]=(char)(TMAX>>8);					//Рег44  Температура выпрямителей аварийная, 1гЦ
modbus_registers[87]=(char)(TMAX);
modbus_registers[88]=(char)(TSIGN>>8);					//Рег45  Температура выпрямителей сигнальная, 1гЦ
modbus_registers[89]=(char)(TSIGN);
modbus_registers[90]=(char)(TBATMAX>>8);				//Рег46  Температура батареи аварийная, 1гЦ
modbus_registers[91]=(char)(TBATMAX);
modbus_registers[92]=(char)(TBATSIGN>>8);				//Рег47  Температура батареи сигнальная, 1гЦ
modbus_registers[93]=(char)(TBATSIGN);
modbus_registers[94]=(char)(speedChrgCurr>>8);					//Рег48  Ток ускоренного заряда, 0.1А
modbus_registers[95]=(char)(speedChrgCurr);
modbus_registers[96]=(char)(speedChrgVolt>>8);				//Рег49	 Напряжение ускоренного заряда, 0.1В 
modbus_registers[97]=(char)(speedChrgVolt);
modbus_registers[98]=(char)(speedChrgTimeInHour>>8);				//Рег50	 Время ускоренного заряда, 1ч
modbus_registers[99]=(char)(speedChrgTimeInHour);
modbus_registers[100]=(char)(U_OUT_KONTR_MAX>>8);					//Рег51	 Контроль выходного напряжения, Umax, 0.1В
modbus_registers[101]=(char)(U_OUT_KONTR_MAX);
modbus_registers[102]=(char)(U_OUT_KONTR_MIN>>8);					//Рег52	 Контроль выходного напряжения, Umin, 0.1В
*/
modbus_registers[103]=(char)(3);
modbus_registers[104]=(char)(4);				//Рег53	 Контроль выходного напряжения, Tзадержки, 1сек.
modbus_registers[105]=(char)(5);




if(prot==0)
	{
	modbus_tx_buff[0]=adr;
	modbus_tx_buff[1]=func;
	modbus_tx_buff[2]=(char)((reg_quantity/8)+1);
	memcpy((char*)&modbus_tx_buff[3],(char*)&modbus_registers[(reg_adr-1)*2],reg_quantity*2);
	
	crc_temp=CRC16_2(modbus_tx_buff,(reg_quantity*2)+3);

	modbus_tx_buff[3+(reg_quantity*2)]=(char)crc_temp;
	modbus_tx_buff[4+(reg_quantity*2)]=crc_temp>>8;

	SET_REG(PINSEL1,0,(22-16)*2,2); //Вход EN у 485
	IO0DIR|=(1<<22);
	IO0SET|=(1<<22);

	for (i=0;i<(5+(reg_quantity*2));i++)
		{
		putchar1(modbus_tx_buff[i]);
		}

	}

}

//-----------------------------------------------
void modbus_hold_registers_transmit(unsigned char adr,unsigned char func,unsigned short reg_adr,unsigned short reg_quantity, char prot)
{
signed char modbus_registers[150];
//char modbus_tx_buff[150];
unsigned short crc_temp;
char i;

//Ires=123456;
//U=654;
//modbus_rtu_plazma[0]++;//=modbus_rx_counter;

modbus_registers[0]=(signed char)(*(((char*)(&Iset[0]))+1));				//Рег1	установленный ток 1-го канала
modbus_registers[1]=(signed char)(*(((char*)(&Iset[0]))));					//    	
modbus_registers[2]=(signed char)(*(((char*)(&Tpset[0]))+1));				//Рег2	время подъема тока 1-го канала
modbus_registers[3]=(signed char)(*(((char*)(&Tpset[0]))));					//   	 
modbus_registers[4]=(signed char)(*(((char*)(&Tvset[0]))+1));				//Рег3	время выдержки тока 1-го канала
modbus_registers[5]=(signed char)(*(((char*)(&Tvset[0]))));					//    	 
modbus_registers[6]=(signed char)(*(((char*)(&AVT_REP_SET[0]))+1));			//Рег4	колличество повторов для 1-го канала
modbus_registers[7]=(signed char)(*(((char*)(&AVT_REP_SET[0]))));			//   	 
modbus_registers[8]=(signed char)(*(((char*)(&AVT_PER_SET[0]))+1));			//Рег5	период повторов для 1-го канала 
modbus_registers[9]=(signed char)(*(((char*)(&AVT_PER_SET[0]))));			//    	 

modbus_registers[10]=(signed char)(*(((char*)(&Iset[1]))+1));				//Рег6	установленный ток 2-го канала
modbus_registers[11]=(signed char)(*(((char*)(&Iset[1]))));					//    	
modbus_registers[12]=(signed char)(*(((char*)(&Tpset[1]))+1));				//Рег7	время подъема тока 2-го канала
modbus_registers[13]=(signed char)(*(((char*)(&Tpset[1]))));				//   	 
modbus_registers[14]=(signed char)(*(((char*)(&Tvset[1]))+1));				//Рег8	время выдержки тока 2-го канала
modbus_registers[15]=(signed char)(*(((char*)(&Tvset[1]))));				//    	 
modbus_registers[16]=(signed char)(*(((char*)(&AVT_REP_SET[1]))+1));		//Рег9	колличество повторов для 2-го канала
modbus_registers[17]=(signed char)(*(((char*)(&AVT_REP_SET[1]))));			//   	 
modbus_registers[18]=(signed char)(*(((char*)(&AVT_PER_SET[1]))+1));		//Рег10	период повторов для 2-го канала 
modbus_registers[19]=(signed char)(*(((char*)(&AVT_PER_SET[1]))));			//    

modbus_registers[20]=(signed char)(*(((char*)(&Iset[2]))+1));				//Рег11	установленный ток 3-го канала
modbus_registers[21]=(signed char)(*(((char*)(&Iset[2]))));					//    	
modbus_registers[22]=(signed char)(*(((char*)(&Tpset[2]))+1));				//Рег12	время подъема тока 3-го канала
modbus_registers[23]=(signed char)(*(((char*)(&Tpset[2]))));				//   	 
modbus_registers[24]=(signed char)(*(((char*)(&Tvset[2]))+1));				//Рег13	время выдержки тока 3-го канала
modbus_registers[25]=(signed char)(*(((char*)(&Tvset[2]))));				//    	 
modbus_registers[26]=(signed char)(*(((char*)(&AVT_REP_SET[2]))+1));		//Рег14	колличество повторов для 3-го канала
modbus_registers[27]=(signed char)(*(((char*)(&AVT_REP_SET[2]))));			//   	 
modbus_registers[28]=(signed char)(*(((char*)(&AVT_PER_SET[2]))+1));		//Рег15	период повторов для 3-го канала 
modbus_registers[29]=(signed char)(*(((char*)(&AVT_PER_SET[2]))));			//    	 

modbus_registers[30]=(signed char)(*(((char*)(&Iset[3]))+1));				//Рег16	установленный ток 4-го канала
modbus_registers[31]=(signed char)(*(((char*)(&Iset[3]))));					//    	
modbus_registers[32]=(signed char)(*(((char*)(&Tpset[3]))+1));				//Рег17	время подъема тока 4-го канала
modbus_registers[33]=(signed char)(*(((char*)(&Tpset[3]))));				//   	 
modbus_registers[34]=(signed char)(*(((char*)(&Tvset[3]))+1));				//Рег18	время выдержки тока 4-го канала
modbus_registers[35]=(signed char)(*(((char*)(&Tvset[3]))));				//    	 
modbus_registers[36]=(signed char)(*(((char*)(&AVT_REP_SET[3]))+1));		//Рег19	колличество повторов для 4-го канала
modbus_registers[37]=(signed char)(*(((char*)(&AVT_REP_SET[3]))));			//   	 
modbus_registers[38]=(signed char)(*(((char*)(&AVT_PER_SET[3]))+1));		//Рег20	период повторов для 4-го канала 
modbus_registers[39]=(signed char)(*(((char*)(&AVT_PER_SET[3]))));			//    

modbus_registers[40]=(signed char)(*(((char*)(&Iset[4]))+1));				//Рег21	установленный ток 5-го канала
modbus_registers[41]=(signed char)(*(((char*)(&Iset[4]))));					//    	
modbus_registers[42]=(signed char)(*(((char*)(&Tpset[4]))+1));				//Рег22	время подъема тока 5-го канала
modbus_registers[43]=(signed char)(*(((char*)(&Tpset[4]))));				//   	 
modbus_registers[44]=(signed char)(*(((char*)(&Tvset[4]))+1));				//Рег23	время выдержки тока 5-го канала
modbus_registers[45]=(signed char)(*(((char*)(&Tvset[4]))));				//    	 
modbus_registers[46]=(signed char)(*(((char*)(&AVT_REP_SET[4]))+1));		//Рег24	колличество повторов для 5-го канала
modbus_registers[47]=(signed char)(*(((char*)(&AVT_REP_SET[4]))));			//   	 
modbus_registers[48]=(signed char)(*(((char*)(&AVT_PER_SET[4]))+1));		//Рег25	период повторов для 5-го канала 
modbus_registers[49]=(signed char)(*(((char*)(&AVT_PER_SET[4]))));			//    	 

modbus_registers[50]=(signed char)(*(((char*)(&Iset[5]))+1));				//Рег26	установленный ток 6-го канала
modbus_registers[51]=(signed char)(*(((char*)(&Iset[5]))));					//    	
modbus_registers[52]=(signed char)(*(((char*)(&Tpset[5]))+1));				//Рег27	время подъема тока 6-го канала
modbus_registers[53]=(signed char)(*(((char*)(&Tpset[5]))));				//   	 
modbus_registers[54]=(signed char)(*(((char*)(&Tvset[5]))+1));				//Рег28	время выдержки тока 6-го канала
modbus_registers[55]=(signed char)(*(((char*)(&Tvset[5]))));				//    	 
modbus_registers[56]=(signed char)(*(((char*)(&AVT_REP_SET[5]))+1));		//Рег29	колличество повторов для 6-го канала
modbus_registers[57]=(signed char)(*(((char*)(&AVT_REP_SET[5]))));			//   	 
modbus_registers[58]=(signed char)(*(((char*)(&AVT_PER_SET[5]))+1));		//Рег30	период повторов для 6-го канала 
modbus_registers[59]=(signed char)(*(((char*)(&AVT_PER_SET[5]))));			//    

modbus_registers[60]=(signed char)(*(((char*)(&Iset[6]))+1));				//Рег31	установленный ток 7-го канала
modbus_registers[61]=(signed char)(*(((char*)(&Iset[6]))));					//    	
modbus_registers[62]=(signed char)(*(((char*)(&Tpset[6]))+1));				//Рег32	время подъема тока 7-го канала
modbus_registers[63]=(signed char)(*(((char*)(&Tpset[6]))));				//   	 
modbus_registers[64]=(signed char)(*(((char*)(&Tvset[6]))+1));				//Рег33	время выдержки тока 7-го канала
modbus_registers[65]=(signed char)(*(((char*)(&Tvset[6]))));				//    	 
modbus_registers[66]=(signed char)(*(((char*)(&AVT_REP_SET[6]))+1));		//Рег34	колличество повторов для 7-го канала
modbus_registers[67]=(signed char)(*(((char*)(&AVT_REP_SET[6]))));			//   	 
modbus_registers[68]=(signed char)(*(((char*)(&AVT_PER_SET[6]))+1));		//Рег35	период повторов для 7-го канала 
modbus_registers[69]=(signed char)(*(((char*)(&AVT_PER_SET[6]))));			//    	 

modbus_registers[70]=(signed char)(*(((char*)(&Iset[7]))+1));				//Рег36	установленный ток 8-го канала
modbus_registers[71]=(signed char)(*(((char*)(&Iset[7]))));					//    	
modbus_registers[72]=(signed char)(*(((char*)(&Tpset[7]))+1));				//Рег37	время подъема тока 8-го канала
modbus_registers[73]=(signed char)(*(((char*)(&Tpset[7]))));				//   	 
modbus_registers[74]=(signed char)(*(((char*)(&Tvset[7]))+1));				//Рег38	время выдержки тока 8-го канала
modbus_registers[75]=(signed char)(*(((char*)(&Tvset[7]))));				//    	 
modbus_registers[76]=(signed char)(*(((char*)(&AVT_REP_SET[7]))+1));		//Рег39	колличество повторов для 8-го канала
modbus_registers[77]=(signed char)(*(((char*)(&AVT_REP_SET[7]))));			//   	 
modbus_registers[78]=(signed char)(*(((char*)(&AVT_PER_SET[7]))+1));		//Рег40	период повторов для 8-го канала 
modbus_registers[79]=(signed char)(*(((char*)(&AVT_PER_SET[7]))));			//    





if(prot==0)
	{
	modbus_tx_buff[0]=adr;
	modbus_tx_buff[1]=func;
	modbus_tx_buff[2]=(char)(reg_quantity*2);
	memcpy((char*)&modbus_tx_buff[3],(char*)&modbus_registers[(reg_adr-1)*2],reg_quantity*2);
	
	crc_temp=CRC16_2(modbus_tx_buff,(reg_quantity*2)+3);

	modbus_tx_buff[3+(reg_quantity*2)]=(char)crc_temp;
	modbus_tx_buff[4+(reg_quantity*2)]=crc_temp>>8;

	SET_REG(PINSEL1,0,(22-16)*2,2); //Вход EN у 485
	IO0DIR|=(1<<22);
	IO0SET|=(1<<22);

	for (i=0;i<(5+(reg_quantity*2));i++)
		{
		putchar1(modbus_tx_buff[i]);
		}

	}

}

//-----------------------------------------------
void modbus_input_registers_transmit(unsigned char adr,unsigned char func,unsigned short reg_adr,unsigned short reg_quantity, char prot)
{
signed char modbus_registers[1000];	 //o_10
//char modbus_tx_buff[200];
unsigned short crc_temp;
char i;
short tempS;

tempS = proc_rep-1;
modbus_registers[0]=(signed char)(*(((char*)(&tempS))+1));		//Рег1   	Количество оставшихся пусков
modbus_registers[1]=(signed char)(*(((char*)(&tempS))));
modbus_registers[2]=(signed char)(*(((char*)(&Ii))+1));			//Рег2   	Ток, 0.1А
modbus_registers[3]=(signed char)(*(((char*)(&Ii))));
modbus_registers[4]=(signed char)(*(((char*)(&Ui))+1));			//Рег3   	Напряжение, 0.1В
modbus_registers[5]=(signed char)(*(((char*)(&Ui))));
tempS = wrk_time/1000;
modbus_registers[6]=(signed char)(*(((char*)(&tempS))+1));		//Рег4   	Время включения, 1сек.
modbus_registers[7]=(signed char)(*(((char*)(&tempS))));
tempS = wrk_remain_time/1000;
modbus_registers[8]=(signed char)(*(((char*)(&tempS))+1));		//Рег5   	Время выключения, 1сек.
modbus_registers[9]=(signed char)(*(((char*)(&tempS))));

if((wrk_stat==stOFF)&&(proc_stat==stCH))
	{
	tempS = wrk_period_time/1000UL;
	modbus_registers[10]=(signed char)(*(((char*)(&tempS))+1));	//Рег6   	Время до вкл след канала, 1сек.
	modbus_registers[10]=(signed char)(*(((char*)(&tempS))));
	tempS = wrk_ch+1;
	modbus_registers[12]=(signed char)(*(((char*)(&tempS))+1));	//Рег7   	номер вкл след канала, 1сек.
	modbus_registers[13]=(signed char)(*(((char*)(&tempS))));
	}

else if(wrk_pause_time)
	{
	tempS = wrk_pause_time/1000UL;
	modbus_registers[10]=(signed char)(*(((char*)(&tempS))+1));	//Рег6   	Время до вкл след канала, 1сек.
	modbus_registers[10]=(signed char)(*(((char*)(&tempS))));
	tempS = wrk_ch+1;
	modbus_registers[12]=(signed char)(*(((char*)(&tempS))+1));	//Рег7   	номер вкл след канала, 1сек.
	modbus_registers[13]=(signed char)(*(((char*)(&tempS))));
	}

else 
	{
	tempS = 0;
	modbus_registers[10]=(signed char)(*(((char*)(&tempS))+1));	//Рег6   	Время до вкл след канала, 1сек.
	modbus_registers[10]=(signed char)(*(((char*)(&tempS))));
	tempS = 0;
	modbus_registers[12]=(signed char)(*(((char*)(&tempS))+1));	//Рег7   	номер вкл след канала, 1сек.
	modbus_registers[13]=(signed char)(*(((char*)(&tempS))));
	}


//o_10_e
//if(prot==MODBUS_RTU_PROT)
	{
	modbus_tx_buff[0]=adr;
	modbus_tx_buff[1]=func;

	modbus_tx_buff[2]=(char)(reg_quantity*2);

	memcpy((signed char*)&modbus_tx_buff[3],(signed char*)&modbus_registers[(reg_adr-1)*2],reg_quantity*2);

	crc_temp=CRC16_2(modbus_tx_buff,(reg_quantity*2)+3);

	modbus_tx_buff[3+(reg_quantity*2)]=(char)crc_temp;
	modbus_tx_buff[4+(reg_quantity*2)]=crc_temp>>8;

	for (i=0;i<(5+(reg_quantity*2));i++)
		{
		putchar1(modbus_tx_buff[i]);
		}

	}

}

//-----------------------------------------------
void modbus_in(void)
{
short crc16_calculated;		//вычисляемая из принятых данных CRC
short crc16_incapsulated;	//встроеннная в посылку CRC
unsigned short modbus_rx_arg0;		//встроенный в посылку первый аргумент
unsigned short modbus_rx_arg1;		//встроенный в посылку второй аргумент
//unsigned short modbus_rx_arg2;		//встроенный в посылку третий аргумент
//unsigned short modbus_rx_arg3;		//встроенный в посылку четвертый аргумент
unsigned char modbus_func;			//встроенный в посылку код функции

memcpy(modbus_an_buffer,rx_buffer1,rx_wr_index1);
modbus_rx_counter=rx_wr_index1;
//modbus_rx_buffer_ptr=0;
//bMODBUS_TIMEOUT=0;
		rx_counter1=0;
		rx_wr_index1=0;


//modbus_rtu_plazma[1]=modbus_rx_counter;

//memcpy(modbus_an_buffer,UIB1,modbus_rx_counter);
//modbus_rx_counter=modbus_rx_buffer_ptr;
//modbus_rx_buffer_ptr=0;
//bMODBUS_TIMEOUT=0;
	
crc16_calculated  = CRC16_2((char*)modbus_an_buffer, modbus_rx_counter-2);
crc16_incapsulated = *((short*)&modbus_an_buffer[modbus_rx_counter-2]);

//modbus_plazma1=modbus_rx_counter;
//modbus_plazma2=crc16_calculated;
//modbus_plazma3=crc16_incapsulated;

modbus_func=modbus_an_buffer[1];
modbus_rx_arg0=(((unsigned short)modbus_an_buffer[2])*((unsigned short)256))+((unsigned short)modbus_an_buffer[3]);
modbus_rx_arg1=(((unsigned short)modbus_an_buffer[4])*((unsigned short)256))+((unsigned short)modbus_an_buffer[5]);
//modbus_rx_arg2=(((unsigned short)modbus_an_buffer[6])*((unsigned short)256))+((unsigned short)modbus_an_buffer[7]);
//modbus_rx_arg3=(((unsigned short)modbus_an_buffer[8])*((unsigned short)256))+((unsigned short)modbus_an_buffer[9]);

//#define IPS_CURR_AVG_MODBUS_ADRESS	1

modbus_rtu_plazma[4]=crc16_calculated;
modbus_rtu_plazma[5]=crc16_incapsulated;

if((crc16_calculated==crc16_incapsulated) ||(modbus_func==0x10))
	{
	modbus_rtu_plazma[1]++;
	if(modbus_an_buffer[0]==ADR)
		{
		 modbus_rtu_plazma[3]++;

		if(modbus_func==1)		//чтение произвольного кол-ва битов хранения
			{
			unsigned short crc_temp;
			char i;
			//modbus_hold_coils_transmit(ADR,modbus_func,modbus_rx_arg0,modbus_rx_arg1,0);
			if((modbus_rx_arg0==1))
				{
				char temp=0,i;

				for(i=0;i<8;i++)
					{
					if(	(wrk_stat==stON)&&
						((proc_stat==stALG2)||(proc_stat==stON)||(proc_stat==stCH))&&
						(wrk_ch==i))
						{
						temp|=(1<<i);
						}
					}

				modbus_tx_buff[0]=ADR;
				modbus_tx_buff[1]=modbus_func;
				modbus_tx_buff[2]=1;
				modbus_tx_buff[3]=temp;

				
				crc_temp=CRC16_2(modbus_tx_buff,4);
			
				modbus_tx_buff[4]=(char)crc_temp;
				modbus_tx_buff[5]=crc_temp>>8;
			
				SET_REG(PINSEL1,0,(22-16)*2,2); //Вход EN у 485
				IO0DIR|=(1<<22);
				IO0SET|=(1<<22);
			
				for (i=0;i<6;i++)
					{
					putchar1(modbus_tx_buff[i]);
					}
			
				}
			}

		else if(modbus_func==5)		//чтение произвольного кол-ва битов хранения
			{
			unsigned short crc_temp;
			char i;
			//modbus_hold_coils_transmit(ADR,modbus_func,modbus_rx_arg0,modbus_rx_arg1,0);
			
			if((modbus_rx_arg0==11))
				{
				if((modbus_rx_arg1==0xff00)&&(wrk_state!=wrkON))
					{
					
					//wrk_state=wrkON;
					//pwmI_start=POWER*9;
					//wrk_cnt_cnt=(signed long)T_WRK_MAX*300L;
					//lc640_write_int(EE_WRK_STAT,wrk_state);
					//ist=istT;
					//t_u_min1=-10;
					//t_u_min2=-10;
					}
				else if((modbus_rx_arg1==0)&&(wrk_state!=wrkOFF))
					{
					//wrk_state=wrkOFF;
					//lc640_write_int(EE_WRK_STAT,wrk_state);
					}


				modbus_tx_buff[0]=ADR;
				modbus_tx_buff[1]=modbus_func;
				modbus_tx_buff[2]=0;
				modbus_tx_buff[3]=11;
				modbus_tx_buff[4]=0;
				modbus_tx_buff[5]=modbus_rx_arg1;
				
				crc_temp=CRC16_2(modbus_tx_buff,6);
				
				modbus_tx_buff[6]=(char)crc_temp;
				modbus_tx_buff[7]=crc_temp>>8;
			
				SET_REG(PINSEL1,0,(22-16)*2,2); //Вход EN у 485
				IO0DIR|=(1<<22);
				IO0SET|=(1<<22);
			
				for (i=0;i<8;i++)
					{
					putchar1(modbus_tx_buff[i]);
					}
			
				}
			}

		else if(modbus_func==3)		//чтение произвольного кол-ва регистров хранения
			{
			//modbus_plazma++;
			modbus_hold_registers_transmit(ADR,modbus_func,modbus_rx_arg0,modbus_rx_arg1,0);
			}

		else if(modbus_func==4)		//чтение произвольного кол-ва регистров	входов
			{
			modbus_input_registers_transmit(ADR,modbus_func,modbus_rx_arg0,modbus_rx_arg1,0);
			}

		else if(modbus_func==6) 	//запись регистра хранения
			{
			unsigned short crc_temp;
			char i;
			if(modbus_rx_arg0==1)			//ток 1-го канала
				{
				gran(&modbus_rx_arg1,250,550);
	    		lc640_write_int(ADR_EE_I_SET[0],modbus_rx_arg1);
				}
			if(modbus_rx_arg0==2)			//время подъема 1-го канала
				{
				gran(&modbus_rx_arg1,5,60);
	    		lc640_write_int(ADR_EE_TP_SET[0],modbus_rx_arg1);
				}

			if(modbus_rx_arg0==3)			//время выдержки 1-го канала
				{
				gran(&modbus_rx_arg1,1,200);
	    		lc640_write_int(ADR_EE_TV_SET[0],modbus_rx_arg1);
				}

			if(modbus_rx_arg0==4)			//колличество повторов для 1-го канала
				{
				gran(&modbus_rx_arg1,1,99);
	    		lc640_write_int(ADR_EE_AVT_REP_SET[0],modbus_rx_arg1);
				}

			if(modbus_rx_arg0==5)			//период повторов для 1-го канала
				{
				gran(&modbus_rx_arg1,1,1440);
	    		lc640_write_int(ADR_EE_AVT_PER_SET[0],modbus_rx_arg1);
				}

			if(modbus_rx_arg0==6)			//ток 2-го канала
				{
				gran(&modbus_rx_arg1,250,550);
	    		lc640_write_int(ADR_EE_I_SET[1],modbus_rx_arg1);
				}
			if(modbus_rx_arg0==7)			//время подъема 2-го канала
				{
				gran(&modbus_rx_arg1,5,60);
	    		lc640_write_int(ADR_EE_TP_SET[1],modbus_rx_arg1);
				}

			if(modbus_rx_arg0==8)			//время выдержки 2-го канала
				{
				gran(&modbus_rx_arg1,1,200);
	    		lc640_write_int(ADR_EE_TV_SET[1],modbus_rx_arg1);
				}

			if(modbus_rx_arg0==9)			//колличество повторов для 2-го канала
				{
				gran(&modbus_rx_arg1,1,99);
	    		lc640_write_int(ADR_EE_AVT_REP_SET[1],modbus_rx_arg1);
				}

			if(modbus_rx_arg0==10)			//период повторов для 2-го канала
				{
				gran(&modbus_rx_arg1,1,1440);
	    		lc640_write_int(ADR_EE_AVT_PER_SET[1],modbus_rx_arg1);
				}
			if(modbus_rx_arg0==11)			//ток 3-го канала
				{
				gran(&modbus_rx_arg1,250,550);
	    		lc640_write_int(ADR_EE_I_SET[2],modbus_rx_arg1);
				}
			if(modbus_rx_arg0==12)			//время подъема 3-го канала
				{
				gran(&modbus_rx_arg1,5,60);
	    		lc640_write_int(ADR_EE_TP_SET[2],modbus_rx_arg1);
				}

			if(modbus_rx_arg0==13)			//время выдержки 3-го канала
				{
				gran(&modbus_rx_arg1,1,200);
	    		lc640_write_int(ADR_EE_TV_SET[2],modbus_rx_arg1);
				}

			if(modbus_rx_arg0==14)			//колличество повторов для 3-го канала
				{
				gran(&modbus_rx_arg1,1,99);
	    		lc640_write_int(ADR_EE_AVT_REP_SET[2],modbus_rx_arg1);
				}

			if(modbus_rx_arg0==15)			//период повторов для 3-го канала
				{
				gran(&modbus_rx_arg1,1,1440);
	    		lc640_write_int(ADR_EE_AVT_PER_SET[2],modbus_rx_arg1);
				}

			if(modbus_rx_arg0==16)			//ток 4-го канала
				{
				gran(&modbus_rx_arg1,250,550);
	    		lc640_write_int(ADR_EE_I_SET[3],modbus_rx_arg1);
				}
			if(modbus_rx_arg0==17)			//время подъема 4-го канала
				{
				gran(&modbus_rx_arg1,5,60);
	    		lc640_write_int(ADR_EE_TP_SET[3],modbus_rx_arg1);
				}

			if(modbus_rx_arg0==18)			//время выдержки 4-го канала
				{
				gran(&modbus_rx_arg1,1,200);
	    		lc640_write_int(ADR_EE_TV_SET[3],modbus_rx_arg1);
				}

			if(modbus_rx_arg0==19)			//колличество повторов для 4-го канала
				{
				gran(&modbus_rx_arg1,1,99);
	    		lc640_write_int(ADR_EE_AVT_REP_SET[3],modbus_rx_arg1);
				}

			if(modbus_rx_arg0==20)			//период повторов для 4-го канала
				{
				gran(&modbus_rx_arg1,1,1440);
	    		lc640_write_int(ADR_EE_AVT_PER_SET[3],modbus_rx_arg1);
				}
			if(modbus_rx_arg0==21)			//ток 5-го канала
				{
				gran(&modbus_rx_arg1,250,550);
	    		lc640_write_int(ADR_EE_I_SET[4],modbus_rx_arg1);
				}
			if(modbus_rx_arg0==22)			//время подъема 5-го канала
				{
				gran(&modbus_rx_arg1,5,60);
	    		lc640_write_int(ADR_EE_TP_SET[4],modbus_rx_arg1);
				}

			if(modbus_rx_arg0==23)			//время выдержки 5-го канала
				{
				gran(&modbus_rx_arg1,1,200);
	    		lc640_write_int(ADR_EE_TV_SET[4],modbus_rx_arg1);
				}

			if(modbus_rx_arg0==24)			//колличество повторов для 5-го канала
				{
				gran(&modbus_rx_arg1,1,99);
	    		lc640_write_int(ADR_EE_AVT_REP_SET[4],modbus_rx_arg1);
				}

			if(modbus_rx_arg0==25)			//период повторов для 5-го канала
				{
				gran(&modbus_rx_arg1,1,1440);
	    		lc640_write_int(ADR_EE_AVT_PER_SET[4],modbus_rx_arg1);
				}

			if(modbus_rx_arg0==26)			//ток 6-го канала
				{
				gran(&modbus_rx_arg1,250,550);
	    		lc640_write_int(ADR_EE_I_SET[5],modbus_rx_arg1);
				}
			if(modbus_rx_arg0==27)			//время подъема 6-го канала
				{
				gran(&modbus_rx_arg1,5,60);
	    		lc640_write_int(ADR_EE_TP_SET[5],modbus_rx_arg1);
				}

			if(modbus_rx_arg0==28)			//время выдержки 6-го канала
				{
				gran(&modbus_rx_arg1,1,200);
	    		lc640_write_int(ADR_EE_TV_SET[5],modbus_rx_arg1);
				}

			if(modbus_rx_arg0==29)			//колличество повторов для 6-го канала
				{
				gran(&modbus_rx_arg1,1,99);
	    		lc640_write_int(ADR_EE_AVT_REP_SET[5],modbus_rx_arg1);
				}

			if(modbus_rx_arg0==30)			//период повторов для 2-го канала
				{
				gran(&modbus_rx_arg1,1,1440);
	    		lc640_write_int(ADR_EE_AVT_PER_SET[5],modbus_rx_arg1);
				}
			if(modbus_rx_arg0==31)			//ток 7-го канала
				{
				gran(&modbus_rx_arg1,250,550);
	    		lc640_write_int(ADR_EE_I_SET[6],modbus_rx_arg1);
				}
			if(modbus_rx_arg0==32)			//время подъема 7-го канала
				{
				gran(&modbus_rx_arg1,5,60);
	    		lc640_write_int(ADR_EE_TP_SET[6],modbus_rx_arg1);
				}

			if(modbus_rx_arg0==33)			//время выдержки 7-го канала
				{
				gran(&modbus_rx_arg1,1,200);
	    		lc640_write_int(ADR_EE_TV_SET[6],modbus_rx_arg1);
				}

			if(modbus_rx_arg0==34)			//колличество повторов для 7-го канала
				{
				gran(&modbus_rx_arg1,1,99);
	    		lc640_write_int(ADR_EE_AVT_REP_SET[6],modbus_rx_arg1);
				}

			if(modbus_rx_arg0==35)			//период повторов для 7-го канала
				{
				gran(&modbus_rx_arg1,1,1440);
	    		lc640_write_int(ADR_EE_AVT_PER_SET[6],modbus_rx_arg1);
				}

			if(modbus_rx_arg0==36)			//ток 8-го канала
				{
				gran(&modbus_rx_arg1,250,550);
	    		lc640_write_int(ADR_EE_I_SET[7],modbus_rx_arg1);
				}
			if(modbus_rx_arg0==37)			//время подъема 8-го канала
				{
				gran(&modbus_rx_arg1,5,60);
	    		lc640_write_int(ADR_EE_TP_SET[7],modbus_rx_arg1);
				}

			if(modbus_rx_arg0==38)			//время выдержки 8-го канала
				{
				gran(&modbus_rx_arg1,1,200);
	    		lc640_write_int(ADR_EE_TV_SET[7],modbus_rx_arg1);
				}

			if(modbus_rx_arg0==39)			//колличество повторов для 8-го канала
				{
				gran(&modbus_rx_arg1,1,99);
	    		lc640_write_int(ADR_EE_AVT_REP_SET[7],modbus_rx_arg1);
				}

			if(modbus_rx_arg0==40)			//период повторов для 8-го канала
				{
				gran(&modbus_rx_arg1,1,1440);
	    		lc640_write_int(ADR_EE_AVT_PER_SET[7],modbus_rx_arg1);
				}

			if(modbus_rx_arg0==41)			//Алгоритм работы
				{
				gran(&modbus_rx_arg1,1,2);
	    		lc640_write_int(EE_ALG_MODE,modbus_rx_arg1);
				}
				
			if(modbus_rx_arg0==42)			//Время задержки переключения каналов
				{
				gran(&modbus_rx_arg1,1,500);
	    		lc640_write_int(EE_TZPK,modbus_rx_arg1);
				}
				
			if(modbus_rx_arg0==43)			//Период повторов для 2-го алгоритма
				{
				gran(&modbus_rx_arg1,1,99);
	    		lc640_write_int(EE_REP_NUM_ALG_2,modbus_rx_arg1);
				}
				
			if(modbus_rx_arg0==44)			//Активный канал для 1-го алгоритма
				{
				gran(&modbus_rx_arg1,0,7);
	    		lc640_write_int(EE_AKT_CH_ALG_1,modbus_rx_arg1);
				}

			if(modbus_rx_arg0==45)			//Командный регистр
				{
				if(modbus_rx_arg1==0)	//стоп    
					{
					stop_process();
					wrk_pause_time=0;
					}
				else if((modbus_rx_arg1>=1)&&(modbus_rx_arg1<=8)) start_chanel_num(modbus_rx_arg1-1);
				else if(modbus_rx_arg1==22)
					{
					start_ALG2();
					}
				else if(modbus_rx_arg1==100)
					{
					start_test_process();
					}

	    		lc640_write_int(EE_AKT_CH_ALG_1,modbus_rx_arg1);
				}
																								
			modbus_tx_buff[0]=ADR;
			modbus_tx_buff[1]=modbus_func;
			modbus_tx_buff[2]=*((char*)&modbus_rx_arg0);
			modbus_tx_buff[3]=*(((char*)&modbus_rx_arg0)+1);
			modbus_tx_buff[4]=*((char*)&modbus_rx_arg1);
			modbus_tx_buff[5]=*(((char*)&modbus_rx_arg1)+1);
			
			crc_temp=CRC16_2(modbus_tx_buff,6);
			
			modbus_tx_buff[6]=(char)crc_temp;
			modbus_tx_buff[7]=crc_temp>>8;
		
			SET_REG(PINSEL1,0,(22-16)*2,2); //Вход EN у 485
			IO0DIR|=(1<<22);
			IO0SET|=(1<<22);
		
			for (i=0;i<8;i++)
				{
				putchar1(modbus_tx_buff[i]);
				}
			}

		else if(modbus_func==15)		//запись произвольного кол-ва битов хранения
			{
			unsigned short crc_temp;
			char i;
			unsigned short modbus_rx_arg[4];
			//modbus_hold_coils_transmit(ADR,modbus_func,modbus_rx_arg0,modbus_rx_arg1,0);
			
			//modbus_rx_arg[0]=((unsigned short)modbus_an_buffer[7]);//(((unsigned short)modbus_an_buffer[7])*((unsigned short)256))+((unsigned short)modbus_an_buffer[8]);

			//modbus_rtu_plazma[0]=modbus_rx_arg[0];

			if((modbus_rx_arg0==11)&&(modbus_rx_arg1==1))
				{
				if((modbus_rx_arg[0]==0x0001)&&(wrk_state!=wrkON))
					{
					
					//wrk_state=wrkON;
					//pwmI_start=POWER*9;
					//wrk_cnt_cnt=(signed long)T_WRK_MAX*300L;
					//lc640_write_int(EE_WRK_STAT,wrk_state);
					//ist=istT;
					//t_u_min1=-10;
					//t_u_min2=-10;
					}
				else if((modbus_rx_arg[0]==0)&&(wrk_state!=wrkOFF))
					{
					//wrk_state=wrkOFF;
					//lc640_write_int(EE_WRK_STAT,wrk_state);
					}


				modbus_tx_buff[0]=ADR;
				modbus_tx_buff[1]=modbus_func;
				modbus_tx_buff[2]=0;
				modbus_tx_buff[3]=11;
				modbus_tx_buff[4]=0;
				modbus_tx_buff[5]=modbus_rx_arg1;
				
				crc_temp=CRC16_2(modbus_tx_buff,6);
				
				modbus_tx_buff[6]=(char)crc_temp;
				modbus_tx_buff[7]=crc_temp>>8;
			
				SET_REG(PINSEL1,0,(22-16)*2,2); //Вход EN у 485
				IO0DIR|=(1<<22);
				IO0SET|=(1<<22);
			
				for (i=0;i<8;i++)
					{
					putchar1(modbus_tx_buff[i]);
					}
			
				}
			}

		else if(modbus_func==16) 	//запись регистра хранения
			{
			unsigned short crc_temp;
			char i;
			unsigned short modbus_rx_arg[4];
//modbus_rtu_plazma[2]++;

			modbus_rx_arg[0]=(((unsigned short)modbus_an_buffer[7])*((unsigned short)256))+((unsigned short)modbus_an_buffer[8]);
			modbus_rx_arg[1]=(((unsigned short)modbus_an_buffer[11])*((unsigned short)256))+((unsigned short)modbus_an_buffer[12]);

			if(modbus_rx_arg0==15) 		
				{
				if(modbus_rx_arg1>=1)
					{
					//if((modbus_rx_arg[0]>=10)&&(modbus_rx_arg[0]<=1000))
  					//lc640_write_int(EE_I_MAX,modbus_rx_arg[0]);
					}
				if(modbus_rx_arg1>=2)
					{
					//if((modbus_rx_arg[1]>=10)&&(modbus_rx_arg[1]<=7000))
	     			//lc640_write_int(EE_U_MAX,modbus_rx_arg[1]); 
					}
				}
			if(modbus_rx_arg0==17)		//напряжение максимальное
				{
				if(modbus_rx_arg1>=1)
					{
					//if((modbus_rx_arg[0]>=10)&&(modbus_rx_arg[0]<=7000))
	     			//lc640_write_int(EE_U_MAX,modbus_rx_arg[0]); 
					}
				}
				
			modbus_tx_buff[0]=ADR;
			modbus_tx_buff[1]=modbus_func;
			modbus_tx_buff[2]=*(((char*)&modbus_rx_arg0)+1);
			modbus_tx_buff[3]=*((char*)&modbus_rx_arg0);
			modbus_tx_buff[4]=*(((char*)&modbus_rx_arg1)+1);
			modbus_tx_buff[5]=*((char*)&modbus_rx_arg1);
			
			crc_temp=CRC16_2(modbus_tx_buff,6);
			
			modbus_tx_buff[6]=(char)crc_temp;
			modbus_tx_buff[7]=crc_temp>>8;
		
			SET_REG(PINSEL1,0,(22-16)*2,2); //Вход EN у 485
			IO0DIR|=(1<<22);
			IO0SET|=(1<<22);
		
			for (i=0;i<8;i++)
				{
				putchar1(modbus_tx_buff[i]);
				}
			}
		} 
	}
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




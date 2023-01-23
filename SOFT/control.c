#include "control.h"
#include <LPC21XX.H> 
#include "main.h"
#include "gran.h"
#include "25lc640.h"
#include "eeprom_map.h"
#include "pcf8563.h"
#include "tlv2542.h"
#include "beep.h"
#include "ad7705.h"
#include "common_func.h"

//-----------------------------------------------
//АПВ
unsigned main_apv_cnt,hour_apv_cnt[2],reset_apv_cnt[2];
unsigned short apv_cnt_sec[2],apv_cnt[2];
char apv_cnt_1;
short adc_buff_out_[3];

//-----------------------------------------------
//Управление источниками
char cnt_src[2];
signed short cntrl_stat=600,old_cntrl_stat,cntrl_stat1,cntrl_stat2;
signed short u_necc,u_necc_;
char cnt_blok;
enum_ibs Ibs=iiM,Ibso=iiM;
char bmm_cnt,bmp_cnt;
char bS=1;
signed char num_necc;
signed char cnt_num_necc;
char tzas_cnt;
char bBLOK;
signed long time,time_p,i_ch,pwm_ch,pwm_ch_max;



//-----------------------------------------------
//Математика
char bit_minus;

//-----------------------------------------------
//Звук
char cnt_beep;

//const char dumm[1000];

//-----------------------------------------------
//Температура

signed short t_sign_cnt,t_max_cnt;
enum_temper_state temper_state,temper_state_old;
char crazy_beep;			


//-----------------------------------------------
//Защита по снижению напряжения
signed short t_u_min1,t_u_min2;
char bSTOP_umin1,bSTOP_umin2;


//-----------------------------------------------
//Защита от теплового перегрева(интегратор тока и мощности)
signed long current_sigma=0;
char current_sigma_stat=0;
signed long power_sigma=0;
char power_sigma_stat=0;
signed long power_sigma_cnt_30min=0;

signed long plazma_long;


//-----------------------------------------------
//Повышенная мощность
signed short p_pov_cnt;


short pwm_U,pwm_I;

char bpwm_hndl_1gz;
char cnt_pwm_hndl_1gz;
char cnt_pwm_hndl_1gz_max;

//-----------------------------------------------
void cntrl_drv(void)
{
signed short temp_SS;
temp_SS=cntrl_stat1;
gran(&temp_SS,30,1010);
T0MR1=temp_SS;


temp_SS=cntrl_stat2;
gran(&temp_SS,30,1010);
T0MR3=temp_SS;


}


 //-----------------------------------------------
void av_out_hndl(void)
{
if(wrk_state==wrkOFF)
	{
	av_out_cnt=0;
	av_out_stat=avOFF;
	}
else 
	{
	if((Ires<100000UL)&&(U<200))
		{
		av_out_cnt++;
		if(av_out_cnt>95)av_out_stat=avON;
		}
	else 
		{
		av_out_cnt--;
		if(av_out_cnt<5)av_out_stat=avOFF;
		}

	}
gran(&av_out_cnt,0,100);
av_out_ind_cnt++;
gran_ring(&av_out_ind_cnt,0,100);
}

 //-----------------------------------------------
void kv_hndl(void)
{
/*if((wrk_state==wrkOFF)||(I_VK<10))
	{
	kv_cnt=0;
	av_kv_stat=avOFF;
	}
else 
	{
	if(Ires>(I_VK*100))
		{
		kv_cnt++;
		if(kv_cnt>18)av_kv_stat=avON;
		}
	else 
		{
		kv_cnt--;
		if(kv_cnt<2)av_kv_stat=avOFF;
		}

	}
//av_kv_stat=avON;
gran(&kv_cnt,0,20);
kv_ind_cnt++;
gran_ring(&kv_ind_cnt,0,100); */
}

//-----------------------------------------------
void out_drv(void)
{
SET_REG(IO1DIR,1,25,1);
if(	(av_kv_stat==avON) ||
	(temper_state!=tsNORM)||
	((Ires<100000UL)&&(U<200)))SET_REG(IO1CLR,1,25,1);
else SET_REG(IO1SET,1,25,1);
}

//-----------------------------------------------
void pwr_hndl(void)
{
if(ind==iK)
	{
	if(phase==0)
		{
		pwr_stat=stOFF;
		}
	else 
		{
		pwr_stat=stON;
		}
	}

else
	{
	if((wrk_stat==stON)||(wrk_stat==stTST))pwr_stat=stON;
	else pwr_stat=stOFF;
	} 
}

//-----------------------------------------------
void pwr_drv(void)
{
SET_REG(PINSEL1,0,(29-16)*2,2);
SET_REG(IO0DIR,1,29,1);
if(pwr_stat==stON)SET_REG(IO0CLR,1,29,1);
else SET_REG(IO0SET,1,29,1); 
//SET_REG(IO0CLR,1,29,1);
}



//-----------------------------------------------
void rele_hndl(void)
{
if((ind==iK)&&(phase==1))
	{
	rele_stat=0;
	}
else if(wrk_stat==stON)
	{
	rele_stat=wrk_ch;
	rele_vent_cnt=600;
	}
else if(wrk_stat==stTST)
	{
	rele_stat=0;
	rele_vent_cnt=600;
	}
else rele_stat=10;

if(rele_vent_cnt) rele_vent_cnt--;
}

//-----------------------------------------------
void rele_drv(void)
{
SET_REG(PINSEL0,0,(4)*2,2);
SET_REG(IO0DIR,1,4,1);

IO1DIR|=(1UL<<24)|(1UL<<25)|(1UL<<26)|(1UL<<29);
IO0DIR|=(1UL<<4);

if(rele_stat_const[rele_stat]&0x01)
	{
	IO0SET|=(1UL<<4);
	}
else 
	{
	IO0CLR|=(1UL<<4);
	}




if(rele_stat_const[rele_stat]&0x02)
	{
	IO1SET|=(1UL<<26);
	}
else 
	{
	IO1CLR|=(1UL<<26);
	}

if(rele_stat_const[rele_stat]&0x04)
	{
	IO1SET|=(1UL<<24);
	}
else 
	{
	IO1CLR|=(1UL<<24);
	}

if((rele_stat>=0)&&(rele_stat<=7))
	{
	IO1CLR|=(1UL<<25);
	}
else 
	{
	IO1SET|=(1UL<<25);
	}


if(rele_vent_cnt)
	{
	IO1SET|=(1UL<<29);
	}
else 
	{
	IO1CLR|=(1UL<<29);
	}


}

//-----------------------------------------------
void pwm_hndl(void)
{

if( ((Ii>Iset[wrk_ch])&&((Ii-Iset[wrk_ch])>=10)) || ((Ii<Iset[wrk_ch])&&((Iset[wrk_ch])-Ii>=10)) )	cnt_pwm_hndl_1gz_max=3;
else if( ((Ii>Iset[wrk_ch])&&((Ii-Iset[wrk_ch])>=5)) || ((Ii<Iset[wrk_ch])&&((Iset[wrk_ch])-Ii>=5)) )	cnt_pwm_hndl_1gz_max=5;
else cnt_pwm_hndl_1gz_max=25;

cnt_pwm_hndl_1gz++;
if(cnt_pwm_hndl_1gz>=cnt_pwm_hndl_1gz_max)
	{
	bpwm_hndl_1gz=1;
	cnt_pwm_hndl_1gz=0;
	}
if(ind==iK)
	{
	if(phase==0)
		{
		pwm_I=10;
		pwm_U=10;
		}
	else 
		{
		pwm_I=1000;
		pwm_U=100;
		}
	}

else
	{
	if(wrk_stat==stON)
		{
		

		i_ch=Iset[wrk_ch];
		//pwm_ch_max=(signed short)((925UL*(unsigned long)i_ch)/(unsigned long)(IMAX-50));
		pwm_ch_max=(signed short)((1000UL*(unsigned long)i_ch)/(unsigned long)(IMAX));
		time=wrk_time;
		time_p=Tpset[wrk_ch]*1000;
		if(time>=time_p)
			{
			if(bpwm_hndl_1gz)
				{
				bpwm_hndl_1gz=0;
				if(Ii_>Iset[wrk_ch])	pwm_I--;
				else if(Ii_<Iset[wrk_ch])	pwm_I++;
				gran(&pwm_I,pwm_ch_max-10,pwm_ch_max+10);
				} 
			}
		else if(time==time_p) pwm_I=pwm_ch_max;
		else 
			{
			//pwm_I=(signed short)(((signed long)(pwm_ch_max/2.2))+((((signed long)(pwm_ch_max/1.8))*((signed long)time))/((signed long)time_p)));
			pwm_I=((((signed long)(pwm_ch_max))*((signed long)time))/((signed long)time_p));
			}
		pwm_U=1000;
//		pwm_I=1000;
		}

	else if(wrk_stat==stTST)
		{
		pwm_I=plazma_pwm;
		pwm_U=1000;
		}
	else 
		{
		pwm_I=0;
		pwm_U=0;
		}
	} 
}

//-----------------------------------------------
void stop_process (void)
{
proc_stat=stOFF;
wrk_stat=stOFF;
wrk_time=0;
wrk_pause_time=0;
}

//-----------------------------------------------
char start_chanel_num (char num)
{
if(Konset[num])
	{
	wrk_time_max=(Tpset[num]+Tvset[num])*1000UL;
	wrk_remain_time=(Tpset[num]+Tvset[num])*1000UL;
	wrk_period_time=AVT_PER_SET[num]*60000UL;
	wrk_repit_cnt=AVT_REP_SET[num]-1;
	wrk_time=0;
	wrk_ch=num;
	ch_pending_start[num]=0;
	}
else return 0;

proc_stat=stCH;
wrk_stat=stON;
return 1;
}

//-----------------------------------------------
void start_chanel (void)
{
char i;

for(i=0;i<8;i++)
	{
	if(Konset[i])
		{
		wrk_time_max=(Tpset[i]+Tvset[i])*1000UL;
		wrk_remain_time=(Tpset[i]+Tvset[i])*1000UL;
		wrk_period_time=AVT_PER_SET[i]*60000UL;
		wrk_repit_cnt=AVT_REP_SET[i]-1;
		wrk_time=0;
		wrk_ch=i;
		ch_pending_start[i]=0;
		break;
		}
	}
proc_stat=stALG2;
wrk_stat=stON;
}


//-----------------------------------------------
void start_ALG2 (void)
{
char i;

for(i=0;i<8;i++)
	{
	if(Konset[i])
		{
		wrk_time_max=(Tpset[i]+Tvset[i])*1000UL;
		wrk_remain_time=wrk_time_max;//(Tpset[i]+Tvset[i])*1000UL;
		//wrk_period_time=AVT_PER_SET[i]*60000UL;
		//wrk_repit_cnt=AVT_REP_SET[i]-1;
		wrk_time=0;
		wrk_ch=i;
		//ch_pending_start[i]=0;
		break;
		}
	}

proc_rep=REP_NUM_ALG_2;
proc_stat=stALG2;
wrk_stat=stON;
}

//-----------------------------------------------
void start_single_ring_process (void)
{
char i;

for(i=0;i<8;i++)
	{
	if(Konset[i])ch_pending_start[i]=1;
	else ch_pending_start[i]=0;
	}

for(i=0;i<8;i++)
	{
	if(ch_pending_start[i])
		{
		wrk_time_max=(Tpset[i]+Tvset[i])*1000UL;
		wrk_remain_time=(Tpset[i]+Tvset[i])*1000UL;
		wrk_time=0;
		wrk_ch=i;
		ch_pending_start[i]=0;
		break;
		}
	}
proc_stat=stON;
wrk_stat=stON;
}

//-----------------------------------------------
void start_test_process (void)
{
char i;

	//	wrk_time_max=(Tpset[i]+Tvset[i])*1000UL;
	//	wrk_remain_time=(Tpset[i]+Tvset[i])*1000UL;
	//	wrk_time=0;
	//	wrk_ch=i;
		//ch_pending_start[i]=0;
		//break;
plazma_pwm=1000;

proc_stat=stTST;
wrk_stat=stTST;
}



//-----------------------------------------------
void pwm_drv(void)
{
gran(&pwm_I,10,1000);
T0MR1 = pwm_I;
T0MR3 = 100;//pwm_U; 
}

//-----------------------------------------------
void wrk_process (void)
{
if(proc_stat==stON)
	{
	if(wrk_stat==stON)
		{
		if(wrk_time<wrk_time_max)
			{
			wrk_time+=100;
			wrk_remain_time-=100;
			}
		else 
			{
			char i;

			wrk_stat=stOFF;

			for(i=0;i<8;i++)
				{
				if(ch_pending_start[i])
					{
					//next_wrk_ch=i;
					wrk_time_max=(Tpset[i]+Tvset[i])*1000UL;
					wrk_remain_time=(Tpset[i]+Tvset[i])*1000UL;
					wrk_time=0;
					wrk_ch=i;
					ch_pending_start[i]=0;
					wrk_pause_time=TZPK*1000UL;
					break;
					}
				}
			if(!wrk_pause_time)proc_stat=stOFF;
			}
		}
	else if(wrk_pause_time)
		{
		wrk_pause_time-=100;
		if(wrk_pause_time<=100)
			{
			wrk_stat=stON;
			}
		}

	}
else if(proc_stat==stCH)
	{
	if(wrk_stat==stON)
		{
		if(wrk_time<wrk_time_max)
			{
			wrk_time+=100;
			wrk_remain_time-=100;
			wrk_period_time-=100;
			}
		else 
			{
			char i;

			wrk_stat=stOFF;

			if(wrk_repit_cnt==0)
				{
				wrk_remain_time=0;
				wrk_period_time=0;
				wrk_time=0;
				proc_stat=stOFF;
				}
			}
		}
	else if(wrk_period_time)
		{
		wrk_period_time-=100;
		if(wrk_period_time<=100)
			{
			wrk_stat=stON;
			wrk_repit_cnt--;
			wrk_time_max=(Tpset[wrk_ch]+Tvset[wrk_ch])*1000UL;
			wrk_time=0;
			wrk_remain_time=(Tpset[wrk_ch]+Tvset[wrk_ch])*1000UL;
			wrk_period_time=AVT_PER_SET[wrk_ch]*60000UL;
			}
		}

	}

else if(proc_stat==stALG2)
	{
	if(wrk_stat==stON)
		{
		if(wrk_time<wrk_time_max)
			{
			wrk_time+=100;
			wrk_remain_time-=100;
			}
		else 
			{
			char i;

			wrk_stat=stOFF;

			if(wrk_ch==7)
				{
				if(proc_rep<=1)
					{
					wrk_stat=stOFF;
					proc_stat=stOFF;
					}
				else if(proc_rep>1)
					{
					wrk_pause_time=TZPK*1000UL;
					proc_rep--;
					for(i=0;i<8;i++)
						{
						if(Konset[i])
							{
							wrk_time_max=(Tpset[i]+Tvset[i])*1000UL;
							wrk_remain_time=wrk_time_max;//(Tpset[i]+Tvset[i])*1000UL;
							wrk_time=0;
							wrk_ch=i;
							//ch_pending_start[i]=0;
							break;
							}
						}
					}
				else 
					{
					wrk_stat=stOFF;
					proc_stat=stOFF;
					}
				}
			else 
				{
				
				for(i=wrk_ch+1;i<8;i++)
					{

					if(Konset[i])
						{
						//next_wrk_ch=i;
						wrk_time_max=(Tpset[i]+Tvset[i])*1000UL;
						wrk_remain_time=(Tpset[i]+Tvset[i])*1000UL;
						wrk_time=0;
						wrk_ch=i;
						//ch_pending_start[i]=0;
						wrk_pause_time=TZPK*1000UL;
						break;
						}
					}
				//
				if(!wrk_pause_time)
					{
					if(proc_rep<=1)
						{
						wrk_stat=stOFF;
						proc_stat=stOFF;
						}
					else 
						{
						proc_rep--;
						for(i=0;i<8;i++)
							{
							if(Konset[i])
								{
								wrk_time_max=(Tpset[i]+Tvset[i])*1000UL;
								wrk_remain_time=wrk_time_max;//(Tpset[i]+Tvset[i])*1000UL;
								wrk_time=0;
								wrk_ch=i;
								//ch_pending_start[i]=0;
								wrk_pause_time=TZPK*1000UL;
								break;
								}
							}
						}
					}
				}
			if(!wrk_pause_time)proc_stat=stOFF;
			}
		}
	else if(wrk_pause_time)
		{
		wrk_pause_time-=100;
		if(wrk_pause_time<=100)
			{
			wrk_stat=stON;
			wrk_pause_time=0;
			}
		}

	}

}




//-----------------------------------------------
void matemat (void)
{
signed long temp_SL;

temp_SL=(signed long)adc_buff_[0];
temp_SL*=K_U;
temp_SL/=2500L;
Ui=(signed short)temp_SL;

//Ui=1234;
						  
temp_SL=(signed long)ad7705_res1;//ad7705_buff_[0];
/*temp_SL-=(signed long)K0_I;
if(temp_SL<0)temp_SL=0;*/
temp_SL*=(signed long)K1_I;
temp_SL/=1000L;
Ii=(signed short)temp_SL;
//I=(signed short)adc_buff_[1]; 
//Ii=4567;

temp_SL=(signed long)ad7705_res1;
/*temp_SL-=(signed long)K0_I;
if(temp_SL<0)temp_SL=0;*/
temp_SL*=(signed long)K1_I;
temp_SL/=1000L;
Ii_=(signed short)temp_SL;

/*if((adc_buff_[12]>200)&&(adc_buff_[12]<1000))bat[0]._nd=0;
else bat[0]._nd=1;*/
temp_SL=(signed long)adc_buff_[5];
temp_SL*=K_T;
temp_SL/=5000L;
temp_SL-=273L;
Ti=(signed short)temp_SL;
}

//-----------------------------------------------
void u_min_drv(void)
{
/*
if(wrk_state==wrkON)
	{

	if((U<U_MIN1)&&(U_MIN1>=10)&&(U_MIN1<=100))
		{
		if(t_u_min1<(T_MIN1*10))
			{
			t_u_min1++;
			if(t_u_min1>=(T_MIN1*10))
				{
				bSTOP_umin1=1;
				}
			}
		}
	else 
		{
		if(t_u_min1<0)	t_u_min1++;
		else t_u_min1=0;
		}

	if((U<U_MIN2)&&(U_MIN2>=10)&&(U_MIN2<=300))
		{
		if(t_u_min2<(T_MIN2*10))
			{
			t_u_min2++;
			if(t_u_min2>=(T_MIN2*10))
				{
				bSTOP_umin2=1;
				}
			}
		}
	else 
		{
		if(t_u_min2<0)	t_u_min2++;
		else t_u_min2=0;
		}


	if(bSTOP_umin1)
		{
		wrk_state=wrkOFF;
		lc640_write_int(EE_WRK_STAT,wrk_state);
		tree_up(iSTOP_umin1,0,0,0);
		bSTOP_umin1=0;
		}

	if(bSTOP_umin2)
		{
		wrk_state=wrkOFF;
		lc640_write_int(EE_WRK_STAT,wrk_state);
		tree_up(iSTOP_umin2,0,0,0);
		bSTOP_umin2=0;
		}


	}

*/
}


//-----------------------------------------------
void temper_drv(void)
{
if((t>T_SIGN)&&(wrk_state==wrkON))
	{
	t_sign_cnt++;
	}
else if(t<(T_SIGN-5))
	{
	t_sign_cnt--;
	}

gran(&t_sign_cnt,0,10); 

if((t>T_MAX)&&(wrk_state==wrkON))
	{
	t_max_cnt++;
	}
else if(t<(T_MAX-5))
	{
	t_max_cnt--;
	}

gran(&t_max_cnt,0,10);


if(t_max_cnt>=9)temper_state=tsAV;
else if(t_max_cnt<=2)
	{
	if(t_sign_cnt>=9)temper_state=tsSIGN;
	else if(t_sign_cnt<=2)temper_state=tsNORM;
	}

if((temper_state!=temper_state_old)&&(temper_state==tsAV))
	{
	crazy_beep=1;
	}

if(crazy_beep)beep_init(0xffffffff,'R');
else if(temper_state==tsSIGN)beep_init(0x00000001,'R');
else beep_init(0x00000000,'R');

temper_state_old=temper_state;
}

//-----------------------------------------------
void time_drv(void)
{   
static char _5hz_cnt_;

hour_cnt_5hz++;
if(hour_cnt_5hz>=18000) hour_cnt_5hz=0;

_5hz_cnt_++;
if(_5hz_cnt_>=5)
	{
	_5hz_cnt_=0;
	hour_cnt_1hz++;
	if(hour_cnt_1hz>=3600)hour_cnt_1hz=0;
//	if(main_cnt<200) main_cnt++;
	
	cnt_ind++;
	if(cnt_ind>9)cnt_ind=0;
	}  


}



 

//-----------------------------------------------
void vent_drv(void)
{
static char vent_stat;


if((t>(T_SIGN-5)) ||  ((P>50)) ||  ((Ig>10000))) vent_stat=0;
else if((t<(T_SIGN-10)) && (P<45) && (Ig<9000)) vent_stat=1;

SET_REG(IO1DIR,1,29,1);
if(vent_stat==0)	SET_REG(IO1CLR,1,29,1);
else 			SET_REG(IO1SET,1,29,1);

}


//-----------------------------------------------
void adc_drv(void)
{
short temp_S;
char i;
if(GET_REG(ADDR,31,1))//ADDR_bit.DONE)
	{
	adc_buff[adc_ch][adc_cnt]=GET_REG(ADDR,6,10);//ADDR_bit.VVDDA;
	if((adc_cnt&0x03)==0)
		{
		temp_S=0;
		for(i=0;i<16;i++)
			{
			temp_S+=adc_buff[adc_ch][i];
			adc_buff_[adc_ch]=temp_S>>4;
			}
		}
	
	
	if(++adc_ch>=8) 
		{
		adc_ch=0;
		if(++adc_cnt>=16) 
			{
			adc_cnt=0;
			}
		}
	
	
	}

SET_REG(PINSEL1,1,(30-16)*2,2);

SET_REG(PINSEL1,0,(21-16)*2,2);
SET_REG(PINSEL1,0,(24-16)*2,2);
SET_REG(PINSEL1,0,(25-16)*2,2);


SET_REG(IO0DIR,1,21,1);
SET_REG(IO0DIR,1,24,1);
SET_REG(IO0DIR,1,25,1);



if(adc_ch&0x01)IO0SET|=((long)1UL<<21);
else IO0CLR|=((long)1UL<<21);
if(adc_ch&0x02)IO0SET|=((long)1UL<<24);
else IO0CLR|=((long)1UL<<24);
if(adc_ch&0x04)IO0SET|=((long)1UL<<25);
else IO0CLR|=((long)1UL<<25);

//ADCR_bit.PDN=1;
SET_REG(ADCR,1,21,1);
//ADCR_bit.CLKDIV=14;
SET_REG(ADCR,14,8,8);
//ADCR_bit.BURST=0;
SET_REG(ADCR,0,16,1);
//ADCR_bit.CLKS=0;
SET_REG(ADCR,0,17,3);
//ADCR_bit.TEST=0;
SET_REG(ADCR,0,22,2);
SET_REG(ADCR,8,0,8);//ADCR_bit.SEL=8;
//ADCR_bit.START=1;
SET_REG(ADCR,1,24,3);

}

//-----------------------------------------------
void current_integral_7_1000(void)
{
/*if(wrk_state==wrkON)
	{
	}
else 
	{
	current_sigma=0;
	}*/

if(Ires>6000000)
	{
	current_sigma+=((Ires/10000)-600);
	}
else if(Ires<6000000)
	{
	if(current_sigma>0)current_sigma-=(600-(Ires/10000));
	}

if(current_sigma<0) current_sigma=0;

if(current_sigma>=72000L)current_sigma_stat=1;
else if(current_sigma<=0L) current_sigma_stat=0; 
}


//-----------------------------------------------
void current_integral_7_200(void)
{


if(Ires>2000000)
	{
	current_sigma+=((Ires/10000)-200);
	}
else if(Ires<2000000)
	{
	if(current_sigma>0)current_sigma-=(200-(Ires/10000));
	}

if(current_sigma<0) current_sigma=0;

if(current_sigma>=54000L)current_sigma_stat=1;
else if(current_sigma<=0L) current_sigma_stat=0; 
}

//-----------------------------------------------
void power_integral_7_200(void)
{


if(P>200)
	{
	power_sigma+=((P)-200);
	}
else if(P<200)
	{
	if(power_sigma>0)power_sigma-=(200-(P));
	}

if(power_sigma<0) power_sigma=0;

if(power_sigma>=54000L)power_sigma_stat=1;
else if(power_sigma<=0L) power_sigma_stat=0; 


if((power_sigma>=54000L)&&(P<250))
	{
	power_sigma_cnt_30min++;
	if(power_sigma_cnt_30min>=1800)
		{
		power_sigma=0;
		power_sigma_cnt_30min=0;
		power_sigma_stat=0;
		}
	}
else power_sigma_cnt_30min=0;

}

//-----------------------------------------------
void p_pov_drv(void)
{

if(p_pov_cnt)p_pov_cnt--;
if(wrk_state==wrkOFF)p_pov_cnt=0;

}

//-----------------------------------------------
void i_pov_drv(void)
{

if(i_pov_cnt)
     {
     i_pov_cnt--;
     i_pov_cnt1=0;
     }

if(wrk_state==wrkOFF)
     {
     i_pov_cnt=0;
     i_pov_cnt1=0;
     }

if(wrk_state!=wrkOFF)
     {
     if(Ires>7000000L)
          {
          i_pov_cnt1++;
          if(i_pov_cnt1>180)
               {
               i_pov_cnt=600;
               }
          }
     else i_pov_cnt1=0;
     }

}


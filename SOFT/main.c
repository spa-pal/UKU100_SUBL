#include "main.h"
#include <LPC21XX.H> 
#include "timer.h" 
#include "beep.h"
#include "control.h"
#include "25lc640.h"
#include "eeprom_map.h"
#include "ret.h"
#include "pcf8563.h"
#include "tlv2542.h"
#include "simbols.h"
#include "graphic.h"
#include "gran.h"
#include "common_func.h"
#include "lcd_AGM1232_uku100.h"
#include "memo.h"
#include "beep.h"
#include "watchdog.h"
#include "ad7705_soft.h"
#include "uart1.h"
#include "curr_version.h"

const char*  name_of_blok="���-1000-220�/12�-55�";

//-----------------------------------------------
//������
char b1000Hz,b100Hz,b50Hz,b10Hz,b5Hz,b2Hz,b1Hz,b33Hz;
short t0cnt,t0cnt0,t0cnt1,t0cnt2,t0cnt3,t0cnt4,t0cnt5,t0cnt6,t0_cnt7,t0_cnt8;
char bFL5,bFL2,bFL,bFL_;

//-----------------------------------------------
//���
short adc_buff[8][16],adc_buff_[8],unet_buff[16],unet_buff_,adc_buff_U_;
char adc_cnt,adc_cnt1,adc_ch;


//-----------------------------------------------
//������������, ������������ �� EEPROM

signed short K_U;
signed short K_T;
signed short K_I_G[2];
signed short K_I_T[2];
signed short T_SIGN;
signed short T_MAX;
signed short I_MAX;
signed short PR;
signed short T_WRK;
signed short AVT_REP_SET[8];
signed short AVT_PER_SET[8];
enum_onoff ZV_AV_TEMPER;
enum_onoff ZV_AV_SRC;
signed short PU[10][5];
signed short PI[10][5];
signed short U_MAX;
signed short T_WRK_MAX;
signed short POWER;
signed short AKT_CH_ALG_1;
signed short REP_NUM_ALG_2;
enum_onoff 	REST;

//signed short T_MIN2;
signed short ADR;
//signed short MODE;
//signed short I_VK;
//enum_onoff 	auto_bl_kb;
//signed short PAROL_KEYS;
//signed short image_W=1;
//signed short P_POV;
//signed short T_POV;
signed short R_DOP;
signed short KOEF;
signed short BAUDRATE;
signed short MODBUS_TYPE;

char fl_simv_num,fl_simv_len;
char default_temp;
signed short av_j_si_max,av_j_si_max_;
char simax;
char phase;
const char sm_[]		={"                    "}; //
const char sm_exit[]	={" �����              "}; //
const char smon[]	={"���."}; 
const char smoff[]	={"���."};
const char sm_mont[13][4]={"���","���","���","���","���","���","���","���","���","���","���","���","���"}; //
char content[63];
unsigned short graph_out=0;
unsigned short capture=0;
unsigned short cont=0;
unsigned char low[488] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x3F, 0xFF, 0xFF, 0x00, 0x3F, 0xFF, 0xFF, 0x00, 0x3F, 0xFF, 0xFF, 0x00, 
	0x3F, 0xFF, 0xFF, 0x00, 0x3F, 0xFF, 0xFF, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 
	0x00, 0x0E, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x3F, 0xFF, 0xFF, 0x00, 0x3F, 0xFF, 0xFF, 0x00, 
	0x3F, 0xFF, 0xFF, 0x00, 0x3F, 0xFF, 0xFF, 0x00, 0x3F, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xFF, 0xFF, 0x00, 
	0x3F, 0xFF, 0xFF, 0x00, 0x3F, 0xFF, 0xFF, 0x00, 0x3F, 0xFF, 0xFF, 0x00, 0x3F, 0xFF, 0xFF, 0x00, 
	0x07, 0xF8, 0x00, 0x00, 0x01, 0xFE, 0x00, 0x00, 0x00, 0x7F, 0x80, 0x00, 0x00, 0x1F, 0xE0, 0x00, 
	0x00, 0x07, 0xF8, 0x00, 0x3F, 0xFF, 0xFF, 0x00, 0x3F, 0xFF, 0xFF, 0x00, 0x3F, 0xFF, 0xFF, 0x00, 
	0x3F, 0xFF, 0xFF, 0x00, 0x3F, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x70, 0x00, 0x0F, 0x80, 0x7C, 0x00, 0x1F, 0x80, 0x7E, 0x00, 
	0x1F, 0x80, 0x7E, 0x00, 0x3F, 0x80, 0x1F, 0x00, 0x3E, 0x0C, 0x0F, 0x00, 0x3C, 0x0C, 0x07, 0x00, 
	0x38, 0x0C, 0x07, 0x00, 0x38, 0x0E, 0x0F, 0x00, 0x3C, 0x1F, 0xFF, 0x00, 0x3F, 0xFF, 0xFF, 0x00, 
	0x1F, 0xFB, 0xFE, 0x00, 0x1F, 0xFB, 0xFC, 0x00, 0x0F, 0xF0, 0xF8, 0x00, 0x03, 0xE0, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xFF, 0xFF, 0x00, 
	0x3F, 0xFF, 0xFF, 0x00, 0x3F, 0xFF, 0xFF, 0x00, 0x3F, 0xFF, 0xFF, 0x00, 0x3F, 0xFF, 0xFF, 0x00, 
	0x00, 0x3F, 0xE0, 0x00, 0x00, 0x3F, 0xF0, 0x00, 0x00, 0xFF, 0xFC, 0x00, 0x07, 0xFF, 0xFF, 0x00, 
	0x3F, 0xFE, 0x7F, 0x00, 0x3F, 0xF8, 0x3F, 0x00, 0x3F, 0xC0, 0x0F, 0x00, 0x3F, 0x00, 0x03, 0x00, 
	0x3C, 0x00, 0x01, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x3F, 0xFF, 0xFF, 0x00, 0x3F, 0xFF, 0xFF, 0x00, 0x3F, 0xFF, 0xFF, 0x00, 0x3F, 0xFF, 0xFF, 0x00, 
	0x3F, 0xFF, 0xFF, 0x00, 0x07, 0xF8, 0x00, 0x00, 0x01, 0xFE, 0x00, 0x00, 0x00, 0x7F, 0x80, 0x00, 
	0x00, 0x1F, 0xE0, 0x00, 0x00, 0x07, 0xF8, 0x00, 0x3F, 0xFF, 0xFF, 0x00, 0x3F, 0xFF, 0xFF, 0x00, 
	0x3F, 0xFF, 0xFF, 0x00, 0x3F, 0xFF, 0xFF, 0x00, 0x3F, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xFF, 0xFF, 0x00, 
	0x3F, 0xFF, 0xFF, 0x00, 0x3F, 0xFF, 0xFF, 0x0C, 0x3F, 0xFF, 0xFF, 0x1C, 0x3F, 0xFF, 0xFF, 0x3C, 
	0x07, 0xF8, 0x00, 0x38, 0x01, 0xFE, 0x00, 0x30, 0x00, 0x7F, 0x80, 0x30, 0x00, 0x1F, 0xE0, 0x30, 
	0x00, 0x07, 0xF8, 0x38, 0x3F, 0xFF, 0xFF, 0x3C, 0x3F, 0xFF, 0xFF, 0x1C, 0x3F, 0xFF, 0xFF, 0x0C, 
	0x3F, 0xFF, 0xFF, 0x00, 0x3F, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
unsigned char vacuum[488] = {
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 0xFF, 0xC0, 0x00, 0x00, 0xFF, 0xC0, 0x00, 0x00, 0xFF, 
	0xC0, 0x00, 0x00, 0xFF, 0xC0, 0x00, 0x00, 0xFF, 0xC7, 0xF1, 0xF8, 0xFF, 0xC7, 0xF1, 0xF8, 0xFF, 
	0xC7, 0xF0, 0xF0, 0xFF, 0xC3, 0xE0, 0x00, 0xFF, 0xC0, 0x00, 0x00, 0xFF, 0xC0, 0x04, 0x00, 0xFF, 
	0xE0, 0x04, 0x01, 0xFF, 0xF0, 0x0F, 0x07, 0xFF, 0xF8, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xCF, 0xFF, 0xFF, 0xFF, 0xC0, 0x7F, 0xFF, 0xFF, 0xC0, 0x03, 0xFF, 0xFF, 
	0xC0, 0x00, 0x1F, 0xFF, 0xC0, 0x00, 0x01, 0xFF, 0xF0, 0x00, 0x00, 0xFF, 0xFE, 0x00, 0x00, 0xFF, 
	0xFE, 0x38, 0x00, 0xFF, 0xFE, 0x3F, 0xC0, 0xFF, 0xFE, 0x38, 0x00, 0xFF, 0xFE, 0x00, 0x00, 0xFF, 
	0xF0, 0x00, 0x00, 0xFF, 0xC0, 0x00, 0x01, 0xFF, 0xC0, 0x00, 0x3F, 0xFF, 0xC0, 0x03, 0xFF, 0xFF, 
	0xC0, 0x7F, 0xFF, 0xFF, 0xCF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xC0, 0x00, 0x00, 0xFF, 0xC0, 0x00, 0x00, 0xFF, 0xC0, 0x00, 0x00, 0xFF, 0xC0, 0x00, 0x00, 0xFF, 
	0xC0, 0x00, 0x00, 0xFF, 0xFF, 0xC0, 0x1F, 0xFF, 0xFF, 0xC0, 0x0F, 0xFF, 0xFF, 0x00, 0x03, 0xFF, 
	0xF8, 0x00, 0x00, 0xFF, 0xC0, 0x01, 0x80, 0xFF, 0xC0, 0x07, 0xC0, 0xFF, 0xC0, 0x3F, 0xF0, 0xFF, 
	0xC0, 0xFF, 0xFC, 0xFF, 0xC3, 0xFF, 0xFE, 0xFF, 0xDF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 
	0xFF, 0xFF, 0xF0, 0xFF, 0xC7, 0xFF, 0xC0, 0xFF, 0xC7, 0xFE, 0x00, 0xFF, 0xC7, 0xF0, 0x00, 0xFF, 
	0xC0, 0x00, 0x01, 0xFF, 0xC0, 0x00, 0x07, 0xFF, 0xE0, 0x00, 0x3F, 0xFF, 0xF0, 0x00, 0x0F, 0xFF, 
	0xFE, 0x00, 0x01, 0xFF, 0xFF, 0xC0, 0x00, 0xFF, 0xFF, 0xF8, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 
	0xFF, 0xFF, 0xE0, 0xFF, 0xFF, 0xFF, 0xFC, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xF0, 0xFF, 
	0xC7, 0xFF, 0xC0, 0xFF, 0xC7, 0xFE, 0x00, 0xFF, 0xC7, 0xF0, 0x00, 0xFF, 0xC0, 0x00, 0x01, 0xFF, 
	0xC0, 0x00, 0x07, 0xFF, 0xE0, 0x00, 0x3F, 0xFF, 0xF0, 0x00, 0x0F, 0xFF, 0xFE, 0x00, 0x01, 0xFF, 
	0xFF, 0xC0, 0x00, 0xFF, 0xFF, 0xF8, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xE0, 0xFF, 
	0xFF, 0xFF, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 0xFF, 
	0xC0, 0x00, 0x00, 0xFF, 0xC0, 0x00, 0x00, 0xFF, 0xC0, 0x00, 0x00, 0xFF, 0xFF, 0xFE, 0x00, 0xFF, 
	0xFF, 0xE0, 0x00, 0xFF, 0xFE, 0x00, 0x01, 0xFF, 0xE0, 0x00, 0x1F, 0xFF, 0xC0, 0x01, 0xFF, 0xFF, 
	0xC0, 0x1F, 0xFF, 0xFF, 0xC0, 0x07, 0xFF, 0xFF, 0xE0, 0x00, 0x7F, 0xFF, 0xFE, 0x00, 0x07, 0xFF, 
	0xFF, 0xE0, 0x00, 0xFF, 0xFF, 0xFE, 0x00, 0xFF, 0xC0, 0x00, 0x00, 0xFF, 0xC0, 0x00, 0x00, 0xFF, 
	0xC0, 0x00, 0x00, 0xFF, 0xC0, 0x00, 0x00, 0xFF, 0xC0, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

 



//-----------------------------------------------
//�����
unsigned short hour_cnt_5hz,hour_cnt_1hz;
char cnt_ind;

//-----------------------------------------------
//�������
unsigned short rotor_can[6];
unsigned short cnt_sec;

//-----------------------------------------------
//������
char speed,l_but,n_but;
char but;                            
unsigned long but_n,but_s;
char but0_cnt;
char but1_cnt;
char but_onL_temp;
unsigned short count_but_idle=1000;

//-----------------------------------------------
//��������� ������ �������
signed short U,t,Ig_u,Usim,P,Pmax;
signed int It;
enum_ist ist=istG;
signed long Ig,Ires;
signed long Idop;

char dummy_dumm[100];
//-----------------------------------------------
//���������� ��������
enum_wrk_state wrk_state=wrkOFF;
signed short pwmI,pwmU,pwmI_start,pwmI_max;
signed long wrk_cnt;
char wrk_phase;
signed short Isim;
signed long wrk_cnt_cnt;
signed short i_pov_cnt,i_pov_cnt1;
char bRESET;

// char dumm @1000;

char plazma_plazma;
char plazma_ind;
char plazma,plazma_a;
char cnt_lcd_init;

//-----------------------------------------------
//�������� �������   
signed short kv_cnt;
enum_av_kv_stat av_kv_stat; 
signed short kv_ind_cnt;

//-----------------------------------------------
//�������� ������
signed short av_out_cnt;
enum_av_kv_stat av_out_stat;
signed short av_out_ind_cnt;

//**********************************************
//���������� ����������
enum_onoff pwr_stat;

char ch_pending_start[8]={0,0,0,0,0,0,0,0};

//***********************************************
//���������
volatile char ptr_ind=0;
stuct_ind a,b[10];
char lcd_buffer_dumm1[50];
char lcd_buffer[LCD_SIZE]={"Hello World"};
char lcd_buffer_dumm2[50];
char lcd_bitmap[1024];
char dig[5];
char zero_on;

const char ABCDEF[]={"0123456789ABCDEF"};
char parol[3];
char phase;

//**********************************************
//��������� � ������������ �� ������
signed short T_MAX; 
enum_onoff AVT_TEN_SWTCH;
signed short T_TEN_SWTCH;
enum_onoff ZV_AV_TEN;
enum_onoff ZV_AV_ALL_TEN;
enum_onoff ZV_AV_TEMPER;
signed short K_U,K0_I,K1_I,K_T;
signed short WRK_DURATION;
signed short WRK_TIME;
signed short I_SET;
signed short TEN_SET;
signed short Iset[10];
signed short Tpset[10];
signed short Tvset[10];
signed short Kvset[10];
signed short Konset[10];
signed short AVT_ON;
signed short ALG_MODE;
signed short TZPK;
signed short IAVAR;
signed short IMAX;
signed short PRL_PROG;
signed short AVT_ON_TUMBLER;

//**********************************************
//���������� ���������
enum_onoff wrk_stat;
enum_onoff proc_stat;
signed short wrk_time_cnt_max;
signed short wrk_time_cnt;
signed short wrk_remain_cnt;
signed short pwm_i_temp;
signed long duration_time_cnt;
signed long wrk_time_max;
signed long wrk_time;
signed long wrk_remain_time;
signed long wrk_pause_time;
signed long wrk_period_time;
signed short wrk_repit_cnt;
signed short wrk_ch;
signed short proc_rep;

signed short rele_vent_cnt;

//**********************************************
//��������� ������
signed short Ui,Ii,Ii_,Ti;
char rele_stat;
const char rele_stat_const[8]={1,0,3,2,5,7,6,4};

short plazma_pwm;

//char plazma_pal;

//**********************************************
//����������� ���������� ������
char strIng[20];
short strIng_ptr_start, strIng_ptr_end, strIng_len, strIng_ptr_komma;
short chNumber;
short setData;
//-----------------------------------------------
char pal_strspn(char* in, char inchar)
{
char inlen;
char out=0;
inlen=strlen(in);

do 
	{
	out++;
	if(out>=inlen)
		{
		out=255;
		break;
		}
	}
while(/*(out<inlen)&*/(in[out]!=inchar));

return out;
}
//-----------------------------------------------
void bitmap_hndl(void)
{
short x,ii,i;
unsigned int ptr_bitmap;

for(ii=0;ii<488;ii++)
	{
	lcd_bitmap[ii]=0x00;
	}

	{
	for(i=0;i<4;i++)
		{
		ptr_bitmap=122*(unsigned)i;
		for(x=(20*i);x<((20*i)+20);x++)
	 		{
			lcd_bitmap[ptr_bitmap++]=caracter[(unsigned)lcd_buffer[x]*6];
			lcd_bitmap[ptr_bitmap++]=caracter[((unsigned)lcd_buffer[x]*6)+1];
			lcd_bitmap[ptr_bitmap++]=caracter[((unsigned)lcd_buffer[x]*6)+2];
			lcd_bitmap[ptr_bitmap++]=caracter[((unsigned)lcd_buffer[x]*6)+3];
			lcd_bitmap[ptr_bitmap++]=caracter[((unsigned)lcd_buffer[x]*6)+4];
			lcd_bitmap[ptr_bitmap++]=caracter[((unsigned)lcd_buffer[x]*6)+5];
			} 
		}
	}	
}

//-----------------------------------------------
void ind_hndl(void)
{
const char* ptrs[30];   
static ind_hndl_cnt;
ind_hndl_cnt++;

if(ind==iMn)
	{
	if((wrk_stat==stON)&&(proc_stat==stALG2))
		{
		ptrs[0]=	" I =   (A  U =   )B ";
		ptrs[1]=	" �������� ������  [ ";
		ptrs[2]=	" T���.     =    <c. ";
		ptrs[3]=	" T����.    =    >�. ";
		ptrs[4]=	" t =   +�C          ";


		//if((sub_ind-index_set)>1)index_set=sub_ind-1;
		//else if(sub_ind<index_set)index_set=sub_ind;
			
		bgnd_par(	"  !/@/#/$/%/^/&/*   ",
					ptrs[index_set],
					ptrs[index_set+1],
					ptrs[index_set+2]);

		if(!Konset[0]||((wrk_ch==0)&&bFL2))sub_bgnd(" ",'!',0);
		else int2lcd(1,'!',0);

		if(!Konset[1]||((wrk_ch==1)&&bFL2))sub_bgnd(" ",'@',0);
		else int2lcd(2,'@',0);

		if(!Konset[2]||((wrk_ch==2)&&bFL2))sub_bgnd(" ",'#',0);
		else int2lcd(3,'#',0);

		if(!Konset[3]||((wrk_ch==3)&&bFL2))sub_bgnd(" ",'$',0);
		else int2lcd(4,'$',0);

		if(!Konset[4]||((wrk_ch==4)&&bFL2))sub_bgnd(" ",'%',0);
		else int2lcd(5,'%',0);

		if(!Konset[5]||((wrk_ch==5)&&bFL2))sub_bgnd(" ",'^',0);
		else int2lcd(6,'^',0);

		if(!Konset[6]||((wrk_ch==6)&&bFL2))sub_bgnd(" ",'&',0);
		else int2lcd(7,'&',0);

		if(!Konset[7]||((wrk_ch==7)&&bFL2))sub_bgnd(" ",'*',0);
		else int2lcd(8,'*',0);

		int2lcd(Ii,'(',1);
		int2lcd(Ui,')',1);
		int2lcd(Ti,'+',0);
		//int2lcd(wrk_ch+1,'!',0);
		//int2lcd(wrk_ch+1,'!',0);
		int2lcd(wrk_time/1000,'<',0);
		int2lcd(wrk_remain_time/1000,'>',0);
		int2lcd(proc_rep-1,'[',0);

		//int2lcdyx(ad7705_buff_[0],0,19,0);
		
		
		}

	else if((wrk_stat==stON)&&(proc_stat==stON))
		{
		ptrs[0]=	" I =   (A  U =   )B ";
		ptrs[1]=	" T���.     =    <c. ";
		ptrs[2]=	" T����.    =    >�. ";
		ptrs[3]=	" t =   +�C          ";


		//if((sub_ind-index_set)>1)index_set=sub_ind-1;
		//else if(sub_ind<index_set)index_set=sub_ind;
			
		bgnd_par(	"  !/@/#/$/%/^/&/*   ",
					ptrs[index_set],
					ptrs[index_set+1],
					ptrs[index_set+2]);

		if(!Konset[0]||((wrk_ch==0)&&bFL2))sub_bgnd(" ",'!',0);
		else int2lcd(1,'!',0);

		if(!Konset[1]||((wrk_ch==1)&&bFL2))sub_bgnd(" ",'@',0);
		else int2lcd(2,'@',0);

		if(!Konset[2]||((wrk_ch==2)&&bFL2))sub_bgnd(" ",'#',0);
		else int2lcd(3,'#',0);

		if(!Konset[3]||((wrk_ch==3)&&bFL2))sub_bgnd(" ",'$',0);
		else int2lcd(4,'$',0);

		if(!Konset[4]||((wrk_ch==4)&&bFL2))sub_bgnd(" ",'%',0);
		else int2lcd(5,'%',0);

		if(!Konset[5]||((wrk_ch==5)&&bFL2))sub_bgnd(" ",'^',0);
		else int2lcd(6,'^',0);

		if(!Konset[6]||((wrk_ch==6)&&bFL2))sub_bgnd(" ",'&',0);
		else int2lcd(7,'&',0);

		if(!Konset[7]||((wrk_ch==7)&&bFL2))sub_bgnd(" ",'*',0);
		else int2lcd(8,'*',0);

		int2lcd(Ii,'(',1);
		int2lcd(Ui,')',1);
		int2lcd(Ti,'+',0);
		//int2lcd(wrk_ch+1,'!',0);
		//int2lcd(wrk_ch+1,'!',0);
		int2lcd(wrk_time/1000,'<',0);
		int2lcd(wrk_remain_time/1000,'>',0);

		//int2lcdyx(ad7705_buff_[0],0,19,0);
		//int2lcdyx(pwm_I,3,5,0);
		//int2lcdyx(pwm_ch_max,3,15,0);
		}
	else if((wrk_stat==stON)&&(proc_stat==stCH))
		{
		ptrs[0]=	" I =   (A  U =   )B ";
		ptrs[1]=	" T���.     =    <c. ";
		ptrs[2]=	" T����.    =    >�. ";
		ptrs[3]=	" t =   +�C          ";


		//if((sub_ind-index_set)>1)index_set=sub_ind-1;
		//else if(sub_ind<index_set)index_set=sub_ind;
			
		bgnd_par(	"����� N! ������   @ ",
					ptrs[index_set],
					ptrs[index_set+1],
					ptrs[index_set+2]);


		int2lcd(wrk_ch+1,'!',0);
	    	int2lcd(wrk_repit_cnt,'@',0);
		int2lcd(Ii,'(',1);
		int2lcd(Ui,')',1);
		int2lcd(Ti,'+',0);
		//int2lcd(wrk_ch+1,'!',0);
		//int2lcd(wrk_ch+1,'!',0);
		int2lcd(wrk_time/1000,'<',0);
		int2lcd(wrk_remain_time/1000,'>',0);

		//int2lcdyx(ad7705_buff_[0],0,19,0);
		
		//int2lcdyx(pwm_ch_max,3,15,0);
		}
	else if((wrk_stat==stOFF)&&(proc_stat==stCH))
		{
		bgnd_par(	"   �� ���������     ",
					"     ������ �!      ",
					" �������� ������  $ ",
					"                    ");
		int2lcd(wrk_ch+1,'!',0);
		int2lcd((wrk_period_time/1000UL),'$',0);
		}
	else if(wrk_pause_time)
		{
		bgnd_par(	"   �� ���������     ",
					"     ������ �!      ",
					"      ��������      ",
					"     ������  $      ");

		int2lcd(wrk_ch+1,'!',0);
		int2lcd((wrk_pause_time/1000UL)+1,'$',0);
		}

	else if(proc_stat==stOFF)
		{

		if(ALG_MODE==1)
			{
			if((!Konset[0])&&(!Konset[1])&&(!Konset[2])&&(!Konset[3])&&(!Konset[4])&&(!Konset[5])&&(!Konset[6])&&(!Konset[7]))
				{
				ptrs[0]=	"     ��� ������     ";
				ptrs[1]=	"    ���������!!!    ";
				}
			else 
				{
				ptrs[0]=	"  �������� �����: [ ";
				ptrs[1]=	"                    ";
				}
			ptrs[2]=	" ��������� (���. �1)";
			}

		else
			{
			ptrs[0]=	"  �������� ������:  ";
			ptrs[1]=	"  !/@/#/$/%/^/&/*   ";
			ptrs[2]=	" ��������� (���. �2)";
			}
		 
		ptrs[3]=	" ����������������   ";
		ptrs[4]=	" ���������          ";
		ptrs[5]=	" ������ ��          ";
		ptrs[6]=	" ����������         ";

		if((sub_ind-index_set)>1)index_set=sub_ind-1;
		else if(sub_ind<index_set)index_set=sub_ind;
			
		bgnd_par(	ptrs[0],
					ptrs[1],
					ptrs[index_set+2],
					ptrs[index_set+3]);
		
		pointer_set(2);
		if(Konset[0])int2lcd(1,'!',0);
		else sub_bgnd(" ",'!',0);
		if(Konset[1])int2lcd(2,'@',0);
		else sub_bgnd(" ",'@',0);
		if(Konset[2])int2lcd(3,'#',0);
		else sub_bgnd(" ",'#',0);
		if(Konset[3])int2lcd(4,'$',0);
		else sub_bgnd(" ",'$',0);
		if(Konset[4])int2lcd(5,'%',0);
		else sub_bgnd(" ",'%',0);
		if(Konset[5])int2lcd(6,'^',0);
		else sub_bgnd(" ",'^',0);
		if(Konset[6])int2lcd(7,'&',0);
		else sub_bgnd(" ",'&',0);
		if(Konset[7])int2lcd(8,'*',0);
		else sub_bgnd(" ",'*',0);

		int2lcd(AKT_CH_ALG_1+1,'[',0);
		}

	else if(proc_stat==stTST)
		{
		ptrs[0]=	"������������ �����N1";
		ptrs[1]=	" I =   (A  U =   )B ";
		ptrs[2]=	" ���       =    <   ";
		ptrs[3]=	"                    ";
	
		//if((sub_ind-index_set)>1)index_set=sub_ind-1;
		//else if(sub_ind<index_set)index_set=sub_ind;
			
		bgnd_par(	ptrs[0],
					ptrs[1],
					ptrs[2],
					ptrs[3]);


		int2lcd(Ii,'(',1);
		int2lcd(Ui,')',1);
		int2lcd(plazma_pwm,'<',0);
		//int2lcd(wrk_ch+1,'!',0);
		//int2lcd(wrk_ch+1,'!',0);
		int2lcd(wrk_time/1000,'<',0);
		int2lcd(wrk_remain_time/1000,'>',0);

		//int2lcdyx(ad7705_buff_[0],0,19,0);
		//int2lcdyx(pwm_I,3,5,0);
		//int2lcdyx(pwm_ch_max,3,15,0);
		}


/*	else
	ptrs[0]=	" N!  �� ����. 0$:0% ";

	if(wrk_stat==stOFF)
	ptrs[1]=	" I=    ^�           ";
	else
	ptrs[1]=	" I=    +� U=    &�  ";

	ptrs[2]=	" ����� ���.   (���. ";
	if(WRK_DURATION<60)
	ptrs[3]=	" ������       )���. ";
	else
	ptrs[3]=	" ������       )�.   ";
	ptrs[4]=	" ������ ��������    ";	
	ptrs[5]=	" ���������          ";
	ptrs[6]=	" ����������         ";

	if((sub_ind-index_set)>3)index_set=sub_ind-3;
	else if(sub_ind<index_set)index_set=sub_ind;
	bgnd_par(	ptrs[index_set],
			ptrs[index_set+1],
			ptrs[index_set+2],
			ptrs[index_set+3]);

	pointer_set(0);

	int2lcd(TEN_SET+1,'!',0);
	int2lcd(I_SET,'^',1);
	int2lcd(I,'+',1);
	int2lcd(U,'&',1);
	//int2lcd(WRK_TIME/30,'*',0);
	int2lcd(WRK_TIME,'(',1);
	if(WRK_DURATION==0)
	sub_bgnd("����.",')',0);
	else if(WRK_DURATION<60)
	int2lcd(WRK_DURATION,')',0);
	else 
	int2lcd(WRK_DURATION/60,')',0);	 

	int2lcd(wrk_remain_cnt/60,'$',0);
	int2lcd(wrk_remain_cnt%60,'%',0);	

	int2lcd((signed short)(duration_time_cnt/60),'@',0);
	int2lcd((signed short)(duration_time_cnt%60),'#',0);

	if((proc_stat==stON)&&bFL2)lcd_buffer[find(':')]=' '; */

/*	int2lcdyx(wrk_ch,0,0,0);
	int2lcdyx(proc_rep,0,19,0); 
	int2lcdyx(proc_stat,1,1,0); */
	//int2lcdyx(rxrxrx,3,5,0); 
	//int2lcdyx(txtxtx,3,10,0);
	//int2lcdyx(pwm_ch_max,0,5,0);
	//int2lcdyx(pwm_I,0,10,0);
	///int2lcdyx(plazma,0,3,0);
	///int2lcdyx(plazma_a,0,7,0);
	///int2lcdyx(rxrxrx,0,11,0);
	}

else if(ind==iSet)
	{
	ptrs[0]=	" ��������         # ";
//	ptrs[1]=	" ����             # ";
	ptrs[1]=	" �������� ���������-";
	ptrs[2]=	" ��� �������     $�.";
//	ptrs[3]=	" ��������� ���    !�";
	ptrs[3]=	" ���������� ��������";
	ptrs[4]=	" ��� ���.-��� �2  @ ";
	ptrs[5]=	" MODBUS       ASCII ";
	if(MODBUS_TYPE==1)
	ptrs[5]=	" MODBUS         RTU ";
	ptrs[6]=	" MODBUS ADDRESS  >h ";
	ptrs[7]=	" MODBUS BAUDRATE    "; 
	ptrs[8]=	"                 Y00";
	ptrs[9]=	" �����              ";	  	 	

	
	if((sub_ind-index_set)>2)index_set=sub_ind-2;
	else if(sub_ind<index_set)index_set=sub_ind;
	bgnd_par(	"     ���������      ",
			ptrs[index_set],
			ptrs[index_set+1],
			ptrs[index_set+2]);

	pointer_set(1);


	int2lcd(ALG_MODE,'#',0);
	int2lcd(REP_NUM_ALG_2,'@',0);
	//if(AVT_ON)sub_bgnd("���.",'#',-2);
	//else sub_bgnd("���.",'#',-2);
	int2lcd(TZPK,'$',0);
	//int2lcd(IAVAR,'!',1);
	//if(PRL_PROG)sub_bgnd("���.",'@',-2);
	//else sub_bgnd("���.",'@',-2);

	char2lcdh((char)ADR,'>');
	int2lcd(BAUDRATE,'Y',0);

/*	int2lcd(T_SIGN,'!',0);
	int2lcd(T_MAX,'@',0);
	if(AVT_TEN_SWTCH==stON)sub_bgnd("���.",'#',-2);
	else sub_bgnd("���.",'#',-2);
	int2lcd(T_TEN_SWTCH,'$',0);
	if(ZV_AV_TEN==stON)sub_bgnd("���.",'%',-2);
	else sub_bgnd("���.",'%',-2);
	if(ZV_AV_ALL_TEN==stON)sub_bgnd("���.",'^',-2);
	else sub_bgnd("���.",'^',-2);
	if(ZV_AV_TEMPER==stON)sub_bgnd("���.",'&',-2);
	else sub_bgnd("���.",'&',-2);*/
	
	//int2lcdyx(lc640_read_int(0x0f),0,2,0);
	//int2lcdyx(sub_ind,0,2,0);
	//int2lcdyx(index_set,0,18,0);
		
	}

else if(ind==iProg)
	{
	ptrs[0]=	" ����� �1           ";
	ptrs[1]=	" ����� �2           ";
	ptrs[2]=	" ����� �3           ";
	ptrs[3]=	" ����� �4           ";
	ptrs[4]=	" ����� �5           ";
	ptrs[5]=	" ����� �6           ";
	ptrs[6]=	" ����� �7           ";
	ptrs[7]=	" ����� �8           ";
	ptrs[8]=	" �����              ";	  	

	
	if((sub_ind-index_set)>2)index_set=sub_ind-2;
	else if(sub_ind<index_set)index_set=sub_ind;
	bgnd_par(	"  ����������������  ",
			ptrs[index_set],
			ptrs[index_set+1],
			ptrs[index_set+2]);
	pointer_set(1);					
	}

else if(ind==iProg_)
	{
	if(Konset[sub_ind1])
		{
		ptrs[0]=	" �������            "; 
		}
	else 
		{
		ptrs[0]=	" ��������           ";
		}
	ptrs[1]=	" I!       @�        ";
	ptrs[2]=	" T�#      $�.       ";
	ptrs[3]=	" T�%      ^�.       ";
	ptrs[4]=	" ������        [���.";
	ptrs[5]=	" ������    (�.0)���.";
	ptrs[6]=	" �����              ";	  	
	ptrs[7]=	"                    ";

	if(AVT_REP_SET[sub_ind1]==1)
		{
		ptrs[5]=	" �����              ";	  	
		ptrs[6]=	"                    ";
		ptrs[7]=	"                    ";
		//sub_bgnd("����������",'[',-5);
		}
	
	if((sub_ind-index_set)>2)index_set=sub_ind-2;
	else if(sub_ind<index_set)index_set=sub_ind;
	bgnd_par(	"      ����� �+      ",
			ptrs[index_set],
			ptrs[index_set+1],
			ptrs[index_set+2]);
	pointer_set(1);
	if(AVT_REP_SET[sub_ind1]==1)
		{
		sub_bgnd("����������",'[',-5);
		}
	int2lcd(sub_ind1+1,'+',0);
	int2lcd(sub_ind1+1,'#',0);
	int2lcd(sub_ind1+1,'%',0);
	int2lcd(sub_ind1+1,'&',0);
	int2lcd(sub_ind1+1,'!',0);
	int2lcd(Iset[sub_ind1],'@',1);
	int2lcd(Tpset[sub_ind1],'$',0);
	int2lcd(Tvset[sub_ind1],'^',0);
	int2lcd(AVT_REP_SET[sub_ind1],'[',0);
	//int2lcd(AVT_PER_SET[sub_ind1],']',0);
	int2lcd(AVT_PER_SET[sub_ind1]/60,'(',0);
	int2lcd(AVT_PER_SET[sub_ind1]%60,')',0);
	//int2lcd(T,'#',0);
	
	//int2lcdyx(sub_ind1,0,1,0);
	//int2lcdyx(Tpset[0],0,3,0);
						
	}

else if(ind==iK)
	{
	ptrs[0]=	" U���            !� ";
	ptrs[1]=	" I���            @� ";
	ptrs[2]=	" T���           #�C " ;
	ptrs[3]=	" ������������ ���   ";
	ptrs[4]=	" ���������        %�";
	ptrs[5]=	" �����              ";
	
	if((sub_ind-index_set)>2)index_set=sub_ind-2;
	else if(sub_ind<index_set)index_set=sub_ind;
	bgnd_par(	"    ����������      ",
			ptrs[index_set],
			ptrs[index_set+1],
			ptrs[index_set+2]);

	pointer_set(1);

	int2lcd(Ui,'!',1);
	int2lcd(Ii,'@',1);
	int2lcd(Ti,'#',0);
	int2lcd(IMAX/10,'%',0);
	//int2lcdyx(adc_buff_[5],0,5,0);
/*	int2lcdyx(adc_buff_[0],1,5,0);
	int2lcdyx(adc_buff_[1],2,5,0);
	

	int2lcdyx(K_U,1,10,0);
	int2lcdyx(K0_I,2,10,0);
	 */
	//int2lcdyx(phase,0,19,0);
	//int2lcdyx(pwm_I,0,17,0);	
	}
else if(ind==iResurs)
	{
	ptrs[0]=	" N1 ������   !:0@   ";
	ptrs[1]=	" N2 ������   #:0$   ";
	ptrs[2]=	" N3 ������   %:0^   ";
	ptrs[3]=	" �����              ";
	
	if((sub_ind-index_set)>2)index_set=sub_ind-2;
	else if(sub_ind<index_set)index_set=sub_ind;
	bgnd_par(	"   ������ ��������  ",
			ptrs[index_set],
			ptrs[index_set+1],
			ptrs[index_set+2]);

	pointer_set(1);
/*
	if(av_ten_stat[0]==stOFF)
		{
		int2lcd(ten_wrk_cnt[0]/60,'!',0);
		int2lcd(ten_wrk_cnt[0]%60,'@',0);
		}
	else 
		{
		sub_bgnd("         ����.",'@',-13);
		}

 	if(av_ten_stat[1]==stOFF)
		{
		int2lcd(ten_wrk_cnt[1]/60,'#',0);
		int2lcd(ten_wrk_cnt[1]%60,'$',0);
		}
	else 
		{
		sub_bgnd("         ����.",'$',-13);
		}

	if(av_ten_stat[2]==stOFF)
		{
		int2lcd(ten_wrk_cnt[2]/60,'%',0);
		int2lcd(ten_wrk_cnt[2]%60,'^',0);
		}
	else 
		{
		sub_bgnd("         ����.",'^',-13);
		}  */


	}

else if(ind==iK)
	{
	ptrs[0]=	" U���            !� ";
	ptrs[1]=	" I���            @� ";
	ptrs[2]=	" T���           #�C " ;
	ptrs[3]=	" �����              ";
	
	if((sub_ind-index_set)>2)index_set=sub_ind-2;
	else if(sub_ind<index_set)index_set=sub_ind;
	bgnd_par(	"    ����������      ",
			ptrs[index_set],
			ptrs[index_set+1],
			ptrs[index_set+2]);

	pointer_set(1);

	int2lcd(Ui,'!',1);
	int2lcd(Ii,'@',1);
	int2lcd(Ti,'#',0);
//int2lcdyx(adc_buff_[2],3,5,0);
/*	int2lcdyx(adc_buff_[0],1,5,0);
	int2lcdyx(adc_buff_[1],2,5,0);
	

	int2lcdyx(K_U,1,10,0);
	int2lcdyx(K0_I,2,10,0);
	int2lcdyx(K1_I,3,10,0); */

		
	}
else if((ind==iSet_prl)||(ind==iK_prl)||(ind==iProg_prl))
	{
	bgnd_par(	"  �������  ������   ",
			"                    ",
			"                    ",
			"                    ");
	int2lcdyx(parol[0],1,8,0);
     int2lcdyx(parol[1],1,9,0);
     int2lcdyx(parol[2],1,10,0);
     lcd_buffer[48+sub_ind]='�';
	}
else if(ind==iDeb)
	{
	if(sub_ind1==0)
		{
		bgnd_par(	"*                   ",
					"                    ",
					"                    ",
					"                    ");
		int2lcdyx(modbus_rtu_plazma[0],0,3,0);
		int2lcdyx(modbus_rtu_plazma[1],0,9,0);
		int2lcdyx(modbus_rtu_plazma[2],1,3,0);
		int2lcdyx(modbus_rtu_plazma[3],1,9,0);
		int2lcdyx(modbus_rtu_plazma[4],2,3,0);
		int2lcdyx(modbus_rtu_plazma[5],2,9,0);
		}

	else if(sub_ind1==1)
		{
		bgnd_par(	"&                   ",
					"                    ",
					"                    ",
					"                    ");

		char2lcdhyx(UIB1[0],0,2);
		char2lcdhyx(UIB1[1],1,2);
		char2lcdhyx(UIB1[2],2,2);
		char2lcdhyx(UIB1[3],3,2);
		char2lcdhyx(UIB1[4],0,6);
		char2lcdhyx(UIB1[5],1,6);
		char2lcdhyx(UIB1[6],2,6);
		char2lcdhyx(UIB1[7],3,6);
		char2lcdhyx(UIB1[8],0,10);
		char2lcdhyx(UIB1[9],1,10);
		char2lcdhyx(UIB1[10],2,10);
		char2lcdhyx(UIB1[11],3,10);
		char2lcdhyx(UIB1[12],0,14);
		char2lcdhyx(UIB1[13],1,14);
		char2lcdhyx(UIB1[14],2,14);
		char2lcdhyx(UIB1[15],3,14);
		}

	else if(sub_ind1==2)
		{
		bgnd_par(	"%                   ",
					"                    ",
					"                    ",
					"                    ");

		char2lcdhyx(strIng[0],0,2);
		char2lcdhyx(strIng[1],1,2);
		char2lcdhyx(strIng[2],2,2);
		char2lcdhyx(strIng[3],3,2);
		char2lcdhyx(strIng[4],0,6);
		char2lcdhyx(strIng[5],1,6);
		char2lcdhyx(strIng[6],2,6);
		char2lcdhyx(strIng[7],3,6);
		char2lcdhyx(strIng[8],0,10);
		char2lcdhyx(strIng[9],1,10);
		char2lcdhyx(strIng[10],2,10);
		char2lcdhyx(strIng[11],3,10);
		char2lcdhyx(strIng[12],0,14);
		char2lcdhyx(strIng[13],1,14);
		//char2lcdhyx(strIng[14],2,14);
		//char2lcdhyx(strIng[15],3,14);

		int2lcdyx(strIng_ptr_start,0,19,0);
		int2lcdyx(strIng_ptr_end,1,19,0);
		int2lcdyx(strIng_len,2,19,0);
		int2lcdyx(strIng_ptr_komma,3,19,0);	
		int2lcdyx(chNumber,2,14,0);
		int2lcdyx(setData,3,14,0);

		}

/*	int2lcdyx(adc_buff_[0],0,4,0);
	int2lcdyx(adc_buff_[1],1,4,0);
	int2lcdyx(adc_buff_[2],2,4,0);
	int2lcdyx(adc_buff_[3],3,4,0);

	int2lcdyx(adc_buff_[4],0,8,0);
	int2lcdyx(adc_buff_[5],1,8,0);
	int2lcdyx(adc_buff_[6],2,8,0);
	int2lcdyx(adc_buff_[7],3,8,0);*/
	}
else if(ind==iFWabout)
	{
	bgnd_par(	" ������             ",
				" ������  0000.00.00 ",
				#ifdef WG12232A
				"                    ",
				#endif
				#ifdef WG12232L3
				" WG12232L3          ",
				#endif
				"                    ");
	int2lcdyx(BUILD_YEAR,1,12,0);
	int2lcdyx(BUILD_MONTH,1,15,0);
	int2lcdyx(BUILD_DAY,1,18,0);
	
	sprintf(&lcd_buffer[9],"%d.%d.%d",HARDVARE_VERSION,SOFT_VERSION,BUILD);
	}
//int2lcdyx(wrk_stat,3,1,0);
//int2lcdyx(proc_stat,3,3,0);
//int2lcdyx(rele_stat,0,19,0);
//int2lcdyx(rele_stat_const[rele_stat],0,15,0);
/*int2lcdyx(proc_stat,3,2,0);

int2lcdyx(wrk_period_time/1000,3,10,0);	*/
}

#define BUT_ON 4
#define BUT_ONL 20 

#define butU   254
#define butU_  126
#define butD   253
#define butD_  125
#define butL   251
#define butL_  123
#define butR   247
#define butR_  119
#define butE   239
#define butE_  111
#define butUD  252
#define butLD  249
#define butLR   243
//-----------------------------------------------
void but_drv(void)
{

but_n=IO1PIN|(0xFFFFFFFFUL&(~(1UL<<BUT0))&(~(1UL<<BUT1))&(~(1UL<<BUT2))&(~(1UL<<BUT3))&(~(1UL<<BUT4)));
if((but_n==0xffffffffUL)||(but_n!=but_s))
 	{
 	speed=0;
 
   	if (((but0_cnt>=BUT_ON)||(but1_cnt!=0))&&(!l_but))
  		{
   	     n_but=1;
          but=*(((char*)&but_s)+2);

          }
   	if (but1_cnt>=but_onL_temp)
  		{
   	     n_but=1;
 
          but=*(((char*)&but_s)+2)&0x7f;
          }
    	l_but=0;
   	but_onL_temp=BUT_ONL;
    	but0_cnt=0;
  	but1_cnt=0;          
     goto but_drv_out;
  	}
if(but_n==but_s)
 	{
  	but0_cnt++;
  	if(but0_cnt>=BUT_ON)
  		{
   		but0_cnt=0;
   		but1_cnt++;
   		if(but1_cnt>=but_onL_temp)
   			{              
    			but=but=*(((char*)&but_s)+2)&0x7f;;
    			but1_cnt=0;
    			n_but=1;
    			     
    			l_but=1;
			if(speed)
				{
    				but_onL_temp=but_onL_temp>>1;
        			if(but_onL_temp<=2) but_onL_temp=2;
				}    
   			}
  		}
 	}
but_drv_out: 
but_s=but_n;



PINSEL2&=~(1UL<<((BUT0-16)*2))&~(1UL<<(((BUT0-16)*2)+1))
	   &~(1UL<<((BUT1-16)*2))&~(1UL<<(((BUT1-16)*2)+1))
	   &~(1UL<<((BUT2-16)*2))&~(1UL<<(((BUT2-16)*2)+1))
	   &~(1UL<<((BUT3-16)*2))&~(1UL<<(((BUT3-16)*2)+1))
	   &~(1UL<<((BUT4-16)*2))&~(1UL<<(((BUT4-16)*2)+1));
IO1DIR&=~(1UL<<BUT0)&~(1UL<<BUT1)&~(1UL<<BUT2)&~(1UL<<BUT3)&~(1UL<<BUT4);
	   
}

//-----------------------------------------------
void but_an(void)
{
signed short temp_SS;

if(!n_but)goto but_an_end;

if(but==butUD)
	{
	if(ind!=iDeb)ind=iDeb;
	else ind=iMn;			  

	}

if(ind==iMn)
	{
	if((wrk_stat==stOFF)/*&&(proc_stat==stOFF)*//*(wrk_pause_time==0)*/)
		{
		if(but==butD)
			{
			sub_ind++;
			gran_char(&sub_ind,0,4);
			phase=0;
			}
		else if(but==butU)
			{
			sub_ind--;
			gran_char(&sub_ind,0,4);
			phase=0;
			}
		else if((but==butE)&&(proc_stat==stCH)&&(wrk_stat==stOFF))
			{
			stop_process();
			}
		else if(sub_ind==0)
			{
			if(but==butE)
				{
				if(ALG_MODE==1)
					{
					if(!((!Konset[0])&&(!Konset[1])&&(!Konset[2])&&(!Konset[3])&&(!Konset[4])&&(!Konset[5])&&(!Konset[6])&&(!Konset[7])))
						{
						start_chanel_num(AKT_CH_ALG_1);
						}
					}
				else
					{
					if(!((!Konset[0])&&(!Konset[1])&&(!Konset[2])&&(!Konset[3])&&(!Konset[4])&&(!Konset[5])&&(!Konset[6])&&(!Konset[7])))
						{
						if(proc_stat!=stOFF)stop_process();
						else start_ALG2();
						}
					}
				}
			else if(but==butE_)
				{
				if(!((!Konset[0])&&(!Konset[1])&&(!Konset[2])&&(!Konset[3])&&(!Konset[4])&&(!Konset[5])&&(!Konset[6])&&(!Konset[7])))
					{
					start_test_process();
					}
				}
			if((but==butR)&&(ALG_MODE==1))
				{
				char i;
				AKT_CH_ALG_1++;
				gran_ring(&AKT_CH_ALG_1,0,7);
				for(i=0;i<8;i++)
					{
					if(!Konset[AKT_CH_ALG_1])
						{
						AKT_CH_ALG_1++;
						gran_ring(&AKT_CH_ALG_1,0,7);
						}
					else break;
					}
				lc640_write_int(EE_AKT_CH_ALG_1,AKT_CH_ALG_1);
				}

			if((but==butL)&&(ALG_MODE==1))
				{
				char i;
				AKT_CH_ALG_1--;
				gran_ring(&AKT_CH_ALG_1,0,7);
				for(i=0;i<8;i++)
					{
					if(!Konset[AKT_CH_ALG_1])
						{
						AKT_CH_ALG_1--;
						gran_ring(&AKT_CH_ALG_1,0,7);
						}
					else break;
					}
				lc640_write_int(EE_AKT_CH_ALG_1,AKT_CH_ALG_1);
				}
			}
		else if(sub_ind==1)
			{
			if(but==butE)
				{
				if(PRL_PROG)
					{
					tree_up(iProg_prl,0,0,0);
					parol_init();
					}
				else tree_up(iProg,0,0,0);
				}
			}
		else if(sub_ind==2)
			{
			if(but==butE)
				{
				tree_up(iSet_prl,0,0,0);
				parol_init();
				}			
			}
		else if(sub_ind==3)
			{
			if(but==butE)
		     	{
		     	tree_up(iFWabout,0,0,0);
		     	}			
			}
		else if(sub_ind==4)
			{
			if(but==butE)
				{
				tree_up(iK_prl,0,0,0);
				parol_init();
				}			
			}
 		}

	else if(wrk_stat==stTST)
		{
	    if((but==butR)||(but==butR_))
			{
			plazma_pwm=((plazma_pwm/50)+1)*50;
			gran(&plazma_pwm,0,1000);
			speed=1;
			}
	    else if((but==butL)||(but==butL_))
			{
			plazma_pwm=((plazma_pwm/50)-1)*50;
			gran(&plazma_pwm,0,1000);
			speed=1;
			}
		else if(but==butE_)
			{
			stop_process();
			}	
		}
	else if(proc_stat==stALG2)
		{
		if(but==butD)
			{
			index_set++;
			gran_char(&index_set,0,1);
			phase=0;
			}
		else if(but==butU)
			{
			index_set--;
			gran_char(&index_set,0,1);
			phase=0;
			}
		else if(but==butE)
			{
			stop_process();
			wrk_pause_time=0;
			}				
		}
	else 
		{
		if(but==butE)
			{
			stop_process();
			}			
		}
/*		if(but==butE)
			{
			if(proc_stat==stOFF)start_process();
			else stop_process();
						
		}	}*/



	}

else if(ind==iResurs)
	{
	if(but==butD)
		{
		sub_ind++;
		gran_char(&sub_ind,0,3);
		phase=0;
		}
	else if(but==butU)
		{
		sub_ind--;
		gran_char(&sub_ind,0,3);
		phase=0;
		}

	else if(sub_ind==3)
	     {
		if(but==butE)tree_down(0,0);
	     }	}

else if((ind==iSet_prl)||(ind==iK_prl)||(ind==iProg_prl))
	{
	ret(50);
	if(but==butR)
		{
		sub_ind++;
		gran_ring_char(&sub_ind,0,2);
		}
	else if(but==butL)
		{
		sub_ind--;
		gran_ring_char(&sub_ind,0,2);
		}	
	else if(but==butU)
		{
		parol[sub_ind]++;
		gran_ring_char(&parol[sub_ind],0,9);
		}	
	else if(but==butD)
		{
		parol[sub_ind]--;
		gran_ring_char(&parol[sub_ind],0,9);
		}	
	else if(but==butE)
		{
		unsigned short tempU;
		tempU=parol[2]+(parol[1]*10U)+(parol[0]*100U);
		
		if(ind==iSet_prl)
			{
	     	if(tempU==PAROL_SET) 
				{
				tree_down(0,0);
				ret(0);
				tree_up(iSet,0,0,0);
				//ret(1000);
				}
			else 
				{
		          tree_down(0,0);
	    	          show_mess("                    ",
	          			"       ������       ",
	          			"     ��������!!!    ",
	          			"                    ",1000);
				}
			}


		else	if(ind==iK_prl)
			{
	     	if(tempU==PAROL_KALIBR) 
				{
				tree_down(0,0);
				ret(0);
				tree_up(iK,0,0,0);
//				phase=0;
				}
			else if(tempU==333) 
				{
				tree_down(0,0);
	    	          show_mess("                    ",
	          			"       ������       ",
	          			"     ��������!!!    ",
	          			"                    ",1000);
				
				lc640_write_int(EE_ALG_MODE,1);

				lc640_write_int(ADR_EE_KON_SET[0],1);
				lc640_write_int(ADR_EE_KON_SET[1],1);
				lc640_write_int(ADR_EE_KON_SET[2],1);
				lc640_write_int(ADR_EE_KON_SET[3],1);
				lc640_write_int(ADR_EE_KON_SET[4],1);
				lc640_write_int(ADR_EE_KON_SET[5],1);
				lc640_write_int(ADR_EE_KON_SET[6],1);
				lc640_write_int(ADR_EE_KON_SET[7],1);

				lc640_write_int(ADR_EE_I_SET[0],550);
				lc640_write_int(ADR_EE_I_SET[1],500);
				lc640_write_int(ADR_EE_I_SET[2],450);
				lc640_write_int(ADR_EE_I_SET[3],400);
				lc640_write_int(ADR_EE_I_SET[4],350);
				lc640_write_int(ADR_EE_I_SET[5],300);
				lc640_write_int(ADR_EE_I_SET[6],250);
				lc640_write_int(ADR_EE_I_SET[7],550);

				lc640_write_int(ADR_EE_TP_SET[0],5);
				lc640_write_int(ADR_EE_TP_SET[1],5);
				lc640_write_int(ADR_EE_TP_SET[2],5);
				lc640_write_int(ADR_EE_TP_SET[3],5);
				lc640_write_int(ADR_EE_TP_SET[4],5);
				lc640_write_int(ADR_EE_TP_SET[5],5);
				lc640_write_int(ADR_EE_TP_SET[6],5);
				lc640_write_int(ADR_EE_TP_SET[7],5);

				lc640_write_int(ADR_EE_TV_SET[0],100);
				lc640_write_int(ADR_EE_TV_SET[1],100);
				lc640_write_int(ADR_EE_TV_SET[2],100);
				lc640_write_int(ADR_EE_TV_SET[3],100);
				lc640_write_int(ADR_EE_TV_SET[4],100);
				lc640_write_int(ADR_EE_TV_SET[5],100);
				lc640_write_int(ADR_EE_TV_SET[6],100);
				lc640_write_int(ADR_EE_TV_SET[7],100);

				lc640_write_int(ADR_EE_AVT_REP_SET[0],1);
				lc640_write_int(ADR_EE_AVT_REP_SET[1],1);
				lc640_write_int(ADR_EE_AVT_REP_SET[2],1);
				lc640_write_int(ADR_EE_AVT_REP_SET[3],1);
				lc640_write_int(ADR_EE_AVT_REP_SET[4],1);
				lc640_write_int(ADR_EE_AVT_REP_SET[5],1);
				lc640_write_int(ADR_EE_AVT_REP_SET[6],1);
				lc640_write_int(ADR_EE_AVT_REP_SET[7],1);

				lc640_write_int(ADR_EE_AVT_PER_SET[0],10);
				lc640_write_int(ADR_EE_AVT_PER_SET[1],10);
				lc640_write_int(ADR_EE_AVT_PER_SET[2],10);
				lc640_write_int(ADR_EE_AVT_PER_SET[3],10);
				lc640_write_int(ADR_EE_AVT_PER_SET[4],10);
				lc640_write_int(ADR_EE_AVT_PER_SET[5],10);
				lc640_write_int(ADR_EE_AVT_PER_SET[6],10);
				lc640_write_int(ADR_EE_AVT_PER_SET[7],10);

				}

			else if(tempU==444) 
				{
				tree_down(0,0);
	    	          show_mess("                    ",
	          			"       ������       ",
	          			"     ��������!!!    ",
	          			"                    ",1000);
				
				lc640_write_int(EE_ALG_MODE,1);

				lc640_write_int(ADR_EE_KON_SET[0],0);
				lc640_write_int(ADR_EE_KON_SET[1],0);
				lc640_write_int(ADR_EE_KON_SET[2],0);
				lc640_write_int(ADR_EE_KON_SET[3],0);
				lc640_write_int(ADR_EE_KON_SET[4],0);
				lc640_write_int(ADR_EE_KON_SET[5],0);
				lc640_write_int(ADR_EE_KON_SET[6],0);
				lc640_write_int(ADR_EE_KON_SET[7],0);

				lc640_write_int(ADR_EE_I_SET[0],250);
				lc640_write_int(ADR_EE_I_SET[1],250);
				lc640_write_int(ADR_EE_I_SET[2],250);
				lc640_write_int(ADR_EE_I_SET[3],250);
				lc640_write_int(ADR_EE_I_SET[4],250);
				lc640_write_int(ADR_EE_I_SET[5],250);
				lc640_write_int(ADR_EE_I_SET[6],250);
				lc640_write_int(ADR_EE_I_SET[7],250);

				lc640_write_int(ADR_EE_TP_SET[0],5);
				lc640_write_int(ADR_EE_TP_SET[1],5);
				lc640_write_int(ADR_EE_TP_SET[2],5);
				lc640_write_int(ADR_EE_TP_SET[3],5);
				lc640_write_int(ADR_EE_TP_SET[4],5);
				lc640_write_int(ADR_EE_TP_SET[5],5);
				lc640_write_int(ADR_EE_TP_SET[6],5);
				lc640_write_int(ADR_EE_TP_SET[7],5);

				lc640_write_int(ADR_EE_TV_SET[0],100);
				lc640_write_int(ADR_EE_TV_SET[1],100);
				lc640_write_int(ADR_EE_TV_SET[2],100);
				lc640_write_int(ADR_EE_TV_SET[3],100);
				lc640_write_int(ADR_EE_TV_SET[4],100);
				lc640_write_int(ADR_EE_TV_SET[5],100);
				lc640_write_int(ADR_EE_TV_SET[6],100);
				lc640_write_int(ADR_EE_TV_SET[7],100);

				lc640_write_int(ADR_EE_AVT_REP_SET[0],1);
				lc640_write_int(ADR_EE_AVT_REP_SET[1],1);
				lc640_write_int(ADR_EE_AVT_REP_SET[2],1);
				lc640_write_int(ADR_EE_AVT_REP_SET[3],1);
				lc640_write_int(ADR_EE_AVT_REP_SET[4],1);
				lc640_write_int(ADR_EE_AVT_REP_SET[5],1);
				lc640_write_int(ADR_EE_AVT_REP_SET[6],1);
				lc640_write_int(ADR_EE_AVT_REP_SET[7],1);

				lc640_write_int(ADR_EE_AVT_PER_SET[0],10);
				lc640_write_int(ADR_EE_AVT_PER_SET[1],10);
				lc640_write_int(ADR_EE_AVT_PER_SET[2],10);
				lc640_write_int(ADR_EE_AVT_PER_SET[3],10);
				lc640_write_int(ADR_EE_AVT_PER_SET[4],10);
				lc640_write_int(ADR_EE_AVT_PER_SET[5],10);
				lc640_write_int(ADR_EE_AVT_PER_SET[6],10);
				lc640_write_int(ADR_EE_AVT_PER_SET[7],10);

				}
			else 
				{
		          tree_down(0,0);
	    	          show_mess("                    ",
	          			"       ������       ",
	          			"     ��������!!!    ",
	          			"                    ",1000);
				}
			}
			
		if(ind==iProg_prl)
			{
	     	if(tempU==PAROL_PROG) 
				{
				tree_down(0,0);
				ret(0);
				tree_up(iProg,0,0,0);
				//ret(1000);
				}
			else 
				{
		          tree_down(0,0);
	    	          show_mess("                    ",
	          			"       ������       ",
	          			"     ��������!!!    ",
	          			"                    ",1000);
				}
			}			 
	

		}
	}

else if(ind==iProg)
	{
	if(but==butD)
		{
		sub_ind++;
		
		gran_char(&sub_ind,0,8);
		phase=0;
		}
	else if(but==butU)
		{
		sub_ind--;
		gran_char(&sub_ind,0,8);
		phase=0;
		}
	else if(but==butD_)
		{
		sub_ind=8;
		}
	else if((sub_ind>=0)&&(sub_ind<=7))
		{
		if(but==butE)tree_up(iProg_,0,0,sub_ind);
		}
	else if(sub_ind==8)
	     {
		if(but==butE)tree_down(0,0);
	     }
	//rele_stat=sub_ind;
	}

else if(ind==iProg_)
	{
	if(but==butD)
		{
		sub_ind++;
		if(AVT_REP_SET[sub_ind1]>1)gran_char(&sub_ind,0,6);
		else gran_char(&sub_ind,0,5);
		phase=0;
		}
	else if(but==butU)
		{
		sub_ind--;
		if(AVT_REP_SET[sub_ind1]>1)gran_char(&sub_ind,0,6);
		else gran_char(&sub_ind,0,5);
		phase=0;
		}
	else if(but==butD_)
		{
		tree_down(0,0);
		}
	else if(sub_ind==0)
		{
	    if((but==butR)||(but==butR_)||(but==butL)||(but==butL_)||(but==butE)||(but==butE_))
			{
			if(Konset[sub_ind1])Konset[sub_ind1]=0;
			else Konset[sub_ind1]=1;
			lc640_write_int(ADR_EE_KON_SET[sub_ind1],Konset[sub_ind1]);
			}
	    speed=0;
		}	
	else if(sub_ind==1)
		{
	    if(but==butR)Iset[sub_ind1]++;
	    else if(but==butR_)Iset[sub_ind1]+=10;
	    else if(but==butL)Iset[sub_ind1]--;
	    else if(but==butL_)Iset[sub_ind1]-=10;
	    gran(&Iset[sub_ind1],250,550);
	    lc640_write_int(ADR_EE_I_SET[sub_ind1],Iset[sub_ind1]);
		if(sub_ind1==0)
			{
			lc640_write_int(ADR_EE_I_SET[1],Iset[0]);
			lc640_write_int(ADR_EE_I_SET[2],Iset[0]);
			lc640_write_int(ADR_EE_I_SET[3],Iset[0]);
			lc640_write_int(ADR_EE_I_SET[4],Iset[0]);
			lc640_write_int(ADR_EE_I_SET[5],Iset[0]);
			lc640_write_int(ADR_EE_I_SET[6],Iset[0]);
			lc640_write_int(ADR_EE_I_SET[7],Iset[0]);
			} 
	    speed=1;
		}
	else if(sub_ind==2)
		{
	    if(but==butR)Tpset[sub_ind1]++;
	    else if(but==butR_)Tpset[sub_ind1]+=10;
	    else if(but==butL)Tpset[sub_ind1]--;
	    else if(but==butL_)Tpset[sub_ind1]-=10;
	    gran(&Tpset[sub_ind1],5,60);
	    lc640_write_int(ADR_EE_TP_SET[sub_ind1],Tpset[sub_ind1]);
		if(sub_ind1==0)
			{
			lc640_write_int(ADR_EE_TP_SET[1],Tpset[0]);
			lc640_write_int(ADR_EE_TP_SET[2],Tpset[0]);
			lc640_write_int(ADR_EE_TP_SET[3],Tpset[0]);
			lc640_write_int(ADR_EE_TP_SET[4],Tpset[0]);
			lc640_write_int(ADR_EE_TP_SET[5],Tpset[0]);
			lc640_write_int(ADR_EE_TP_SET[6],Tpset[0]);
			lc640_write_int(ADR_EE_TP_SET[7],Tpset[0]);
			} 
	    speed=1;
		}

	else if(sub_ind==3)
		{
	    if(but==butR)Tvset[sub_ind1]++;
	    else if(but==butR_)Tvset[sub_ind1]+=10;
	    else if(but==butL)Tvset[sub_ind1]--;
	    else if(but==butL_)Tvset[sub_ind1]-=10;
	    gran(&Tvset[sub_ind1],1,200);
	    lc640_write_int(ADR_EE_TV_SET[sub_ind1],Tvset[sub_ind1]);
		if(sub_ind1==0)
			{
			lc640_write_int(ADR_EE_TV_SET[1],Tvset[0]);
			lc640_write_int(ADR_EE_TV_SET[2],Tvset[0]);
			lc640_write_int(ADR_EE_TV_SET[3],Tvset[0]);
			lc640_write_int(ADR_EE_TV_SET[4],Tvset[0]);
			lc640_write_int(ADR_EE_TV_SET[5],Tvset[0]);
			lc640_write_int(ADR_EE_TV_SET[6],Tvset[0]);
			lc640_write_int(ADR_EE_TV_SET[7],Tvset[0]);
			} 
	    speed=1;
		}

/*	else if((sub_ind==4)&&(AVT_ON))
		{
	    if(but==butR)Kvset[sub_ind1]++;
	    else if(but==butR_)Kvset[sub_ind1]+=10;
	    else if(but==butL)Kvset[sub_ind1]--;
	    else if(but==butL_)Kvset[sub_ind1]-=10;
	    gran(&Kvset[sub_ind1],1,50);
	    lc640_write_int(ADR_EE_KV_SET[sub_ind1],Kvset[sub_ind1]);
		if(sub_ind1==0)
			{
			lc640_write_int(ADR_EE_KV_SET[1],Kvset[0]);
			lc640_write_int(ADR_EE_KV_SET[2],Kvset[0]);
			lc640_write_int(ADR_EE_KV_SET[3],Kvset[0]);
			lc640_write_int(ADR_EE_KV_SET[4],Kvset[0]);
			lc640_write_int(ADR_EE_KV_SET[5],Kvset[0]);
			lc640_write_int(ADR_EE_KV_SET[6],Kvset[0]);
			lc640_write_int(ADR_EE_KV_SET[7],Kvset[0]);
			} 
	    speed=1;
		}*/

	else if(sub_ind==4)
		{
	    if(but==butR)AVT_REP_SET[sub_ind1]++;
	    else if(but==butR_)AVT_REP_SET[sub_ind1]+=10;
	    else if(but==butL)AVT_REP_SET[sub_ind1]--;
	    else if(but==butL_)AVT_REP_SET[sub_ind1]-=10;
	    gran(&AVT_REP_SET[sub_ind1],1,99);
	    lc640_write_int(ADR_EE_AVT_REP_SET[sub_ind1],AVT_REP_SET[sub_ind1]);
		if(sub_ind1==0)
			{
			lc640_write_int(ADR_EE_AVT_REP_SET[1],AVT_REP_SET[0]);
			lc640_write_int(ADR_EE_AVT_REP_SET[2],AVT_REP_SET[0]);
			lc640_write_int(ADR_EE_AVT_REP_SET[3],AVT_REP_SET[0]);
			lc640_write_int(ADR_EE_AVT_REP_SET[4],AVT_REP_SET[0]);
			lc640_write_int(ADR_EE_AVT_REP_SET[5],AVT_REP_SET[0]);
			lc640_write_int(ADR_EE_AVT_REP_SET[6],AVT_REP_SET[0]);
			lc640_write_int(ADR_EE_AVT_REP_SET[7],AVT_REP_SET[0]);
			} 
	    speed=1;
		}

	else if((sub_ind==5)&&(AVT_REP_SET[sub_ind1]>1))
		{
	    if(but==butR)AVT_PER_SET[sub_ind1]++;
	    else if(but==butR_)AVT_PER_SET[sub_ind1]+=10;
	    else if(but==butL)AVT_PER_SET[sub_ind1]--;
	    else if(but==butL_)AVT_PER_SET[sub_ind1]-=10;
	    gran(&AVT_PER_SET[sub_ind1],(Tvset[sub_ind1]+Tpset[sub_ind1]+59)/60/*+1*/,1440/*Tvset[sub_ind1]+Tpset[sub_ind1]+3600*/);
	    lc640_write_int(ADR_EE_AVT_PER_SET[sub_ind1],AVT_PER_SET[sub_ind1]);
		if(sub_ind1==0)
			{
			lc640_write_int(ADR_EE_AVT_PER_SET[1],AVT_PER_SET[0]);
			lc640_write_int(ADR_EE_AVT_PER_SET[2],AVT_PER_SET[0]);
			lc640_write_int(ADR_EE_AVT_PER_SET[3],AVT_PER_SET[0]);
			lc640_write_int(ADR_EE_AVT_PER_SET[4],AVT_PER_SET[0]);
			lc640_write_int(ADR_EE_AVT_PER_SET[5],AVT_PER_SET[0]);
			lc640_write_int(ADR_EE_AVT_PER_SET[6],AVT_PER_SET[0]);
			lc640_write_int(ADR_EE_AVT_PER_SET[7],AVT_PER_SET[0]);
			} 
	    speed=1;
		}

	else if((sub_ind==6)||((sub_ind==5)&&(AVT_REP_SET[sub_ind1]==1)))
	    {
		if(but==butE)tree_down(0,0);
	    }
	}

else if(ind==iSet)
	{

	if(but==butD)
		{
		sub_ind++;
		gran_char(&sub_ind,0,9);
		if(sub_ind==2)
			{
			sub_ind=3;
			//index_set=1;
			}

		if(sub_ind==3)
			{
			//sub_ind=5;
			index_set=2;
			}

		if(sub_ind==4)
			{
			sub_ind=5;
			//index_set=1;
			}
	/*	if(sub_ind==3)
			{
			sub_ind++;
			//index_set=3;
			}
		if(sub_ind==5)
			{
			index_set=4;
			}
		if(sub_ind==6)
			{
			sub_ind++;
			index_set=5;
			} */
		if(sub_ind==7)
			{
			index_set=6;
			} 
		if(sub_ind==8)
			{
			sub_ind++;
			//index_set=7;
			} 
		}
	else if(but==butU)
		{
		sub_ind--;
		gran_char(&sub_ind,0,9);
		if(sub_ind==2)
			{
			sub_ind--;
			index_set=1;
			}
		if(sub_ind==4)
			{
			sub_ind--;
			index_set=3;
			}
	/*	if(sub_ind==5)
			{
			sub_ind--;
			index_set=4;
			}
		if(sub_ind==6)
			{
			sub_ind--;
			}   */
		if(sub_ind==8)
			{
			sub_ind--;
			} 
		}
	else if(but==butD_)
		{
		sub_ind=8;
		}
	else if(sub_ind==0)
	    	{
		if(ALG_MODE==1)ALG_MODE=2;
		else ALG_MODE=1;
	    	lc640_write_int(EE_ALG_MODE,ALG_MODE);
	    	speed=0;
	    	}

	else if(sub_ind==1)
	     {
	     if(but==butR)TZPK++;
	     else if(but==butR_)TZPK+=10;
	     else if(but==butL)TZPK--;
	     else if(but==butL_)TZPK-=10;
	     gran(&TZPK,1,500);
	     lc640_write_int(EE_TZPK,TZPK);
	     speed=1;
	     }

	else if(sub_ind==3)
	     {
	    	if(but==butR)REP_NUM_ALG_2++;
	    	else if(but==butR_)REP_NUM_ALG_2+=2;
	    	else if(but==butL)REP_NUM_ALG_2--;
	    	else if(but==butL_)REP_NUM_ALG_2-=2;
	    	gran(&REP_NUM_ALG_2,1,99);
	    	lc640_write_int(EE_REP_NUM_ALG_2,REP_NUM_ALG_2);
	    	speed=1;
		}
/*
	else if(sub_ind==5)
	    	{	
		if(but==butR)ADR++;
	    	else if(but==butR_)ADR+=10;
	    	else if(but==butL)ADR--;
	    	else if(but==butL_)ADR-=10;
	    	gran(&ADR,16,80);
	    	lc640_write_int(EE_ADR,ADR);
	    	speed=1;
	    	}
	else if(sub_ind==6)
	    	{
		if((but==butR)||(but==butR_))
               {
               speed=1;
               if(BAUDRATE==96)BAUDRATE=192;
               else if(BAUDRATE==192)BAUDRATE=384;
               else if(BAUDRATE==384)BAUDRATE=576;
               else if(BAUDRATE==576)BAUDRATE=1152;
               else BAUDRATE=96;
               lc640_write_int(EE_BAUDRATE,BAUDRATE);
               uart1_init();
               }
          else if((but==butL)||(but==butL_))
               {
               speed=1;
               if(BAUDRATE==96)BAUDRATE=1152;
               else if(BAUDRATE==192)BAUDRATE=96;
               else if(BAUDRATE==384)BAUDRATE=192;
               else if(BAUDRATE==576)BAUDRATE=384;
               else BAUDRATE=576;
               lc640_write_int(EE_BAUDRATE,BAUDRATE);
               uart1_init();
               }
	     }
	else if(sub_ind==7)
	     {
		if(but==butE)tree_down(0,0);
	     }*/
	    

 	else if(sub_ind==5)
		{
		if((but==butR)||(but==butR_))
			{
			lc640_write_int(EE_MODBUS_TYPE,1);
			}
		else if((but==butL)||(but==butL_))
			{
			lc640_write_int(EE_MODBUS_TYPE,0);
			}
		}
  	else if(sub_ind==6)
	     {
		if(but==butR)ADR++;
	     else if(but==butR_)ADR+=10;
	     else if(but==butL)ADR--;
	     else if(but==butL_)ADR-=10;
	     gran(&ADR,1,99);
	     lc640_write_int(EE_ADR,ADR);
	     speed=1;
	     }
	else if(sub_ind==7)
		{
		if((but==butR)||(but==butR_))
               {
               speed=1;
               if(BAUDRATE==96)BAUDRATE=192;
               else if(BAUDRATE==192)BAUDRATE=384;
               else if(BAUDRATE==384)BAUDRATE=576;
               else if(BAUDRATE==576)BAUDRATE=1152;
               else BAUDRATE=96;
               lc640_write_int(EE_BAUDRATE,BAUDRATE);
               uart1_init();
               }
          else if((but==butL)||(but==butL_))
               {
               speed=1;
               if(BAUDRATE==96)BAUDRATE=1152;
               else if(BAUDRATE==192)BAUDRATE=96;
               else if(BAUDRATE==384)BAUDRATE=192;
               else if(BAUDRATE==576)BAUDRATE=384;
               else BAUDRATE=576;
               lc640_write_int(EE_BAUDRATE,BAUDRATE);
               uart1_init();
               }
	     }


	
	else if(sub_ind==9)
	     {
		if(but==butE)tree_down(0,0);
	     }	
     		 	
     }
     
  
	   	

		     
else if(ind==iK)
	{
	if(but==butD)
		{
		sub_ind++;
		gran_char(&sub_ind,0,5);
		if(sub_ind==3)
			{
			index_set=2;
			}
		if(sub_ind==4)
			{
			sub_ind=5;
			//index_set=3;
			}
		phase=0;
		}
	else if(but==butU)
		{
		sub_ind--;
		gran_char(&sub_ind,0,5);
		if(sub_ind==4)
			{
			sub_ind=3;
			//index_set=2;
			}
		phase=0;
		}
	else if(sub_ind==0)
		{
		temp_SS=K_U;
	     if(but==butR)
	     	{
	     	temp_SS++;
		    phase=1;
	     	}
	     else if(but==butR_)
	     	{
	     	temp_SS+=5;
	     	phase=1;
	     	}	
	     else if(but==butL)
	     	{
	     	temp_SS--;
	     	phase=1;
	     	}
	     else if(but==butL_)
	     	{
	     	temp_SS-=5;
	     	phase=1;
	     	}
	     gran(&temp_SS,400,600);
		lc640_write_int(EE_K_U,temp_SS);					
		speed=1;			
		}
	else if(sub_ind==1)
		{
		if((but==butE)||(but==butLR))
		     {
		     if(phase==0)
		          {
		          //lc640_write_int(EE_K0_I,adc_buff_[1]);
		          phase=1;
		          }
			else phase=0;
		     }	
		else
			{
			temp_SS=K1_I;
			if(but==butR)temp_SS++;
			else if(but==butR_)temp_SS+=2;
			else if(but==butL)temp_SS--;
			else if(but==butL_)temp_SS-=2;
						
			gran(&temp_SS,350,500);
			lc640_write_int(EE_K1_I,temp_SS);
			phase=1;
			}
				
		speed=1;			
		}
	else if(sub_ind==2)
		{
		temp_SS=K_T;
		if(but==butR)temp_SS++;
		else if(but==butR_)temp_SS+=3;
		else if(but==butL)temp_SS--;
		else if(but==butL_)temp_SS-=3;
		gran(&temp_SS,2000,2200);
		lc640_write_int(EE_K_T,temp_SS);				
		speed=1;			
		}
	else if(sub_ind==3)
	    {
	    if(but==butR)IMAX=((IMAX/10)+1)*10;
	    else if(but==butR_)IMAX=((IMAX/10)+10)*10;
	    else if(but==butL)IMAX=((IMAX/10)-1)*10;
	    else if(but==butL_)IMAX=((IMAX/10)-10)*10;
	    gran(&IMAX,1,1000);
	    lc640_write_int(EE_IMAX,IMAX);
	    speed=1;
	    }			
	else if(sub_ind==5)
	     {
		if(but==butE)tree_down(0,0);
	     }			

     }

else if(ind==iSM)
	{
	tree_down(0,0);
	}

else if(ind==iDeb)
	{
	//ret(50);
	if(but==butR)
		{
		sub_ind1++;
		gran_ring_char(&sub_ind,0,2);
		}
	else if(but==butL)
		{
		sub_ind1--;
		gran_ring_char(&sub_ind,0,2);
		}	
	}
else if(ind==iFWabout)
	{
	ret(1000);
	if(but==butE)
	     {
	     tree_down(0,0);
	     ret(0);
	     }
	}
but_an_end:
n_but=0;
}

//***********************************************
//***********************************************
//***********************************************
//***********************************************
//***********************************************
__irq void timer1_interrupt() 
{
T1IR = 0xff;

if(tx_wd_cnt)
	{
	tx_wd_cnt--;
	if(!tx_wd_cnt)
		{
		SET_REG(PINSEL1,0,(22-16)*2,1); //���� PV � 485
		IO0DIR|=(1<<22);
		IO0CLR|=(1<<22);
		}
	}

if(++t0_cnt8>=33)
     {
     t0_cnt8=0;
     b33Hz=1;
	}

if(++t0cnt>=10)
     {
     t0cnt=0;
     b100Hz=1;

     if(++t0cnt0>=10)
	     {
	     t0cnt0=0;
	     b10Hz=1;
	     //beep_drv();
	     }

     if(t0cnt0==5)
	     {
	     ///beep_drv();
	     }

     if(++t0cnt1>=20)
	     {
	     t0cnt1=0;
	     b5Hz=1;
	     
 		if(bFL5)bFL5=0;
  		else bFL5=1;	     
	     }

     if(++t0cnt2>=50)
	     {
	     t0cnt2=0;
	     b2Hz=1;
	     
	     if(bFL2)bFL2=0;
	     else bFL2=1;
	     }         

     if(++t0cnt3>=100)
	     {
	     t0cnt3=0;
	     b1Hz=1;
	     }
     }

if (_485_last_cnt)
	{
	_485_last_cnt--;
	if(_485_last_cnt==0)
		{
		char i;
		for(i=0;i<rx_counter1;i++)
			{
			UIB1[i]=rx_buffer1[i] ;

			//UIB1[0]=0x34;
			}
		
		bRXIN1=1;
			//plazma++;
		rx_counter1=0;
		rx_wr_index1=0;

		}
	}
if(modbus_timeout_cnt<6)
	{
	modbus_timeout_cnt++;
	if(modbus_timeout_cnt>=6)
		{
		bMODBUS_TIMEOUT=1;
		}
	}
else if (modbus_timeout_cnt>6)
	{
	modbus_timeout_cnt=0;
	bMODBUS_TIMEOUT=0;
	}

VICVectAddr = 0;
}

//***********************************************
__irq void timer0_interrupt() 
{	
T0IR = 0xff;

SET_REG(T0EMR,1,1,1);
SET_REG(T0EMR,1,3,1);

VICVectAddr = 0;
}
/*
//***********************************************
__irq void can1_rx_interrupt() 
{

}*/

//===============================================
//===============================================
//===============================================
//===============================================
int main (void)
{

SET_REG(PINSEL1,0,(23-16)*2,1); //���� PV � 485
IO0DIR|=(1<<23);
IO0CLR|=(1<<23);

IO1DIR|=(1<<26);
IO1SET|=(1<<26);   //������������� ��������

t0_init();
t1_init();
 
IO1DIR|=(1UL<<31);	
IO1SET=(1UL<<31);	//������������� TLV


IO1DIR|=(1UL<<21);
IO1SET=(1UL<<21);  	//������������� 25LC640

IO1DIR|=(1UL<<21);
IO1SET=(1UL<<21);  	//������� �� ���������

lcd_init();
lcd_init();
lcd_init();
lcd_init();
lcd_init();
lcd_on();
lcd_clear();


ad7705_reset();
delay_ms(20);

ad7705_write(0x20);
ad7705_write(BIN8(1101)); 
ad7705_write(0x10);
ad7705_write(0x44);

ad7705_reset();
delay_ms(20);

ad7705_write(0x20);
ad7705_write(BIN8(1101)); 
ad7705_write(0x10);
ad7705_write(0x44);

ad7705_reset();
delay_ms(20);  

ad7705_write(0x20);
ad7705_write(BIN8(1101)); 
ad7705_write(0x10);
ad7705_write(0x44); 

SET_REG(PINSEL1,0,(23-16)*2,1);	//���� PV � 485
IO0DIR|=(1<<23);
IO0SET|=(1<<23);

memo_read();


__enable_irq();

BAUDRATE=lc640_read_int(EE_BAUDRATE);
uart1_init();


watchdog_init(60000000,2000);


///lc640_write_int(EE_REST_CNT,lc640_read_int(EE_REST_CNT)+1);
for(;;)
	{ 
	if(bRXIN1)
		{
		bRXIN1=0;
		if(MODBUS_TYPE==0)uart_in_an1();
		}
	if(bMODBUS_TIMEOUT)
		{
		bMODBUS_TIMEOUT=0;
		modbus_rtu_plazma[0]++;
		//uart_in_an1();
		//if(MODBUS_TYPE==0)uart_in_an1();
		if(MODBUS_TYPE==1)modbus_in();
		}

/*		ind_hndl();
		bitmap_hndl();
		lcd_out(lcd_bitmap); */
	/*	sec_cnt++; */
	if(b1000Hz)
		{
		b1000Hz=0;

		
		}
	
	if(b100Hz)
		{
		b100Hz=0;

		but_drv();
		but_an();
		if(!bRESET)watchdog_reset();
		adc_drv();

		}
		 
	if(b50Hz)
		{
		b50Hz=0;
		
		//pwm_drv();
		//ten_drv();
		}

	if(b10Hz)
		{
		b10Hz=0;

		#ifndef SIMULATOR
		ind_hndl();
		bitmap_hndl();
		lcd_out(lcd_bitmap);
		#endif
		//IO0CLR|=(1UL<<4);
		wrk_process();		

		
		pwr_hndl();
		pwr_drv();
		pwm_hndl();
		pwm_drv();
		ret_hndl();
		rele_hndl();
		rele_drv();
		
		}

	if(b5Hz)
		{
		b5Hz=0;
		memo_read();
	matemat();
		///ten_hndl();
		ad7705_drv();
		rele_drv();
		}

	if(b2Hz)
		{
		b2Hz=0;
		
  		}

	if(b1Hz)
		{
		b1Hz=0;

		///sec_cnt++;
			
		temper_drv();
		ret_ind_sec_hndl();	

		//uart_out_adr1(memo_out0,6);

		}
			 
	}
}


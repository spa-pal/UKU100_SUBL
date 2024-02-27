#include "25lc640.h"
#include "memo.h"
#include "eeprom_map.h"
#include "main.h"

const int PTR_AV_TEN_STAT[4] = {EE_AV_TEN_STAT0,EE_AV_TEN_STAT1,EE_AV_TEN_STAT2,EE_AV_TEN_STAT3};
const signed short ADR_EE_I_SET[]={EE_I_SET0,EE_I_SET1,EE_I_SET2,EE_I_SET3,EE_I_SET4,EE_I_SET5,EE_I_SET6,EE_I_SET7};
const signed short ADR_EE_KON_SET[]={EE_KON_SET0,EE_KON_SET1,EE_KON_SET2,EE_KON_SET3,EE_KON_SET4,EE_KON_SET5,EE_KON_SET6,EE_KON_SET7};
const signed short ADR_EE_TP_SET[]={EE_TP_SET0,EE_TP_SET1,EE_TP_SET2,EE_TP_SET3,EE_TP_SET4,EE_TP_SET5,EE_TP_SET6,EE_TP_SET7};
const signed short ADR_EE_TV_SET[]={EE_TV_SET0,EE_TV_SET1,EE_TV_SET2,EE_TV_SET3,EE_TV_SET4,EE_TV_SET5,EE_TV_SET6,EE_TV_SET7};
const signed short ADR_EE_AVT_REP_SET[]={EE_AVT_REP_SET0,EE_AVT_REP_SET1,EE_AVT_REP_SET2,EE_AVT_REP_SET3,EE_AVT_REP_SET4,EE_AVT_REP_SET5,EE_AVT_REP_SET6,EE_AVT_REP_SET7};
const signed short ADR_EE_AVT_PER_SET[]={EE_AVT_PER_SET0,EE_AVT_PER_SET1,EE_AVT_PER_SET2,EE_AVT_PER_SET3,EE_AVT_PER_SET4,EE_AVT_PER_SET5,EE_AVT_PER_SET6,EE_AVT_PER_SET7};;
//-----------------------------------------------
void memo_read (void)
{

AVT_ON=lc640_read_int(EE_AVT_ON);
AVT_ON_TUMBLER=lc640_read_int(EE_AVT_ON_TUMBLER);
IMAX=lc640_read_int(EE_IMAX);
IAVAR=lc640_read_int(EE_IAVAR);  
TZPK=lc640_read_int(EE_TZPK);
PRL_PROG=lc640_read_int(EE_PRL_PROG);
//ZV_AV_ALL_TEN=lc640_read_int(EE_ZV_AV_ALL_TEN);
ADR=lc640_read_int(EE_ADR);
K_U=lc640_read_int(EE_K_U);
K0_I=lc640_read_int(EE_K0_I);
K1_I=lc640_read_int(EE_K1_I);
K_T=lc640_read_int(EE_K_T);
BAUDRATE=lc640_read_int(EE_BAUDRATE);
MODBUS_TYPE=lc640_read_int(EE_MODBUS_TYPE);
WRK_TIME=lc640_read_int(EE_WRK_TIME);
I_SET=lc640_read_int(EE_I_SET);
TEN_SET=lc640_read_int(EE_TEN_SET);
ALG_MODE=lc640_read_int(EE_ALG_MODE);
AKT_CH_ALG_1=lc640_read_int(EE_AKT_CH_ALG_1);
REP_NUM_ALG_2=lc640_read_int(EE_REP_NUM_ALG_2);
//av_ten_stat[2]=(char)lc640_read_int(EE_AV_TEN_STAT2);
//av_ten_stat[3]=(char)lc640_read_int(EE_AV_TEN_STAT3);
//av_ten_stat[4]=(char)lc640_read_int(EE_AV_TEN_STAT4);
Iset[0]=lc640_read_int(EE_I_SET0);
Iset[1]=lc640_read_int(EE_I_SET1);
Iset[2]=lc640_read_int(EE_I_SET2);
Iset[3]=lc640_read_int(EE_I_SET3);
Iset[4]=lc640_read_int(EE_I_SET4);
Iset[5]=lc640_read_int(EE_I_SET5);
Iset[6]=lc640_read_int(EE_I_SET6);
Iset[7]=lc640_read_int(EE_I_SET7);
Konset[0]=lc640_read_int(EE_KON_SET0);
Konset[1]=lc640_read_int(EE_KON_SET1);
Konset[2]=lc640_read_int(EE_KON_SET2);
Konset[3]=lc640_read_int(EE_KON_SET3);
Konset[4]=lc640_read_int(EE_KON_SET4);
Konset[5]=lc640_read_int(EE_KON_SET5);
Konset[6]=lc640_read_int(EE_KON_SET6);
Konset[7]=lc640_read_int(EE_KON_SET7);
Tpset[0]=lc640_read_int(EE_TP_SET0);
Tpset[1]=lc640_read_int(EE_TP_SET1);
Tpset[2]=lc640_read_int(EE_TP_SET2);
Tpset[3]=lc640_read_int(EE_TP_SET3);
Tpset[4]=lc640_read_int(EE_TP_SET4);
Tpset[5]=lc640_read_int(EE_TP_SET5);
Tpset[6]=lc640_read_int(EE_TP_SET6);
Tpset[7]=lc640_read_int(EE_TP_SET7);
Tvset[0]=lc640_read_int(EE_TV_SET0);
Tvset[1]=lc640_read_int(EE_TV_SET1);
Tvset[2]=lc640_read_int(EE_TV_SET2);
Tvset[3]=lc640_read_int(EE_TV_SET3);
Tvset[4]=lc640_read_int(EE_TV_SET4);
Tvset[5]=lc640_read_int(EE_TV_SET5);
Tvset[6]=lc640_read_int(EE_TV_SET6);
Tvset[7]=lc640_read_int(EE_TV_SET7);
AVT_REP_SET[0]=lc640_read_int(EE_AVT_REP_SET0);
AVT_REP_SET[1]=lc640_read_int(EE_AVT_REP_SET1);
AVT_REP_SET[2]=lc640_read_int(EE_AVT_REP_SET2);
AVT_REP_SET[3]=lc640_read_int(EE_AVT_REP_SET3);
AVT_REP_SET[4]=lc640_read_int(EE_AVT_REP_SET4);
AVT_REP_SET[5]=lc640_read_int(EE_AVT_REP_SET5);
AVT_REP_SET[6]=lc640_read_int(EE_AVT_REP_SET6);
AVT_REP_SET[7]=lc640_read_int(EE_AVT_REP_SET7);

AVT_PER_SET[0]=lc640_read_int(EE_AVT_PER_SET0);
AVT_PER_SET[1]=lc640_read_int(EE_AVT_PER_SET1);
AVT_PER_SET[2]=lc640_read_int(EE_AVT_PER_SET2);
AVT_PER_SET[3]=lc640_read_int(EE_AVT_PER_SET3);
AVT_PER_SET[4]=lc640_read_int(EE_AVT_PER_SET4);
AVT_PER_SET[5]=lc640_read_int(EE_AVT_PER_SET5);
AVT_PER_SET[6]=lc640_read_int(EE_AVT_PER_SET6);
AVT_PER_SET[7]=lc640_read_int(EE_AVT_PER_SET7);
}



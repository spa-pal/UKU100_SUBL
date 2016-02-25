

extern short pwm_U,pwm_I;
extern signed long time,time_p,i_ch,pwm_ch,pwm_ch_max;

//-----------------------------------------------
//АПВ
extern unsigned main_apv_cnt,hour_apv_cnt[2],reset_apv_cnt[2];
extern unsigned short apv_cnt_sec[2],apv_cnt[2];
extern char apv_cnt_1;
extern short adc_buff_out_[3];

//-----------------------------------------------
//Управление источниками
extern char cnt_src[2];
extern signed short cntrl_stat,old_cntrl_stat,cntrl_stat1,cntrl_stat2;
extern signed short u_necc,u_necc_;
extern char cnt_blok;
typedef enum {iiB,iiM} enum_ibs;
extern enum_ibs Ibs,Ibso;
extern char bmm_cnt,bmp_cnt;
extern char bS;
extern signed char num_necc;
extern signed char cnt_num_necc;
extern char tzas_cnt;
extern char bBLOK;




//-----------------------------------------------
//Математика
extern char bit_minus;

//-----------------------------------------------
//Звук
extern char cnt_beep;

//-----------------------------------------------
//Температура

extern signed short t_sign_cnt,t_max_cnt;
typedef enum{tsNORM,tsSIGN,tsAV} enum_temper_state;
extern enum_temper_state temper_state,temper_state_old;
extern char crazy_beep;


//-----------------------------------------------
//Защита по снижению напряжения
extern signed short t_u_min1,t_u_min2;
extern char bSTOP_umin1,bSTOP_umin2;

//-----------------------------------------------
//Повышенная мощность
extern signed short p_pov_cnt;

//-----------------------------------------------
//Защита от теплового перегрева(интегратор тока)
extern signed long current_sigma;
extern char current_sigma_stat;
extern signed long power_sigma;
extern char power_sigma_stat;
extern signed long power_sigma_cnt_30min;

extern signed long plazma_long;


void beep_hndl(void);
void cntrl_drv(void);
void zar_drv(void);
void spc_drv(void);
void cntrl_blok_hndl(void);
void ext_drv(void);
void rs232_transmit_of_temper(char in1,char in2,short in3);
void rs232_transmit_of_sk(char in1,char in2);
void bat_drv(void);
void kb_drv(void);
void matemat(void);
void samokalibr_drv(void);
void time_drv(void);
void kz_on(void);
void kz_off(void);
void bp_on(char in);
void bp_off(char in);
void kb_start(char in);
void unet_drv(void);
void src_drv(char in);
void blok_src_reset(char in);
void apv2_hndl(char in);
void adc_drv(void);
void next_date_hndl(char period);
void avz_hndl(void);
void mnemo_hndl(void);
void pwm_drv(void);
void temper_drv(void);
void vent_drv(void);
void u_min_drv(void);
void kv_hndl(void);
void out_drv(void);
void av_out_hndl(void);
void p_pov_drv(void);
void i_pov_drv(void);
void current_integral_1_7000(void);

//-----------------------------------------------
void pwr_hndl(void);
void pwr_drv(void);
void rele_hndl(void);
void rele_drv(void);
void wrk_process(void);
void pwm_hndl(void);
void start_single_ring_process (void);
void stop_process (void);

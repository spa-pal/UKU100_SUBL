#define sec__  time_union.time_str.sec___
#define min__  time_union.time_str.min___
#define hour__  time_union.time_str.hour___
#define day__  time_union.time_str.day___
#define month__  time_union.time_str.month___
#define year__  time_union.time_str.year___


#pragma pack(1)
typedef struct  
	{
     unsigned int sec___:6; 
     unsigned int min___:6; 
     unsigned int hour___:5;
     unsigned int day___:5;
     unsigned int month___:4;
     unsigned int year___:6;
     }ss;

typedef union 
     {
     ss time_str;
     unsigned long time_long;
     }s;


//-----------------------------------------------
//Время
extern s time_union,time_union1;
extern signed char sec_bcd;
extern signed char min_bcd;
extern signed char hour_bcd;
extern signed char day_bcd;
extern signed char month_bcd;
extern signed char year_bcd;


void i2c_init_soft(void);
void i2c_start_soft(void);
void i2c_stop_soft(void);
char* pcf8563_read_bytes_soft(char adr,char nums);
char* pcf8563_read(char nums);
char* pcf8563_write(char adr, char in);



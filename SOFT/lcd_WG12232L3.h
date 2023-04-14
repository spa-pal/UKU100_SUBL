#ifndef _LCD_AGM1232_UKU100_H_

#define _LCD_AGM1232_UKU100_H_


#define A0		30
#define A0_PORT	1
#define E1		10
#define E1_PORT	0
#define E2		23
#define E2_PORT	1
#define RW     	11
#define RW_PORT	0

#define D0     	12
#define D1     	13
#define D2     	14
#define D3     	15
#define D4     	16
#define D5     	17
#define D6     	18
#define D7     	19
#define D_PORT		30

#define RES		20
#define RES_PORT	0


#define _RESET_				226
#define _DISPLAY_ON_             	175
#define _DISPLAY_OFF_            	174
#define _SET_PAGE_               	184
#define _SET_COLUMN_             	0
#define _SET_RAW_             	0xc0
#define _SET_DISPLAY_START_LINE_ 	192
#define Max_Col		60
#define Max_page 		3   


void lcd1_chk(void);
void lcd1_wr(char in);
void lcd2_chk(void);
void lcd2_wr(char in);
char data1_wr(char in);
void data2_wr(char in);
void lcd_set_page(char in);
void lcd_set_col(char in);
void lcd_set_raw(char in);
void lcd_init(void);
void status(void);
void delay(void);
void ltstrobe(char in);
void lcd_init_(void);
void lcd_clear(void);
void lcd_on(void);
void lcd_off(void);
void lcd_out(char* adr);

#endif

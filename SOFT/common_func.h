





void clr_scrn(void);
char find(char xy);
void bin2bcd_int(unsigned int in);
void bcd2lcd_zero(char sig);
void int2lcd_m(signed short in,char xy,char des);
void int2lcd_mm(signed short in,char xy,char des);
void int2lcd_mmm(signed short in,char xy,char des);
void long2lcd_mmm(signed long in,char xy,char des);
void long2lcdyx_mmm(unsigned long in,char y,char x,char des);
void int2lcdyx(unsigned short in,char y,char x,char des);
void int2lcd(unsigned short in,char xy,char des);
void long2lcdhyx(unsigned long in,char y,char x);
void char2lcdh(char in,char yx);
void char2lcdhyx(char in,char y,char x);
void int2lcdhyx(int in,char y,char x);
void char2lcdbyx(char in,char y,char x);
signed short ptr_carry(signed short in,signed short modul,signed short carry);
void sub_bgnd(char const *adr,char xy,signed char offset);
void bgnd_par(char const *ptr0,char const *ptr1,char const *ptr2,char const *ptr3);
void fl_simv(char y, char x, char len);
void event2ind(char num, char simbol);
void tree_up(char tind, char tsub_ind, char tindex_set, char tsub_ind1);
void tree_down(signed char offset_ind,signed char offset_sub_ind);
void parol_init(void);
void pointer_set(char num_of_first_row);
void event_data2ind(char num, char simbol);
void show_mess(char* p1, char* p2, char* p3, char* p4,int m_sec);
void int2lcdh(int in,char yx);
void long2lcdyx(signed long in,char y,char x,char des);

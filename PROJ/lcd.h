/*
 * lcd.h
 *
 * Created: 01-09-2018 20:44:30
 *  Author: soham
 */ 


#ifndef LCD_H_
#define LCD_H_

#define lcd_d PORTA
#define lcd_dd DDRA //full
#define lcd_c PORTB
#define lcd_cd DDRB 
#define rs 0
#define en 1
#define vcc 2 

void lcd_begin(unsigned char,unsigned char,unsigned char); //dimensions(col,row),mode
void lcd_clear(void);
void lcd_setcursor(unsigned char,unsigned char); //col,row
void lcd_cursor(void);
void lcd_cursoroff(void);
void lcd_off(void);
void lcd_on(void);
void lcd_ctrl(unsigned char);
void lcd_print(unsigned char*);
void lcd_printa(unsigned char);
unsigned char bit,row,col;


#endif /* LCD_H_ */

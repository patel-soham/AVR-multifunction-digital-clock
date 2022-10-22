/*
 * lcd.c
 *
 * Created: 01-09-2018 20:44:12
 *  Author: soham
 */ 
#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"

void lcd_begin(unsigned char c,unsigned char b,unsigned char a) // dimensions ,mode
{
	bit=a;
	row=b;
	col=c;
	lcd_cd|=(1<<rs)|(1<<en)|(1<<vcc); 
	lcd_c|=1<<vcc; 
	if(bit==8)
	{
		lcd_dd=0xff;
		lcd_ctrl(0x38); //8 bit mode
	} 
	else
	{
		lcd_dd|=0x0f;
		lcd_ctrl(0x33); //
		lcd_ctrl(0x32); //
		lcd_ctrl(0x28); //
	} 
	lcd_ctrl(0x01); //clear
	lcd_ctrl(0x06); //move l to r
	lcd_ctrl(0x0C); //no cursor
	lcd_ctrl(0x80); //starting address till 8f
}
void lcd_clear(void)
{
		lcd_ctrl(0x01);
	    lcd_ctrl(0x80);
}
void lcd_setcursor(unsigned char a,unsigned char b)
{
	//a is col and b is row
	if(b==0)
	lcd_ctrl((0x80)+a);
	if(b==1)
	lcd_ctrl((0xc0)+a); //add of row 1 c0 to cf
	if(row==4 && col==20)
	{
		if(b==2)
		lcd_ctrl((0x94)+a);
		if(b==3)
		lcd_ctrl((0xd4)+a);
	}
	
}

void lcd_cursor(void)
{
	lcd_ctrl(0x0E); //cursor on
}
void lcd_cursoroff(void)
{
	lcd_ctrl(0x0C);
}
void lcd_off(void)
{
	lcd_c&=~(1<<2); //vcc off
}
void lcd_on(void)
{
	lcd_c|=(1<<2); //vcc on
}
void lcd_ctrl(unsigned char a)
{
	lcd_c&=~(1<<rs); //rs 0 for cmd
	if(bit==8)
	{
			lcd_d=a;
			lcd_c|=1<<en;
			_delay_ms(5);
			lcd_c&=~(1<<en); //en 1 to 0
	}
	else
	{
		lcd_d=(lcd_d & 0x0f)|(a & 0xf0);
		//lcd_d=a;
		lcd_c|=1<<en;
		_delay_ms(10);
		lcd_c&=~(1<<en);
		a=a<<4;
		lcd_d=(lcd_d & 0x0f)|(a & 0xf0);;
		//lcd_d=a;
		lcd_c|=1<<en;
		_delay_ms(5);
		lcd_c&=~(1<<en);
	}

}
void lcd_print(unsigned char *b)
{
	lcd_c|=1<<rs; //rs 1 for data
	if(bit==8)
	{
		while(*b !='\0')
		{
			lcd_d=*b;
			lcd_c|=1<<en;
			_delay_ms(5);
			lcd_c&=~(1<<en);
			*b++;
		}	
	}
	else
	{
		while(*b !='\0')
		{
			//lcd_d=*b;
			lcd_d=(lcd_d & 0x0f)|(*b & 0xf0);
			lcd_c|=1<<en;
			_delay_ms(5);
			lcd_c&=~(1<<en);
			*b=*b<<4;
			lcd_d=(lcd_d & 0x0f)|(*b & 0xf0);
			lcd_c|=1<<en;
			_delay_ms(5);
			lcd_c&=~(1<<en);
			*b++;
		}
	}
	
}
void lcd_printa(unsigned char q) //ascii value directly
{
		lcd_c|=1<<rs;
		if(bit==8)
		{
			lcd_d=q;
			lcd_c|=1<<en;
			_delay_ms(5);
			lcd_c&=~(1<<en);
		}
		else
		{
			lcd_d=(lcd_d & 0x0f)|(q & 0xf0);
			//lcd_d=q;
			lcd_c|=1<<en;
			_delay_ms(5);
			lcd_c&=~(1<<en);
			q=q<<4;
			lcd_d=(lcd_d & 0x0f)|(q & 0xf0);
			//lcd_d=q;
			lcd_c|=1<<en;
			_delay_ms(5);
			lcd_c&=~(1<<en);
		}
}
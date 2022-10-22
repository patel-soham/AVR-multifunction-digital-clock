/*
 * PROJ.c
 *
 * Created: 30-09-2018 23:21:50
 *  Author: soham
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"
#include "i2c.h"
#include "ds1307.h"

#define mode 2
#define inc 3
#define dec 4
#define toggle 5
#define set 6
#define reset 7

unsigned char s,h,mi,x,da,dt,y,m,t,temp=0,temp1;
unsigned char u[8],u1[8];
unsigned char s0,s1,m0,m1,h0,h1,alm,buff;
data rtc,rtca;
int main(void)
{
	DDRB&=(~(1<<mode))|(~(1<<inc))|(~(1<<dec))|(~(1<<toggle))|(~(1<<set))|(~(1<<reset));
	PORTB|=(1<<mode)|(1<<inc)|(1<<dec)|(1<<toggle)|(1<<set)|(1<<reset);
	lcd_begin(20,4,8);
	rtc_setup();
	clock();
}
void rtc_setup()
{
	rtc.sec=0x57;
	rtc.min=0x59;
	rtc.hr=0x49;
	rtc.day=0x03;
	rtc.date=0x24;
	rtc.month=0x10;
	rtc.yr=0x18;
	rtc_set(&rtc);
}

void timer_set()
{
 t1:lcd_clear();
	u1[8]=0; u1[7]=0; u1[6]=0; u1[5]=0; u1[4]=0; u1[3]=0; u1[2]=0; u1[1]=0; u1[0]=0;
    lcd_setcursor(7,0);
	lcd_print("TIMER");
	lcd_setcursor(0,1);
	lcd_cursor();
	lcd_print("00:00:00");
	lcd_setcursor(7,1);
	temp1=7;
	while(1)
	{
		if(!(PINB&(1<<reset)))
		{
			while(!(PINB&(1<<reset)));
			goto t1;
		}
		if(!(PINB&(1<<mode)))
		{
			while(!(PINB&(1<<mode)));
			alarm_set();
		}
		if(!(PINB&(1<<inc)))
		{
			while(!(PINB&(1<<inc)));
			temp=u1[temp1];
			temp=temp+1;
			if(temp==10)
			{
				temp=0;
			}
			if(temp==6 && (temp1==6 || temp1==3 || temp1==0))
			{
				temp=0;
			}
			u1[temp1]=temp;
			lcd_printa((temp&0x0f)+48);
			lcd_setcursor(temp1,1);
		}
		if(!(PINB&(1<<dec)))
		{
			while(!(PINB&(1<<dec)));
			temp=u1[temp1];
			if(temp==0)
			{
				if(temp1==6 || temp1==3 || temp1==0)
				{
					temp=6;
				}
				else
				{
					temp=10;
				}
			}
			temp=temp-1;
			u1[temp1]=temp;
			lcd_printa((temp&0x0f)+48);
			lcd_setcursor(temp1,1);
		}
		if(!(PINB&(1<<toggle)))
		{
			while(!(PINB&(1<<toggle)));
			temp=0;
			if(temp1==0)
			{
				temp1=8;
			}
			if(temp1==6)
			{
				temp1=5;
			}
			if(temp1==3)
			{
				temp1=2;
			}
			temp1=temp1-1;
			lcd_setcursor(temp1,1);
		}
		if(!(PINB&(1<<set)))
		{
			while(!(PINB&(1<<set)));
			if((u1[1]==0 && u1[0]==0 && u1[2]==0 && u1[3]==0 && u1[4]==0 && u1[5]==0 && u1[6]==0 && u1[7]==0))
			{
				lcd_setcursor(7,3);
				lcd_print("ERROR");
				_delay_ms(1000);
				goto t1;
			}
			timer();
		}
		
	}
}

void timer()
{
	lcd_cursoroff();
	lcd_clear();
	lcd_setcursor(5,1);
	int ts0=u1[7],ts1=u1[6],tm0=u1[4],tm1=u1[3],th0=u1[1],th1=u1[0];
	for(;th1>=0;--th1)
	{
		for(;th0>=0;--th0)
		{
			for(;tm1>=0;--tm1)
			{
				for(;tm0>=0;--tm0)
				{
					for(;ts1>=0;--ts1)
					{
						for(;ts0>=0;--ts0)
						{
							lcd_printa(th1+48);
							lcd_printa(th0+48);
							lcd_print(":");
							lcd_printa(tm1+48);
							lcd_printa(tm0+48);
							lcd_print(":");
							lcd_printa(ts1+48);
							lcd_printa(ts0+48);
							if(!(PINB&(1<<reset)))
							{
								while(!(PINB&(1<<reset)));
								timer_set();
							}
							_delay_ms(1000);
							if(!(PINB&(1<<reset)))
							{
								while(!(PINB&(1<<reset)));
								timer_set();
							}
							lcd_setcursor(5,1);
						}
						if(ts1!=0)
						{
							ts0=9;
						}
					}
					if(tm0!=0)
					{
						ts1=5;
						ts0=9;
					}
				}
				if(tm1!=0)
				{
					tm0=9;
					ts1=5;
					ts0=9;
				}
			}
			if(th0!=0)
			{
				tm1=5;
				tm0=9;
				ts1=5;
				ts0=9;
			}
		}
		if(th1!=0)
		{
			th0=9;
			tm1=5;
			tm0=9;
			ts1=5;
			ts0=9;
		}
	}
	while(1)
	{
		lcd_setcursor(5,3);
		lcd_print("TIME OUT");
		if(!(PINB&(1<<set)))
		{
			while(!(PINB&(1<<set)));
			timer_set();
		}
	}
}

void alarm_set()
{
 a1:lcd_clear();
    lcd_setcursor(7,0);
	lcd_print("ALARM");
	lcd_setcursor(0,1);
	lcd_cursor();
	lcd_print("00:00:00 AM");
	while(alm==1)
	{
		lcd_cursoroff();
		lcd_setcursor(0,1);
		lcd_print("Alarm already set");
		lcd_setcursor(0,2);
		lcd_print("for ");
		lcd_printa((h1>>4)+48);
		lcd_printa(h0+48);
		lcd_print(":");
		lcd_printa(m1+48);
		lcd_printa(m0+48);
		lcd_print(":");
		lcd_printa(s1+48);
		lcd_printa(s0+48);
		if(buff==1)
		{
			lcd_print(" PM");
		}
		else
		{
			lcd_print(" AM");
		}
		lcd_setcursor(0,3);
		lcd_print("RESET ?");
		if(!(PINB&(1<<reset)))
		{
			while(!(PINB&(1<<reset)));
			lcd_clear();
			lcd_setcursor(4,1);
			lcd_print("Alarm removed");
			_delay_ms(1000);
			lcd_clear();
			lcd_setcursor(7,0);
			lcd_print("ALARM");
			lcd_setcursor(0,1);
			lcd_cursor();
			lcd_print("00:00:00 AM");
			u[8]=0; u[7]=0; u[6]=0; u[5]=0; u[4]=0; u[3]=0; u[2]=0; u[1]=0; u[0]=0;
			break;
		}
		if(!(PINB&(1<<mode)))
		{
			while(!(PINB&(1<<mode)));
			clock();
		}
	}
	lcd_setcursor(7,1);
	temp1=7;
	while(1)
	{
		if(!(PINB&(1<<mode)))
		{
			while(!(PINB&(1<<mode)));
			clock();
		}
		if(!(PINB&(1<<reset)))
		{
			while(!(PINB&(1<<reset)));
			u[8]=0; u[7]=0; u[6]=0; u[5]=0; u[4]=0; u[3]=0; u[2]=0; u[1]=0; u[0]=0;
			goto a1;
		}
		if(!(PINB&(1<<inc)))
		{
			while(!(PINB&(1<<inc)));
			temp=u[temp1];
			temp=temp+1;
			if( (temp==10) || (temp==6 && (temp1==6 || temp1==3)) || (temp==2 && temp1==0) )
			{
				temp=0;
			}
			u[temp1]=temp;
			lcd_printa((temp&0x0f)+48);
			lcd_setcursor(temp1,1);
		}
		if(!(PINB&(1<<dec)))
		{
			while(!(PINB&(1<<dec)));
			temp=u[temp1];
			if(temp==0)
			{
				if(temp1==6 || temp1==3)
				{
					temp=6;
				}
				else if(temp1==0)
				{
					temp=2;
				}
				else
				{
					temp=10;
				}
			}
			temp=temp-1;
			u[temp1]=temp;
			lcd_printa((temp&0x0f)+48);
			lcd_setcursor(temp1,1);
		}
		if(!(PINB&(1<<toggle)))
		{
			while(!(PINB&(1<<toggle)));
			temp=0;
			if(temp1==0)
			{
				temp1=8;
			}
			if(temp1==6)
			{
				temp1=5;
			}
			if(temp1==3)
			{
				temp1=2;
			}
			temp1=temp1-1;
			lcd_setcursor(temp1,1);
		}
		if(!(PINB&(1<<set)))
		{
			while(!(PINB&(1<<set)));
			if( ( u[1]>2 && u[0]==1) || (u[1]==0 && u[0]==0) )
			{
				lcd_setcursor(7,3);
				lcd_print("ERROR");
				_delay_ms(1000);
				goto a1;
			}
			while(1)
			{
				lcd_setcursor(9,1);
				if(!(PINB&(1<<toggle)))
				{
					while(!(PINB&(1<<toggle)));
					if(buff==0)
					{
						lcd_print("P");
						lcd_setcursor(9,1);
						buff=1;	
					}
					else
					{
						lcd_print("A");
						lcd_setcursor(9,1);
						buff=0;
					}
				}
				if(!(PINB&(1<<set)))
				{
					while(!(PINB&(1<<set)));
					alarm();
				}
			}
		}
		
	}
}

void alarm()
{
	lcd_clear();
	lcd_cursoroff();
	lcd_setcursor(5,1);
	lcd_print("ALARM SET");
	alm=1;
	s0=u[7];
	s1=u[6];
	s1=s1<<4;
	m0=u[4];
	m1=u[3];
	m1=m1<<4;
	h0=u[1];
	h1=u[0];
	h1=h1<<4;
	if(buff==1)
	{
		h1|=0x20;	
	}
	rtca.sec=((s1|s0));
	rtca.min=((m1|m0));
	rtca.hr=((h1|h0));
	_delay_ms(1000);
	clock();
}

void alarm_ring()
{
	lcd_clear();
	while(1)
	{
		lcd_setcursor(5,1);
		lcd_print("WAKE UP BUDDY");
		if(!(PINB&(1<<set)))
		{
			while(!(PINB&(1<<set)));
			alm=0;
			clock();
		}
	}
	
}

void clock()
{
	s=127;h=127;mi=127;x=127;da=127;dt=127;y=127;m=127;t=127;
	lcd_clear();
	lcd_cursoroff();
	lcd_setcursor(2,0);
	lcd_print(":");
	lcd_setcursor(5,0);
	lcd_print(":");
	while(1)
	{
		if(!(PINB&(1<<mode)))
		{
			while(!(PINB&(1<<mode)));
			timer_set();
		}
		rtc_get(&rtc);
		if( (alm==1) && ((rtc.hr&0x3f)==rtca.hr) && ((rtc.min&0x7f)==rtca.min) && ((rtc.sec&0x7f)==rtca.sec) )
		{
			alarm_ring();
		}
		t=(rtc.hr&0x1f);
		if(h!=t)
		{
			hour();
		}
		
		t=(rtc.min&0x7f);
		if(mi!=t)
		{
			min();
		}
		
		t=(rtc.sec&0x7f);
		if(s!=t)
		{
			sec();
		}
		
		t=(rtc.hr&0x20)>>5;
		if(x!=t)
		{
			stat();
		}
		
		t=(rtc.date&0x3f);
		if(dt!=t)
		{
			date();
		}
		
		t=rtc.month&0x1f;
		if(m!=t)
		{
			month();
		}
		
		t=(rtc.yr);
		if(y!=t)
		{
			year();
		}
		
		t=rtc.day&0x07;
		if(da!=t)
		{
			day();
		}
	}
}

void month()
{
	lcd_setcursor(3,1);
	switch(t)
	{
		case 0x01: lcd_print("Jan ");
		break;
		case 0x02: lcd_print("Feb ");
		break;
		case 0x03: lcd_print("Mar ");
		break;
		case 0x04: lcd_print("Apr ");
		break;
		case 0x05: lcd_print("May ");
		break;
		case 0x06: lcd_print("Jun ");
		break;
		case 0x07: lcd_print("Jul ");
		break;
		case 0x08: lcd_print("Aug ");
		break;
		case 0x09: lcd_print("Sep ");
		break;
		case 0x10: lcd_print("Oct ");
		break;
		case 0x11: lcd_print("Nov ");
		break;
		case 0x12: lcd_print("Dec ");
		break;
	}
	m=t;
}

void hour()
{
	lcd_setcursor(1,0);
	lcd_printa((rtc.hr&0x0f)+48);
	lcd_setcursor(0,0);
	lcd_printa(((rtc.hr>>4)&0x01)+48);
	h=t;
}

void min()
{
	lcd_setcursor(4,0);
	lcd_printa((rtc.min&0x0f)+48);
	lcd_setcursor(3,0);
	lcd_printa(((rtc.min>>4)&0x07)+48);
	mi=t;
}

void stat()
{
	lcd_setcursor(9,0);
	if(t)
	{
		lcd_print(" PM ");
	}
	else
	{
		lcd_print(" AM ");
	}
	x=t;
}

void date()
{
	lcd_setcursor(1,1);
	lcd_printa((rtc.date&0x0f)+48);
	lcd_setcursor(0,1);
	lcd_printa(((rtc.date>>4)&0x03)+48);
	lcd_setcursor(2,1);
	lcd_print(" ");
	dt=t;
}

void year()
{
	lcd_setcursor(7,1);
	lcd_print("20");
	lcd_setcursor(10,1);
	lcd_printa((rtc.yr&0x0f)+48);
	lcd_setcursor(9,1);
	lcd_printa(((rtc.yr>>4)&0x0f)+48);
	y=t;
}

void day()
{
	lcd_setcursor(0,2);
	switch(t)
	{
		case 1: lcd_print("Monday   ");
		break;
		case 2: lcd_print("Tuesday  ");
		break;
		case 3: lcd_print("Wednesday");
		break;
		case 4: lcd_print("Thursday ");
		break;
		case 5: lcd_print("Friday   ");
		break;
		case 6: lcd_print("Saturday ");
		break;
		case 7: lcd_print("Sunday   ");
		break;
		case 0: rtc.day=0x01;
		rtc_set(&rtc);
		break;
	}
	da=t;
}

void sec()
{
	lcd_setcursor(7,0);
	lcd_printa((rtc.sec&0x0f)+48);
	lcd_setcursor(6,0);
	lcd_printa(((rtc.sec>>4)&0x07)+48);
	s=t;
}
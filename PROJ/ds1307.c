/*
 * ds1307.c
 *
 * Created: 01-10-2018 00:13:52
 *  Author: soham
 */ 
#include <avr/io.h>
#include "ds1307.h"
#include "i2c.h"

void rtc_set(data *a)
{
	i2c_init();
	i2c_start();
	i2c_write(0xd0); //address of slave
	i2c_write(0x00); //int address of sec (starting)
	i2c_write(a->sec); 
	i2c_write(a->min);
	i2c_write(a->hr);
	i2c_write(a->day);
	i2c_write(a->date);
	i2c_write(a->month);
	i2c_write(a->yr);
	i2c_stop();
}

void rtc_get(data *b)
{
	i2c_init();
	i2c_start();
	i2c_write(0xd0); //address of master (actually slave)
	i2c_write(0x00); //int address starting
	i2c_start();
	i2c_write(0xd1); //adress of master with read byte 1
	b->sec=i2c_read(1); 
	b->min=i2c_read(1); 
	b->hr=i2c_read(1); 
	b->day=i2c_read(1); 
	b->date=i2c_read(1);
	b->month=i2c_read(1);  
	b->yr=i2c_read(0); 
	i2c_stop();
}

unsigned char conv(unsigned char dat)
{
	unsigned char a,b;
	a=dat&0x0f;
	b=a;
	a=dat&0x70;
	b=(10*a)+b;
}

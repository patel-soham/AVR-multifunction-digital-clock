/*
 * i2c.c
 *
 * Created: 30-09-2018 23:42:03
 *  Author: soham
 */
#include <avr/io.h>
#include "i2c.h"
 
void i2c_init(void)
{
	TWBR=72; //for 50hz sclk n 8mhz avr
	TWCR|=1<<TWEN;
}

void i2c_start(void)
{
	TWCR=(1<<TWSTA)|(1<<TWINT)|(1<<TWEN);
	while(!(TWCR&(1<<TWINT))); 
} 

void i2c_stop(void)
{
	TWCR=(1<<TWSTO)|(1<<TWINT)|(1<<TWEN);
	//while(!(TWCR&(1<<TWINT)));
}

void i2c_write(unsigned char z)
{
	TWDR=z;
	TWCR=(1<<TWINT)|(1<<TWEN);
	while(!(TWCR&(1<<TWINT)));
}
unsigned char i2c_read(unsigned char ack)
{
	if(ack)
	{
		TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	}
	else
	{
		TWCR=(1<<TWINT)|(1<<TWEN);
	}
	while(!(TWCR&(1<<TWINT)));
	return TWDR;
}

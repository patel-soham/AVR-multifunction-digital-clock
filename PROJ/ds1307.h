/*
 * ds1307.h
 *
 * Created: 01-10-2018 00:14:12
 *  Author: soham
 */ 


#ifndef DS1307_H_
#define DS1307_H_

typedef struct{unsigned char sec,min,hr,day,date,month,yr;}data;
	
void rtc_set(data *a);
void rtc_get(data *b);
unsigned char conv(unsigned char dat);

#endif /* DS1307_H_ */
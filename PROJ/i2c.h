/*
 * i2c.h
 *
 * Created: 30-09-2018 23:42:21
 *  Author: soham
 */ 


#ifndef I2C_H_
#define I2C_H_

void i2c_init(void);
void i2c_start();
void i2c_stop(void);
void i2c_write(unsigned char);
unsigned char i2c_read(unsigned char);

#endif /* I2C_H_ */
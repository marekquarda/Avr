/*
 * i2cmaster.h
 *
 * Author : Marek Xara Quarda
 */ 
#ifndef I2CMASTER_H_
#define I2CMASTER_H_

#include <avr/io.h>
#include <inttypes.h>
#include "compat/twi.h"

// data direction
#define I2C_READ        1
#define I2C_WRITE       0

// functions
extern void i2c_init(void);
extern void i2c_stop(void);
extern unsigned char i2c_start(unsigned char addr);
extern unsigned char i2c_rep_start(unsigned char addr);
extern void i2c_start_wait(unsigned char addr);
extern unsigned char i2c_write(unsigned char data);
extern unsigned char i2c_readAck(void);
extern unsigned char i2c_readNak(void);
extern unsigned char i2c_read(unsigned char ack);

#define i2c_read(ack) ?i2c_readAck() : i2c_readNak();

#endif
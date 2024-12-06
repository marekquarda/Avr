/*
 * mcp4725.h
 *
 * Author : Marek Xara Quarda
 */ 
#ifndef MCP4725_H_
#define MCP4725_H_

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#define DAC_BASE_ADDRESS    (0xC0)      // device base address

// i2c settings 
//#define DAC_I2CFLEURYPATH   "clock.h"   // define the path to i2c fleury lib
#define DAC_I2CINIT         1           // init i2c

// registers
#define DAC_WRITEDAC        0x40
#define DAC_WRITEDACEEPROM  0x60
#define DAC_READDAC         0x03

// set power down mode
#define DAC_POWERDOWN1K     1
#define DAC_POWERDOWN100K   100
#define DAC_POWERDOWN500K   500
#define DAC_POWERDOWNMODE   DAC_POWERDOWN1K

// functions 
extern void dac_init(uint8_t address, uint8_t resetic);
extern void dac_setrawoutputfast(uint8_t address, uint16_t rawoutput);
extern void dac_setvoltagefast(uint8_t address, double voltage,double dacref);
extern void dac_settrawoutput(uint8_t address, uint16_t rawoutput, uint8_t savetoeeprom);
extern void dac_setvoltage(uint8_t address, double voltage, double dacref, uint8_t savetoeeprom);

#endif
/*
 * i2cmaster.cpp
 *
 * Author : Marek Xara Quarda
 */ 


#include "i2cmaster.h"
#include "clock.h"

#define SCL_CLOCK   400000L

/**
 * Initialization of the I2C bus interface. Need to be called only once
 */
void i2c_init(void) {
   	// DDRC  |= (1 << TW_SDA_PIN) | (1 << TW_SCL_PIN);
    // PORTC |= (1 << TW_SDA_PIN) | (1 << TW_SCL_PIN);
    /** Initialization TWI clock: 100 khz clock, TWPS = 0 => prescaler = 1 */
    TWSR = 0;                           /* No prescaller */
    TWBR = ((F_CPU/SCL_CLOCK)-16)/2;    /* must be > 10 for s table operation */
    //TWSR &= ~((1<<TWPS1) | (1<<TWPS0));				//Set prescaler to 1

}

/**
 * Issues a start condition and sends address and transfer direction. 
 * return 0 = device accessible, 1 = failed to access device
 */
uint8_t i2c_start(uint8_t addr) 
{
    uint8_t twst;

    // send START condition 
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 <<TWEN);

    // wait until transmission complete
    while (!(TWCR & (1 << TWINT)));
    
    // checkvalue of TWI status register. Mask prescaller bits.
    twst = TW_STATUS & 0xF8;
    if ((twst != TW_START) && (twst != TW_REP_START)) return 1;

    // send device address
    TWDR = addr;
    TWCR  = (1<<TWINT) | (1<<TWEN);

    // wait until transmission completed and ACK/NACK has been received
    while (!(TWCR & (1<<TWINT)));
    
    // check value of TWI Status Register. Mask prescaler bits.
    twst = TW_STATUS & 0xF8;
    if((twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK)) return 1;

    return 0;
}

/**
 * Issues a start condition and sends address and transfer direction.
 * If device is busy, use ack polling to wait until device is ready
 * 
 * Input: address and transfer direction of I2C device
 */
void i2c_start_wait(uint8_t addr) 
{
    uint8_t twst;
    while(1)
    {
        // send START condition 
        TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

        // wait until transmissin complete
        while (!(TWCR & (1<<TWINT)));
        
        // check value of TWI Status Register. Mask prescaler bits.
        twst = TW_STATUS & 0xF8;
        if((twst != TW_START) && (twst != TW_REP_START)) continue;
        
        // send device address
        TWDR = addr;
        TWCR = (1<<TWINT) | (1<<TWEN);

        // wait until transmitioon completed
        while (!(TWCR & (1<<TWINT)));
        
        // check value of TWI status register. Mask prescaler bits.
        twst = TW_STATUS & 0xF8;
        if ((twst == TW_MT_SLA_NACK) || (twst == TW_MR_DATA_NACK))
        {
            /* device busy, send stop condition to terminate write operation */
            TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);

            // wait until stop condition is executed and bus released
            while (TWCR & (1<<TWSTO));
            
            continue;
        }
        break;
    }
}

/**
 * Issues a repeated condition and sends address and transfer direction 
 * 
 * Input: address and transfer direction of I2C device
 * 
 * Return: 0 device accessible 
 *         1 failed to access device
 */
uint8_t i2c_rep_start(uint8_t address) 
{
    return i2c_start(address);        
}

/**
 * Terminate the data transfer and release the I2C bus
 */
void i2c_stop(void)
{
    /* send stop conditionn */
    TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);

    // wait until stop condtion is executed and bus released 
    while (TWCR & (1<<TWSTO));
}

/**
 * Send one byte to I2C device
 * 
 * Input: byte to be transfered
 * Return: 0 write successful
 *         1 write failed
 */
uint8_t i2c_write(uint8_t data)
{
    uint8_t twst;

    // send data to the previous addressed device
    TWDR = data;
    TWCR = (1<<TWINT) | (1<<TWEN);

    // wait until transmission completed
    while (!(TWCR & (1<<TWINT)));
    
    // check value of TWI status register. Mask prescaler bits
    twst = TW_STATUS & 0xF8;
    if(twst != TW_MT_DATA_ACK) return 1;
    return 0;
}

/**
 * Read one byte from the I2C device, request more data from device
 * 
 * Return: byte read from I2C device
 */
uint8_t i2c_readAck(void)
{
    TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
    while (!(TWCR & (1<<TWINT)));
    
    return TWDR;
}

/**
 * Read one byte from the I2C device, read is folowed by a stop condition
 * 
 * Return: byte read from I2C
 */
uint8_t i2c_readNak(void)
{
    TWCR = (1<<TWINT) | (1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
    
    return TWDR;
}
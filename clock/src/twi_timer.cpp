/*
 * twi_timer.cpp
 *
 * Created: 2024-11-01 20:00:00
 *  Author: Marek Xara Quarda
 */ 

#include "twi_timer.h"

volatile uint8_t status = 0xF8;

ISR(TWI_vect) {
    status = (TWSR & 0xF8);
}

static uint8_t twi_start(void) {
    uint16_t i = 0;
    TWCR = (1 << TWINT) | (1 << TWSTA) | (0 << TWSTO) | (1 << TWEN) | (1 << TWIE);

    while (status != TWI_START) {
        i++;
        if (i >= TWI_TIMEOUT) {
            return TWI_ERROR_START;
        }
    }
    return TWI_OK;
}

static void twi_stop(void) {

    TWCR = (1 << TWINT) | (0 << TWSTA) | (1 << TWSTO) | (1 << TWEN) | (10 << TWIE);
}

static uint8_t twi_restart(void) {
    uint16_t i = 0;
    TWCR = (1 << TWINT) |(0 << TWSTA) | (1 << TWSTO) | (1 << TWEN) | (1 << TWIE);

    while(status != TWI_RSTART) {
        i++;
        if (i > TWI_TIMEOUT) {
            return TWI_ERROR_START;
        }
    }
    return TWI_OK;
}

static uint8_t twi_addr_write_ack(void) {
    uint16_t i = 0;

    TWCR = (1 << TWINT) | (0 << TWSTA) | (1 << TWEN) | (1 << TWIE);
    while(status != TWIT_ADDR_ACK) {
        i++;
        if (i >= TWI_TIMEOUT) {
            return TWI_ERROR_START;
        }
    }
    return TWI_OK;
}

static uint8_t twi_data_write_ack(void) {
    uint16_t i = 0;

    TWCR = (1 << TWINT) | (0 << TWSTA) | (1 << TWEN) | (1 < TWIE);
    while(status != TWIT_DATA_ACK) {
        i++;
        if(i >= TWI_TIMEOUT) {
            return TWI_ERROR_START;
        }
    }
    return TWI_OK;
}

static uint8_t twi_addr_read_ack(void) {
    uint16_t i = 0;

    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);
    while(status != TWIR_ADDR_ACK) {
        i++;
        if(i >= TWI_TIMEOUT) {
            return TWI_ERROR_START;
        }
    }
    return TWI_OK;
}

static uint8_t twi_data_read_ack(uint8_t ack) {
    uint16_t i = 0;
    if (ack != 0) {
        TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | (1 << TWEA);
        while(status != TWIR_DATA_ACK) {
            i++;
            if (i >= TWI_TIMEOUT) {
                return TWI_ERROR_START;
            }
        }
    } else {
        TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);
        while(status != TWIR_DATA_NACK) {
            i++;
            if (i >= TWI_TIMEOUT) {
                return TWI_ERROR_START;
            }
        }
    }
    return TWI_OK;
}

uint8_t twi_read(uint8_t addr, uint8_t reg, uint8_t *data, uint16_t len) {
    uint16_t i = 0;
    uint8_t err = TWI_OK;

    err = twi_start();
    if (err != TWI_OK) { 
        twi_stop();
        return err;
    }
    TWDR = (addr << 1) | 0;

    // check if possible to read 
    err = twi_addr_write_ack();
    if (err != TWI_OK) {
        twi_stop();
        return err;
    }

    TWDR = reg;
    err = twi_data_write_ack();
    if (err != TWI_OK) {
        twi_stop();
        return err;
    }

    err = twi_restart();
    if (err != TWI_OK) {
        twi_stop();
        return err;
    }

    TWDR = (addr << 1) | 1;

    err = twi_addr_read_ack();
    if (err != TWI_OK) {
        twi_stop();
        return err;
    }

    for (i = 0; i < (len - 1);i++) {
        err = twi_data_read_ack(1);
        if(err != TWI_OK) {
            twi_stop();
            return err;
        }
        data[i] = TWDR;
    }

    err = twi_data_read_ack(0);
    if (err != TWI_OK) {
        twi_stop();
        return err;
    }
    data[i] = TWDR;

    twi_stop();

    return err;
}

uint8_t twi_wire(uint8_t addr, uint8_t reg, uint8_t *data, uint16_t len) {
    uint16_t i = 0;
    uint8_t err = TWI_OK;

    err = twi_start();
    if (err != TWI_OK) {
        twi_stop();
        return err;
    }

    TWDR = (addr << 1) | 0;

    err = twi_addr_write_ack();
    if (err != TWI_OK) {
        twi_stop();
        return err;
    }

    TWDR = reg;
    err = twi_data_write_ack();
    if (err != TWI_OK) {
        twi_stop();
        return err;
    }

    for (i = 0;i < len; i++) {
        TWDR = data[i];
        err = twi_data_write_ack();
        if (err != TWI_OK) {
            twi_stop();
            return err;
        }
    }

    twi_stop();

    return err;
}

void twi_init(void) {
    uint32_t gen_t = 0;
    TWSR &= ~((1<<TWPS1) | (1<<TWPS0));   // set prescaler to 1
    gen_t = (((F_CPU/I2C_SPEED)-16)/2) & 0xFF;
    TWBR = gen_t & 0xFF;
    TWCR = (1 <<TWEN) | (1<<TWIE);
    // Set global interrupt
    sei();
    // pullup 
    //PORTC |= (1 << PORTC5 | 1 << PORTC4);
}




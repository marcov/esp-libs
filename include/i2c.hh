/*
 * i2c.h
 *
 *  Created on: Feb 4, 2016
 *      Author: Marco Vedovati
 */

#ifndef __I2C_HH__
#define __I2C_HH__

#include <stdint.h>
#include <stdbool.h>

class I2C {
private:
    static constexpr unsigned I2C_READ_TRIES = 20;
    static constexpr unsigned I2C_CLK_FREQ = 50000;
    unsigned slave_addr;
    int sda_pin;
    int scl_pin;

public:
    I2C(int sda, int scl) : sda_pin{sda}, scl_pin{scl}  {}

    void    init(volatile void *p_i2c_peripheral);
    int     gen_start(uint8_t slave_address, bool mode_tx);
    int     tx(uint8_t * p_data, uint32_t len, bool gen_stop);
    int     rx(uint8_t * p_data, uint32_t len);
    void    gen_stop(void);
    int     wait_busy(void);
    void    reset(void);
};



#endif /* __I2C_HH__ */


#include <Arduino.h>
#include <Wire.h>
#include "i2c.hh"


void I2C::init(volatile void *p_i2c_peripheral)
{
    (void)p_i2c_peripheral;

    // Serial.println("i2c init sda:" + String(sda_pin) + " scl: " + String(scl_pin));
    Wire.begin(sda_pin, scl_pin);
    Wire.setClock(I2C_CLK_FREQ);
}

int I2C::gen_start(uint8_t slave_address, bool mode_tx)
{
    slave_addr = slave_address;
    if (!mode_tx)   return 0;
    Wire.beginTransmission(slave_address);

    return 0;
}

int I2C::tx(uint8_t * p_data, uint32_t len, bool gen_stop)
{
    Wire.write(p_data, len);

    Wire.endTransmission(gen_stop);

    return 0;
}


int I2C::rx(uint8_t * p_data, uint32_t len)
{
    unsigned tries = 0;

    Wire.requestFrom(slave_addr, len, true);

    while (Wire.available() < len)
    {
        if (++tries >= I2C_READ_TRIES)  return -1;
        delay(50);
    }

    for (unsigned i = 0; i < len; i++)
    {
        p_data[i] = Wire.read();
    }

    return 0;
}

void I2C::gen_stop(void)
{
}

int I2C::wait_busy(void)
{
    return 0;
}

void I2C::reset(void)
{

}

#ifndef __SI7005SENSOR_HH__
#define __SI7005SENSOR_HH__

#include <Arduino.h>
#include "sensor.hh"
#include "i2c.hh"


class Si7005Sensor: public Sensor {

private:
    static constexpr uint8_t SI7005_I2C_ADDR        = 0x40;
    static constexpr int32_t SI7005_MEASURE_INVALID = INT32_MAX;
    static constexpr uint8_t SI7005_ID_VALUE        = 0x50;
    I2C   i2c{0,0};
    void  (*  mydelay)(unsigned long ms);

    typedef enum
    {
        SI7005_STATUS_REG_ADR   = 0x00,
        SI7005_DATA_REG_ADR     = 0x01,
        SI7005_CONF_REG_ADR     = 0x03,
        SI7005_ID_REG_ADR       = 0x11,
    } register_t;

    int     read_register(uint8_t * p_rd_buffer, register_t reg, uint8_t reg_len);
    int     start_conversion_common(uint8_t conf_val);
    int     start_conversion_temperature(void);
    int     start_conversion_humidity(void);
    int     wait_ready(void);
    int32_t get_temperature_x10(void);
    int32_t get_humidity_x10(void);
    uint8_t get_id(void);
    float   get_temperature(void);
    float   get_humidity(void);

public:
    Si7005Sensor(uint8_t i2c_sda, uint8_t i2c_sdl, void (*delayfx)(unsigned long)) : mydelay{delayfx} {
        i2c = I2C {i2c_sda, i2c_sdl};
    }

    bool  init();
    float readTemperature();
    float readHumidity();
};


#endif /* #ifndef __SI7005_HH__ */

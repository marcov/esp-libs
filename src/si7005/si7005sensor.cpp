#include "si7005sensor.hh"

#if defined(SI7005_LOG_ENABLED)
#  define SI7005_LOG(...) Serial.println(String("si7005 ") + __VA_ARGS__)
#else
#  define SI7005_LOG(...)
#endif

bool Si7005Sensor::init() {
    i2c.init(nullptr);

    return (get_id() == SI7005_ID_VALUE);
}

int Si7005Sensor::read_register(uint8_t * p_rd_buffer, register_t reg, uint8_t reg_len)
{
    uint8_t tx_buffer[1];

    if (i2c.wait_busy())
    {
        return -1;
    }

    if (i2c.gen_start(SI7005_I2C_ADDR, true))
    {
        return -1;
    }


    tx_buffer[0] = reg;
    if (i2c.tx(tx_buffer, 1, false))
    {
        return  -1;
    }


    //restart
    if (i2c.gen_start(SI7005_I2C_ADDR, false))
    {
        return -1;
    }

    if (i2c.rx(p_rd_buffer, reg_len))
    {
        return -1;
    }

    if (i2c.wait_busy())
    {
        return -1;
    }

    return 0;
}


int Si7005Sensor::start_conversion_common(uint8_t conf_val)
{
    uint8_t i2cbuffer[2];


    SI7005_LOG("start conversion");


    if (i2c.gen_start(SI7005_I2C_ADDR, true))
    {
        return -1;
    }
    // Start conversion
    i2cbuffer[0] = SI7005_CONF_REG_ADR;
    i2cbuffer[1] = conf_val;

    if (i2c.tx(i2cbuffer, 2, true))
    {
        return -1;
    }

    if (i2c.wait_busy())
    {
        return -1;
    }

    return 0;
}

int Si7005Sensor::start_conversion_temperature(void)
{
    return Si7005Sensor::start_conversion_common(0x11);
}

int Si7005Sensor::start_conversion_humidity(void)
{
    return Si7005Sensor::start_conversion_common(0x01);
}

int Si7005Sensor::wait_ready(void)
{
    uint8_t i2cbuffer[1];

    SI7005_LOG("waiting ready...");
    while (true)
    {
        mydelay(50);

        if (Si7005Sensor::read_register(i2cbuffer, SI7005_STATUS_REG_ADR, 1))
        {
            return -1;
        }

        if ((i2cbuffer[0] & 0x01) != 0)
        {
            SI7005_LOG("conversion in progress");
        }
        else
        {
            SI7005_LOG("conversion done");
            break;
        }
    }

    return 0;
}


float Si7005Sensor::get_temperature(void)
{
    uint8_t i2cbuffer[2];
    unsigned tmp;
    float fTemp;

    memset(i2cbuffer, 0, sizeof(i2cbuffer));

    if (Si7005Sensor::read_register(i2cbuffer, SI7005_DATA_REG_ADR, 2))
    {
        return 0.0f;
    }

    tmp = (((unsigned)(i2cbuffer[0]) << 8) | (unsigned)(i2cbuffer[1]));

    // Use only 14 bits
    tmp >>= 2;

    SI7005_LOG(String("t 14bit hex:") + String(tmp,16));

    fTemp = static_cast<float>(tmp) / 32.0f - 50.0f;

    return fTemp;
}


int32_t Si7005Sensor::get_temperature_x10(void)
{
    uint8_t i2cbuffer[2];
    int32_t temperature;

    memset(i2cbuffer, 0, sizeof(i2cbuffer));

    if (Si7005Sensor::read_register(i2cbuffer, SI7005_DATA_REG_ADR, 2))
    {
        return SI7005_MEASURE_INVALID;
    }


    temperature = (((uint32_t)(i2cbuffer[0]) << 8) | (uint32_t)(i2cbuffer[1]));

    // Use only 14 bits
    temperature >>= 2;

    SI7005_LOG(String("t 14bit hex:") + String(temperature, 16));

    temperature = (temperature * 10 / 32) - (50 * 10);

    return temperature;
}

int32_t Si7005Sensor::get_humidity_x10(void)
{
    uint8_t i2cbuffer[2];
    uint32_t humidity;

    memset(i2cbuffer, 0, sizeof(i2cbuffer));

    if (Si7005Sensor::read_register(i2cbuffer, SI7005_DATA_REG_ADR, 2))
    {
        return SI7005_MEASURE_INVALID;
    }

    humidity = (((uint32_t)(i2cbuffer[0]) << 8) | (uint32_t)(i2cbuffer[1]));

    // use only 12 bits
    humidity >>= 4;

    SI7005_LOG(String("t 12bit hex:") + String(humidity, 16));

    humidity = (humidity * 10 / 16) - (24 * 10);

    return humidity;
}


float Si7005Sensor::get_humidity(void)
{
    int32_t hum_x10 = Si7005Sensor::get_humidity_x10();

    if (hum_x10 == SI7005_MEASURE_INVALID)
    {
        return 0.0f;
    }

    return (float)hum_x10 / 10.0f;
}


uint8_t Si7005Sensor::get_id(void)
{
    uint8_t i2cbuffer[2];

    if (Si7005Sensor::read_register(i2cbuffer, SI7005_ID_REG_ADR, 1) == 0)
    {
        return i2cbuffer[0];
    }

    SI7005_LOG("get id failed");

    return 0x00;
}



float Si7005Sensor::readTemperature() {
    start_conversion_temperature();
    wait_ready();
    return get_temperature();
}

float Si7005Sensor::readHumidity() {
    start_conversion_humidity();
    wait_ready();
    return get_humidity();
}

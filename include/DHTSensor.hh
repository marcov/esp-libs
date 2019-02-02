#ifndef __DHTSENSOR_HH__
#define __DHTSENSOR_HH__

#include <DHT.h>
#include "sensor.hh"

class DHTSensor : public Sensor {

private:
    DHT dht{0,0};

public:
    DHTSensor(uint8_t dht_pin, uint8_t dht_model) {
        dht =  DHT{dht_pin, dht_model};
    }

    bool init() {
        dht.begin();
        return true;
    }

    float readTemperature() {
        return dht.readTemperature();
    }

    float readHumidity() {
        return dht.readHumidity();
    }
};

#endif /* #ifndef __DHTSENSOR_HH__ */

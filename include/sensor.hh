#ifndef __SENSOR_HH__
#define __SENSOR_HH__

class Sensor {

private:

public:
    virtual bool  init() = 0;
    virtual float readTemperature() = 0;
    virtual float readHumidity()    = 0;

};

#endif /* #ifndef __SENSOR_HH__ */

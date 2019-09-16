#ifndef _MS5611SENSOR_H_
#define _MS5611SENSOR_H_

#include "mbed.h"
#include "ms5611.h"
#include "DataTypes.h"
#include "Sensor.h"


class MS5611Sensor : public Sensor, private ms5611
{
public:
    MS5611Sensor(PinName sda, PinName scl);
    ~MS5611Sensor();

protected:
    virtual mbed_error_status_t setup() override;
    virtual mbed_error_status_t read(SensorData*) override;

private:

};

#endif // _MS5611SENSOR_H_
#ifndef _MS5611SENSOR_H_
#define _MS5611SENSOR_H_

#include "mbed.h"
#include "ms5611.h"
#include "DataTypes.h"
#include "Sensor.h"


class MS5611Sensor : public Sensor, private ms5611
{
public:
    MS5611Sensor(const char name[], PinName sda, PinName scl);
    virtual ~MS5611Sensor();

    virtual SensorData* lastValue() override;

protected:
    virtual mbed_error_status_t setup() override;
    virtual mbed_error_status_t read() override;

private:
    PressureData _last_value;
};

#endif // _MS5611SENSOR_H_
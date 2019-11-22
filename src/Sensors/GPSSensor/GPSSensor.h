#ifndef _GPSSENSOR_H_
#define _GPSSENSOR_H_

#include "mbed.h"
#include "DataTypes.h"
#include "Sensor.h"

class GPSSensor : public Sensor
{
public:
    GPSSensor(const char name[], PinName rx, PinName pps);
    virtual ~GPSSensor();

    virtual SensorData* lastValue() override;

protected:
    virtual mbed_error_status_t setup() override;
    virtual mbed_error_status_t read() override;

private:
    GPSData _last_value;
};

#endif // _MS5611SENSOR_H_
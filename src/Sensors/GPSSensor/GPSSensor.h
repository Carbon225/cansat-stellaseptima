#ifndef _GPSSENSOR_H_
#define _GPSSENSOR_H_

#include "Sensor.h"
#include "GPSData.h"
#include "GPS.h"

class GPSSensor : public Sensor, private GPS
{
public:
    GPSSensor(const char name[], PinName rx, PinName pps = NC);
    virtual ~GPSSensor();

    virtual SensorData* lastValue() override;

protected:
    virtual mbed_error_status_t setup() override;
    virtual mbed_error_status_t read() override;

private:
    GPSData _last_value;
};

#endif // _MS5611SENSOR_H_
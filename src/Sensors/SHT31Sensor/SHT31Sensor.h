#ifndef _SHT31SENSOR_H_
#define _SHT31SENSOR_H_

#include "mbed.h"
#include "Sht31.h"
#include "DataTypes.h"
#include "Sensor.h"


class SHT31Sensor : public Sensor, private Sht31
{
public:
    SHT31Sensor(const char name[], PinName sda, PinName scl);
    virtual ~SHT31Sensor();

    virtual SensorData* lastValue() override;

protected:
    virtual mbed_error_status_t setup() override;
    virtual mbed_error_status_t read() override;

private:
    SHT31Data _last_value;
};

#endif // _SHT31SENSOR_H_
#ifndef _BMP280SENSOR_H_
#define _BMP280SENSOR_H_

#include "mbed.h"
#include "BMP280.h"
#include "DataTypes.h"
#include "Sensor.h"


class BMP280Sensor : public Sensor, private BMP280
{
public:
    BMP280Sensor(PinName sda, PinName scl);
    virtual ~BMP280Sensor();

    virtual SensorData* lastValue() override;

protected:
    virtual mbed_error_status_t setup() override;
    virtual mbed_error_status_t read() override;

private:
    PressureData _last_value;
};

#endif // _MS5611SENSOR_H_
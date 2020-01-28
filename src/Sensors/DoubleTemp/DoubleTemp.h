#ifndef _DOUBLETEMP_H_
#define _DOUBLETEMP_H_

#include "Sensor.h"
#include "DoubleTempData.h"
#include "Sht31.h"

class DoubleTemp : public Sensor
{
public:
    DoubleTemp(const char name[], PinName sda1, PinName scl1, PinName sda2, PinName scl2);
    virtual ~DoubleTemp();

    virtual SensorData* lastValue() override;

protected:
    virtual mbed_error_status_t setup() override;
    virtual mbed_error_status_t read() override;

private:
    Sht31 _sht1, _sht2;
    DoubleTempData _last_value;
};

#endif
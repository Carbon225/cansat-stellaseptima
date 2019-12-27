#ifndef _TRIPLEBARO_H_
#define _TRIPLEBARO_H_

#include "Sensor.h"
#include "PressureData.h"
#include "ms5611.h"
#include "BMP280.h"

#define PRESS_DIVERGENCE 2.f

class TripleBaro : public Sensor
{
public:
    TripleBaro(const char name[], PinName sda1, PinName scl1, PinName sda2, PinName scl2);
    virtual ~TripleBaro();

    virtual SensorData* lastValue() override;

protected:
    virtual mbed_error_status_t setup() override;
    virtual mbed_error_status_t read() override;

private:
    static double calculateTMR(double v1, double v2, double v3, double divergence);

    PressureData _last_value;

    ms5611 _ms1;
    BMP280 _bmp1, _bmp2;
};

#endif
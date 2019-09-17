#include "MS5611Sensor.h"
#include "BLELogger.h"

MS5611Sensor::MS5611Sensor(PinName sda, PinName scl)
: Sensor("ms5611"), ms5611(sda, scl)
{

}

MS5611Sensor::~MS5611Sensor()
{

}

SensorData* MS5611Sensor::lastValue()
{
    return &_last_value;
}

mbed_error_status_t MS5611Sensor::setup()
{
    ms5611::cmd_reset();
    LOGI("MS5611 started\n");
    return MBED_SUCCESS;
}

mbed_error_status_t MS5611Sensor::read()
{
    _last_value.temperature = ms5611::calcTemp();
    _last_value.pressure = ms5611::calcPressure();

    return MBED_SUCCESS;
}
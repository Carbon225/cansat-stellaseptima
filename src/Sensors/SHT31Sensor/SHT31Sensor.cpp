#include "SHT31Sensor.h"
#include "BLELogger.h"

SHT31Sensor::SHT31Sensor(PinName sda, PinName scl)
: Sensor("sht31"), Sht31(sda, scl)
{

}

SHT31Sensor::~SHT31Sensor()
{

}

SensorData* SHT31Sensor::lastValue()
{
    return &_last_value;
}

mbed_error_status_t SHT31Sensor::setup()
{
    LOGI("SHT31 started\n");
    return 0;
}

mbed_error_status_t SHT31Sensor::read()
{
    _last_value.temperature = Sht31::readTemperature();
    _last_value.humidity = Sht31::readHumidity();

    return MBED_SUCCESS;
}
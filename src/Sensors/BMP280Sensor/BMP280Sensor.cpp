#include "BMP280Sensor.h"
#include "BLELogger.h"

BMP280Sensor::BMP280Sensor(PinName sda, PinName scl)
: Sensor("bmp280"), BMP280(sda, scl)
{

}

BMP280Sensor::~BMP280Sensor()
{

}

SensorData* BMP280Sensor::lastValue()
{
    return &_last_value;
}

mbed_error_status_t BMP280Sensor::setup()
{
    BMP280::initialize();
    LOGI("BMP280 started\n");
    return MBED_SUCCESS;
}

mbed_error_status_t BMP280Sensor::read()
{
    float temp = BMP280::getTemperature();
    float press = BMP280::getPressure();

    _last_value.pressure = press;

    return MBED_SUCCESS;
}
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
    static float temp = BMP280::getTemperature();
    static float press = BMP280::getPressure();

    LOGI("temp = %.2f press = %.2f\n", temp, press);
    LOGI("%#x %#x\n", temp, press);

    _last_value.pressure = press;
    // return MBED_ERROR_CODE_NOT_READY;

    return MBED_SUCCESS;
}
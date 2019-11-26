#include "GPSSensor.h"
#include "BLELogger.h"

GPSSensor::GPSSensor(const char name[], PinName rx, PinName pps)
: Sensor(name), _last_value(name), GPS(rx, pps)
{

}

GPSSensor::~GPSSensor()
{

}

SensorData* GPSSensor::lastValue()
{
    _last_value.lat = (double) GPS::GLL().latitude.value / (double) GPS::GLL().latitude.scale;
    _last_value.lng = (double) GPS::GLL().longitude.value / (double) GPS::GLL().longitude.scale;
    return &_last_value;
}

mbed_error_status_t GPSSensor::setup()
{
    GPS::begin();
    return MBED_SUCCESS;
}

mbed_error_status_t GPSSensor::read()
{
    return MBED_SUCCESS;
}
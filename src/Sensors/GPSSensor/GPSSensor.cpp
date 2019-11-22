#include "GPSSensor.h"
#include "BLELogger.h"

GPSSensor::GPSSensor(const char name[], PinName rx, PinName pps)
: Sensor(name), _last_value(name)
{

}

GPSSensor::~GPSSensor()
{

}

SensorData* GPSSensor::lastValue()
{
    return &_last_value;
}

mbed_error_status_t GPSSensor::setup()
{
    return MBED_SUCCESS;
}

mbed_error_status_t GPSSensor::read()
{
    return MBED_SUCCESS;
}
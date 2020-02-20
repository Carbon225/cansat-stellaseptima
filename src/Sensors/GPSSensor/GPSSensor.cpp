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
    minmea_sentence_gll gll = GPS::GLL();
    _last_value.lat = minmea_tocoord(&gll.latitude);
    _last_value.lng = minmea_tocoord(&gll.longitude);
    return &_last_value;
}

mbed_error_status_t GPSSensor::setup()
{
    // GPS::begin();
    LOGI("GPS started\n");
    return MBED_SUCCESS;
}

mbed_error_status_t GPSSensor::read()
{
    GPS::read();
    return MBED_SUCCESS;
}
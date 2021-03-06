#include "DoubleTemp.h"
#include "BLELogger.h"

DoubleTemp::DoubleTemp(const char name[], PinName sda1, PinName scl1, PinName sda2, PinName scl2)
: Sensor(name), _last_value(name),
  _sht1(sda1, scl1), _sht2(sda2, scl2)
{

}

DoubleTemp::~DoubleTemp()
{

}

SensorData* DoubleTemp::lastValue()
{
    return &_last_value;
}

mbed_error_status_t DoubleTemp::setup()
{
    LOGI("SHT31 started\n");
    return 0;
}

mbed_error_status_t DoubleTemp::read()
{
    double temp1 = _sht1.readTemperature();
    ThisThread::sleep_for(20);
    double hum1 = _sht1.readHumidity();

    double temp2 = _sht2.readTemperature();
    ThisThread::sleep_for(20);
    double hum2 = _sht2.readHumidity();

    _last_value.temp1 = temp1;
    _last_value.temp2 = temp2;
    _last_value.hum1 = hum1;
    _last_value.hum2 = hum2;

    bool data1Valid = SHT31Data(temp1, hum1).valid();
    bool data2Valid = SHT31Data(temp2, hum2).valid();

    if (data1Valid && data2Valid) {
        _last_value.temperature = (temp1 + temp2) / 2.f;
        _last_value.humidity = (hum1 + hum2) / 2.f;
    }
    else if (data1Valid) {
        _last_value.temperature = temp1;
        _last_value.humidity = hum1;
    }
    else if (data2Valid) {
        _last_value.temperature = temp2;
        _last_value.humidity = hum2;
    }
    else {
        _last_value.temperature = -100.f;
        _last_value.humidity = 200.f;
        return MBED_ERROR_READ_FAILED;
    }

    return MBED_SUCCESS;
}
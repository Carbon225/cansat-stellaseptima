#include "SHT31Sensor.h"
#include "BLELogger.h"

SHT31Sensor::SHT31Sensor(PinName sda, PinName scl)
: Sensor(sizeof(SHT31Data), "sht31"), Sht31(sda, scl)
{

}

SHT31Sensor::~SHT31Sensor()
{

}

mbed_error_status_t SHT31Sensor::setup()
{
    LOGI("SHT31 started\n");
    return 0;
}

mbed_error_status_t SHT31Sensor::read(SensorData* data)
{
    double temp = Sht31::readTemperature();
    double hum = Sht31::readHumidity();

    new(data) SHT31Data(temp, hum);

    return MBED_SUCCESS;
}
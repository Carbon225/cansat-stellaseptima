#include "SHT31Sensor.h"

SHT31Sensor::SHT31Sensor(PinName sda, PinName scl)
: Sensor(), Sht31(sda, scl)
{

}

SHT31Sensor::~SHT31Sensor()
{

}

mbed_error_status_t SHT31Sensor::setup()
{
    printf("SHT31 started\n");
    return 0;
}

mbed_error_status_t SHT31Sensor::read(SensorData* data)
{
    double temp = Sht31::readTemperature();
    double hum = Sht31::readHumidity();

    new(data) SHT31Data(temp, hum);

    return MBED_SUCCESS;
}
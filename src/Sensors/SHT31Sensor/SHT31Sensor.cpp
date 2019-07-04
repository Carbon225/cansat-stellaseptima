#include "SHT31Sensor.h"

extern Serial pc;


SHT31Sensor::SHT31Sensor(PinName sda, PinName scl)
: Sensor(), Sht31(sda, scl)
{

}

SHT31Sensor::~SHT31Sensor()
{

}

int SHT31Sensor::setup()
{
    pc.printf("SHT31 started\n");
    return 0;
}

int SHT31Sensor::read(SensorData* data)
{
    double temp = Sht31::readTemperature();
    double hum = Sht31::readHumidity();

    new(data) SHT31Data(temp, hum);

    return MBED_SUCCESS;
}
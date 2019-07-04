#include "MS5611Sensor.h"

extern Serial pc;


MS5611Sensor::MS5611Sensor(PinName sda, PinName scl)
: Sensor(), ms5611(sda, scl)
{

}

MS5611Sensor::~MS5611Sensor()
{

}

int MS5611Sensor::setup()
{
    ms5611::cmd_reset();
    pc.printf("MS5611 started\n");
    return MBED_SUCCESS;
}

int MS5611Sensor::read(SensorData* data)
{
    double temp = ms5611::calcTemp();
    double press = ms5611::calcPressure();

    new(data) MS5611Data(press, temp);

    return MBED_SUCCESS;
}
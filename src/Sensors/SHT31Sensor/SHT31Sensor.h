#ifndef _SHT31SENSOR_H_
#define _SHT31SENSOR_H_

#include "mbed.h"
#include "Sht31.h"
#include "DataTypes.h"
#include "Sensor.h"


class SHT31Sensor : public Sensor, private Sht31
{
public:
    SHT31Sensor(PinName sda, PinName scl);
    ~SHT31Sensor();

protected:
    virtual int setup() override;
    virtual int read(SensorData*) override;

private:
    
};

#endif // _SHT31SENSOR_H_
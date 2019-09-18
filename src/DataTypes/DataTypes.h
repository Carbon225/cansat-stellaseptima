#ifndef _DATATYPES_H_
#define _DATATYPES_H_

#include "mbed.h"

enum DataTypes {
    Pressure_dt,
    MS5611_dt,
    SHT31_dt
};

class SensorData
{
public:
    SensorData(DataTypes t);
    virtual ~SensorData() {};
    const DataTypes type;

    virtual bool valid() = 0;
};

// Pressure

class PressureData : public SensorData
{
public:
    PressureData();
    PressureData(double pressure);

    double pressure;

    virtual bool valid() override;
};

// SHT31

class SHT31Data : public SensorData
{
public:
    SHT31Data();
    SHT31Data(double temp, double hum);

    double temperature;
    double humidity;

    virtual bool valid() override;
};

/*
union SensorDataUnion
{
    SensorData a;
    MS5611Data b;
    SHT31Data c;
};
*/

#endif // _DATATYPES_H_
#ifndef _DATATYPES_H_
#define _DATATYPES_H_

#include "mbed.h"

enum DataTypes {
    MS5611_dt,
    SHT31_dt
};

class SensorData
{
public:
    SensorData(DataTypes t);
    const DataTypes type;
};

class MS5611Data : public SensorData
{
public:
    MS5611Data(double pressure, double temp);

    double pressure;
    double temperature;
};

class SHT31Data : public SensorData
{
public:
    SHT31Data(double temp, double hum);

    double temperature;
    double humidity;
};

union SensorDataUnion
{
    SensorData a;
    MS5611Data b;
};

#endif // _DATATYPES_H_
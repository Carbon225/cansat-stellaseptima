#include "DataTypes.h"

SensorData::SensorData(DataTypes t, const char n[64])
: type(t)
{
    if (name) {
        name = new char[strlen(n) + 1];
        strcpy(name, n);
    }
    else {
        name = new char[1];
        name[0] = '\0';
    }
}

SensorData::~SensorData()
{
    delete name;
}

// Pressure

PressureData::PressureData(const char name[])
: PressureData(0.f, name)
{}

PressureData::PressureData(double p, const char name[])
: SensorData(DataTypes::Pressure_dt, name), pressure(p)
{}

bool PressureData::valid()
{
    return pressure > 700.f && pressure < 1300.f;
}

// SHT31

SHT31Data::SHT31Data(const char name[])
: SHT31Data(-100.f, 200.f, name)
{}

SHT31Data::SHT31Data(double t, double h, const char name[])
: SensorData(DataTypes::SHT31_dt, name), temperature(t), humidity(h)
{}

bool SHT31Data::valid()
{
    return temperature > -20.f && temperature < 70.f &&
           humidity > 0.f && humidity < 100.f;
}
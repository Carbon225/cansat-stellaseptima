#include "DataTypes.h"

SensorData::SensorData(DataTypes t)
: type(t)
{}

// Pressure

PressureData::PressureData()
: PressureData(0.f)
{}

PressureData::PressureData(double p)
: SensorData(DataTypes::Pressure_dt), pressure(p)
{}

bool PressureData::valid()
{
    return pressure > 700.f && pressure < 1300.f;
}

// SHT31

SHT31Data::SHT31Data()
: SHT31Data(-100.f, 200.f)
{}

SHT31Data::SHT31Data(double t, double h)
: SensorData(DataTypes::SHT31_dt), temperature(t), humidity(h)
{}

bool SHT31Data::valid()
{
    return temperature > -20.f && temperature < 70.f &&
           humidity > 0.f && humidity < 100.f;
}
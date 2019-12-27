#include "PressureData.h"

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
#include "SHTData.h"

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
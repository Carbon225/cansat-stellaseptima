#include "DataTypes.h"

SensorData::SensorData(DataTypes t)
: type(t)
{}

// MS5611

MS5611Data::MS5611Data()
: MS5611Data(0.f, -100.f)
{}

MS5611Data::MS5611Data(double p, double t)
: SensorData(DataTypes::MS5611_dt), pressure(p), temperature(t)
{}

bool MS5611Data::valid()
{
    return temperature > -20.f && temperature < 70.f &&
           pressure > 700.f && pressure < 1300.f;
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
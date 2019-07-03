#include "DataTypes.h"

SensorData::SensorData(DataTypes t)
: type(t)
{}

MS5611Data::MS5611Data(double p, double t)
: SensorData(DataTypes::MS5611_dt), pressure(p), temperature(t)
{}

SHT31Data::SHT31Data(double t, double h)
: SensorData(DataTypes::SHT31_dt), temperature(t), humidity(h)
{}
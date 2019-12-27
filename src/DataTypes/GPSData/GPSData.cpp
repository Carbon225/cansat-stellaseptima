#include "GPSData.h"

GPSData::GPSData(const char name[])
: GPSData(0.0, 0.0, name)
{}

GPSData::GPSData(double pLat, double pLng, const char name[])
: SensorData(DataTypes::GPS_dt, name), lat(pLat), lng(pLng)
{}

bool GPSData::valid()
{
    return true;
}
#ifndef _DATATYPES_H_
#define _DATATYPES_H_

#include "mbed.h"

enum DataTypes {
    Pressure_dt,
    SHT31_dt,
    GPS_dt
};

class SensorData
{
public:
    SensorData(DataTypes t, const char name[] = nullptr);
    virtual ~SensorData();
    const DataTypes type;

    char *name;

    virtual bool valid() = 0;
};

// Pressure

class PressureData : public SensorData
{
public:
    PressureData(const char name[] = nullptr);
    PressureData(double pressure, const char name[] = nullptr);

    double pressure;

    virtual bool valid() override;
};

// SHT31

class SHT31Data : public SensorData
{
public:
    SHT31Data(const char name[] = nullptr);
    SHT31Data(double temp, double hum, const char name[] = nullptr);

    double temperature;
    double humidity;

    virtual bool valid() override;
};

// GPS

class GPSData : public SensorData
{
public:
    GPSData(const char name[] = nullptr);
    GPSData(double lat, double lng, const char name[] = nullptr);

    double lat;
    double lng;

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
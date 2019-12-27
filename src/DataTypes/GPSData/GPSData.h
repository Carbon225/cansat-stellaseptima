#ifndef _GPSDATA_H_
#define _GPSDATA_H_

#include "SensorData.h"

class GPSData : public SensorData
{
public:
    GPSData(const char name[] = nullptr);
    GPSData(double lat, double lng, const char name[] = nullptr);

    double lat;
    double lng;

    virtual bool valid() override;
};

#endif
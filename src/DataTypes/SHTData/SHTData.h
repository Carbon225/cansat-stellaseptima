#ifndef _SHTDATA_H_
#define _SHTDATA_H_

#include "SensorData.h"

class SHT31Data : public SensorData
{
public:
    SHT31Data(const char name[] = nullptr);
    SHT31Data(double temp, double hum, const char name[] = nullptr);

    double temperature;
    double humidity;

    virtual bool valid() override;
};

#endif
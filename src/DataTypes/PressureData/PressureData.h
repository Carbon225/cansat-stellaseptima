#ifndef _PRESSUREDATA_H_
#define _PRESSUREDATA_H_

#include "SensorData.h"

class PressureData : public SensorData
{
public:
    PressureData(const char name[] = nullptr);
    PressureData(double pressure, const char name[] = nullptr);

    double pressure;

    virtual bool valid() override;
};

#endif
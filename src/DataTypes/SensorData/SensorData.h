#ifndef _SENSORDATA_H_
#define _SENSORDATA_H_

#include "DataTypes.h"

class SensorData
{
public:
    SensorData(DataTypes t, const char name[] = nullptr);
    virtual ~SensorData();
    const DataTypes type;

    char *name;

    virtual bool valid() = 0;
};

#endif
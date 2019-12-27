#include "SensorData.h"
#include <cstring>

SensorData::SensorData(DataTypes t, const char n[])
: type(t)
{
    if (n) {
        name = new char[strlen(n) + 1];
        strcpy(name, n);
    }
    else {
        name = new char[1];
        name[0] = '\0';
    }
}

SensorData::~SensorData()
{
    delete name;
}
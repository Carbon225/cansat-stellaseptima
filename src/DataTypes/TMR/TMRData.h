#ifndef _TMRDATA_H_
#define _TMRDATA_H_

#include "PressureData.h"

class TMRData : public PressureData
{
public:
    TMRData(const char name[] = nullptr);
    TMRData(double p, double p1, double p2, double p3, const char name[] = nullptr);

    double pressure1, pressure2, pressure3;
};

#endif
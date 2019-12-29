#ifndef _DOUBLETEMPDATA_H_
#define _DOUBLETEMPDATA_H_

#include "SHTData.h"

class DoubleTempData : public SHT31Data
{
public:
    DoubleTempData(const char name[] = nullptr);
    DoubleTempData(double t, double h, double t1, double h1, double t2, double h2, const char name[] = nullptr);

    double temp1, temp2;
    double hum1, hum2;
};

#endif
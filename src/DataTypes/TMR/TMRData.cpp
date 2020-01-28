#include "TMRData.h"

TMRData::TMRData(const char name[])
: TMRData(0.f, 0.f, 0.f, 0.f, name)
{}

TMRData::TMRData(double p, double p1, double p2, double p3, const char name[])
: PressureData(p, name), pressure1(p1), pressure2(p2), pressure3(p3)
{}
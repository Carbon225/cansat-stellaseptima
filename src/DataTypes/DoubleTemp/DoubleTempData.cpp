#include "DoubleTempData.h"

DoubleTempData::DoubleTempData(const char name[])
: DoubleTempData(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, name)
{}

DoubleTempData::DoubleTempData(double t, double h, double t1, double h1, double t2, double h2, const char name[])
: SHT31Data(t, h, name), temp1(t1), temp2(t2), hum1(h1), hum2(h2)
{}
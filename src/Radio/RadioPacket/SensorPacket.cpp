#include "SensorPacket.h"

SensorPacket::SensorPacket(int id, double temp, double press)
{
    _data = id;
    _data |= scaleToBits(temp, -10.f, 40.f, 255) << 4;
    _data |= scaleToBits(press, 700.f, 1100.f, 4095) << 12;
}
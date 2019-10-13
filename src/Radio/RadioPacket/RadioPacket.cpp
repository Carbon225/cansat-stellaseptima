#include "RadioPacket.h"

double map(double unscaledNum, double minAllowed, double maxAllowed,double min, double max) {
  return (maxAllowed - minAllowed) * (unscaledNum - min) / (max - min) + minAllowed;
}

RadioPacket::RadioPacket(int id, double temp, double press)
{
    _data = id;
    _data |= ((int) round(map(temp, -10.f, 40.f, 0, 255))) << 4;
    _data |= ((int) round(map(press, 700.f, 1100.f, 0, 4096))) << 12;

    // TODO crc
}

packet_t RadioPacket::toBinary()
{
    return _data;
}
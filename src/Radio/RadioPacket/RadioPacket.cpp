#include "RadioPacket.h"

double map(double unscaledNum, double minAllowed, double maxAllowed,double min, double max)
{
  return (maxAllowed - minAllowed) * (unscaledNum - min) / (max - min) + minAllowed;
}

packet_t RadioPacket::toBinary()
{
    return _data;
}
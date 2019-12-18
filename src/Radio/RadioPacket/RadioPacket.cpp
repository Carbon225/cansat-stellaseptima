#include "RadioPacket.h"

int scaleToBits(double x, double min, double max, double range) {
    return (x - min) * range / max;
}

double scaleFromBits(double x, double min, double max, double range) {
    return x * max / range + min;
}

packet_t* RadioPacket::toBinary()
{
    return &_data;
}
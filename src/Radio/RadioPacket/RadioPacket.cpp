#include "RadioPacket.h"

int scaleToBits(double x, double min, double max, double range) {
    return (x - min) * range / (max - min);
}

double scaleFromBits(double x, double min, double max, double range) {
    return x * (max - min) / range + min;
}

packet_t* RadioPacket::toBinary()
{
    return &_data;
}
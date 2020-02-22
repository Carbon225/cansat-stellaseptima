#include "SensorPacket.h"

SensorPacket::SensorPacket(int id, double temp, double press)
{
    _data = id & 15;
    _data |= (scaleToBits(temp, -10.f, 40.f, 255) & 255) << 4;
    _data |= (scaleToBits(press, 700.f, 1100.f, 8191) & 8191) << 12;
}

packet_t SensorPacket::encode(int id, double temp, double press, double hum)
{
    packet_t data = id & 15;
    data |= (scaleToBits(temp, -10.f, 40.f, 255) & 255) << 4;
    data |= (scaleToBits(press, 700.f, 1100.f, 8191) & 8191) << 12;
    data |= (scaleToBits(hum, 0.f, 100.f, 127) & 127) << 25;

    return data;
}
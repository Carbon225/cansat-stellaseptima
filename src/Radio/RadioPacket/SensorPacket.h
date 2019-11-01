#ifndef _SENSORPACKET_H_
#define _SENSORPACKET_H_

#include "RadioPacket.h"

class SensorPacket : public RadioPacket
{
public:
    SensorPacket(int id, double temp, double press);
};

#endif
#ifndef _RADIOPACKET_H_
#define _RADIOPACKET_H_

#include "mbed.h"
#include "DataTypes.h"

typedef uint32_t packet_t;

class RadioPacket
{
public:
    RadioPacket(int id, SensorData*);

    packet_t toBinary();

private:
    const int _id;
    const SensorData *_sensorData;
};

#endif
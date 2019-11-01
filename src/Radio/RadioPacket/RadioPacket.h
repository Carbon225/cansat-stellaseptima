#ifndef _RADIOPACKET_H_
#define _RADIOPACKET_H_

#include "mbed.h"
#include "DataTypes.h"

/*

Packet structure

4 bits - packet number (detect missing packets)
20 bits - data (GPS or sensor data decided by packet number % n)
8 bits - crc

   time            data              crc
   ****    ********************    ********
   0  3    4                 23    24    31 

*/


typedef uint32_t packet_t;

class RadioPacket
{
public:
    packet_t toBinary();

protected:
    packet_t _data;
};

double map(double unscaledNum, double minAllowed, double maxAllowed,double min, double max);

#endif
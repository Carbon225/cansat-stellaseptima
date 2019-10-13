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
   0  3    4                 27    28    31 

*/


typedef uint32_t packet_t;

class RadioPacket
{
public:
    RadioPacket(int id, double temp, double press);
    // TODO RadioPacket(int id, double lat, double lng);

    packet_t toBinary();

private:
    packet_t _data;
};

#endif
#ifndef _GPSPACKET_H_
#define _GPSPACKET_H_

#include "RadioPacket.h"

class GPSPacket : public RadioPacket
{
public:
    GPSPacket(int id, double lat, double lng);
};

#endif
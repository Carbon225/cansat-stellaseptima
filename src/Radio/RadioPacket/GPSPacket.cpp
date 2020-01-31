#include "GPSPacket.h"

#define LAT_MIN 50.02111
#define LAT_MAX 50.14055
#define LNG_MIN 19.50527
#define LNG_MAX 19.88

GPSPacket::GPSPacket(int id, double lat, double lng)
{
    _data = id;
    _data |= scaleToBits(lat, LAT_MIN, LAT_MAX, 16383) << 4;
    _data |= scaleToBits(lng, LNG_MIN, LNG_MAX, 16383) << 18;
}
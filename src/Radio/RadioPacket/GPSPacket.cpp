#include "GPSPacket.h"

#define LAT_MIN 50.069082
#define LAT_MAX 50.072783
#define LNG_MIN 19.943569
#define LNG_MAX 19.957398

GPSPacket::GPSPacket(int id, double lat, double lng)
{
    _data = id;
    _data |= ((int) round(map(lat, LAT_MIN, LAT_MAX, 0, 1024))) << 4;
    _data |= ((int) round(map(lng, LNG_MIN, LNG_MAX, 0, 1024))) << 14;
}
#ifndef _SENSORSERVICE_H_
#define _SENSORSERVICE_H_

#include "ble/BLE.h"

class SensorService
{
public:
    SensorService(BLEDevice &ble);

    const static UUID SERVICE_UUID;
    const static UUID MS5611_CHAR_UUID;
    const static UUID BMP1_CHAR_UUID;
    const static UUID BMP2_CHAR_UUID;
    const static UUID SHT1_CHAR_UUID;
    const static UUID SHT2_CHAR_UUID;
    const static UUID LAT_CHAR_UUID;
    const static UUID LNG_CHAR_UUID;

    void setMS(double);
    void setBMP1(double);
    void setBMP2(double);
    void setSHT1(double);
    void setSHT2(double);
    void setGPS(double lat, double lng);

private:
    BLEDevice &_ble;

    ReadOnlyGattCharacteristic<double> _ms5611;
    ReadOnlyGattCharacteristic<double> _bmp1;
    ReadOnlyGattCharacteristic<double> _bmp2;
    ReadOnlyGattCharacteristic<double> _sht1;
    ReadOnlyGattCharacteristic<double> _sht2;
    ReadOnlyGattCharacteristic<double> _lat;
    ReadOnlyGattCharacteristic<double> _lng;
};

#endif
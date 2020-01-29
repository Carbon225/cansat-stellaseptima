#ifndef _CONFIGSERVICE_H_
#define _CONFIGSERVICE_H_

#include "ble/BLE.h"

class ConfigService
{
public:
    ConfigService(BLEDevice &ble);

    const static UUID SERVICE_UUID;
    const static UUID LORA_FREQ_CHAR_UUID;
    const static UUID GROUND_PRESSURE_CHAR_UUID;

    void setLoraFreq(long freq);
    void setGroundPressure(double pressure);

    void onData(const GattWriteCallbackParams *params);

private:
    BLEDevice &_ble;

    ReadWriteGattCharacteristic<long> _loraFreq;
    ReadWriteGattCharacteristic<double> _groundPressure;
};

#endif
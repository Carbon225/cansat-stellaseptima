#ifndef _PARACHUTESERVICE_H_
#define _PARACHUTESERVICE_H_

#include "ble/BLE.h"

class ParachuteService
{
public:
    ParachuteService(BLEDevice &ble);

    const static UUID SERVICE_UUID;
    const static UUID CALIBRATE_CHAR_UUID;
    const static UUID TRIGGER_CHAR_UUID;

    void onData(const GattWriteCallbackParams *params);

private:
    BLEDevice &_ble;

    WriteOnlyGattCharacteristic<bool> _calibrate;
    WriteOnlyGattCharacteristic<bool> _trigger;
};

#endif
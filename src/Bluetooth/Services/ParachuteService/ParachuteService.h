#ifndef _PARACHUTESERVICE_H_
#define _PARACHUTESERVICE_H_

#include "ble/BLE.h"

class ParachuteService
{
public:
    ParachuteService(BLEDevice &ble, double openingPressure, double hysteresis);

    const static UUID SERVICE_UUID;
    const static UUID OPEN_PRESS_CHAR_UUID;
    const static UUID CALIBRATE_CHAR_UUID;
    const static UUID HYSTERESIS_chAR_UUID;
    const static UUID TRIGGER_CHAR_UUID;

    void onData(const GattWriteCallbackParams *params);

private:
    BLEDevice &_ble;

    WriteOnlyGattCharacteristic<double> _openingPressure;
    WriteOnlyGattCharacteristic<bool> _calibrate;
    WriteOnlyGattCharacteristic<double> _hysteresis;
    WriteOnlyGattCharacteristic<bool> _trigger;
};

#endif
#ifndef _CONFIGSERVICE_H_
#define _CONFIGSERVICE_H_

#include "ble/BLE.h"

class ConfigService
{
public:
    ConfigService(BLEDevice &ble);

    const static UUID SERVICE_UUID;
};

#endif
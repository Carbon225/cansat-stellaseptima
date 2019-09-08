#ifndef __BLE_LED_SERVICE_H__
#define __BLE_LED_SERVICE_H__

#include "mbed.h"
#include "ble/BLE.h"
 
class LEDService {
public:
    const static uint16_t LED_SERVICE_UUID              = 0xA000;
    const static uint16_t LED_STATE_CHARACTERISTIC_UUID = 0xA001;
 
    LEDService(BLEDevice &_ble, bool initialValueForLEDCharacteristic) :
        ble(_ble), ledState(LED_STATE_CHARACTERISTIC_UUID, &initialValueForLEDCharacteristic)
    {
        GattCharacteristic *charTable[] = {&ledState};
        GattService         ledService(LED_SERVICE_UUID, charTable, sizeof(charTable) / sizeof(GattCharacteristic *));
 
        ble.gattServer().addService(ledService);
    }
 
    GattAttribute::Handle_t getValueHandle() const
    {
        return ledState.getValueHandle();
    }
 
private:
    BLEDevice                         &ble;
    ReadWriteGattCharacteristic<bool> ledState;
};
 
#endif /* #ifndef __BLE_LED_SERVICE_H__ */
 
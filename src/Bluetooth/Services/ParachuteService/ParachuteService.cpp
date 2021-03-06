#include "ParachuteService.h"
#include "BLELogger.h"
#include "TripleBaro.h"
#include "ConfigManager.h"
#include "Parachute.h"

namespace Sensors
{
    extern TripleBaro baro;
}

extern Parachute parachute;

static bool INITIAL = false;

const UUID ParachuteService::SERVICE_UUID("6a527fc4-2b4b-4974-847d-af7f31a9b8fa");
const UUID ParachuteService::CALIBRATE_CHAR_UUID("ce2db003-5699-4700-8295-eb8a44868c46");
const UUID ParachuteService::TRIGGER_CHAR_UUID("3a110848-caae-4b2c-adad-f86e19383406");

ParachuteService::ParachuteService(BLEDevice &ble)
: _ble(ble),
  _calibrate(CALIBRATE_CHAR_UUID, &INITIAL, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE),
  _trigger(TRIGGER_CHAR_UUID, &INITIAL)
{
    GattCharacteristic *charTable[] = {
        &_calibrate,
        &_trigger
    };

    GattService parachuteService(SERVICE_UUID, charTable, sizeof(charTable) / sizeof(GattCharacteristic*));
    ble.gattServer().addService(parachuteService);
}

void ParachuteService::onData(const GattWriteCallbackParams *params)
{
    if (params->handle == _calibrate.getValueHandle()) {
        double pressure = ((TMRData*)Sensors::baro.lastValue())->pressure;
        ConfigManager::Instance().setGroundPressure(pressure);
        mbed_event_queue()->call(callback(&ConfigManager::Instance(), &ConfigManager::writeConfig));
        mbed_event_queue()->call(&NVIC_SystemReset);
    }
    else if (params->handle == _trigger.getValueHandle()) {
        parachute.open();
    }
}
#include "ParachuteService.h"
#include "BLELogger.h"

static bool INITIAL = false;

const UUID ParachuteService::SERVICE_UUID("6a527fc4-2b4b-4974-847d-af7f31a9b8fa");
const UUID ParachuteService::OPEN_PRESS_CHAR_UUID("ce28b2a2-1361-40a6-8624-9026ceaad7bf");
const UUID ParachuteService::HYSTERESIS_chAR_UUID("333dd8e8-47d1-4495-85da-250182e0a8ab");
const UUID ParachuteService::CALIBRATE_CHAR_UUID("ce2db003-5699-4700-8295-eb8a44868c46");

ParachuteService::ParachuteService(BLEDevice &ble, double openingPressure, double hysteresis)
: _ble(ble),
  _openingPressure(OPEN_PRESS_CHAR_UUID, &openingPressure, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE),
  _hysteresis(HYSTERESIS_chAR_UUID, &hysteresis, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE),
  _calibrate(CALIBRATE_CHAR_UUID, &INITIAL, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE)
{
    GattCharacteristic *charTable[] = {
        &_openingPressure,
        &_hysteresis,
        &_calibrate
    };

    GattService parachuteService(SERVICE_UUID, charTable, sizeof(charTable) / sizeof(GattCharacteristic*));
    ble.gattServer().addService(parachuteService);
}

void ParachuteService::onData(const GattWriteCallbackParams *params)
{
    if (params->handle == _calibrate.getValueHandle()) {
        LOGI("CALIBRATE\n");
    }
}
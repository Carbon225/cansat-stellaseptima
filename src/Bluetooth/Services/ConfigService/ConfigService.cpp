#include "ConfigService.h"
#include "ConfigManager.h"
#include "BLELogger.h"

static long DEF_FREQ = 4346E5;
static double DEF_PRESS = 980.f;

const UUID ConfigService::SERVICE_UUID("eb135d7e-6c47-4189-ad93-73405befc17f");
const UUID ConfigService::LORA_FREQ_CHAR_UUID("68a074c7-193c-43cf-a1bf-4194eb0d45cd");
const UUID ConfigService::GROUND_PRESSURE_CHAR_UUID("8063c5ac-7394-48f4-977d-3483140304df");

ConfigService::ConfigService(BLEDevice &ble)
: _ble(ble),
  _loraFreq(LORA_FREQ_CHAR_UUID, &DEF_FREQ),
  _groundPressure(GROUND_PRESSURE_CHAR_UUID, &DEF_PRESS)
{
    GattCharacteristic *charTable[] = {
        &_loraFreq,
        &_groundPressure
    };

    GattService configService(SERVICE_UUID, charTable, sizeof(charTable) / sizeof(GattCharacteristic*));
    ble.gattServer().addService(configService);
}

void ConfigService::onData(const GattWriteCallbackParams *params)
{
    if (params->handle == _loraFreq.getValueHandle()) {
        ConfigManager::Instance().setLoraFreq(*(long*)params->data);
        mbed_event_queue()->call(callback(&ConfigManager::Instance(), &ConfigManager::writeConfig));
        mbed_event_queue()->call(&NVIC_SystemReset);
    }
    else if (params->handle == _groundPressure.getValueHandle()) {
        ConfigManager::Instance().setGroundPressure(*(double*)params->data);
        mbed_event_queue()->call(callback(&ConfigManager::Instance(), &ConfigManager::writeConfig));
    }
}
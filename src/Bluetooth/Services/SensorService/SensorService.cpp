#include "SensorService.h"
#include "BLELogger.h"

static double INITIAL_VALUE = 0.f;

const UUID SensorService::SERVICE_UUID("371c8b3f-b069-4eab-8216-05a55013ad7f");

const UUID SensorService::MS5611_CHAR_UUID("ef438fcd-ebe7-47a7-9218-43b7ed7a4bf6");
const UUID SensorService::BMP1_CHAR_UUID("dbf4ee71-ed2e-44ba-bf82-f0367dc3decc");
const UUID SensorService::BMP2_CHAR_UUID("0f088069-76b2-469f-8870-8d01568868fc");

const UUID SensorService::TEMP1_CHAR_UUID("b5637d21-378b-4ca3-9232-518a48de2161");
const UUID SensorService::HUM1_CHAR_UUID("92ab929a-dff4-40f5-8ded-64aa0e5e84a7");
const UUID SensorService::TEMP2_CHAR_UUID("3a0d093b-2cb4-4357-b6bc-d8449fd57296");
const UUID SensorService::HUM2_CHAR_UUID("8a4e38fc-fb19-4e25-82f1-3023fd94de48");

const UUID SensorService::LAT_CHAR_UUID("e4766769-9041-4526-86c3-37452d8a703b");
const UUID SensorService::LNG_CHAR_UUID("91c4d86b-016d-47d4-8cc2-7bfe8a68eda6");

SensorService::SensorService(BLEDevice &ble)
: _ble(ble),

  _ms5611(MS5611_CHAR_UUID, &INITIAL_VALUE, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY),
  _bmp1(BMP1_CHAR_UUID, &INITIAL_VALUE, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY),
  _bmp2(BMP2_CHAR_UUID, &INITIAL_VALUE, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY),

  _temp1(TEMP1_CHAR_UUID, &INITIAL_VALUE, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY),
  _hum1(HUM1_CHAR_UUID, &INITIAL_VALUE, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY),
  _temp2(TEMP2_CHAR_UUID, &INITIAL_VALUE, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY),
  _hum2(HUM2_CHAR_UUID, &INITIAL_VALUE, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY),

  _lat(LAT_CHAR_UUID, &INITIAL_VALUE, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY),
  _lng(LNG_CHAR_UUID, &INITIAL_VALUE, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY)
{
    GattCharacteristic *charTable[] = {
        &_ms5611,
        &_bmp1,
        &_bmp2,
        &_temp1,
        &_hum1,
        &_temp2,
        &_hum2,
        &_lat,
        &_lng
    };

    GattService sensorService(SERVICE_UUID, charTable, sizeof(charTable) / sizeof(GattCharacteristic*));
    ble.gattServer().addService(sensorService);
}

void SensorService::setMS(double x)
{
    _ble.gattServer().write(_ms5611.getValueHandle(), (uint8_t*)&x, sizeof(double));
}

void SensorService::setBMP1(double x)
{
    _ble.gattServer().write(_bmp1.getValueHandle(), (uint8_t*)&x, sizeof(double));
}

void SensorService::setBMP2(double x)
{
    _ble.gattServer().write(_bmp2.getValueHandle(), (uint8_t*)&x, sizeof(double));
}

void SensorService::setSHT1(double temp, double hum)
{
    _ble.gattServer().write(_temp1.getValueHandle(), (uint8_t*)&temp, sizeof(double));
    _ble.gattServer().write(_hum1.getValueHandle(), (uint8_t*)&hum, sizeof(double));
}

void SensorService::setSHT2(double temp, double hum)
{
    _ble.gattServer().write(_temp2.getValueHandle(), (uint8_t*)&temp, sizeof(double));
    _ble.gattServer().write(_hum2.getValueHandle(), (uint8_t*)&hum, sizeof(double));
}

void SensorService::setGPS(double lat, double lng)
{
    _ble.gattServer().write(_lat.getValueHandle(), (uint8_t*)&lat, sizeof(double));
    _ble.gattServer().write(_lng.getValueHandle(), (uint8_t*)&lng, sizeof(double));
}
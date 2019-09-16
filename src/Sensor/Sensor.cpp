#include "Sensor.h"
#include "BLELogger.h"

Sensor::Sensor(size_t data_size, const char *name)
: _store(nullptr), 
  _delay_ms(10),
  _last_value((SensorData*) new uint8_t[data_size]),
  _sensor_thread(osPriorityNormal, 512, NULL, name)
{
    
}

Sensor::~Sensor()
{
    delete _last_value;
}

SensorData* Sensor::lastValue()
{
    return _last_value;
}

void Sensor::setDataStore(SensorDataStore *store)
{
    _store = store;
}

void Sensor::start(int delay_ms)
{
    _delay_ms = delay_ms;
    _sensor_thread.start(callback(this, &Sensor::_sensor_task));
}

void Sensor::stop()
{
    _sensor_thread.terminate();
}

void Sensor::_sensor_task()
{
    int ret = setup();
    if (ret != MBED_SUCCESS) {
        LOGI("Sensor returned error code %d\n", ret);

        return;
    }

    while (true) {
        if (read(_last_value) == MBED_SUCCESS) {
            if (_store) {
                _store->saveData(_last_value);
            }
        }
        
        wait_ms(_delay_ms);
    }
}
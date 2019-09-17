#include "Sensor.h"
#include "BLELogger.h"

Sensor::Sensor(const char *name)
: _store(nullptr), 
  _delay_ms(10),
  _sensor_thread(osPriorityNormal, 512, NULL, name)
{
}

Sensor::~Sensor()
{
    stop();
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
        if (read() == MBED_SUCCESS) {
            if (_store) {
                _store->saveData(lastValue());
            }
        }
        
        wait_ms(_delay_ms);
    }
}
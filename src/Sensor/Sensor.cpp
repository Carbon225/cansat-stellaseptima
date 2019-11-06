#include "Sensor.h"
#include "BLELogger.h"

Sensor::Sensor(const char name[])
: _delay_ms(10),
  _sensor_thread(osPriorityNormal, 1024, NULL, name)
{

}

Sensor::~Sensor()
{
    stop();
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
        read();
        ThisThread::sleep_for(_delay_ms);
    }
}
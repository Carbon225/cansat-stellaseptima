#include "Sensor.h"
#include "BLELogger.h"

Thread Sensor::_sensorsThread(osPriorityNormal, 2048, NULL, "sensors");
EventQueue Sensor::_sensorsEvQueue(64 * EVENTS_EVENT_SIZE);

Sensor::Sensor(const char name[])

{

}

Sensor::~Sensor()
{
    stop();
}

void Sensor::start(int delay_ms)
{
    if (_sensorsThread.get_state() == Thread::Deleted) {
        _sensorsThread.start(callback(&_sensorsEvQueue, &EventQueue::dispatch_forever));
    }

    int ret = setup();
    if (ret != MBED_SUCCESS) {
        LOGI("Sensor setup returned error code %d\n", ret);
        return;
    }

    if (delay_ms > 0) {
        _eventId = _sensorsEvQueue.call_every(delay_ms, callback(this, &Sensor::read));
    }
}

void Sensor::stop()
{
    _sensorsEvQueue.cancel(_eventId);
}
#include "SensorDataStore.h"

SensorDataStore::SensorDataStore()
: _saveQueue(16 * EVENTS_EVENT_SIZE),
  _saveThread(osPriorityNormal, 2046, NULL, "data")
{
    
}

SensorDataStore::~SensorDataStore()
{
    _saveThread.terminate();
    _saveQueue.break_dispatch();
}

void SensorDataStore::schedule(Sensor *sensor, int delay)
{
    if (_saveThread.get_state() == Thread::Deleted) {
        _saveThread.start(callback(&_saveQueue, &EventQueue::dispatch_forever));
    }
    _saveQueue.call_every(delay, this, &SensorDataStore::saveData, sensor->lastValue());
}
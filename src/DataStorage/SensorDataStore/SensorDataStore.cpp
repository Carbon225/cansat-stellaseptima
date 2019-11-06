#include "SensorDataStore.h"

SensorDataStore::SensorDataStore()
: _saveQueue(16 * EVENTS_EVENT_SIZE),
  _saveThread(osPriorityNormal, 1024)
{
    _saveThread.start(callback(&_saveQueue, &EventQueue::dispatch_forever));
}

SensorDataStore::~SensorDataStore()
{
    _saveThread.terminate();
    _saveQueue.break_dispatch();
}

void SensorDataStore::schedule(Sensor *sensor, int delay)
{
    _saveQueue.call_every(delay, this, &SensorDataStore::saveData, sensor->lastValue());
}
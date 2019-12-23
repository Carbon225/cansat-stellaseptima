#ifndef _FSDATASTORE_H_
#define _FSDATASTORE_H_

#include "mbed.h"
#include "SensorDataStore.h"

class FSDataStore : public SensorDataStore {
public:
    FSDataStore(const char *path);
    virtual ~FSDataStore();

    void listFiles();

    virtual mbed_error_status_t init() override;
    virtual mbed_error_status_t deinit() override;
    virtual mbed_error_status_t saveData(Sensor* sensor) override;
    virtual mbed_error_status_t reset() override;

protected:
    virtual int _encode(SensorData *data, uint8_t *out = nullptr) override;

private:
    char _dataPath[128];
};


#endif
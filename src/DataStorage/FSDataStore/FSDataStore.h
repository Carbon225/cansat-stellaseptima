#ifndef _FSDATASTORE_H_
#define _FSDATASTORE_H_

#include "mbed.h"
#include "SensorDataStore.h"

class FSDataStore : public SensorDataStore {
public:
    FSDataStore(FileSystem *fs);

    virtual mbed_error_status_t init();
    virtual mbed_error_status_t deinit();
    virtual mbed_error_status_t saveData(SensorData* data);
    virtual mbed_error_status_t reset();

private:
    FileSystem *_fs;
};


#endif
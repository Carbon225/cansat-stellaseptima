#ifndef _INTERNALDATASTORE_H_
#define _INTERNALDATASTORE_H_

#include "mbed.h"
#include "FSDataStore.h"
#include "FlashIAPBlockDevice.h"
#include "LittleFileSystem.h"

class InternalDataStore : public FSDataStore {
public:
    InternalDataStore(const char mount[]);
    virtual ~InternalDataStore();

    virtual mbed_error_status_t init() override;
    virtual mbed_error_status_t deinit() override;

protected:
    virtual int _encode(SensorData *data, uint8_t *out = nullptr) override;

private:
    FlashIAPBlockDevice _bd;
    LittleFileSystem _fs;
};


#endif
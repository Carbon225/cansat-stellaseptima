#ifndef _SDDATASTORE_H_
#define _SDDATASTORE_H_

#include "mbed.h"
#include "FSDataStore.h"
#include "SDBlockDevice.h"
#include "FATFileSystem.h"

class SDDataStore : public FSDataStore {
public:
    SDDataStore(const char mount[]);
    // SDDataStore(const char mount[], PinName mosi, PinName miso, PinName clk, PinName cs);
    virtual ~SDDataStore();

    virtual mbed_error_status_t init() override;
    virtual mbed_error_status_t deinit() override;

protected:
    // virtual int _encode(SensorData *data, uint8_t *out = nullptr) override;

private:
    SDBlockDevice _bd;
    FATFileSystem _fs;
};


#endif
#ifndef _FLASHDATASTORE_H_
#define _FLASHDATASTORE_H_

#include "mbed.h"
#include "FSDataStore.h"
#include "DataFlashBlockDevice.h"
#include "LittleFileSystem.h"

class FlashDataStore : public FSDataStore {
public:
    FlashDataStore(const char mount[]);
    // SDDataStore(const char mount[], PinName mosi, PinName miso, PinName clk, PinName cs);
    virtual ~FlashDataStore();

    virtual mbed_error_status_t init() override;
    virtual mbed_error_status_t deinit() override;

protected:
    // virtual int _encode(SensorData *data, uint8_t *out = nullptr) override;

private:
    DataFlashBlockDevice _bd;
    LittleFileSystem _fs;
};


#endif
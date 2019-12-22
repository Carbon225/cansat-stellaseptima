#include "FlashDataStore.h"
#include "BLELogger.h"

FlashDataStore::FlashDataStore(const char mount[]) // ex. /sd
: FSDataStore(mount),
  _bd(MBED_CONF_DATAFLASH_SPI_MOSI, MBED_CONF_DATAFLASH_SPI_MISO, MBED_CONF_DATAFLASH_SPI_CLK, MBED_CONF_DATAFLASH_SPI_CS),
  _fs(mount + 1) // trim /
{
    
}

FlashDataStore::~FlashDataStore()
{
    deinit();
}

mbed_error_status_t FlashDataStore::init()
{
    LOGI("Starting DataFlash\n");
    _bd.init();
    
    LOGI("Mounting flash filesystem\n");
    int err = _fs.mount(&_bd);
    if (err) {
        LOGI("Mounting failed\n");
        LOGI("Reformatting\n");
        
        if (_fs.reformat(&_bd) != 0) {
            LOGI("Reformatting failed\n");
            _bd.deinit();
            _active = false;
            return MBED_ERROR_INITIALIZATION_FAILED;
        }
    }
    LOGI("DataFlash OK\n");

    return FSDataStore::init();
}

mbed_error_status_t FlashDataStore::deinit()
{
    FSDataStore::deinit();
    _fs.unmount();
    _bd.deinit();
    _active = false;
    return MBED_SUCCESS;
}
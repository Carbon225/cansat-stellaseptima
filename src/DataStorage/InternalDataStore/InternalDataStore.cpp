#include "InternalDataStore.h"
#include "BLELogger.h"

InternalDataStore::InternalDataStore(const char mount[]) // ex. /sd
: FSDataStore(mount),
  _fs(mount + 1) // trim /
{
    
}

InternalDataStore::~InternalDataStore()
{
    deinit();
}

mbed_error_status_t InternalDataStore::init()
{
    LOGI("Starting internal flash\n");
    _bd.init();

    LOGI("Flash block device size: %llu\n",         _bd.size());
    LOGI("Flash block device read size: %llu\n",    _bd.get_read_size());
    LOGI("Flash block device program size: %llu\n", _bd.get_program_size());
    LOGI("Flash block device erase size: %llu\n",   _bd.get_erase_size());
    
    LOGI("Mounting filesystem\n");
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
    LOGI("Internal flash OK\n");

    return FSDataStore::init();
}

mbed_error_status_t InternalDataStore::deinit()
{
    FSDataStore::deinit();
    _fs.unmount();
    _bd.deinit();
    _active = false;
    return MBED_SUCCESS;
}
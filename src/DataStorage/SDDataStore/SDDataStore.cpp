#include "SDDataStore.h"

SDDataStore::SDDataStore(const char mount[]) // ex. /sd
: FSDataStore(mount),
  _bd(MBED_CONF_SD_SPI_MOSI, MBED_CONF_SD_SPI_MISO, MBED_CONF_SD_SPI_CLK, MBED_CONF_SD_SPI_CS),
  _fs(mount + 1) // trim /
{
    
}

SDDataStore::~SDDataStore()
{
    deinit();
}

mbed_error_status_t SDDataStore::init()
{
    printf("Starting SD store\n");
    _bd.init();
    
    printf("Mounting filesystem...");
    fflush(stdout);
    int err = _fs.mount(&_bd);
    if (err) {
        printf("Fail\nReformatting...");
        fflush(stdout);
        
        if (_fs.reformat(&_bd) != 0) {
            printf("Fail\n");
            _bd.deinit();
            _active = false;
            return MBED_ERROR_INITIALIZATION_FAILED;
        }
    }
    printf("OK\n");

    return FSDataStore::init();
}

mbed_error_status_t SDDataStore::deinit()
{
    FSDataStore::deinit();
    _fs.unmount();
    _bd.deinit();
    _active = false;
    return MBED_SUCCESS;
}
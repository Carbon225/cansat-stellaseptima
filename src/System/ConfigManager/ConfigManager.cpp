#include "ConfigManager.h"
#include "BLELogger.h"
#include "Partitions.h"

static ConfigManager _manager;

ConfigManager& ConfigManager::Instance()
{
    return _manager;
}

ConfigManager::ConfigManager()
: _bd(Partitions::config()),
  _fs("config")
{

}

ConfigManager::~ConfigManager()
{

}

mbed_error_status_t ConfigManager::init()
{
    LOGI("Starting config partition\n");
    _bd->init();

    LOGI("Config block device size: %llu\n",         _bd->size());
    LOGI("Config block device read size: %llu\n",    _bd->get_read_size());
    LOGI("Config block device program size: %llu\n", _bd->get_program_size());
    LOGI("Config block device erase size: %llu\n",   _bd->get_erase_size());
    
    LOGI("Mounting config filesystem\n");
    int err = _fs.mount(_bd);
    if (err) {
        LOGI("Mounting failed\n");
        LOGI("Reformatting\n");
        
        if (_fs.reformat(_bd) != 0) {
            LOGI("Reformatting failed\n");
            _bd->deinit();
            return MBED_ERROR_INITIALIZATION_FAILED;
        }
    }
    LOGI("Config partition OK\n");

    return MBED_SUCCESS;
}

mbed_error_status_t ConfigManager::readConfig()
{
    LOGI("Reading config\n");

    FILE *f = fopen("/config/config.conf", "rb");
    if (!f) {
        LOGI("Creating default config\n");
        return writeConfig();
    }

    uint8_t data[sizeof(long) + sizeof(double)];
    fread(data, 1, sizeof(long) + sizeof(double), f);
    fclose(f);

    _loraFreq = *(long*)data;
    _groundPressure = *(double*)(data + sizeof(long));

    CansatBLE::Instance().configService()->setLoraFreq(_loraFreq);
    CansatBLE::Instance().configService()->setGroundPressure(_groundPressure);

    return MBED_SUCCESS;
}

mbed_error_status_t ConfigManager::writeConfig()
{
    // LOGI("Writing config\n");

    FILE *f = fopen("/config/config.conf", "wb");
    if (!f) {
        // LOGI("Error opening file\n");
        return MBED_ERROR_CREATE_FAILED;
    }

    uint8_t data[sizeof(long) + sizeof(double)];
    *(long*)data = _loraFreq;
    *(double*)(data + sizeof(long)) = _groundPressure;

    fwrite(data, 1, sizeof(long) + sizeof(double), f);
    fclose(f);

    return MBED_SUCCESS;
}

void ConfigManager::setLoraFreq(long freq)
{
    _loraFreq = freq;
}

long ConfigManager::getLoraFreq()
{
    return _loraFreq;
}

void ConfigManager::setGroundPressure(double pressure)
{
    _groundPressure = pressure;
}

double ConfigManager::getGroundPressure()
{
    return _groundPressure;
}
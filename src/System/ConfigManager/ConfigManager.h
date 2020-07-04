#ifndef _CONFIGMANAGER_H_
#define _CONFIGMANAGER_H_

#include "mbed.h"
#include "LittleFileSystem.h"

/* Config structure
Addr      Size      Type
0         long      lora freq
0+long    double    ground pressure
 */

class ConfigManager
{
public:
    ConfigManager();
    ~ConfigManager();

    static ConfigManager& Instance();

    mbed_error_status_t init();
    mbed_error_status_t readConfig();
    mbed_error_status_t writeConfig();

    void setLoraFreq(long freq);
    long getLoraFreq();

    void setGroundPressure(double pressure);
    double getGroundPressure();

private:
    BlockDevice *_bd;
    LittleFileSystem _fs;

    long _loraFreq = 4342E5;
    double _groundPressure = 980.f;
};

#endif
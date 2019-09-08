#include "SDBlockDevice.h"

BlockDevice* BlockDevice::get_default_instance()
{
    static SDBlockDevice sd(MBED_CONF_SD_SPI_MOSI, MBED_CONF_SD_SPI_MISO, MBED_CONF_SD_SPI_CLK, MBED_CONF_SD_SPI_CS);
    return &sd;
}
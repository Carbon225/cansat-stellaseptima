#include "pinmap_ex.h"

const PinMapI2C PinMap_I2C[] = {
    {MBED_CONF_APP_I2C1_SDA, MBED_CONF_APP_I2C1_SCL, 0},
    {MBED_CONF_APP_I2C2_SDA, MBED_CONF_APP_I2C2_SCL, 1},
    {NC, NC, NC}
};

const PinMapSPI PinMap_SPI[] = {
    {MBED_CONF_APP_SPI1_MOSI, MBED_CONF_APP_SPI1_MISO, MBED_CONF_APP_SPI1_CLK, 2},
    {NC, NC, NC, NC}
};

const PinMapUART PinMap_UART[] = {
    {NC, MBED_CONF_APP_GPS_RX, 1},
    {NC, NC, NC}
};
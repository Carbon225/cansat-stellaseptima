#include "Radio.h"

Radio::Radio()
: LoRaClass(MBED_CONF_APP_SPI1_MOSI, MBED_CONF_APP_SPI1_MISO, MBED_CONF_APP_SPI1_CLK, MBED_CONF_APP_LORA_CS, P0_29, MBED_CONF_APP_LORA_DIO0)
{

}
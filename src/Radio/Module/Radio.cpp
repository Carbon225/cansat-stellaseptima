#include "Radio.h"

Radio::Radio(PinName mosi, PinName miso, PinName clk, PinName cs, PinName rst, PinName dio0)
: LoRaClass(mosi, miso, clk, cs, rst, dio0)
{

}
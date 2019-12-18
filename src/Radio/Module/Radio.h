#ifndef _RADIO_H_
#define _RADIO_H_

#include "LoRa.h"

class Radio : public LoRaClass
{
public:
    Radio(PinName mosi, PinName miso, PinName clk, PinName cs, PinName rst, PinName dio0);

private:

};

#endif
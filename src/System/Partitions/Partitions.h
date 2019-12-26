#ifndef _PARTITIONS_H_
#define _PARTITIONS_H_

#include "BlockDevice.h"

namespace Partitions
{
    BlockDevice* flash();
    BlockDevice* data();
    BlockDevice* usb();
    BlockDevice* config();

    void initialize();
}

#endif
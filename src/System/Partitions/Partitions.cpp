#include "Partitions.h"

#include "BLELogger.h"
#include "MBRBlockDevice.h"
#include "FlashIAPBlockDevice.h"

static FlashIAPBlockDevice flashBD;
static MBRBlockDevice configPart(&flashBD, 1);
static MBRBlockDevice lfsPart(&flashBD, 2);

BlockDevice* Partitions::flash()
{
    return &flashBD;
}

BlockDevice* Partitions::data()
{
    return &lfsPart;
}

BlockDevice* Partitions::usb()
{
    return NULL;
}

BlockDevice* Partitions::config()
{
    return &configPart;
}

void Partitions::initialize()
{
    LOGI("Initializing partitions...\n");

    flashBD.init();
    MBRBlockDevice::partition(&flashBD, 1, 0x83, 0x0, 0x8000);
    MBRBlockDevice::partition(&flashBD, 2, 0x83, 0x8000);

    LOGI("Complete\n");
}
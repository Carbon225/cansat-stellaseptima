#include "FlashIAPBlockDevice.h"

BlockDevice* BlockDevice::get_default_instance()
{
    static FlashIAPBlockDevice bd;
    return &bd;
}
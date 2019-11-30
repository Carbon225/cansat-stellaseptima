#include "mbed.h"
#include "LittleFileSystem.h"

FileSystem* FileSystem::get_default_instance()
{
    static LittleFileSystem default_fs("fs", BlockDevice::get_default_instance());

    return &default_fs;
}
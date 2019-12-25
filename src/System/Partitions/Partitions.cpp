#include "Partitions.h"

#include "MBRBlockDevice.h"
#include "FlashIAPBlockDevice.h"
#include "FATFileSystem.h"
#include "LittleFileSystem.h"

void Partitions::initialize()
{
    static FlashIAPBlockDevice flash;
    static LittleFileSystem lfs("lfs");
    static FATFileSystem fatfs("fat");

    flash.init();
    MBRBlockDevice::partition(&flash, 1, 0x83, 0);
    MBRBlockDevice::partition(&flash, 2, 0x83, 0x4F000);
    static MBRBlockDevice fatPart(&flash, 1);
    static MBRBlockDevice lfsPart(&flash, 2);

    int err = fatfs.mount(&fatPart);
    if (err) {
        if (fatfs.reformat(&fatPart) != 0) {
            fatPart.deinit();
            flash.deinit();
            return;
        }
    }

    err = lfs.mount(&lfsPart);
    if (err) {
        if (lfs.reformat(&lfsPart) != 0) {
            lfsPart.deinit();
            flash.deinit();
            return;
        }
    }
}
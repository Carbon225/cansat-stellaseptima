#include "USBDrive.h"

#include "USBMSD.h"
#include "Partitions.h"
#include "rtos.h"
#include "BLELogger.h"
#include "FATFileSystem.h"

void USBDrive::connectIRQ()
{
    static Thread usbThread(osPriorityAboveNormal, 1024, NULL, "usb");
    usbThread.start(connect);
}

void USBDrive::connect()
{
    static USBMSD usb(Partitions::flash());
    usb.init();
    usb.connect();
    while (true) {
        usb.process();
    }
}

// deprecated
void USBDrive::prepareFS()
{
    // LOGI("Mounting USB FS\n");
    // static FATFileSystem fs("usb");
    // BlockDevice *bd = Partitions::usb();
    
    // int err = bd->init();
    // if (err) {
    //     LOGI("error = %d\n", err);
    //     return;
    // }

    // LOGI("USB size: %llu\n",         bd->size());
    // LOGI("USB read size: %llu\n",    bd->get_read_size());
    // LOGI("USB program size: %llu\n", bd->get_program_size());
    // LOGI("USB erase size: %llu\n",   bd->get_erase_size());

    // err = fs.mount(bd);
    // if (err) {
    //     LOGI("error = %d\n", err);
    //     LOGI("Formatting USB\n");
    //     if (fs.reformat(bd) != 0) {
    //         LOGI("Formatting failed\n");
    //         bd->deinit();
    //         return;
    //     }
    // }
    // LOGI("Filesystem OK\n");
}
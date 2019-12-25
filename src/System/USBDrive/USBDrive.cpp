#include "USBDrive.h"

#include "USBMSD.h"
#include "FlashIAPBlockDevice.h"

void USBDrive::connect()
{
    static FlashIAPBlockDevice bd;
    static USBMSD usb(&bd);
    usb.init();
    usb.connect();
    while (true) {
        usb.process();
    }
}
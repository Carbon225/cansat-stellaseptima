#include "BLELogger.h"
#include "USBSerial.h"

#define BLE_UART_BUF_SIZE 20

static USBSerial mySerial;

FileHandle* mbed::mbed_override_console(int) {
    return &mySerial;
}

void disableUSBSerial()
{
    mySerial.disconnect();
    mySerial.deinit();
}

void LOGI(const char *fmt, ...)
{
    static Mutex serialMutex;

    serialMutex.lock();

    va_list args;
    va_start(args, fmt);

    if (vprintf(fmt, args) <= BLE_UART_BUF_SIZE && CansatBLE::Instance().uart()) {
        char buf[BLE_UART_BUF_SIZE + 1] = {'\0'};
        // memset(buf, 0, BLE_UART_BUF_SIZE + 1);
        int len = vsprintf(buf, fmt, args);
        CansatBLE::Instance().uart()->writeString(buf);
        // CansatBLE::Instance().uart()->write(buf, len);
    }

    va_end(args);

    serialMutex.unlock();
}
#include "BLELogger.h"
#include "BufferedSoftSerial.h"

static BufferedSoftSerial pc(USBTX, NC, "pc");

void LOGI(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    if (pc.vprintf(fmt, args) <= 20 && BLEController.uart()) {
        static char buf[21];
        memset(buf, 0, 21);
        int len = vsprintf(buf, fmt, args);
        BLEController.uart()->write(buf, len);
    }

    va_end(args);
}
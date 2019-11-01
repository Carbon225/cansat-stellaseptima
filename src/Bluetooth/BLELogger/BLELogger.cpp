#include "BLELogger.h"

void LOGI(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    if (vprintf(fmt, args) <= 20 && BLEController.uart()) {
        static char buf[21];
        memset(buf, 0, 21);
        int len = vsprintf(buf, fmt, args);
        BLEController.uart()->write(buf, len);
    }

    va_end(args);
}
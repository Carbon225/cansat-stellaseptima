#ifndef _BLELOGGER_H_
#define _BLELOGGER_H_

#include "mbed.h"
#include "CansatBLE.h"

void disableUSBSerial();

void LOGI(const char *fmt, ...);

#endif
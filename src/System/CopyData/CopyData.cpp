#include "CopyData.h"
#include "mbed.h"
#include "SDBlockDevice.h"
#include "FATFileSystem.h"
#include "BLELogger.h"
#include <filesystem>

static SDBlockDevice sd(MBED_CONF_SD_SPI_MOSI, MBED_CONF_SD_SPI_MISO, MBED_CONF_SD_SPI_CLK, MBED_CONF_SD_SPI_CS, 1000000);
static FATFileSystem fs("sd");

void copyData(const char from[], const char to[])
{   
    DIR *rootDir;
    struct dirent *flightEnt;
    if ((rootDir = opendir(from)) != NULL) {
        while ((flightEnt = readdir(rootDir)) != NULL) {
            if (flightEnt->d_name[0] == '.') {
                continue;
            }

            char flightPath[64] = {'\0'};
            sprintf(flightPath, "%s/%s", from, flightEnt->d_name);
            DIR *flightDir;

            if ((flightDir = opendir(flightPath)) != NULL) {
                LOGI("Copying contents of %s\n", flightPath);
                struct dirent *sensorEnt;
                while ((sensorEnt = readdir(flightDir)) != NULL) {
                    if (sensorEnt->d_name[0] == '.') {
                        continue;
                    }

                    char sensorPath[64] = {'\0'};
                    sprintf(sensorPath, "%s/%s", flightPath, sensorEnt->d_name);
                    LOGI("- %s\n", sensorPath);
                }
            }
            closedir(flightDir);
        }
        closedir(rootDir);
    }

    fs.unmount();
    sd.deinit();
}
#include "DataToPC.h"
#include "BLELogger.h"

void sendFlight(const char flightPath[])
{
    LOGI("FLIGHT %s\n", flightPath);

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(flightPath)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_name[0] == '.') {
                continue;
            }

            LOGI("SENSOR %s\n", ent->d_name);
        }
        closedir(dir);
    }
}

void DataToPC::overConsole(const char sourcePath[])
{
    LOGI("STARTING DATA DUMP\n");
    LOGI("STARTING DATA DUMP\n");
    LOGI("STARTING DATA DUMP\n");
    LOGI("DATA BEGIN\n");

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(sourcePath)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            int num = -1;
            sscanf(ent->d_name, "%u", &num);
            if (num > -1) {
                char flightPath[128] = {'\0'};
                sprintf(flightPath, "%s/%s", sourcePath, ent->d_name);
                sendFlight(flightPath);
            }
        }
        closedir(dir);
    }

    LOGI("DATA END\n");
}
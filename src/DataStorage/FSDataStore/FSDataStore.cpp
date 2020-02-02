#include "FSDataStore.h"
#include "BLELogger.h"
#include "PressureData.h"
#include "SHTData.h"
#include "GPSData.h"

static int getNewFlightNumber(const char path[])
{
    int maxNum = 0;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(path)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            int num = -1;
            sscanf(ent->d_name, "%u", &num);
            if (num > maxNum) {
                maxNum = num;
            }
        }
        closedir(dir);
    }

    return maxNum + 1;
}

FSDataStore::FSDataStore(const char path[])
{
    memcpy(_dataPath, 0, sizeof(_dataPath));
    sprintf(_dataPath, "%s", path);
}

FSDataStore::~FSDataStore()
{
    deinit();
}

unsigned long FSDataStore::freeSpace()
{
    struct statvfs stat;
    statvfs(_dataPath, &stat);
    return stat.f_bavail * stat.f_bsize;
}

void FSDataStore::listFiles()
{
    LOGI("ls %s\n", _dataPath);
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(_dataPath)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            LOGI("- %s\n", ent->d_name);
        }
        closedir(dir);
    }
}

int FSDataStore::_encode(SensorData *data, uint8_t *out)
{
    switch (data->type) {
        case DataTypes::Pressure_dt:
        if (out) {
            memcpy(out, &((PressureData*)data)->pressure, sizeof(PressureData::pressure));
        }
        return sizeof(PressureData::pressure);

        case DataTypes::SHT31_dt:
        if (out) {
            memcpy(out, &((SHT31Data*)data)->temperature, sizeof(SHT31Data::temperature));
            memcpy(out + sizeof(SHT31Data::temperature), &((SHT31Data*)data)->humidity, sizeof(SHT31Data::humidity));
        }
        return sizeof(SHT31Data::temperature) + sizeof(SHT31Data::humidity);

        case DataTypes::GPS_dt:
        if (out) {
            memcpy(out, &((GPSData*)data)->lat, sizeof(GPSData::lat));
            memcpy(out + sizeof(GPSData::lat), &((GPSData*)data)->lng, sizeof(GPSData::lng));
        }
        break;
    }

    return 0;
}

mbed_error_status_t FSDataStore::init()
{
    sprintf(_dataPath + strlen(_dataPath), "/%d", getNewFlightNumber(_dataPath));
    mkdir(_dataPath, 0777);   
    _active = true;
    return MBED_SUCCESS;
}

mbed_error_status_t FSDataStore::deinit()
{
    _active = false;
    return MBED_SUCCESS;
}

mbed_error_status_t FSDataStore::saveData(Sensor *sensor)
{
    SensorData *data = sensor->lastValue();

    if (!isActive()) {
        MBED_WARNING(MBED_MAKE_ERROR(MBED_MODULE_FILESYSTEM, MBED_ERROR_CODE_NOT_READY), "Sensor data store not initialized");
        return MBED_ERROR_NOT_READY;
    }

    if (strlen(data->name) == 0) {
        return MBED_ERROR_INVALID_DATA_DETECTED;
    }

    uint8_t encoded[_encode(data)];
    _encode(data, encoded);

    char path[128] = {'\0'};
    sprintf(path, "%s/%s.dat", _dataPath, data->name);

    FILE *f = fopen(path, "ab");
    if (f) {
        fwrite(encoded, 1, sizeof(encoded), f);
        fclose(f);
    }
    else {
        return MBED_ERROR_WRITE_FAILED;
    }

    return MBED_SUCCESS;
}

mbed_error_status_t FSDataStore::reset()
{
    if (!isActive()) {
        MBED_WARNING(MBED_MAKE_ERROR(MBED_MODULE_FILESYSTEM, MBED_ERROR_CODE_NOT_READY), "Sensor data store not initialized");
        return MBED_ERROR_NOT_READY;
    }

    LOGI("Resetting store\n");

    // remove _data_path ?

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(_dataPath)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_name[0] != '.') {
                LOGI("Removing %s\n", ent->d_name);
                char buf[128] = {'\0'};
                sprintf(buf, "%s/%s", _dataPath, ent->d_name);
                remove(buf);
            }
        }
        closedir(dir);
    }

    return MBED_SUCCESS;
}
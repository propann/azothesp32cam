#include "storage_manager.h"

#include <FS.h>
#include <SD_MMC.h>

bool StorageManager::begin() {
    return SD_MMC.begin("/sdcard", true, false, SDMMC_FREQ_DEFAULT);
}

bool StorageManager::saveJpeg(const uint8_t* data, size_t len, const char* path) {
    if (data == nullptr || len == 0 || path == nullptr) {
        return false;
    }

    File file = SD_MMC.open(path, FILE_WRITE);
    if (!file) {
        return false;
    }

    const size_t written = file.write(data, len);
    file.close();
    return written == len;
}

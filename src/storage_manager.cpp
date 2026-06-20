#include "storage_manager.h"

#include <FS.h>
#include <SD_MMC.h>
#include "img_converters.h"

bool StorageManager::begin() {
    ready_ = SD_MMC.begin("/sdcard", true, false, SDMMC_FREQ_DEFAULT);
    return ready_;
}

bool StorageManager::isReady() const {
    return ready_;
}

bool StorageManager::saveJpeg(const uint8_t* data, size_t len, const char* path) {
    if (!ready_ || data == nullptr || len == 0 || path == nullptr) {
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

bool StorageManager::saveRgb565AsJpeg(const camera_fb_t* frame, const char* path, int quality) {
    if (!ready_ || frame == nullptr || frame->format != PIXFORMAT_RGB565 || path == nullptr) {
        return false;
    }

    uint8_t* jpeg = nullptr;
    size_t jpegLen = 0;
    if (!frame2jpg(const_cast<camera_fb_t*>(frame), quality, &jpeg, &jpegLen)) {
        return false;
    }

    const bool saved = saveJpeg(jpeg, jpegLen, path);
    free(jpeg);
    return saved;
}

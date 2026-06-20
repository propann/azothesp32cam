#pragma once

#include <stddef.h>
#include <stdint.h>

#include "esp_camera.h"

class StorageManager {
public:
    bool begin();
    bool isReady() const;
    bool saveJpeg(const uint8_t* data, size_t len, const char* path);
    bool saveRgb565AsJpeg(const camera_fb_t* frame, const char* path, int quality);

private:
    bool ready_ = false;
};

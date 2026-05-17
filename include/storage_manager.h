#pragma once

#include <stddef.h>
#include <stdint.h>

class StorageManager {
public:
    bool begin();
    bool saveJpeg(const uint8_t* data, size_t len, const char* path);
};

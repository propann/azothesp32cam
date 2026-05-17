#pragma once

#include <stddef.h>
#include <stdint.h>

struct CameraFrameInfo {
    size_t length;
    uint16_t width;
    uint16_t height;
    uint32_t format;
};

class CameraManager {
public:
    bool begin();
    bool captureFrame(CameraFrameInfo& info);
    void releaseFrame();
    bool isReady() const;

private:
    bool initialized_ = false;
    struct camera_fb_t* currentFrame_ = nullptr;
};

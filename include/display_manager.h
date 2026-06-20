#pragma once

#include "camera_manager.h"

class DisplayManager {
public:
    bool begin();
    bool drawPreview(const CameraFrameInfo& frame);
};

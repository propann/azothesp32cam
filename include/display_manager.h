#pragma once

#include <stddef.h>
#include <stdint.h>

#include "camera_manager.h"
#include "touch_manager.h"

struct UiButton {
    int x;
    int y;
    int w;
    int h;
    const char* label;
    uint16_t color;
};

class DisplayManager {
public:
    bool begin();
    void drawSplash();
    void drawBootUi(bool cameraReady, bool storageReady, const char* cameraModel);
    void drawLiveOverlay(const char* effectName, int intensity, const char* sensorName, int sensorValue, bool storageReady);
    void drawMenuOverlay(const char* effectName, int intensity, const char* sensorName, int sensorValue);
    void drawCaptureFlash();
    void drawError(const char* title, esp_err_t err);
    void drawPreview(const CameraFrameInfo& frame);
    void drawFrameStatus(const char* message);
    void drawButton(const UiButton& button);
    void drawTouchMarker(int x, int y);
    void drawStatusLine(const char* message, int color);
    bool contains(const UiButton& button, const TouchPoint& point) const;
};

#pragma once

#include <stddef.h>
#include <stdint.h>

#include "esp_camera.h"

struct CameraFrameInfo {
    uint8_t* data;
    size_t length;
    uint16_t width;
    uint16_t height;
    uint32_t format;
};

enum class SensorControl : uint8_t {
    Brightness = 0,
    Contrast,
    Saturation,
    SpecialEffect,
    WhiteBalance,
    Count
};

class CameraManager {
public:
    bool begin();
    camera_fb_t* capture();
    bool getFrameInfo(const camera_fb_t* frame, CameraFrameInfo& info) const;
    void releaseFrame();
    esp_err_t lastError() const;
    const char* modelName() const;
    void nextSensorControl(int direction);
    void adjustSensorControl(int delta);
    const char* sensorControlName() const;
    int sensorControlValue() const;

private:
    void applySensorControl();

    bool initialized_ = false;
    camera_fb_t* currentFrame_ = nullptr;
    esp_err_t lastError_ = ESP_OK;
    const char* modelName_ = "Inconnue";
    SensorControl sensorControl_ = SensorControl::Brightness;
    int sensorValues_[static_cast<int>(SensorControl::Count)] = {0, 0, 0, 0, 0};
};

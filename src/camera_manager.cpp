#include "camera_manager.h"

#include <Arduino.h>

namespace {
constexpr int kPinPwdn = -1;
constexpr int kPinReset = -1;
constexpr int kPinXclk = 15;
constexpr int kPinSiod = 4;
constexpr int kPinSioc = 5;

constexpr int kPinD0 = 11;
constexpr int kPinD1 = 9;
constexpr int kPinD2 = 8;
constexpr int kPinD3 = 10;
constexpr int kPinD4 = 12;
constexpr int kPinD5 = 18;
constexpr int kPinD6 = 17;
constexpr int kPinD7 = 16;
constexpr int kPinVsync = 6;
constexpr int kPinHref = 7;
constexpr int kPinPclk = 13;
constexpr int kSensorControlCount = static_cast<int>(SensorControl::Count);

camera_config_t makeCameraConfig() {
    camera_config_t config{};
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = kPinD0;
    config.pin_d1 = kPinD1;
    config.pin_d2 = kPinD2;
    config.pin_d3 = kPinD3;
    config.pin_d4 = kPinD4;
    config.pin_d5 = kPinD5;
    config.pin_d6 = kPinD6;
    config.pin_d7 = kPinD7;
    config.pin_xclk = kPinXclk;
    config.pin_pclk = kPinPclk;
    config.pin_vsync = kPinVsync;
    config.pin_href = kPinHref;
    config.pin_sccb_sda = kPinSiod;
    config.pin_sccb_scl = kPinSioc;
    config.pin_pwdn = kPinPwdn;
    config.pin_reset = kPinReset;
    config.xclk_freq_hz = 20000000;
    const bool hasPsram = psramFound();
    config.frame_size = hasPsram ? FRAMESIZE_HVGA : FRAMESIZE_QVGA;
    config.pixel_format = PIXFORMAT_RGB565;
    config.grab_mode = CAMERA_GRAB_LATEST;
    config.fb_location = hasPsram ? CAMERA_FB_IN_PSRAM : CAMERA_FB_IN_DRAM;
    config.jpeg_quality = 12;
    config.fb_count = hasPsram ? 2 : 1;
    return config;
}
}  // namespace

bool CameraManager::begin() {
    pinMode(kPinSiod, OUTPUT);
    pinMode(kPinSioc, OUTPUT);
    digitalWrite(kPinSiod, LOW);
    digitalWrite(kPinSioc, LOW);
    delay(50);

    const camera_config_t config = makeCameraConfig();
    lastError_ = esp_camera_init(&config);
    if (lastError_ != ESP_OK) {
        Serial.printf("[CAM_ERR] esp_camera_init a echoue avec code 0x%x\n", lastError_);
        initialized_ = false;
        return false;
    }

    sensor_t* sensor = esp_camera_sensor_get();
    if (sensor != nullptr) {
        if (sensor->id.PID == 0x5640) {
            modelName_ = "OV5640 AF";
            sensor->set_vflip(sensor, 1);
            sensor->set_hmirror(sensor, 1);
            sensor->set_whitebal(sensor, 1);
            sensor->set_awb_gain(sensor, 1);
            sensor->set_exposure_ctrl(sensor, 1);
        } else if (sensor->id.PID == 0x2642) {
            modelName_ = "OV2640";
            sensor->set_vflip(sensor, 1);
        } else {
            modelName_ = "OmniVision";
        }
    }

    initialized_ = true;
    Serial.printf("[CAM] Camera prete: %s RGB565 %s PSRAM=%s\n",
                  modelName_,
                  psramFound() ? "HVGA 480x320" : "QVGA 320x240",
                  psramFound() ? "oui" : "non");
    return true;
}

camera_fb_t* CameraManager::capture() {
    if (!initialized_) {
        return nullptr;
    }

    releaseFrame();
    currentFrame_ = esp_camera_fb_get();
    return currentFrame_;
}

bool CameraManager::getFrameInfo(const camera_fb_t* frame, CameraFrameInfo& info) const {
    if (frame == nullptr) {
        return false;
    }

    info.data = frame->buf;
    info.length = frame->len;
    info.width = frame->width;
    info.height = frame->height;
    info.format = static_cast<uint32_t>(frame->format);
    return true;
}

void CameraManager::releaseFrame() {
    if (currentFrame_ != nullptr) {
        esp_camera_fb_return(currentFrame_);
        currentFrame_ = nullptr;
    }
}

esp_err_t CameraManager::lastError() const {
    return lastError_;
}

const char* CameraManager::modelName() const {
    return modelName_;
}

void CameraManager::nextSensorControl(int direction) {
    int next = static_cast<int>(sensorControl_) + direction;
    while (next < 0) {
        next += kSensorControlCount;
    }
    sensorControl_ = static_cast<SensorControl>(next % kSensorControlCount);
}

void CameraManager::adjustSensorControl(int delta) {
    const int index = static_cast<int>(sensorControl_);
    int minValue = -2;
    int maxValue = 2;

    if (sensorControl_ == SensorControl::SpecialEffect) {
        minValue = 0;
        maxValue = 6;
    } else if (sensorControl_ == SensorControl::WhiteBalance) {
        minValue = 0;
        maxValue = 4;
    }

    sensorValues_[index] = constrain(sensorValues_[index] + delta, minValue, maxValue);
    applySensorControl();
}

const char* CameraManager::sensorControlName() const {
    switch (sensorControl_) {
        case SensorControl::Brightness:
            return "BRIGHT";
        case SensorControl::Contrast:
            return "CONTRAST";
        case SensorControl::Saturation:
            return "SAT";
        case SensorControl::SpecialEffect:
            return "SENSOR FX";
        case SensorControl::WhiteBalance:
            return "WB";
        case SensorControl::Count:
            return "SENSOR";
    }
    return "SENSOR";
}

int CameraManager::sensorControlValue() const {
    return sensorValues_[static_cast<int>(sensorControl_)];
}

void CameraManager::applySensorControl() {
    if (!initialized_) {
        return;
    }

    sensor_t* sensor = esp_camera_sensor_get();
    if (sensor == nullptr) {
        return;
    }

    switch (sensorControl_) {
        case SensorControl::Brightness:
            sensor->set_brightness(sensor, sensorControlValue());
            break;
        case SensorControl::Contrast:
            sensor->set_contrast(sensor, sensorControlValue());
            break;
        case SensorControl::Saturation:
            sensor->set_saturation(sensor, sensorControlValue());
            break;
        case SensorControl::SpecialEffect:
            sensor->set_special_effect(sensor, sensorControlValue());
            break;
        case SensorControl::WhiteBalance:
            sensor->set_wb_mode(sensor, sensorControlValue());
            break;
        case SensorControl::Count:
            break;
    }

    Serial.printf("[CAM] %s=%d\n", sensorControlName(), sensorControlValue());
}

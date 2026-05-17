#include "camera_manager.h"

#include "esp_camera.h"

namespace {
// IMPORTANT: ajuster ce mapping à votre carte ESP32-S3 CAM OV5640 exacte.
// Les valeurs ci-dessous servent de base de travail commune pour modules S3 + OV5640.
constexpr int kPinPwdn = -1;
constexpr int kPinReset = -1;
constexpr int kPinXclk = 10;
constexpr int kPinSiod = 40;
constexpr int kPinSioc = 39;

constexpr int kPinD7 = 48;
constexpr int kPinD6 = 11;
constexpr int kPinD5 = 12;
constexpr int kPinD4 = 14;
constexpr int kPinD3 = 16;
constexpr int kPinD2 = 18;
constexpr int kPinD1 = 17;
constexpr int kPinD0 = 15;
constexpr int kPinVsync = 38;
constexpr int kPinHref = 47;
constexpr int kPinPclk = 13;

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
    config.frame_size = FRAMESIZE_QVGA;
    config.pixel_format = PIXFORMAT_JPEG;
    config.grab_mode = CAMERA_GRAB_LATEST;
    config.fb_location = CAMERA_FB_IN_PSRAM;
    config.jpeg_quality = 12;
    config.fb_count = 2;
    return config;
}
}  // namespace

bool CameraManager::begin() {
    const camera_config_t config = makeCameraConfig();
    const esp_err_t err = esp_camera_init(&config);
    initialized_ = (err == ESP_OK);
    return initialized_;
}

bool CameraManager::captureFrame(CameraFrameInfo& info) {
    if (!initialized_) {
        return false;
    }

    releaseFrame();
    currentFrame_ = esp_camera_fb_get();
    if (currentFrame_ == nullptr) {
        return false;
    }

    info.length = currentFrame_->len;
    info.width = currentFrame_->width;
    info.height = currentFrame_->height;
    info.format = static_cast<uint32_t>(currentFrame_->format);
    return true;
}

void CameraManager::releaseFrame() {
    if (currentFrame_ != nullptr) {
        esp_camera_fb_return(currentFrame_);
        currentFrame_ = nullptr;
    }
}

bool CameraManager::isReady() const {
    return initialized_;
}

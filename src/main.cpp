#include <Arduino.h>

#define LGFX_USE_V1
#include <LovyanGFX.hpp>

#include "camera_manager.h"
#include "display_manager.h"
#include "encoder_manager.h"
#include "glitch_engine.h"
#include "storage_manager.h"
#include "touch_manager.h"

namespace {
DisplayManager g_display;
TouchManager g_touch;
CameraManager g_camera;
StorageManager g_storage;
GlitchEngine g_glitch;
EncoderManager g_encoder;

enum class AppState {
    Live,
    Menu,
};

constexpr UiButton kMenuButton{370, 36, 96, 38, "MENU", TFT_WHITE};
constexpr UiButton kShootButton{188, 262, 104, 48, "SHOOT", TFT_RED};
constexpr UiButton kMenuGlitchButton{140, 88, 200, 38, "GLITCH +", TFT_CYAN};
constexpr UiButton kMenuSensorButton{140, 136, 200, 38, "CAPTEUR +", TFT_ORANGE};
constexpr UiButton kMenuIntensityButton{140, 184, 200, 38, "INT +", TFT_GREEN};
constexpr UiButton kMenuBackButton{140, 232, 200, 38, "RETOUR", TFT_RED};

AppState g_state = AppState::Live;
uint32_t g_lastTouchActionMs = 0;
uint32_t g_captureCount = 0;

bool touchActionReady() {
    const uint32_t now = millis();
    if (now - g_lastTouchActionMs < 280) {
        return false;
    }
    g_lastTouchActionMs = now;
    return true;
}

void saveCapture(const camera_fb_t* frame) {
    if (frame == nullptr || !g_storage.isReady()) {
        g_display.drawStatusLine("Capture impossible: SD ou camera indisponible", TFT_ORANGE);
        return;
    }

    char path[40];
    snprintf(path, sizeof(path), "/azoth_%06lu.jpg", static_cast<unsigned long>(g_captureCount++));
    g_display.drawCaptureFlash();

    if (g_storage.saveRgb565AsJpeg(frame, path, 82)) {
        Serial.printf("[SD] Capture sauvegardee: %s\n", path);
        g_display.drawStatusLine(path, TFT_GREEN);
    } else {
        Serial.println("[SD] Echec sauvegarde JPEG");
        g_display.drawStatusLine("Echec sauvegarde JPEG", TFT_RED);
    }
}

void handleTouch(const TouchState& touch, const camera_fb_t* frame) {
    if (touch.count >= 2 && abs(touch.pinchDelta) > 10) {
        g_glitch.adjustIntensity(touch.pinchDelta / 8);
        return;
    }

    if (!touch.justPressed || touch.count == 0 || !touchActionReady()) {
        return;
    }

    const TouchPoint& point = touch.points[0];
    if (g_state == AppState::Live) {
        if (g_display.contains(kShootButton, point)) {
            saveCapture(frame);
        } else if (g_display.contains(kMenuButton, point)) {
            g_state = AppState::Menu;
        }
        return;
    }

    if (g_display.contains(kMenuGlitchButton, point)) {
        g_glitch.nextEffect(1);
    } else if (g_display.contains(kMenuSensorButton, point)) {
        g_camera.nextSensorControl(1);
    } else if (g_display.contains(kMenuIntensityButton, point)) {
        g_camera.adjustSensorControl(1);
        g_glitch.adjustIntensity(4);
    } else if (g_display.contains(kMenuBackButton, point)) {
        g_state = AppState::Live;
    }
}

void handleEncoder(const EncoderState& encoder, const camera_fb_t* frame) {
    if (encoder.rotation != 0) {
        if (g_state == AppState::Live) {
            g_glitch.nextEffect(encoder.rotation);
        } else {
            g_camera.adjustSensorControl(encoder.rotation);
            g_glitch.adjustIntensity(encoder.rotation * 3);
        }
    }

    if (encoder.clicked) {
        if (g_state == AppState::Live) {
            saveCapture(frame);
        } else {
            g_state = AppState::Live;
        }
    }
}
}

void setup() {
    Serial.begin(115200);
    delay(500);
    Serial.println("\n=== AZOTH GLITCHCAM S3 ===");

    g_display.begin();
    g_display.drawSplash();

    const bool storageReady = g_storage.begin();
    const bool cameraReady = g_camera.begin();
    g_touch.begin();
    g_encoder.begin();

    Serial.printf("[BOOT] Camera=%s | SD=%s | Touch=OK | Encoder=OK\n",
                  cameraReady ? "OK" : "KO",
                  storageReady ? "OK" : "KO");

    g_display.drawBootUi(cameraReady, storageReady, g_camera.modelName());
    if (!cameraReady) {
        g_display.drawError("ERREUR CAMERA", g_camera.lastError());
        delay(1500);
    } else {
        delay(900);
    }
}

void loop() {
    camera_fb_t* frame = g_camera.capture();
    CameraFrameInfo frameInfo{};
    if (g_camera.getFrameInfo(frame, frameInfo)) {
        g_glitch.apply(frameInfo.data, frameInfo.width, frameInfo.height);
        g_display.drawPreview(frameInfo);
    }

    const TouchState touch = g_touch.update();
    const EncoderState encoder = g_encoder.update();

    handleTouch(touch, frame);
    handleEncoder(encoder, frame);

    if (g_state == AppState::Live) {
        g_display.drawLiveOverlay(
            g_glitch.effectName(),
            g_glitch.intensity(),
            g_camera.sensorControlName(),
            g_camera.sensorControlValue(),
            g_storage.isReady());
    } else {
        g_display.drawMenuOverlay(
            g_glitch.effectName(),
            g_glitch.intensity(),
            g_camera.sensorControlName(),
            g_camera.sensorControlValue());
    }

    for (uint8_t i = 0; i < touch.count; ++i) {
        g_display.drawTouchMarker(touch.points[i].x, touch.points[i].y);
    }

    g_camera.releaseFrame();
    delay(10);
}

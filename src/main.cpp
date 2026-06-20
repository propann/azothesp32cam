#include <Arduino.h>

#include "camera_manager.h"
#include "display_manager.h"

namespace {
constexpr uint32_t kSplashDurationMs = 1800;
constexpr uint8_t kWarmupFrameCount = 5;

DisplayManager g_display;
CameraManager g_camera;

bool captureAndDisplay() {
    // Les premieres trames ont souvent une exposition et une balance des blancs
    // instables. Elles sont rendues au capteur sans etre affichees.
    for (uint8_t index = 0; index < kWarmupFrameCount; ++index) {
        camera_fb_t* warmupFrame = g_camera.capture();
        if (warmupFrame == nullptr) {
            Serial.printf("[CAM] Trame de stabilisation %u indisponible\n", index + 1);
            delay(80);
            continue;
        }
        g_camera.releaseFrame();
        delay(80);
    }

    camera_fb_t* frame = g_camera.capture();
    CameraFrameInfo frameInfo{};
    if (!g_camera.getFrameInfo(frame, frameInfo)) {
        g_camera.releaseFrame();
        return false;
    }

    Serial.printf("[CAM] Capture %ux%u, %u octets, format=%u\n",
                  frameInfo.width,
                  frameInfo.height,
                  static_cast<unsigned>(frameInfo.length),
                  static_cast<unsigned>(frameInfo.format));

    const bool displayed = g_display.drawPreview(frameInfo);
    g_camera.releaseFrame();
    return displayed;
}
}  // namespace

void setup() {
    Serial.begin(115200);
    delay(500);
    Serial.println("\n=== AZOTH CAMERA DISPLAY TEST ===");

    g_display.begin();
    g_display.drawSplash();
    const uint32_t splashStartedAt = millis();

    const bool cameraReady = g_camera.begin();

    const uint32_t splashElapsed = millis() - splashStartedAt;
    if (splashElapsed < kSplashDurationMs) {
        delay(kSplashDurationMs - splashElapsed);
    }

    if (!cameraReady) {
        Serial.printf("[CAM] Initialisation impossible: 0x%x\n", g_camera.lastError());
        g_display.drawError("ERREUR CAMERA", g_camera.lastError());
        return;
    }

    if (!captureAndDisplay()) {
        Serial.println("[CAM] Capture RGB565 invalide");
        g_display.drawFrameStatus("CAPTURE IMPOSSIBLE");
        return;
    }

    Serial.println("[DISPLAY] Capture affichee au centre");
}

void loop() {
    // Image fixe de validation: ne pas ecraser la capture affichee.
    delay(1000);
}

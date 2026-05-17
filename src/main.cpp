#include <Arduino.h>
#include "camera_manager.h"
#include "display_manager.h"
#include "storage_manager.h"
#include "touch_manager.h"

DisplayManager g_display;
TouchManager g_touch;
CameraManager g_camera;
StorageManager g_storage;

namespace {
constexpr int kTouchPollDelayMs = 30;
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n=== AZOTH GLITCHCAM S3 : INITIALISATION ===");

    g_display.begin();
    g_display.drawBootUi();

    const bool touchReady = g_touch.begin(48, 46);
    const bool cameraReady = g_camera.begin();
    const bool storageReady = g_storage.begin();

    Serial.printf("[BOOT] Touch=%s | Camera=%s | SD=%s\n",
                  touchReady ? "OK" : "KO",
                  cameraReady ? "OK" : "KO",
                  storageReady ? "OK" : "KO");

    g_display.drawStatusLine(cameraReady ? "Camera prete" : "Camera KO: verifier pinmap OV5640", cameraReady ? TFT_GREEN : TFT_RED);
}

void loop() {
    TouchPoint point{};
    if (!g_touch.readPoint(point)) {
        delay(kTouchPollDelayMs);
        return;
    }

    g_display.drawTouchMarker(point.x, point.y);
    Serial.printf("[TOUCH] X:%d Y:%d\n", point.x, point.y);

    CameraFrameInfo frameInfo{};
    if (g_camera.captureFrame(frameInfo)) {
        Serial.printf("[CAM] frame %ux%u len=%u fmt=%u\n",
                      frameInfo.width,
                      frameInfo.height,
                      static_cast<unsigned>(frameInfo.length),
                      static_cast<unsigned>(frameInfo.format));
        g_display.drawStatusLine("Capture camera OK (voir logs serie)", TFT_CYAN);
        g_camera.releaseFrame();
    } else {
        g_display.drawStatusLine("Capture camera KO", TFT_RED);
    }

    delay(120);
}

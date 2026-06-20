#include <Arduino.h>

#include "camera_manager.h"
#include "display_manager.h"

namespace {
DisplayManager g_display;
CameraManager g_camera;
uint32_t g_frameCount = 0;
uint32_t g_lastErrorMs = 0;
}  // namespace

void setup() {
    Serial.begin(115200);
    delay(300);
    Serial.println("\n=== AZOTH LIVE VIEW 480x320 RGB565 ===");

    g_display.begin();
    if (!g_camera.begin()) {
        Serial.printf("[CAM_ERR] Initialisation impossible: 0x%x\n", g_camera.lastError());
    }
}

void loop() {
    camera_fb_t* frame = g_camera.capture();
    CameraFrameInfo frameInfo{};

    if (g_camera.getFrameInfo(frame, frameInfo) && g_display.drawPreview(frameInfo)) {
        ++g_frameCount;
        if (g_frameCount % 120 == 0) {
            Serial.printf("[LIVE] frame=%lu size=%ux%u bytes=%u\n",
                          static_cast<unsigned long>(g_frameCount),
                          frameInfo.width,
                          frameInfo.height,
                          static_cast<unsigned>(frameInfo.length));
        }
    } else if (millis() - g_lastErrorMs >= 1000) {
        g_lastErrorMs = millis();
        Serial.println("[LIVE_ERR] Frame RGB565 480x320 indisponible");
    }

    // pushImage est termine avant de rendre le tampon a la camera.
    g_camera.releaseFrame();
    delay(1);
}

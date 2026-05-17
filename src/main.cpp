#include <Arduino.h>
#include "display_manager.h"
#include "touch_manager.h"

DisplayManager g_display;
TouchManager g_touch;

namespace {
constexpr int kTouchPollDelayMs = 30;
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n=== AZOTH GLITCHCAM S3 : INITIALISATION DES PERIPHERIQUES ===");

    g_display.begin();
    g_display.drawBootUi();

    Serial.print("[I2C] Initialisation du contrôleur FT6336U... ");
    const bool touchReady = g_touch.begin(48, 46);
    Serial.println(touchReady ? "OK ! Configuration terminée." : "ECHEC !");
}

void loop() {
    TouchPoint point{};
    if (g_touch.readPoint(point)) {
        g_display.drawTouchMarker(point.x, point.y);
        Serial.printf("[TOUCH] X : %d | Y : %d\n", point.x, point.y);
    }
    delay(kTouchPollDelayMs);
}

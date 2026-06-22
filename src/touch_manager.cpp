#include "touch_manager.h"

#include <Arduino.h>
#include <Wire.h>

namespace {
constexpr int kPinSda = 48;
// GPIO46 est une broche de strapping. Le bootloader UART du S3 exige
// GPIO0=LOW et GPIO46=LOW au reset ; le pull-up I2C du FT6336U empêchait donc
// systématiquement l'entrée en mode téléchargement.
constexpr int kPinScl = 41;
}

bool TouchManager::begin() {
    Wire1.begin(kPinSda, kPinScl);
    Wire1.setClock(400000);
    return true;
}

TouchState TouchManager::update() {
    TouchState next{};
    const bool hadPress = state_.pressed;

    if (!readRaw(next)) {
        next.count = 0;
    }

    next.pressed = next.count > 0;
    next.justPressed = next.pressed && !hadPress;
    next.justReleased = !next.pressed && hadPress;

    if (next.count >= 2) {
        const int currentDistance = distance(next.points[0], next.points[1]);
        next.pinchDelta = lastPinchDistance_ == 0 ? 0 : currentDistance - lastPinchDistance_;
        lastPinchDistance_ = currentDistance;
    } else {
        lastPinchDistance_ = 0;
    }

    state_ = next;
    return state_;
}

const TouchState& TouchManager::state() const {
    return state_;
}

bool TouchManager::readRaw(TouchState& next) {
    Wire1.beginTransmission(kTouchAddr);
    Wire1.write(0x02);
    if (Wire1.endTransmission() != 0) {
        return false;
    }

    Wire1.requestFrom(kTouchAddr, static_cast<uint8_t>(11));
    if (Wire1.available() < 5) {
        return false;
    }

    const uint8_t touchCount = Wire1.read() & 0x0F;
    next.count = touchCount > 2 ? 2 : touchCount;
    if (next.count == 0) {
        return false;
    }

    for (uint8_t i = 0; i < next.count; ++i) {
        if (Wire1.available() < 4) {
            next.count = i;
            return i > 0;
        }

        const uint8_t xMsb = Wire1.read() & 0x0F;
        const uint8_t xLsb = Wire1.read();
        const uint8_t yMsb = Wire1.read() & 0x0F;
        const uint8_t yLsb = Wire1.read();

        const int rawX = (xMsb << 8) | xLsb;
        const int rawY = (yMsb << 8) | yLsb;
        next.points[i].x = rawX;
        next.points[i].y = 320 - rawY;
        next.points[i].valid = true;

        if (i == 0 && next.count > 1) {
            if (Wire1.available() > 0) {
                Wire1.read();
            }
            if (Wire1.available() > 0) {
                Wire1.read();
            }
        }
    }

    return true;
}

int TouchManager::distance(const TouchPoint& a, const TouchPoint& b) {
    const int dx = a.x - b.x;
    const int dy = a.y - b.y;
    return abs(dx) + abs(dy);
}

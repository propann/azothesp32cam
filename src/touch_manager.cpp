#include "touch_manager.h"

#include <Wire.h>

bool TouchManager::begin(int sdaPin, int sclPin) {
    Wire.begin(sdaPin, sclPin);
    return true;
}

bool TouchManager::readPoint(TouchPoint& point) {
    Wire.beginTransmission(kTouchAddr);
    Wire.write(0x02);
    if (Wire.endTransmission() != 0) {
        return false;
    }

    Wire.requestFrom(kTouchAddr, static_cast<uint8_t>(5));
    if (Wire.available() < 5) {
        return false;
    }

    const uint8_t touchCount = Wire.read() & 0x0F;
    if (touchCount == 0) {
        return false;
    }

    const uint8_t xMsb = Wire.read() & 0x0F;
    const uint8_t xLsb = Wire.read();
    const uint8_t yMsb = Wire.read() & 0x0F;
    const uint8_t yLsb = Wire.read();

    point.x = (xMsb << 8) | xLsb;
    point.y = (yMsb << 8) | yLsb;
    return true;
}

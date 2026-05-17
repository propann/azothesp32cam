#pragma once

#include <stdint.h>

struct TouchPoint {
    int x;
    int y;
};

class TouchManager {
public:
    bool begin(int sdaPin, int sclPin);
    bool readPoint(TouchPoint& point);

private:
    static constexpr uint8_t kTouchAddr = 0x38;
};

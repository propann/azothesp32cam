#pragma once

#include <stdint.h>

struct TouchPoint {
    int x = -1;
    int y = -1;
    bool valid = false;
};

struct TouchState {
    TouchPoint points[2];
    uint8_t count = 0;
    bool pressed = false;
    bool justPressed = false;
    bool justReleased = false;
    int pinchDelta = 0;
};

class TouchManager {
public:
    bool begin();
    TouchState update();
    const TouchState& state() const;

private:
    bool readRaw(TouchState& next);
    static int distance(const TouchPoint& a, const TouchPoint& b);

    static constexpr uint8_t kTouchAddr = 0x38;
    TouchState state_{};
    int lastPinchDistance_ = 0;
};

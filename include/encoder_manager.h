#pragma once

#include <stdint.h>

struct EncoderState {
    int8_t rotation = 0;
    bool pressed = false;
    bool clicked = false;
};

class EncoderManager {
public:
    bool begin();
    EncoderState update();
    const EncoderState& state() const;

private:
    static constexpr int kPinA = 1;
    static constexpr int kPinB = 2;
    static constexpr int kPinButton = 42;
    static constexpr uint32_t kDebounceMs = 35;

    EncoderState state_{};
    uint8_t lastEncoded_ = 0;
    bool lastButtonLevel_ = true;
    uint32_t lastButtonChangeMs_ = 0;
};

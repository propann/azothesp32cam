#include "encoder_manager.h"

#include <Arduino.h>

bool EncoderManager::begin() {
    pinMode(kPinA, INPUT_PULLUP);
    pinMode(kPinB, INPUT_PULLUP);
    pinMode(kPinButton, INPUT_PULLUP);

    const uint8_t a = digitalRead(kPinA) == HIGH ? 1 : 0;
    const uint8_t b = digitalRead(kPinB) == HIGH ? 1 : 0;
    lastEncoded_ = static_cast<uint8_t>((a << 1) | b);
    lastButtonLevel_ = digitalRead(kPinButton) == HIGH;
    return true;
}

EncoderState EncoderManager::update() {
    state_.rotation = 0;
    state_.clicked = false;

    const uint8_t a = digitalRead(kPinA) == HIGH ? 1 : 0;
    const uint8_t b = digitalRead(kPinB) == HIGH ? 1 : 0;
    const uint8_t encoded = static_cast<uint8_t>((a << 1) | b);
    const uint8_t transition = static_cast<uint8_t>((lastEncoded_ << 2) | encoded);

    if (transition == 0b1101 || transition == 0b0100 || transition == 0b0010 || transition == 0b1011) {
        state_.rotation = 1;
    } else if (transition == 0b1110 || transition == 0b0111 || transition == 0b0001 || transition == 0b1000) {
        state_.rotation = -1;
    }
    lastEncoded_ = encoded;

    const bool level = digitalRead(kPinButton) == HIGH;
    const uint32_t now = millis();
    if (level != lastButtonLevel_ && now - lastButtonChangeMs_ > kDebounceMs) {
        lastButtonChangeMs_ = now;
        lastButtonLevel_ = level;
        state_.pressed = !level;
        if (!level) {
            state_.clicked = true;
        }
    }

    return state_;
}

const EncoderState& EncoderManager::state() const {
    return state_;
}

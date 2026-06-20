#include "glitch_engine.h"

#include <Arduino.h>

namespace {
constexpr int kEffectCount = static_cast<int>(GlitchEffect::Count);

uint16_t swapChannels(uint16_t pixel) {
    const uint16_t r = (pixel & 0xF800) >> 11;
    const uint16_t g = (pixel & 0x07E0) >> 5;
    const uint16_t b = pixel & 0x001F;
    return static_cast<uint16_t>((b << 11) | ((g & 0x3F) << 5) | r);
}
}

void GlitchEngine::setEffectIndex(int index) {
    while (index < 0) {
        index += kEffectCount;
    }
    effect_ = static_cast<GlitchEffect>(index % kEffectCount);
}

void GlitchEngine::nextEffect(int direction) {
    setEffectIndex(effectIndex() + direction);
}

void GlitchEngine::adjustIntensity(int delta) {
    intensity_ = constrain(intensity_ + delta, 0, 96);
}

void GlitchEngine::apply(uint8_t* rgb565Data, int width, int height) {
    if (rgb565Data == nullptr || width <= 0 || height <= 0 || effect_ == GlitchEffect::None || intensity_ == 0) {
        return;
    }

    uint16_t* pixels = reinterpret_cast<uint16_t*>(rgb565Data);
    const int count = width * height;

    switch (effect_) {
        case GlitchEffect::RgbShift: {
            const int shift = constrain(intensity_ / 8, 1, 12);
            for (int y = 0; y < height; y += 2) {
                const int row = y * width;
                for (int x = shift; x < width; ++x) {
                    const uint16_t src = pixels[row + x - shift];
                    const uint16_t dst = pixels[row + x];
                    pixels[row + x] = static_cast<uint16_t>((dst & 0x07E0) | (src & 0xF81F));
                }
            }
            break;
        }
        case GlitchEffect::ByteCorrupt: {
            const int step = constrain(128 - intensity_, 16, 128);
            for (int i = step; i < count; i += step) {
                pixels[i] ^= static_cast<uint16_t>(0x001F | ((i & 0x3F) << 5));
            }
            break;
        }
        case GlitchEffect::ScanlineSort: {
            const int band = constrain(18 - intensity_ / 8, 4, 18);
            for (int y = 0; y < height; y += band) {
                const int row = y * width;
                for (int x = 0; x < width / 2; x += 3) {
                    const int a = row + x;
                    const int b = row + width - 1 - x;
                    const uint16_t tmp = pixels[a];
                    pixels[a] = swapChannels(pixels[b]);
                    pixels[b] = tmp;
                }
            }
            break;
        }
        case GlitchEffect::MirrorBands: {
            const int band = constrain(8 + intensity_ / 3, 8, 40);
            for (int y = 0; y < height; ++y) {
                if ((y / band) % 2 == 0) {
                    continue;
                }
                const int row = y * width;
                for (int x = 0; x < width / 2; ++x) {
                    const uint16_t tmp = pixels[row + x];
                    pixels[row + x] = pixels[row + width - 1 - x];
                    pixels[row + width - 1 - x] = tmp;
                }
            }
            break;
        }
        case GlitchEffect::Solarize: {
            const uint16_t threshold = static_cast<uint16_t>(0x4208 + intensity_ * 128);
            for (int i = 0; i < count; i += 2) {
                if ((pixels[i] & 0x7BEF) > threshold) {
                    pixels[i] = static_cast<uint16_t>(~pixels[i]);
                }
            }
            break;
        }
        case GlitchEffect::Noise: {
            const int step = constrain(72 - intensity_ / 2, 8, 72);
            uint32_t seed = millis();
            for (int i = 0; i < count; i += step) {
                seed = seed * 1103515245UL + 12345UL;
                pixels[i] ^= static_cast<uint16_t>((seed >> 12) & 0xFFFF);
            }
            break;
        }
        case GlitchEffect::FrameTear: {
            const int tear = constrain(intensity_ / 3, 2, 32);
            for (int y = tear; y < height - tear; y += tear * 3) {
                const int row = y * width;
                const int offset = ((y / tear) % 2 == 0 ? tear : -tear);
                for (int x = tear; x < width - tear; ++x) {
                    pixels[row + x] = pixels[row + x + offset];
                }
            }
            break;
        }
        case GlitchEffect::ChannelMask: {
            const uint16_t masks[] = {0xF800, 0x07E0, 0x001F, 0xF81F, 0xFFE0};
            const uint16_t mask = masks[(millis() / 500) % 5];
            const int stride = constrain(9 - intensity_ / 16, 2, 9);
            for (int y = 0; y < height; y += stride) {
                const int row = y * width;
                for (int x = 0; x < width; ++x) {
                    pixels[row + x] &= mask;
                }
            }
            break;
        }
        case GlitchEffect::None:
        case GlitchEffect::Count:
            break;
    }
}

int GlitchEngine::effectIndex() const {
    return static_cast<int>(effect_);
}

int GlitchEngine::intensity() const {
    return intensity_;
}

const char* GlitchEngine::effectName() const {
    switch (effect_) {
        case GlitchEffect::None:
            return "NONE";
        case GlitchEffect::RgbShift:
            return "RGB SHIFT";
        case GlitchEffect::ByteCorrupt:
            return "BYTE ROT";
        case GlitchEffect::ScanlineSort:
            return "SCAN SORT";
        case GlitchEffect::MirrorBands:
            return "MIRROR";
        case GlitchEffect::Solarize:
            return "SOLAR";
        case GlitchEffect::Noise:
            return "NOISE";
        case GlitchEffect::FrameTear:
            return "TEAR";
        case GlitchEffect::ChannelMask:
            return "CHANNEL";
        case GlitchEffect::Count:
            return "UNKNOWN";
    }
    return "UNKNOWN";
}

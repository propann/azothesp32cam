#pragma once

#include <stdint.h>

enum class GlitchEffect : uint8_t {
    None = 0,
    RgbShift,
    ByteCorrupt,
    ScanlineSort,
    MirrorBands,
    Solarize,
    Noise,
    FrameTear,
    ChannelMask,
    Count
};

class GlitchEngine {
public:
    void setEffectIndex(int index);
    void nextEffect(int direction);
    void adjustIntensity(int delta);
    void apply(uint8_t* rgb565Data, int width, int height);
    int effectIndex() const;
    int intensity() const;
    const char* effectName() const;

private:
    GlitchEffect effect_ = GlitchEffect::None;
    int intensity_ = 24;
};

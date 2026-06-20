#include "display_manager.h"

#include <Arduino.h>

#define LGFX_USE_V1
#include <LovyanGFX.hpp>

namespace {
class LGFXAzothS3 : public lgfx::LGFX_Device {
    lgfx::Panel_ST7796 panel_;
    lgfx::Bus_SPI bus_;

public:
    LGFXAzothS3() {
        auto busConfig = bus_.config();
        busConfig.spi_host = SPI2_HOST;
        busConfig.spi_mode = 0;
        busConfig.freq_write = 20000000;
        busConfig.freq_read = 8000000;
        busConfig.pin_sclk = 21;
        busConfig.pin_mosi = 47;
        busConfig.pin_miso = -1;
        busConfig.pin_dc = 3;
        bus_.config(busConfig);
        panel_.setBus(&bus_);

        auto panelConfig = panel_.config();
        panelConfig.pin_cs = -1;
        panelConfig.pin_rst = -1;
        panelConfig.panel_width = 320;
        panelConfig.panel_height = 480;
        panelConfig.offset_x = 0;
        panelConfig.offset_y = 0;
        panelConfig.readable = false;
        panelConfig.invert = false;
        panelConfig.rgb_order = false;
        panel_.config(panelConfig);

        setPanel(&panel_);
    }
};

LGFXAzothS3 g_tft;
}  // namespace

bool DisplayManager::begin() {
    g_tft.init();
    g_tft.setRotation(1);
    g_tft.setSwapBytes(true);
    g_tft.fillScreen(TFT_BLACK);

    const bool ready = g_tft.width() == 480 && g_tft.height() == 320;
    Serial.printf("[DISPLAY] ST7796 %dx%d ready=%s\n",
                  g_tft.width(),
                  g_tft.height(),
                  ready ? "oui" : "non");
    return ready;
}

bool DisplayManager::drawPreview(const CameraFrameInfo& frame) {
    constexpr uint16_t kWidth = 480;
    constexpr uint16_t kHeight = 320;
    constexpr size_t kFrameBytes = static_cast<size_t>(kWidth) * kHeight * sizeof(uint16_t);

    if (frame.data == nullptr
        || frame.format != PIXFORMAT_RGB565
        || frame.width != kWidth
        || frame.height != kHeight
        || frame.length < kFrameBytes) {
        return false;
    }

    // setSwapBytes(true) applique la permutation RGB565 lors de l'envoi.
    // pushImage est synchrone : le framebuffer peut etre libere au retour.
    g_tft.pushImage(0,
                    0,
                    kWidth,
                    kHeight,
                    reinterpret_cast<const uint16_t*>(frame.data));
    return true;
}

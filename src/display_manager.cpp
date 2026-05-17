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
        busConfig.freq_write = 40000000;
        busConfig.freq_read = 16000000;
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
        panelConfig.inverted = false;
        panelConfig.rgb_order = false;
        panel_.config(panelConfig);

        setPanel(&panel_);
    }
};

LGFXAzothS3 g_tft;
}

bool DisplayManager::begin() {
    g_tft.init();
    g_tft.setRotation(1);
    g_tft.fillScreen(TFT_BLACK);
    return true;
}

void DisplayManager::drawBootUi() {
    g_tft.setTextColor(TFT_RED);
    g_tft.setTextSize(3);
    g_tft.drawString("AZOTH GLITCHCAM S3", 20, 20);

    g_tft.setTextSize(2);
    g_tft.setTextColor(TFT_WHITE);
    g_tft.drawString("Cablage securise : OPERATIONNEL", 20, 60);
    g_tft.drawString("Touch = capture camera + diagnostic", 20, 85);

    g_tft.drawRect(80, 120, 320, 180, TFT_BLUE);
    g_tft.setTextColor(TFT_BLUE);
    g_tft.drawString("ZONE PREVIEW 320x180", 130, 200);
}

void DisplayManager::drawTouchMarker(int x, int y) {
    g_tft.fillRect(x - 6, y - 6, 12, 12, TFT_GREEN);
}

void DisplayManager::drawStatusLine(const char* message, int color) {
    g_tft.fillRect(0, 290, 480, 24, TFT_BLACK);
    g_tft.setTextSize(1);
    g_tft.setTextColor(color);
    g_tft.drawString(message, 8, 296);
}

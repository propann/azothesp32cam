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
        // 20 MHz reste fiable avec un écran ST7796 déporté sur fils Dupont.
        // Repasser à 40 MHz seulement après validation hardware.
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
}

bool DisplayManager::begin() {
    Serial.println("[DISPLAY] Init ST7796 SPI2 SCLK=21 MOSI=47 DC=3 CS=GND RST=3V3");
    g_tft.init();
    delay(120);
    g_tft.setRotation(1);
    g_tft.setSwapBytes(true);
    g_tft.fillScreen(TFT_BLACK);
    Serial.printf("[DISPLAY] Ready width=%d height=%d\n", g_tft.width(), g_tft.height());
    return true;
}

void DisplayManager::drawSplash() {
    previewPrepared_ = false;
    g_tft.fillScreen(TFT_BLACK);
    g_tft.fillRect(0, 0, 160, 18, TFT_RED);
    g_tft.fillRect(160, 0, 160, 18, TFT_GREEN);
    g_tft.fillRect(320, 0, 160, 18, TFT_BLUE);
    g_tft.setTextColor(TFT_ORANGE);
    g_tft.setTextSize(4);
    g_tft.drawCenterString("Azoth", 240, 120);
    g_tft.setTextColor(TFT_WHITE);
    g_tft.setTextSize(3);
    g_tft.drawCenterString("- Photographie -", 240, 170);
}

void DisplayManager::drawBootUi(bool cameraReady, bool storageReady, const char* cameraModel) {
    g_tft.fillScreen(TFT_BLACK);

    g_tft.drawRect(0, 0, 480, 320, TFT_DARKGREY);
    g_tft.drawLine(10, 10, 40, 10, TFT_CYAN); g_tft.drawLine(10, 10, 10, 40, TFT_CYAN);
    g_tft.drawLine(470, 10, 440, 10, TFT_CYAN); g_tft.drawLine(470, 10, 470, 40, TFT_CYAN);
    g_tft.drawLine(10, 310, 40, 310, TFT_CYAN); g_tft.drawLine(10, 310, 10, 280, TFT_CYAN);
    g_tft.drawLine(470, 310, 440, 310, TFT_CYAN); g_tft.drawLine(470, 310, 470, 280, TFT_CYAN);

    g_tft.setTextColor(TFT_RED);
    g_tft.setTextSize(2);
    g_tft.drawString("AZOTH GLITCHCAM v1.0", 25, 25);

    g_tft.setTextSize(1);
    g_tft.setTextColor(TFT_LIGHTGREY);
    g_tft.drawString("RGB565 QVGA SAFE | GLITCH RAM | SDMMC 1-bit", 25, 50);

    g_tft.drawRect(80, 70, 320, 240, 0x18E3);
    g_tft.drawLine(240, 160, 240, 180, TFT_GREEN);
    g_tft.drawLine(230, 170, 250, 170, TFT_GREEN);

    g_tft.setTextColor(cameraReady ? TFT_GREEN : TFT_RED);
    g_tft.drawString(cameraReady ? cameraModel : "Camera KO", 25, 295);
    g_tft.setTextColor(storageReady ? TFT_GREEN : TFT_ORANGE);
    g_tft.drawRightString(storageReady ? "SD OK" : "SD KO", 455, 295);
}

void DisplayManager::drawLiveOverlay(const char* effectName, int intensity, const char* sensorName, int sensorValue, bool storageReady) {
    g_tft.setTextSize(1);
    g_tft.setTextColor(TFT_BLACK);
    g_tft.drawString("AZOTH", 9, 9);
    g_tft.setTextColor(TFT_WHITE);
    g_tft.drawString("AZOTH", 8, 8);
    g_tft.setTextColor(TFT_BLACK);
    g_tft.drawString(effectName, 73, 9);
    g_tft.setTextColor(TFT_CYAN);
    g_tft.drawString(effectName, 72, 8);
    g_tft.setTextColor(TFT_BLACK);
    g_tft.drawString((String("INT ") + String(intensity)).c_str(), 171, 9);
    g_tft.setTextColor(TFT_GREEN);
    g_tft.drawString((String("INT ") + String(intensity)).c_str(), 170, 8);
    g_tft.setTextColor(TFT_BLACK);
    g_tft.drawString((String(sensorName) + " " + String(sensorValue)).c_str(), 239, 9);
    g_tft.setTextColor(TFT_ORANGE);
    g_tft.drawString((String(sensorName) + " " + String(sensorValue)).c_str(), 238, 8);
    g_tft.setTextColor(TFT_BLACK);
    g_tft.drawRightString(storageReady ? "SD" : "NO SD", 471, 9);
    g_tft.setTextColor(storageReady ? TFT_GREEN : TFT_ORANGE);
    g_tft.drawRightString(storageReady ? "SD" : "NO SD", 470, 8);

    drawButton({370, 36, 96, 38, "MENU", TFT_WHITE});
    drawButton({188, 262, 104, 48, "SHOOT", TFT_RED});
}

void DisplayManager::drawMenuOverlay(const char* effectName, int intensity, const char* sensorName, int sensorValue) {
    g_tft.drawRect(100, 42, 280, 236, TFT_BLACK);
    g_tft.drawRect(101, 43, 278, 234, TFT_WHITE);
    g_tft.setTextSize(1);
    g_tft.setTextColor(TFT_BLACK);
    g_tft.drawString((String("FX: ") + effectName + String(" / ") + String(intensity)).c_str(), 117, 53);
    g_tft.drawString((String("CAPTEUR: ") + sensorName + " " + String(sensorValue)).c_str(), 117, 69);
    g_tft.setTextColor(TFT_LIGHTGREY);
    g_tft.drawString((String("FX: ") + effectName + String(" / ") + String(intensity)).c_str(), 116, 52);
    g_tft.drawString((String("CAPTEUR: ") + sensorName + " " + String(sensorValue)).c_str(), 116, 68);
    drawButton({140, 88, 200, 38, "GLITCH +", TFT_CYAN});
    drawButton({140, 136, 200, 38, "CAPTEUR +", TFT_ORANGE});
    drawButton({140, 184, 200, 38, "INT +", TFT_GREEN});
    drawButton({140, 232, 200, 38, "RETOUR", TFT_RED});
}

void DisplayManager::drawCaptureFlash() {
    g_tft.fillScreen(TFT_WHITE);
    delay(45);
}

void DisplayManager::drawError(const char* title, esp_err_t err) {
    g_tft.fillScreen(TFT_WHITE);
    g_tft.setTextColor(TFT_BLUE);
    g_tft.setTextSize(3);
    g_tft.drawCenterString(title, 240, 130);
    g_tft.setTextSize(2);
    char message[40];
    snprintf(message, sizeof(message), "Code erreur: 0x%x", err);
    g_tft.drawCenterString(message, 240, 168);
}

bool DisplayManager::drawPreview(const CameraFrameInfo& frame) {
    const size_t expectedLength = static_cast<size_t>(frame.width) * frame.height * sizeof(uint16_t);
    if (frame.data == nullptr
        || frame.format != PIXFORMAT_RGB565
        || frame.width == 0
        || frame.height == 0
        || frame.length < expectedLength
        || frame.width > g_tft.width()
        || frame.height > g_tft.height()) {
        return false;
    }

    const int x = (g_tft.width() - frame.width) / 2;
    const int y = (g_tft.height() - frame.height) / 2;

    // Efface l'intro une seule fois. Effacer tout l'ecran a chaque trame
    // provoquerait un scintillement et consommerait inutilement le bus SPI.
    if (!previewPrepared_) {
        g_tft.fillScreen(TFT_BLACK);
        previewPrepared_ = true;
    }
    // L'OV5640 fournit chaque pixel RGB565 octet fort en premier. Le type
    // swap565_t (rgb565_2Byte) indique explicitement cet ordre a LovyanGFX.
    g_tft.pushImageDMA(x,
                       y,
                       frame.width,
                       frame.height,
                       reinterpret_cast<const lgfx::swap565_t*>(frame.data));

    // Le framebuffer camera ne doit pas etre libere tant que le DMA le lit.
    g_tft.waitDMA();
    return true;
}

void DisplayManager::drawFrameStatus(const char* message) {
    g_tft.setTextSize(2);
    g_tft.setTextColor(TFT_BLACK);
    g_tft.drawCenterString(message, 241, 151);
    g_tft.setTextColor(TFT_RED);
    g_tft.drawCenterString(message, 240, 150);
}

void DisplayManager::drawButton(const UiButton& button) {
    g_tft.drawRoundRect(button.x + 1, button.y + 1, button.w, button.h, 6, TFT_BLACK);
    g_tft.drawRoundRect(button.x - 1, button.y - 1, button.w, button.h, 6, TFT_BLACK);
    g_tft.drawRoundRect(button.x, button.y, button.w, button.h, 6, button.color);
    g_tft.setTextColor(TFT_BLACK);
    g_tft.setTextSize(2);
    g_tft.drawCenterString(button.label, button.x + button.w / 2 + 1, button.y + button.h / 2 - 7);
    g_tft.setTextColor(button.color);
    g_tft.drawCenterString(button.label, button.x + button.w / 2, button.y + button.h / 2 - 8);
}

void DisplayManager::drawTouchMarker(int x, int y) {
    g_tft.fillCircle(x, y, 5, TFT_GREEN);
}

void DisplayManager::drawStatusLine(const char* message, int color) {
    g_tft.fillRect(10, 290, 460, 20, TFT_BLACK);
    g_tft.setTextSize(1);
    g_tft.setTextColor(color);
    g_tft.drawString(message, 15, 295);
}

bool DisplayManager::contains(const UiButton& button, const TouchPoint& point) const {
    return point.valid
        && point.x >= button.x
        && point.x <= button.x + button.w
        && point.y >= button.y
        && point.y <= button.y + button.h;
}

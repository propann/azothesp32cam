#pragma once

class DisplayManager {
public:
    bool begin();
    void drawBootUi();
    void drawTouchMarker(int x, int y);
};

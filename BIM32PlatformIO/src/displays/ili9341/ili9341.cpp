#include "./ili9341.hpp"

#include "../../pinout.hpp"
#include "../../config/config.hpp"
#include "./pictures/logo.hpp"

/**
 * Display initialisation
 */
void ILI9341::init(void) {
    tft.begin();
    #if defined(BIM32_CYD)
        tft.setRotation(1);
    #else
        tft.setRotation(3);
    #endif
    tft.setSwapBytes(true);
    tft.setTextWrap(false, false);
    tft.fillScreen(0);

    pinMode(TFT_BACKLIGHT, OUTPUT);
    digitalWrite(TFT_BACKLIGHT, HIGH);

    bool calDataValid = false;
    for(uint8_t i=0; i<5; i++) {
        _calData[i] = config.calData(i);
        if(_calData[i]) calDataValid = true;
    }

    #if defined(BIM32_CYD)
        touchscreenSPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
        touchscreen.begin(touchscreenSPI);
        touchscreen.setRotation(1);
    #else
        if(calDataValid) tft.setTouch(_calData);
    #endif
}

void ILI9341::showLogo() {
    _showImg(0, 0, logo, sizeof(logo));
    vTaskDelay(2000);
}

void ILI9341::showHomeScreen() {
    tft.fillScreen(BG_COLOR);
    _drawSkeleton();
}
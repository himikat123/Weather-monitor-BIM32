#include "./ili9341.hpp"

#include <TimeLib.h> // v1.6.1 https://playground.arduino.cc/Code/Time/
#include "../config.hpp"

/**
 * Display Big Clock page
 */
void ILI9341::_bigClockPage() {
    if(!_bigClockSkeleton) {
        tft.fillScreen(TFT_BLACK);
        _closeButton();
        _bigClockSkeleton = true;
        _forced = true;
    }
    if(_prevTHour != hour() || _forced) {
        tft.fillRect(0, 66, 160, 124, BG_COLOR);
        unsigned int hr = config.clock.format() > 1 ? hour() : hourFormat12();
        char buf[3];
        if(config.clock.format() % 2 == 0) sprintf(buf, "%02d", hr);
        else sprintf(buf, "%d", hr);
        _printText(0, 68, 155, 120, String(buf), FONT_SEGMENTS_BIG, RIGHT, CLOCK_COLOR);
        _prevTHour = hour();
    }

    boolean points = millis() % 1000 > 500;
    tft.fillSmoothCircle(164, 98, points ? 6 : 7, points ? CLOCK_COLOR : BG_COLOR, BG_COLOR);
    tft.fillSmoothCircle(162, 144, points ? 6 : 7, points ? CLOCK_COLOR : BG_COLOR, BG_COLOR);

    if(_prevTMinute != minute() || _forced) {
        char buf[3];
        sprintf(buf, "%02d", minute());
        tft.fillRect(172, 66, 143, 122, BG_COLOR);
        _printText(174, 68, 141, 120, String(buf), FONT_SEGMENTS_BIG, LEFT, CLOCK_COLOR);
        _prevTMinute = minute();
    }
    _dateWeekday();
    _forced = false;
}
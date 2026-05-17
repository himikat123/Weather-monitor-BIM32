#include "./ili9341.hpp"

#include <TimeLib.h> // v1.6.1 https://playground.arduino.cc/Code/Time/
#include "../../config/config.hpp"

/**
 * Display Small Clock page
 */
void ILI9341::_smallClockPage() {
    if(!_smallClockSkeleton) {
        tft.fillScreen(TFT_BLACK);
        _closeButton();
        _smallClockSkeleton = true;
        _forced = true;
    }
    if(_prevTHour != hour() || _forced) {
        tft.fillRect(1, 80, 106, 2, BG_COLOR);
        unsigned int hr = config.clock.format() > 1 ? hour() : hourFormat12();
        char buf[3];
        if(config.clock.format() % 2 == 0) sprintf(buf, "%02d", hr);
        else sprintf(buf, "%d", hr);
        _printText(1, 82, 106, 96, String(buf), FONT_SEGMENTS_SML, RIGHT, CLOCK_COLOR);
        _prevTHour = hour();
    }
    if(_prevTMinute != minute() || _forced) {
        char buf[3];
        sprintf(buf, "%02d", minute());
        tft.fillRect(108, 80, 106, 2, BG_COLOR);
        _printText(108, 82, 106, 96, String(buf), FONT_SEGMENTS_SML, CENTER, CLOCK_COLOR);
        _prevTMinute = minute();
    }
    if(_prevTSecond != second() || _forced) {
        char buf[3];
        sprintf(buf, "%02d", second());
        tft.fillRect(219, 80, 106, 2, BG_COLOR);
        _printText(219, 82, 106, 96, String(buf), FONT_SEGMENTS_SML, LEFT, CLOCK_COLOR);
        tft.fillSmoothCircle(112, 100, 3, CLOCK_COLOR, BG_COLOR);
        tft.fillSmoothCircle(110, 134, 3, CLOCK_COLOR, BG_COLOR);
        tft.fillSmoothCircle(215, 100, 3, CLOCK_COLOR, BG_COLOR);
        tft.fillSmoothCircle(213, 134, 3, CLOCK_COLOR, BG_COLOR);
        _prevTSecond = second();
    }
    _dateWeekday();
    _forced = false;
}
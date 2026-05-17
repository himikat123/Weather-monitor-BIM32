#include "./ili9341.hpp"

#include <TimeLib.h> // v1.6.1 https://playground.arduino.cc/Code/Time/
#include "../../config/config.hpp"

/**
 * Display the time and date of the last weather update
 */
void ILI9341::_showUpdTime() {
    if(_prevWeatherUpdated != _weatherUpdated || _forced) {
        time_t t = _weatherUpdated;
        char buf[32] = "";
        unsigned int hr = config.clock.format() > 1 ? hour(t) : hourFormat12(t);
        if(config.clock.format() % 2 == 0) sprintf(buf, "%02d.%02d.%d %d:%02d", day(t), month(t), year(t), hr, minute(t));
        else sprintf(buf, "%02d.%02d.%d %02d:%02d", day(t), month(t), year(t), hr, minute(t));
        _printText(176, 148, 117, 16, t > 0 ? buf : " ", FONT1, LEFT, TEXT_COLOR);
        if(t > 0) {
            tft.drawCircle(167, 153, 5, TEXT_COLOR);
            tft.drawFastHLine(166, 148, 4, BG_COLOR);
            tft.drawFastHLine(162, 148, 3, TEXT_COLOR);
            tft.drawFastVLine(165, 149, 3, TEXT_COLOR);
        }
        _prevWeatherUpdated = _weatherUpdated;
    }
}
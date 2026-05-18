#include "./ili9341.hpp"

#include <TimeLib.h> // v1.6.1 https://playground.arduino.cc/Code/Time/
#include "../../languages/languages.hpp"

void ILI9341::_hourlyWeekday(uint8_t num, uint16_t y) {
    tft.fillRect(num * 32 + 30, y, 32, 20, BG_COLOR);
    String wd = lang.weekdayShortName(weekday(_hrDate[num]));
    _printText(num * 32 + 30, y, 32, 18, wd, FONT1, CENTER, TEXT_COLOR);
}

void ILI9341::_hourlyDate(uint8_t num, uint16_t y) {
    tft.fillRect(num * 32 + 30, y, 32, 14, BG_COLOR);
    uint8_t dt = day(_hrDate[num]);
    String mo = lang.monthShortName(month(_hrDate[num]));
    _printText(num * 32 + 30, y, 32, 12, String(dt) + mo, FONT_TINY, CENTER, TEXT_COLOR);
}

void ILI9341::_hourlyTime(uint8_t num, uint16_t y) {
    tft.fillRect(num * 32 + 30, y, 32, 14, BG_COLOR);
    char buf[10];
    sprintf(buf, "%d:%02d", hour(_hrDate[num]), minute(_hrDate[num]));
    _printText(num * 32 + 30, y, 32, 12, String(buf), FONT_TINY, CENTER, TEXT_COLOR);
}
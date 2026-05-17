#include "./ili9341.hpp"

#include "../../languages/languages.hpp"

void ILI9341::_showWeekday() {
    if(_prevTWeekday != _tWeekday || _forced) {
        _printText(146, 6, 40, 20, lang.weekdayShortName(_tWeekday), FONT2, LEFT, CLOCK_COLOR);
    }
}

void ILI9341::_showWeekdays() {
    if(_prevTWeekday != _tWeekday || _forced) {
        unsigned int wd = _tWeekday;
        _printText(33, 168, 40, 16, lang.weekdayShortName(wd), FONT1, CENTER, TEXT_COLOR);
        if(++wd > 7) wd = 1;
        _printText(139, 168, 40, 16, lang.weekdayShortName(wd), FONT1, CENTER, TEXT_COLOR);
        if(++wd > 7) wd = 1;
        _printText(245, 168, 40, 16, lang.weekdayShortName(wd), FONT1, CENTER, TEXT_COLOR);
        _prevTWeekday = _tWeekday;
    }
}
#include "./ili9341.hpp"

#include <TimeLib.h> // v1.6.1 https://playground.arduino.cc/Code/Time/
#include "../config.hpp"
#include "../languages.hpp"

void ILI9341::_dateWeekday() {
    if(_prevTWeekday != weekday() || _forced) {
        tft.fillRect(29, 0, 261, 36, BG_COLOR);
        _printText(30, 8, 259, 30, lang.weekdayFullName(weekday()), FONT3, CENTER, TEMPERATURE_COLOR);
        _prevTWeekday = weekday();
    }
    if(_prevTDay != day() || _prevTMonth != month() || _prevTYear != year() || _forced) {
        char buf[32];
        const char* monthDay = lang.monthDay(month());
        int len = strlen(monthDay);
        if(config.lang() == "en") snprintf(buf, sizeof(buf), "%s, %02d %04d", monthDay, day(), year());
        else if(config.lang() == "de") snprintf(buf, sizeof(buf), "%02d. %s %04d", day(), monthDay, year());
        else if(config.lang() == "es") {
            snprintf(buf, sizeof(buf), "%02d de %s de %04d", day(), len > 7 ? lang.monthShortName(month()) : monthDay, year());
        }
        else snprintf(buf, sizeof(buf), "%02d %s %04d", day(), monthDay, year());

        tft.fillRect(0, 198, 319, 40, BG_COLOR);
        _printText(0, 200, 319, 30, String(buf), FONT3, CENTER, TEMP_MIN_COLOR);
        _prevTDay = day();
        _prevTMonth = month();
        _prevTYear = year();
    }
}
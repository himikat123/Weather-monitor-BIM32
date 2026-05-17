#include "./nextion.hpp"

#include "../../config/config.hpp"
#include "../../languages/languages.hpp"

/**
 * Sending time and date to the NX4832T035 display that does not have a built-in RTC
 */
void Nextion::_NX4832T035_timeDate() {
    if(_prevTHour != _tHour or _forced) {
        char buf[4];
        if(config.clock.format() % 2 == 0) sprintf(buf, "%d", _tHour);
        else sprintf(buf, "%02d", _tHour);
        _nextion.writeStr("BigClock.hour.txt", String(buf));
        _prevTHour = _tHour;
    }
    if(_prevTMinute != _tMinute or _forced) {
        char buf[4];
        sprintf(buf, "%02d", _tMinute);
        _nextion.writeStr("BigClock.minute.txt", String(buf));
        _prevTMinute = _tMinute;
    }
    if(_prevTDay != _tDay or _prevTMonth != _tMonth or _prevTYear != _tYear or _forced) {
        char buf[32];
        if(config.lang() == "en") snprintf(buf, sizeof(buf), "%s, %02d %04d", lang.monthDay(month()), day(), year());
        else if(config.lang() == "de") snprintf(buf, sizeof(buf), "%02d. %s %04d", day(), lang.monthDay(month()), year());
        else if(config.lang() == "es") snprintf(buf, sizeof(buf), "%02d de %s de %04d", day(), lang.monthDay(month()), year());
        else snprintf(buf, sizeof(buf), "%02d %s %04d", day(), lang.monthDay(month()), year());
        _nextion.writeStr("BigClock.date.txt", String(buf)); 
        _prevTDay = _tDay;
        _prevTMonth = _tMonth;
        _prevTYear = _tYear;
    }
    if(_prevTWeekday != _tWeekday or _forced) {
        unsigned int wd = _tWeekday;
        _nextion.writeStr("BigClock.weekday.txt", lang.weekdayFullName(wd));
        _nextion.writeStr("Main.weekday0.txt", lang.weekdayShortName(wd));
        if(++wd > 7) wd = 1;
        _nextion.writeStr("Main.weekday2.txt", lang.weekdayShortName(wd));
        if(++wd > 7) wd = 1;
        _nextion.writeStr("Main.weekday3.txt", lang.weekdayShortName(wd));
        if(++wd > 7) wd = 1;
        _nextion.writeStr("Main.weekday4.txt", lang.weekdayShortName(wd));
        _prevTWeekday = _tWeekday;
    }
}
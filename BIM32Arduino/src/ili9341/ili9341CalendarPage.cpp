#include "./ili9341.hpp"

#include <TimeLib.h> // v1.6.1 https://playground.arduino.cc/Code/Time/
#include "../languages.hpp"

bool ILI9341::_isLeapYear(unsigned int year) {
	return ((!(year % 4) && (year % 100)) || !(year % 400));
}

uint8_t ILI9341::_numberOfDaysInMonth(uint8_t month, uint16_t year) {
    uint8_t daysInMonth = 0;
    const uint8_t months[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	daysInMonth = month == 2 && _isLeapYear(year) ? 29 : months[month];
    return daysInMonth;
}

void ILI9341::_calendarPage() {
    if(!_calendarSkeleton) {
        tft.fillScreen(TFT_BLACK);
        for(uint8_t i=1; i<=7; i++) {
            String wd = i == 7 ? lang.weekdayShortName(1) : lang.weekdayShortName(i + 1);
            _printText(i * 36, 40, 36, 26, wd, FONT2, CENTER, i < 7 ? WEEKDAY_COLOR : WEEKEND_COLOR);
        }
        _closeButton();
        _leftButton(true);
        _rightButton(true);
        _calendarSkeleton = true;
        _prevCalendarShiftSeconds = -1;
    }

    time_t shiftSeconds = now() + _calendarShiftSeconds;
    if(_calendarShiftDirection > 0) {
        uint8_t daysInMonth = _numberOfDaysInMonth(month(shiftSeconds), year(shiftSeconds)); 
        time_t secondsUntilNextMonth = (daysInMonth - day(shiftSeconds)) * SECS_PER_DAY + SECS_PER_DAY;
        _calendarShiftSeconds += secondsUntilNextMonth;
    }
    if(_calendarShiftDirection < 0) { 
        time_t secondsUntilPrevMonth =  day(shiftSeconds) * SECS_PER_DAY + SECS_PER_DAY;
        _calendarShiftSeconds -= secondsUntilPrevMonth;
    }
    shiftSeconds = now() + _calendarShiftSeconds;

    int8_t firstWeekday = 0;
    for(uint8_t i=0; i<31; i++) {
        if(day(shiftSeconds - (i * SECS_PER_DAY)) == 1) {
            firstWeekday = weekday(shiftSeconds - (i * SECS_PER_DAY)) - 1;
            break;
        }
    }
    firstWeekday -= 1;
    if(firstWeekday < 0) firstWeekday = 6;
    uint8_t daysInMonth = _numberOfDaysInMonth(month(shiftSeconds), year(shiftSeconds));

    if(_prevCalendarShiftSeconds != _calendarShiftSeconds || _prevTDay != day()) {
        char mon[32];
        sprintf(mon, "%s %04d", lang.monthFullName(month(shiftSeconds)), year(shiftSeconds));
        _printText(30, 8, 260, 20, String(mon), FONT2, CENTER, MONTH_COLOR, BG_COLOR);
        uint8_t cday = 1;
        bool clndRun = false;
        for(uint8_t w=0; w<6; w++) {
            for(uint8_t d=0; d<7; d++) {
                bool today = day() == cday && month() == month(shiftSeconds) && year() == year(shiftSeconds);
                uint16_t tdColor = today ? TODAY_COLOR : (d == 6 ? WEEKEND_COLOR : TEXT_COLOR);
                uint16_t bgColor = today ? TODAY_BG_COLOR : BG_COLOR;
                if(firstWeekday == d) clndRun = true;
                if(cday > daysInMonth) clndRun = false;
                if(clndRun) {
                    _printText(d * 36 + 36, w * 28 + 64, 32, 28, String(cday), FONT2, CENTER, tdColor, bgColor, true);
                    cday++;
                }
                else _printText(d * 36 + 36, w * 28 + 64, 32, 28, "  ", FONT2, CENTER, TEXT_COLOR);
            }
        }
    }
    _prevCalendarShiftSeconds = _calendarShiftSeconds;
    _prevTDay = day();
    _calendarShiftDirection = 0;
}
#include "DS3232.h" // v0.4.1 https://github.com/RobTillaart/DS3232
#include <TimeLib.h> // v1.6.1 https://playground.arduino.cc/Code/Time/

#include "./sensors.hpp"

/**
 * Initialize DS3231 RTC
 */
void Sensors::_DS3231Init() {
    if(rtc.begin() == DS3232_OK) {
        _ds3231_det = true;
        get_ds3231_timeDate();
    }
}

/**
 * Get time & date from DS3231 RTC
 */
void Sensors::get_ds3231_timeDate() {
    if(_ds3231_det) {
        rtc.read();
        setTime(
            rtc.hours(), 
            rtc.minutes(), 
            rtc.seconds(), 
            rtc.day(), 
            rtc.month(), 
            2000 + rtc.year()
        );
    }
}

/**
 * Set time & date to DS3231 RTC
 */
void Sensors::set_ds3231_timeDate() {
    if(_ds3231_det) {
        if(year() > 2000) rtc.setYear(year() - 2000);
        rtc.setMonth(month());
        rtc.setDay(day());
        rtc.setWeekDay(weekday());
        rtc.setHours(hour());
        rtc.setMinutes(minute());
        rtc.setSeconds(second());
        rtc.write();
    }
}
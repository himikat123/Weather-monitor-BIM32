#include <Arduino.h>
#include <TimeLib.h> // v1.6.1 https://playground.arduino.cc/Code/Time/

#include "./timeNTP.hpp"
#include "../state/state.hpp"
#include "../config/config.hpp"
#include "../sensors/sensors.hpp"
#include "../displays/nextion/nextion.hpp"

/**
 * Time synchronization with NTP server
 */
void TimeNTP::syncNTP(void) {
    if(state.net_connected) {
        configTime(config.clock.utc() * 3600, 0, config.clock.ntp());
        struct tm tmstruct;
        vTaskDelay(1000);
        tmstruct.tm_year = 0;
        getLocalTime(&tmstruct);
        if(tmstruct.tm_year >= 121) {
            setTime(tmstruct.tm_hour, tmstruct.tm_min, tmstruct.tm_sec, tmstruct.tm_mday, tmstruct.tm_mon + 1, tmstruct.tm_year + 1900);
            unsigned int summertime = config.clock.dlst() ? isSummertime() ? 3600 : 0 : 0;
            unsigned int t = now() + summertime;
            setTime(t);
            state.clockSynchronize = true;
            state.clockSynchronized = true;
            nextion.setDisplayRTC();
            sensors.set_ds3231_timeDate();
            Serial.print("successful: ");
            Serial.printf("%d:%02d:%02d %02d.%02d.%d\r\n", hour(), minute(), second(), day(), month(), year());
        }
        else {
            Serial.println("failed");
            state.clockSynchronized = false;
        }
    }
    else state.clockSynchronized = false;
}

/**
 * Check the time and date for daylight saving time
 */
boolean TimeNTP::isSummertime() {
    if(month() < 3 || month() > 10) return false;
    if(month() > 3 && month() < 10) return true;
    if(
        (month() == 3 && (hour() + 24 * day()) >= (1 + 24 * (31 - (5 * year() / 4 + 4) % 7))) || 
        (month() == 10 && (hour() + 24 * day()) < (1 + 24 * (31 - (5 * year() / 4 + 1) % 7)))
    ) return true;
    else return false;
}